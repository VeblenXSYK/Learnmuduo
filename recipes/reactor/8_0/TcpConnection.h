#ifndef MUDUO_NET_TCPCONNECTION_H
#define MUDUO_NET_TCPCONNECTION_H

#include "Callbacks.h"
#include "InetAddress.h"

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace muduo
{

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
class TcpConnection : boost::noncopyable,
	public boost::enable_shared_from_this<TcpConnection>
{
public:
	/// Constructs a TcpConnection with a connected sockfd
	///
	/// User should not create this object.
	TcpConnection(EventLoop* loop,
	              const std::string& name,
	              int sockfd,
	              const InetAddress& localAddr,
	              const InetAddress& peerAddr);
	~TcpConnection();

	EventLoop* getLoop() const
	{
		return loop_;
	}
	const std::string& name() const
	{
		return name_;
	}
	const InetAddress& localAddress()
	{
		return localAddr_;
	}
	const InetAddress& peerAddress()
	{
		return peerAddr_;
	}
	bool connected() const
	{
		return state_ == kConnected;
	}

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}
	
	void setCloseCallback(const CloseCallback& cb)
	{
		closeCallback_ = cb;
	}

	/// Internal use only.

	// called when TcpServer accepts a new connection
	void connectEstablished();   // should be called only once
	// called when TcpServer has removed me from its map
	void connectDestroyed();  // should be called only once

private:
	enum StateE { kConnecting, kConnected, kDisconnected };

	void setState(StateE s)
	{
		state_ = s;
	}
	void handleRead();
	void handleWrite();
	void handleClose();
	void handleError();

	EventLoop* loop_;
	std::string name_;				//连接名称(ipaddr:port#序号)
	StateE state_;  // FIXME: use atomic variable
	// we don't expose those classes to client.
	boost::scoped_ptr<Socket> socket_;			//TcpConnection析构后会调用Socket的析构函数，其会关闭socket连接
	boost::scoped_ptr<Channel> channel_;		//TcpConnection析构后会调用Channel的析构函数
	InetAddress localAddr_;			//本地地址(ipaddr:port)
	InetAddress peerAddr_;			//远程地址(ipaddr:port)
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

}

#endif  // MUDUO_NET_TCPCONNECTION_H
