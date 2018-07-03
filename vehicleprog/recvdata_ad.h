#ifndef RECVDATA_AD_H
#define RECVDATA_AD_H

#include <muduo/base/Logging.h>
#include <muduo/net/Socket.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

using namespace muduo;
using namespace muduo::net;

class DataHandle;
class RecvAD
{
public:
	RecvAD(EventLoop *loop, const InetAddress &serverAddr, DataHandle *dh);
	void start();
	
private:
	void netDataTimeoutCallback(int sockfd);
	void netDataErrorCallback(int sockfd);
	void sendOnOffCmd(int sockfd);
	
	EventLoop *loop_;
	Socket sock_;
	Channel channel_;
};

#endif
