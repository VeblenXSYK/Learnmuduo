#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "datetime/Timestamp.h"
#include "thread/Mutex.h"
#include "Callbacks.h"
#include "Channel.h"

namespace muduo
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
class TimerQueue : boost::noncopyable
{
public:
	TimerQueue(EventLoop* loop);
	~TimerQueue();

	///
	/// Schedules the callback to be run at given time,
	/// repeats if @c interval > 0.0.
	///
	/// Must be thread safe. Usually be called from other threads.
	TimerId addTimer(const TimerCallback& cb,
	                 Timestamp when,
	                 double interval);

	void cancel(TimerId timerId);
	void cancelInLoop(TimerId timerId);

private:

	// FIXME: use unique_ptr<Timer> instead of raw pointers.
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;
	typedef std::pair<Timer*, int64_t> ActiveTimer;
	typedef std::set<ActiveTimer> ActiveTimerSet;

	void addTimerInLoop(Timer* timer);
	// called when timerfd alarms
	void handleRead();
	// move out all expired timers
	std::vector<Entry> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);

	bool insert(Timer* timer);

	EventLoop* loop_;
	const int timerfd_;
	Channel timerfdChannel_;
	// Timer list sorted by expiration
	TimerList timers_;						//保存的是目前有效的Timer的指针（按到期时间排序）
	
	bool callingExpiredTimers_;				//自注销标志
	ActiveTimerSet activeTimers_;			//保存的是目前有效的Timer的指针（按对象地址排序）
	ActiveTimerSet cancelingTimers_;		//保存自注销定时器
	
};

}
#endif  // MUDUO_NET_TIMERQUEUE_H
