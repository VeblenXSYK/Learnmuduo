#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>
#include <boost/scoped_ptr.hpp>
#include "thread/Thread.h"
#include "thread/Mutex.h"
#include "datetime/Timestamp.h"
#include "TimerId.h"
#include "Callbacks.h"

namespace muduo
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop : boost::noncopyable
{
public:

	typedef boost::function<void()> Functor;

	EventLoop();
	~EventLoop();

	///
	/// Loops forever.
	///
	/// Must be called in the same thread as creation of the object.
	///
	void loop();
	
	void quit();
	
	///
	/// Time when poll returns, usually means data arrivial.
	///
	Timestamp pollReturnTime() const { return pollReturnTime_; }
	
	// timers

	///
	/// Runs callback at 'time'.
	///
	TimerId runAt(const Timestamp& time, const TimerCallback& cb);
	///
	/// Runs callback after @c delay seconds.
	///
	TimerId runAfter(double delay, const TimerCallback& cb);
	///
	/// Runs callback every @c interval seconds.
	///
	TimerId runEvery(double interval, const TimerCallback& cb);

	void cancel(TimerId timerId);
	
	// internal use only
	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	
	void runInLoop(const Functor& cb);
	void queueInLoop(const Functor& cb);

	void assertInLoopThread()
	{
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	}

	bool isInLoopThread() const
	{
		return threadId_ == CurrentThread::tid();
	}

private:

	void abortNotInLoopThread();
	void handleRead();
	void doPendingFunctors();
	
	typedef std::vector<Channel*> ChannelList;

	bool looping_; 							/* atomic */
	bool quit_;								/* atomic */
	bool callingPendingFunctors_;			/* atomic */
	const pid_t threadId_;
	Timestamp pollReturnTime_;				//从poll返回的时刻
	boost::scoped_ptr<Poller> poller_;
	boost::scoped_ptr<TimerQueue> timerQueue_;
	int wakeupFd_;
	// unlike in TimerQueue, which is an internal class,
	// we don't expose Channel to client.
	boost::scoped_ptr<Channel> wakeupChannel_;
	ChannelList activeChannels_;
	MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;
};

}

#endif  // MUDUO_NET_EVENTLOOP_H
