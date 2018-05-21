#include <stdio.h>
#include "EventLoop.h"
#include "thread/Thread.h"
#include "logging/Logging.h"

void threadFunc()
{
	printf("threadFunc(): pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());

	muduo::EventLoop loop;
	loop.loop();
}

int main()
{	
	printf("main(): pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());

	muduo::Thread thread(threadFunc);
	thread.start();

	muduo::EventLoop loop;
	loop.loop();
	
	//muduo::EventLoop loop2;
	
	pthread_exit(NULL);
}
