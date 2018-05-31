#define __STDC_LIMIT_MACROS
#include "TimerQueue.h"

#include "logging/Logging.h"
#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"

#include <boost/bind.hpp>

#include <sys/timerfd.h>

namespace muduo
{
namespace detail
{

int createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
	                               TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		LOG_SYSFATAL << "Failed in timerfd_create";
	}
	return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
	int64_t microseconds = when.microSecondsSinceEpoch()
	                       - Timestamp::now().microSecondsSinceEpoch();
	if (microseconds < 100) {
		microseconds = 100;
	}
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(
	                microseconds / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(
	                 (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
	return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
	uint64_t howmany;			//howmany表示超时的次数
	ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
	LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
	if (n != sizeof howmany) {
		LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
	}
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
	// wake up loop by timerfd_settime()
	struct itimerspec newValue;
	struct itimerspec oldValue;
	bzero(&newValue, sizeof newValue);
	bzero(&oldValue, sizeof oldValue);
	newValue.it_value = howMuchTimeFromNow(expiration);
	int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
	if (ret) {
		LOG_SYSERR << "timerfd_settime()";
	}
}

}
}

using namespace muduo;
using namespace muduo::detail;

TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop),
	  timerfd_(createTimerfd()),
	  timerfdChannel_(loop, timerfd_),
	  timers_()
{
	timerfdChannel_.setReadCallback(boost::bind(&TimerQueue::handleRead, this));	//用来读timerfd
	// we are always reading the timerfd, we disarm it with timerfd_settime.
	timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
	::close(timerfd_);
	// do not remove channel, since we're in EventLoop::dtor();
	for (TimerList::iterator it = timers_.begin(); it != timers_.end(); ++it) {
		delete it->second;
	}
}

//线程安全
TimerId TimerQueue::addTimer(const TimerCallback& cb,
                             Timestamp when,
                             double interval)
{
	Timer* timer = new Timer(cb, when, interval);
	//调用runInLoop,把实际工作转移到IO线程来做，addTimer只负责转发
	loop_->runInLoop(boost::bind(&TimerQueue::addTimerInLoop, this, timer));
	return TimerId(timer);
}

//完成修改定时器列表的工作
void TimerQueue::addTimerInLoop(Timer* timer)
{
	loop_->assertInLoopThread();
	bool earliestChanged = insert(timer);

	if (earliestChanged) {
		resetTimerfd(timerfd_, timer->expiration());
	}
}

void TimerQueue::handleRead()
{
	loop_->assertInLoopThread();
	Timestamp now(Timestamp::now());
	//读timerfd，避免定时器事件一直触发
	readTimerfd(timerfd_, now);

	std::vector<Entry> expired = getExpired(now);

	// safe to callback outside critical section
	for (std::vector<Entry>::iterator it = expired.begin(); it != expired.end(); ++it) {
		it->second->run();
	}

	reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	std::vector<Entry> expired;
	Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator it = timers_.lower_bound(sentry);		//返回第一个未到期的Timer的迭代器
	assert(it == timers_.end() || now < it->first);
	std::copy(timers_.begin(), it, back_inserter(expired));		//拷贝到期的Timer到expired
	timers_.erase(timers_.begin(), it);							//移除到期的Timer

	return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
	Timestamp nextExpire;

	for (std::vector<Entry>::const_iterator it = expired.begin(); it != expired.end(); ++it) {
		if (it->second->repeat()) {
			it->second->restart(now);
			insert(it->second);
		} else {
			// FIXME move to a free list
			delete it->second;
		}
	}

	if (!timers_.empty()) {
		nextExpire = timers_.begin()->second->expiration();
	}

	if (nextExpire.valid()) {
		resetTimerfd(timerfd_, nextExpire);
	}
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;
	Timestamp when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	if (it == timers_.end() || when < it->first) {
		earliestChanged = true;
	}
	std::pair<TimerList::iterator, bool> result = timers_.insert(std::make_pair(when, timer));
	assert(result.second);
	return earliestChanged;
}

