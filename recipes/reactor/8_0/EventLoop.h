#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>
#include <boost/scoped_ptr.hpp>
#include "thread/Thread.h"

namespace muduo
{

class Channel;
class Poller;

class EventLoop : boost::noncopyable
{
public:

	EventLoop();
	~EventLoop();

	///
	/// Loops forever.
	///
	/// Must be called in the same thread as creation of the object.
	///
	void loop();
	
	void quit();
	
	// internal use only
	void updateChannel(Channel* channel);
	// void removeChannel(Channel* channel);

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
	
	typedef std::vector<Channel*> ChannelList;

	bool looping_; 							/* atomic */
	bool quit_;								/* atomic */
	const pid_t threadId_;
	boost::scoped_ptr<Poller> poller_;
	ChannelList activeChannels_;
};

}

#endif  // MUDUO_NET_EVENTLOOP_H
