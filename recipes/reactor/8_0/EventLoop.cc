#include <assert.h>
#include <sys/eventfd.h>
#include <boost/bind.hpp>
#include "EventLoop.h"
#include "TimerQueue.h"
#include "Channel.h"
#include "Poller.h"
#include "logging/Logging.h"

using namespace muduo;

//定义线程局部变量用来EventLoop对象的地址
__thread EventLoop *t_loopInThisThread = 0;
//__thread EventLoop g_eventloop;

//定义poll系统调用的超时时间
const int kPollTimeMs = 10000;

/*
	使用eventfd更高效地(相比于管道)唤醒阻塞在poll调用的IO线程
*/
static int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		LOG_SYSERR << "Failed in eventfd";
		abort();
	}
	return evtfd;
}

EventLoop::EventLoop() 
				: looping_(false), 
				  quit_(false),
				  callingPendingFunctors_(false),
				  threadId_(CurrentThread::tid()),
				  poller_(new Poller(this)),
				  timerQueue_(new TimerQueue(this)),			//创建timerfd
				  wakeupFd_(createEventfd()),					//创建eventfd
				  wakeupChannel_(new Channel(this, wakeupFd_))
				  
{
	LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
	if (t_loopInThisThread) 
	{
		LOG_FATAL << "Another EventLoop " << t_loopInThisThread
		          << " exists in this thread " << threadId_;
	} else {
		t_loopInThisThread = this;
	}
	wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead, this));		//用来读eventfd
	// we are always reading the wakeupfd
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	assert(!looping_);
	::close(wakeupFd_);
	t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;

	while(!quit_)
	{
		activeChannels_.clear();
		pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
		for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it){
			(*it)->handleEvent(pollReturnTime_);
		}
		/*
			doPendingFunctors放在这里调用是有意义的：如果在当前IO线程中调用queueInLoop，由于没有阻塞在poll调用，
			所以可以直接执行doPendingFunctors()；如果将该函数放在handleRead()中执行，则需要通过wakeup唤醒执行
			doPendingFunctors，所以需要多write、poll、read这三次系统调用的时间
		*/
		doPendingFunctors();
	}

	LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
	/*
		对于当前IO线程，此时肯定没有阻塞在poll，所以不必wakeup
	*/
	if(!isInLoopThread())
	{
		wakeup();
	}
}

/*
	在当前IO线程调用，回调函数立即执行
	在其他线程调用，回调函数被加入队列，由IO线程来调用
*/
void EventLoop::runInLoop(const Functor& cb)
{
	if(isInLoopThread())
	{
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
	MutexLockGuard lock(mutex_);	
	pendingFunctors_.push_back(cb);
	}
	
	/*
		如果不是当前IO线程，必须唤醒
		如果是当前IO线程，且此时正在调用doPendingFunctors(即functors中调用queueInLoop)，也需要唤醒，
	这样可以确保新加入的cb能及时调用
	*/
	if(!isInLoopThread() || callingPendingFunctors_)
	{
		wakeup();
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;
	
	{
	MutexLockGuard lock(mutex_);	
	functors.swap(pendingFunctors_);	//避免死锁，因为functors可能再调用queueInLoop
	}
	
	for(size_t i = 0; i < functors.size(); i++)
	{
		functors[i]();
	}
	
	callingPendingFunctors_ = false;
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = :: read(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
	
	//doPendingFunctors();
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
	return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), delay));
	return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), interval));
	return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->removeChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
	          << " was created in threadId_ = " << threadId_
	          << ", current thread id = " <<  CurrentThread::tid();
}

