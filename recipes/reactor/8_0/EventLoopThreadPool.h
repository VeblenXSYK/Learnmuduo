#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include "thread/Condition.h"
#include "thread/Mutex.h"
#include "thread/Thread.h"


#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace muduo
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable
{
public:
	EventLoopThreadPool(EventLoop* baseLoop);
	~EventLoopThreadPool();
	void setThreadNum(int numThreads)
	{
		numThreads_ = numThreads;
	}
	void start();
	EventLoop* getNextLoop();

private:
	EventLoop* baseLoop_;							//TcpServer自己用的loop
	bool started_;
	int numThreads_;								//线程池中的线程个数
	int next_;										//线程池中下一个用于执行新连接IO的线程下标
	boost::ptr_vector<EventLoopThread> threads_;	//线程池，保存的是EventLoopThread的指针
	std::vector<EventLoop*> loops_;					//事件IO池
};

}

#endif  // MUDUO_NET_EVENTLOOPTHREADPOOL_H
