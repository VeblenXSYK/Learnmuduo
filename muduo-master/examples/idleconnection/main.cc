#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <stdio.h>
#include "echo.h"

using namespace muduo;
using namespace muduo::net;

int main(int argc, char* argv[])
{
	InetAddress listenAddr(2008);
	int idleSeconds = 10;
	if (argc > 1) {
		idleSeconds = atoi(argv[1]);
	}
	LOG_INFO << "pid = " << getpid() << ", idle seconds = " << idleSeconds;
	
	EventLoop loop;
	EchoServer server(&loop, listenAddr, idleSeconds);
	server.start();
	loop.loop();
}

