#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include <map>
#include <vector>

#include "datetime/Timestamp.h"
#include "EventLoop.h"

//struct pollfd;		//采用前向声明的方式替代#include<poll.h>

namespace muduo
{

class Channel;

///
/// IO Multiplexing with poll(2).
///
/// This class doesn't own the Channel objects.
/// 每个Poller对象只负责IO复用
class Poller : boost::noncopyable
{
public:
	typedef std::vector<Channel*> ChannelList;

	Poller(EventLoop* loop);
	~Poller();

	/// Polls the I/O events.
	/// Must be called in the loop thread.
	Timestamp poll(int timeoutMs, ChannelList *activeChannels);

	/// Changes the interested I/O events.
	/// Must be called in the loop thread.
	void updateChannel(Channel* channel);

	void assertInLoopThread()
	{
		ownerLoop_->assertInLoopThread();
	}

private:
	void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;

	typedef std::vector<struct pollfd> PollFdList;
	typedef std::map<int, Channel*> ChannelMap;

	EventLoop* ownerLoop_;
	PollFdList pollfds_;
	ChannelMap channels_;
};

}
#endif  // MUDUO_NET_POLLER_H
