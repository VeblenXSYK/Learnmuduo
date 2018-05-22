#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace muduo
{

class EventLoop;

///
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd
/// 每个Channel对象只负责一个IO事件分发，且该对象只属于某一个IO线程
class Channel : boost::noncopyable
{
public:
	typedef boost::function<void()> EventCallback;

	Channel(EventLoop* loop, int fd);

	void handleEvent();
	void setReadCallback(const EventCallback& cb)
	{
		readCallback_ = cb;
	}
	void setWriteCallback(const EventCallback& cb)
	{
		writeCallback_ = cb;
	}
	void setErrorCallback(const EventCallback& cb)
	{
		errorCallback_ = cb;
	}

	int fd() const
	{
		return fd_;
	}
	int events() const
	{
		return events_;
	}
	void set_revents(int revt)
	{
		revents_ = revt;
	}
	bool isNoneEvent() const
	{
		return events_ == kNoneEvent;
	}

	void enableReading()
	{
		events_ |= kReadEvent;
		update();
	}
	// void enableWriting() { events_ |= kWriteEvent; update(); }
	// void disableWriting() { events_ &= ~kWriteEvent; update(); }
	// void disableAll() { events_ = kNoneEvent; update(); }

	// for Poller
	int index()
	{
		return index_;
	}
	void set_index(int idx)
	{
		index_ = idx;
	}

	EventLoop* ownerLoop()
	{
		return loop_;
	}

private:
	void update();

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop* loop_;
	const int  fd_;
	int        events_;			//关心的IO事件，由用户设置
	int        revents_;		//目前活动的事件，由EventLoop/Poller设置
	int        index_; 			// used by Poller.

	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback errorCallback_;
};

}
#endif  // MUDUO_NET_CHANNEL_H
