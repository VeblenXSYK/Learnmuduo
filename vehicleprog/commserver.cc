#include <muduo/base/Logging.h>
#include <boost/bind.hpp>
#include "commserver.h"
#include "protocol.h"


CommServer::CommServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
	: server_(loop, listenAddr, "CommServer"),
	  kMaxConnections_(1),
	  numForeignConnected_(0),
	  numLocalConnected_(0),
	  dispatcher_(boost::bind(&CommServer::onUnknownMessage, this, _1, _2, _3)),
	  codec_(boost::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
{
	//注册消息回调函数
    dispatcher_.registerMessageCallback<commweb::CmdInfo>(boost::bind(&CommServer::onCmdInfo, this, _1, _2, _3));
	
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

void CommServer::onUnknownMessage(const muduo::net::TcpConnectionPtr& conn,
							const MessagePtr& message,
							muduo::Timestamp time)
{
	LOG_INFO << message->GetTypeName();
    conn->shutdown();
}

void CommServer::onCmdInfo(const muduo::net::TcpConnectionPtr& conn,
							const CmdInfoPtr& message,
							muduo::Timestamp)
{
	//LOG_INFO << message->GetTypeName() << message->DebugString();
	
	//序列化proto
	commweb::VehicleInfo vi;
	vi.set_caxlecount(2);
	vi.set_ntype(12);
	vi.set_fw(3600);
	vi.set_fv(3.6);
	for(int i = 0; i < 2; i++)
	{
		commweb::AxleInfo *ai = vi.add_ainfo();
		ai->set_fw(1800);
		ai->set_fv(3.6);
	}

	//发送给客户端
	codec_.send(conn, vi);
}

void CommServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
							muduo::net::Buffer* buf,
							muduo::Timestamp time)
{	
	if (get_pointer(conn) == get_pointer(localconn_))
		//本地连接
		codec_.onMessage(conn, buf, time);
	else
		//外部连接
		;
}

void CommServer::sendVehicleData(const void* vehicledata)
{
	muduo::MutexLockGuard lock(mutex_);
	
	//发送给外部连接(即中心客户端)
	if(foreignconn_.use_count() > 0)
		foreignconn_->send(vehicledata, sizeof(EZ_VEHICLE));
	
	//发送给本地连接(即WebServer)
	if(localconn_.use_count() > 0)
	{
		localconn_->send(vehicledata, sizeof(EZ_VEHICLE));
	}
} 

