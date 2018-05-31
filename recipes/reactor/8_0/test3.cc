#include <stdio.h>
#include <sys/timerfd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "logging/Logging.h"

muduo::EventLoop *g_loop;


void timeoutA(muduo::Timestamp receiveTime)
{
	LOG_TRACE << receiveTime.toFormattedString().c_str() << " TimeoutA";
	//g_loop->quit();
}

void timeoutB(muduo::Timestamp receiveTime)
{
	LOG_TRACE << receiveTime.toFormattedString().c_str() << " TimeoutB";
	//g_loop->quit();
}

int g_timerfdC;
void timeoutC(muduo::Timestamp receiveTime)
{
	LOG_TRACE << receiveTime.toFormattedString().c_str() << " TimeoutC";
	
	//避免level trigger导致poll下次会被立刻触发
	char buf[1024] = {0};
	read(g_timerfdC, buf, 1024);
}

void threadFunc()
{
	LOG_TRACE << "threadFunc(): pid = " << getpid() << ", tid = " << muduo::CurrentThread::tid();

	muduo::EventLoop loop;
	
	//ChannelC
	g_timerfdC = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	muduo::Channel channelC(&loop, g_timerfdC);
	channelC.setReadCallback(timeoutC);
	channelC.enableReading();
	
	struct itimerspec howlong;
	bzero(&howlong, sizeof howlong);
	//设置周期性超时时间值
	howlong.it_interval.tv_sec = 5;
	//设置首次超时时间值
	howlong.it_value.tv_sec = 5;
	//启动定时器
	::timerfd_settime(g_timerfdC, 0, &howlong, NULL);
	
	loop.loop();
}

int main()
{
	//设置日志显示等级
	muduo::Logger::setLogLevel(muduo::Logger::TRACE); 
	
	LOG_TRACE << muduo::Timestamp::now().toFormattedString().c_str() << " started";
	
	// //创建一个线程
	// muduo::Thread thread(threadFunc);
	// thread.start();
	
	muduo::EventLoop loop;
	g_loop = &loop;

	//ChannelA
	//创建一个定时器对象，同时返回一个与之关联的文件描述符
	int timerfdA = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	muduo::Channel channelA(&loop, timerfdA);
	channelA.setReadCallback(timeoutA);
	channelA.enableReading();
	
	//ChannelB
	int timerfdB = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	muduo::Channel channelB(&loop, timerfdB);
	channelB.setReadCallback(timeoutB);
	channelB.enableReading();

	struct itimerspec howlong;
	bzero(&howlong, sizeof howlong);
	//设置超时时间
	howlong.it_value.tv_sec = 5;
	//启动定时器
	::timerfd_settime(timerfdA, 0, &howlong, NULL);
	howlong.it_value.tv_sec = 10;
	::timerfd_settime(timerfdB, 0, &howlong, NULL);

	loop.loop();

	// ::close(timerfdA);
	// ::close(timerfdB);
}
