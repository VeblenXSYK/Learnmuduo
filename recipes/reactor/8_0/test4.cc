#include "EventLoop.h"
#include "TimerId.h"
#include "logging/Logging.h"

#include <boost/bind.hpp>

#include <stdio.h>

muduo::EventLoop* g_loop;
muduo::TimerId toCancel;

void printTid()
{
	printf("pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
	printf("now %s\n", muduo::Timestamp::now().toString().c_str());
}

int cnt = 0;
void print(const char* msg)
{
	printf("msg %s %s\n", muduo::Timestamp::now().toString().c_str(), msg);
	// if (++cnt == 20) {
		// g_loop->quit();
	// }
}

void cancelSelf()
{
	printf("cancelSelf()\n");
	g_loop->cancel(toCancel);
}

int main()
{
	muduo::Logger::setLogLevel(muduo::Logger::TRACE);
	
	printTid();
	
	muduo::EventLoop loop;
	g_loop = &loop;

	print("main");
	// loop.runAfter(1, boost::bind(print, "once1"));
	// loop.runAfter(1.5, boost::bind(print, "once1.5"));
	// loop.runAfter(2.5, boost::bind(print, "once2.5"));
	// loop.runAfter(3.5, boost::bind(print, "once3.5"));
	// loop.runEvery(2, boost::bind(print, "every2"));
	// loop.runEvery(3, boost::bind(print, "every3"));
	toCancel = loop.runEvery(3, cancelSelf);
	loop.loop();
	print("main loop exits");
	
	sleep(1);
}
