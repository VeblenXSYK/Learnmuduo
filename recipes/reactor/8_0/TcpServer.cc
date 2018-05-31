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
	  acceptor_(new Acceptor(loop, listenAddr)),	//创建socket
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
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	conn->setCloseCallback(boost::bind(&TcpServer::removeConnection, this, _1));	//这个回调是给内部调用的
	conn->connectEstablished();
}

/*
	移除所持有的TcpConnectionPtr
*/
void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	loop_->assertInLoopThread();
	LOG_INFO << "TcpServer::removeConnection [" << name_ << "] - connection " << conn->name();
	size_t n = connections_.erase(conn->name());								//把conn从ConnectionMap中移除，conn引用计数降到1
	assert(n == 1); (void)n;
	
	//在EventLoop中执行TcpConnection::connectDestroyed
	loop_->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));	//boost::bind延长conn的生命周期
	
	/*
		Channel::handleEvent中执行TcpConnection::connectDestroyed析构Channel，会产生问题
	*/
	//loop_->runInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));	
}

