#include "TcpServer.h"

#include "logging/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

#include <stdio.h>  // snprintf

using namespace muduo;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
	: loop_(CHECK_NOTNULL(loop)),
	  name_(listenAddr.toHostPort()),
	  acceptor_(new Acceptor(loop, listenAddr)),
	  started_(false),
	  nextConnId_(1)
{
	/*
		新连接到达时，Acceptor会回调newConnection
	*/
	acceptor_->setNewConnectionCallback(boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
	if (!started_) {
		started_ = true;
	}

	if (!acceptor_->listenning()) {
		/*
			相当于调用acceptor_->listen，由于acceptor_是私有成员变量，不能直接使用，需要
			使用boost::get_pointer转换一下？？？
		*/
		//loop_->runInLoop(boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
		loop_->runInLoop(boost::bind(&Acceptor::listen, boost::get_pointer(acceptor_)));
		//loop_->runInLoop(boost::bind(&Acceptor::listen, acceptor_));错误使用
	}
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	loop_->assertInLoopThread();
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", nextConnId_);
	++nextConnId_;
	std::string connName = name_ + buf;

	LOG_INFO << "TcpServer::newConnection [" << name_
	         << "] - new connection [" << connName
	         << "] from " << peerAddr.toHostPort();
	InetAddress localAddr(sockets::getLocalAddr(sockfd));
	// FIXME poll with zero timeout to double confirm the new connection
	//创建TcpConnection对象
	TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
	//将该对象加入ConnectionMap
	connections_[connName] = conn;
	//设置回调
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->connectEstablished();
}

