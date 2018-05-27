#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>

void newConnection(int sockfd, const muduo::InetAddress& peerAddr)
{
	printf("newConnection(): accepted a new connection from %s\n", peerAddr.toHostPort().c_str());
	::write(sockfd, "hehehehehe!!!\n", 15);
	muduo::sockets::close(sockfd);
}

void newConnection1(int sockfd, const muduo::InetAddress& peerAddr)
{
	printf("newConnection1(): accepted a new connection from %s\n", peerAddr.toHostPort().c_str());
	::write(sockfd, "hahahahaha!!!\n", 15);
	muduo::sockets::close(sockfd);
}

int main()
{
	printf("main(): pid = %d\n", getpid());
	
	muduo::EventLoop loop;

	muduo::InetAddress listenAddr(9981);
	muduo::Acceptor acceptor(&loop, listenAddr);
	acceptor.setNewConnectionCallback(newConnection);
	acceptor.listen();
	
	muduo::InetAddress listenAddr1(9982);
	muduo::Acceptor acceptor1(&loop, listenAddr1);
	acceptor1.setNewConnectionCallback(newConnection1);
	acceptor1.listen();

	loop.loop();
	
	return 0;
}
