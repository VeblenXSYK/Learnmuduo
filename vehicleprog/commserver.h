#ifndef COMMSERVER_H
#define COMMSERVER_H

#include <muduo/net/TcpServer.h>

// RFC 862
class CommServer
{
public:
	CommServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, int maxConnections);

	void start();  // calls server_.start();
	
	void sendVehicleData(const void* vehicledata);

private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);

	void onMessage(const muduo::net::TcpConnectionPtr& conn,
	               muduo::net::Buffer* buf,
	               muduo::Timestamp time);

	const int kMaxConnections_;
	int numConnected_; 							//当前活动连接数
	muduo::net::TcpServer server_;				//服务端对象
	muduo::net::TcpConnectionPtr conn_;			//当前连接的客户端对象
};

#endif
