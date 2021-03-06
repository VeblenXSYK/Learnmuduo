#ifndef MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H
#define MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


class Tunnel : public boost::enable_shared_from_this<Tunnel>, boost::noncopyable
{
public:
	Tunnel(muduo::net::EventLoop* loop,
	       const muduo::net::InetAddress& serverAddr,
	       const muduo::net::TcpConnectionPtr& serverConn)
		: client_(loop, serverAddr, serverConn->name()),
		  serverConn_(serverConn)
	{
		LOG_INFO << "Tunnel " << serverConn->peerAddress().toIpPort() << " <-> " << serverAddr.toIpPort();
	}

	~Tunnel()
	{
		LOG_INFO << "~Tunnel";
	}

	void setup()
	{
		client_.setConnectionCallback(boost::bind(&Tunnel::onClientConnection, shared_from_this(), _1));
		client_.setMessageCallback(boost::bind(&Tunnel::onClientMessage, shared_from_this(), _1, _2, _3));
		serverConn_->setHighWaterMarkCallback(boost::bind(&Tunnel::onHighWaterMarkWeak, boost::weak_ptr<Tunnel>(shared_from_this()), kServer, _1, _2), 1024*1024);
	}

	void connect()
	{
		client_.connect();
	}

	void disconnect()
	{
		client_.disconnect();
		// serverConn_.reset();
	}

private:
	void teardown()
	{
		client_.setConnectionCallback(muduo::net::defaultConnectionCallback);
		client_.setMessageCallback(muduo::net::defaultMessageCallback);
		if (serverConn_) 
		{
			serverConn_->setContext(boost::any());
			serverConn_->shutdown();
		}
		clientConn_.reset();
	}

	void onClientConnection(const muduo::net::TcpConnectionPtr& conn)
	{
		LOG_DEBUG << (conn->connected() ? "UP" : "DOWN");
		
		if (conn->connected()) 
		{
			conn->setTcpNoDelay(true);
			conn->setHighWaterMarkCallback(boost::bind(&Tunnel::onHighWaterMarkWeak, boost::weak_ptr<Tunnel>(shared_from_this()), kClient, _1, _2), 1024*1024);
			serverConn_->setContext(conn);
			serverConn_->startRead();
			clientConn_ = conn;
			if (serverConn_->inputBuffer()->readableBytes() > 0) 
			{
				// 发送"在S连接建立前缓存C的数据"
				conn->send(serverConn_->inputBuffer());
			}
		} 
		else 
		{
			teardown();
		}
	}

	void onClientMessage(const muduo::net::TcpConnectionPtr& conn,
	                     muduo::net::Buffer* buf,
	                     muduo::Timestamp)
	{
		LOG_DEBUG << conn->name() << " " << buf->readableBytes();
		if (serverConn_) 
		{
			// 将S发送过来的数据发送给C
			serverConn_->send(buf);
		} 
		else 
		{
			buf->retrieveAll();
			abort();
		}
	}

	enum ServerClient {
		kServer, kClient
	};

	void onHighWaterMark(ServerClient which,
	                     const muduo::net::TcpConnectionPtr& conn,
	                     size_t bytesToSent)
	{
		LOG_INFO << (which == kServer ? "server" : "client")
		         << " onHighWaterMark " << conn->name()
		         << " bytes " << bytesToSent;

		if (which == kServer) {
			if (serverConn_->outputBuffer()->readableBytes() > 0) {
				clientConn_->stopRead();
				serverConn_->setWriteCompleteCallback(boost::bind(&Tunnel::onWriteCompleteWeak, boost::weak_ptr<Tunnel>(shared_from_this()), kServer, _1));
			}
		} else {
			if (clientConn_->outputBuffer()->readableBytes() > 0) {
				serverConn_->stopRead();
				clientConn_->setWriteCompleteCallback(boost::bind(&Tunnel::onWriteCompleteWeak, boost::weak_ptr<Tunnel>(shared_from_this()), kClient, _1));
			}
		}
	}

	static void onHighWaterMarkWeak(const boost::weak_ptr<Tunnel>& wkTunnel,
	                                ServerClient which,
	                                const muduo::net::TcpConnectionPtr& conn,
	                                size_t bytesToSent)
	{
		boost::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
		if (tunnel) {
			tunnel->onHighWaterMark(which, conn, bytesToSent);
		}
	}

	void onWriteComplete(ServerClient which, const muduo::net::TcpConnectionPtr& conn)
	{
		LOG_INFO << (which == kServer ? "server" : "client")
		         << " onWriteComplete " << conn->name();
		if (which == kServer) {
			clientConn_->startRead();
			serverConn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
		} else {
			serverConn_->startRead();
			clientConn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
		}
	}

	static void onWriteCompleteWeak(const boost::weak_ptr<Tunnel>& wkTunnel,
	                                ServerClient which,
	                                const muduo::net::TcpConnectionPtr& conn)
	{
		boost::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
		if (tunnel) {
			tunnel->onWriteComplete(which, conn);
		}
	}

private:
	muduo::net::TcpClient client_;
	muduo::net::TcpConnectionPtr serverConn_;
	muduo::net::TcpConnectionPtr clientConn_;
};
typedef boost::shared_ptr<Tunnel> TunnelPtr;

#endif  // MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H
