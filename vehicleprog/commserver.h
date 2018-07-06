#ifndef COMMSERVER_H
#define COMMSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/base/Mutex.h>

// RFC 862
class CommServer
{
public:
	CommServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr);

	void start();  // calls server_.start();
	
	void sendVehicleData(const void* vehicledata);

private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);

	void onMessage(const muduo::net::TcpConnectionPtr& conn,
	               muduo::net::Buffer* buf,
	               muduo::Timestamp time);

	const int kMaxConnections_;					//最大连接数(区分外部与本地)
	int numForeignConnected_; 					//当前外部活动连接数
	int numLocalConnected_; 					//当前本地活动连接数
	muduo::net::TcpServer server_;				//服务端对象
	muduo::net::TcpConnectionPtr foreignconn_;	//当前连接的外部客户端对象
	muduo::net::TcpConnectionPtr localconn_;	//当前连接的本地客户端对象
	muduo::MutexLock mutex_;					//保护foreignconn_与localconn_
};

#endif
