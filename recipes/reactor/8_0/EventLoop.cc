#include <assert.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "logging/Logging.h"

using namespace muduo;

//定义线程局部变量用来EventLoop对象的地址
__thread EventLoop *t_loopInThisThread = 0;
//__thread EventLoop g_eventloop;

//定义poll系统调用的超时时间
const int kPollTimeMs = 10000;

EventLoop::EventLoop() 
				: looping_(false), 
				  quit_(false),
				  threadId_(CurrentThread::tid()),
				  poller_(new Poller(this))
{
	LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
	if (t_loopInThisThread) 
	{
		LOG_FATAL << "Another EventLoop " << t_loopInThisThread
		          << " exists in this thread " << threadId_;
	} else {
		t_loopInThisThread = this;
	}
}

EventLoop::~EventLoop()
{
	assert(!looping_);
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
		poller_->poll(kPollTimeMs, &activeChannels_);
		for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it){
			(*it)->handleEvent();
		}
	}

	LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
	          << " was created in threadId_ = " << threadId_
	          << ", current thread id = " <<  CurrentThread::tid();
}

