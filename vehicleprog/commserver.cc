#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include "commserver.h"
#include "protocol.h"


CommServer::CommServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
	: server_(loop, listenAddr, "CommServer"),
	  kMaxConnections_(1),
	  numForeignConnected_(0),
	  numLocalConnected_(0)
{
	server_.setConnectionCallback(boost::bind(&CommServer::onConnection, this, _1));
	server_.setMessageCallback(boost::bind(&CommServer::onMessage, this, _1, _2, _3));
}

void CommServer::start()
{
	server_.start();
}

void CommServer::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
	LOG_INFO << "CommServer - " << conn->peerAddress().toIpPort() << " -> "
	         << conn->localAddress().toIpPort() << " is "
	         << (conn->connected() ? "UP" : "DOWN");
	
	//当有新连接上来时，断开先前的连接
	if (conn->connected()) 
	{
		muduo::MutexLockGuard lock(mutex_);
		
		//根据连接上来的地址进行相应的处理
		if (conn->peerAddress().toIp() == "127.0.0.1")
		{
			++numLocalConnected_;
			if (numLocalConnected_ > kMaxConnections_)
			{
				localconn_->shutdown();
				localconn_->forceCloseWithDelay(3.0);  // > round trip of the whole Internet.
				localconn_.reset();
			}
			localconn_ = conn;
		}
		else
		{
			++numForeignConnected_;
			if (numForeignConnected_ > kMaxConnections_) 
			{
				foreignconn_->shutdown();
				foreignconn_->forceCloseWithDelay(3.0);  // > round trip of the whole Internet.
				foreignconn_.reset();
			}
			
			foreignconn_ = conn;
		}
	} 
	else 
	{
		if (conn->peerAddress().toIp() == "127.0.0.1")
			--numLocalConnected_;
		else
			--numForeignConnected_;
	}

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
	muduo::MutexLockGuard lock(mutex_);
	
	if(foreignconn_.use_count() > 0)
		foreignconn_->send(vehicledata, sizeof(EZ_VEHICLE));
	if(localconn_.use_count() > 0)
		localconn_->send(vehicledata, sizeof(EZ_VEHICLE));
} 

