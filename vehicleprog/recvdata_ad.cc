#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "common.h"
#include "datahandle.h"
#include "recvdata_ad.h"

#include <boost/bind.hpp>

RecvAD::RecvAD(EventLoop *loop,
		const InetAddress &serverAddr,
		DataHandle *dh)
		: loop_(loop),
		  sock_(common::createNonblockingUDP()),
		  channel_(loop, sock_.fd())
{
	//connect AD板服务端
	int ret = sockets::connect(sock_.fd(), serverAddr.getSockAddr());
	if (ret < 0) 
	{
		LOG_SYSFATAL << "::connect";
	}
	
	//socket可读时回调DataHandle::netDataReadCallback进行处理
	channel_.setReadCallback(boost::bind(&DataHandle::netDataReadCallback, dh, sock_.fd(), _1));
	channel_.setTimeoutCallback(boost::bind(&RecvAD::netDataTimeoutCallback, this, sock_.fd()));
	channel_.setErrorCallback(boost::bind(&RecvAD::netDataErrorCallback, this, sock_.fd()));
	channel_.enableReading();
}

/*
	socket错误时回调，避免不处理导致出现epoll一直触发的问题
*/
void RecvAD::netDataErrorCallback(int sockfd)
{
	int err = sockets::getSocketError(sockfd);
	LOG_ERROR << "clientErrorCallback - SO_ERROR = " << err << " " << strerror_tl(err);
}

/*
	epoll超时(即socket没有数据可读)回调
*/
void RecvAD::netDataTimeoutCallback(int sockfd)
{
	LOG_INFO << "netDataTimeoutCallback";
	
	//重新发送启动AD板的命令
	sendOnOffCmd(sockfd);
}

/*
	发送 开启/关闭 AD板数据发送的命令
*/
void RecvAD::sendOnOffCmd(int sockfd)
{
	char cmd[3] = { 0x03, 0x00, 0x03};
	
	ssize_t nw = sockets::write(sockfd, cmd, sizeof cmd);
	if (nw < 0) 
	{
		LOG_SYSERR << "::write";
	} 
	else if (implicit_cast<size_t>(nw) != 3) 
	{
		LOG_ERROR << "Expect " << 3 << " bytes, wrote " << nw << " bytes.";
	}
}

/*
	启动AD板数据发送
*/
void RecvAD::start()
{
	sendOnOffCmd(sock_.fd());
}
