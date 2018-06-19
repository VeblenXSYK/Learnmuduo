#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include "commserver.h"
#include "protocol.h"


CommServer::CommServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr, 
					   int maxConnections)
	: server_(loop, listenAddr, "CommServer"),
	  numConnected_(0),
	  kMaxConnections_(maxConnections)
{
	server_.setConnectionCallback(boost::bind(&CommServer::onConnection, this, _1));
	server_.setMessageCallback(boost::bind(&CommServer::onMessage, this, _1, _2, _3));
}

void CommServer::start()
{
	server_.start();
}

void CommServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
	LOG_INFO << "CommServer - " << conn->peerAddress().toIpPort() << " -> "
	         << conn->localAddress().toIpPort() << " is "
	         << (conn->connected() ? "UP" : "DOWN");
			 
	// LOG_INFO << "conn.use_count:" << conn.use_count();
	// LOG_INFO << "conn_.use_count:" << conn_.use_count();
	
	//当有新连接上来时，断开先前的连接
	if (conn->connected()) 
	{
		++numConnected_;
		if (numConnected_ > kMaxConnections_) 
		{
			conn_->shutdown();
			conn_->forceCloseWithDelay(3.0);  // > round trip of the whole Internet.
			conn_.reset();
		}
		
		conn_ = conn;
	} 
	else 
	{
		--numConnected_;
	}
	
	// LOG_INFO << "conn.use_count:" << conn.use_count();
	// LOG_INFO << "conn_.use_count:" << conn_.use_count();
}

void CommServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time)
{
	muduo::string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
	         << "data received at " << time.toString();
	conn->send(msg);
}

void CommServer::sendVehicleData(const void* vehicledata)
{
	if(conn_.use_count() > 0)
		conn_->send(vehicledata, sizeof(EZ_VEHICLE));
} 

