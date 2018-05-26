#include "EventLoopThread.h"
#include "EventLoop.h"
#include <boost/bind.hpp>

using namespace muduo;

EventLoopThread::EventLoopThread()
	: loop_(NULL),
	  exiting_(false),
	  thread_(boost::bind(&EventLoopThread::threadFunc, this)),
	  mutex_(),
	  cond_(mutex_)
{
}

EventLoopThread::~EventLoopThread()
{
	exiting_ = true;
	//loop_->quit();
	//使主线程等待EventLoopThread线程释放资源
	thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
	assert(!thread_.started());
	thread_.start();

	{
		MutexLockGuard lock(mutex_);
		while (loop_ == NULL) {
			//用条件变量等待线程的创建与运行
			cond_.wait();
		}
	}

	return loop_;
}

void EventLoopThread::threadFunc()
{
	EventLoop loop;

	{
		MutexLockGuard lock(mutex_);
		loop_ = &loop;
		cond_.notify();
	}

	loop.loop();
	//assert(exiting_);
}

