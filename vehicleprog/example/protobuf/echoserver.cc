#include <unistd.h>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>

#include "commweb.pb.h"

// RFC 862
class EchoServer
{
public:
	EchoServer(muduo::net::EventLoop* loop,
	           const muduo::net::InetAddress& listenAddr);

	void start();  // calls server_.start();

private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);

	void onMessage(const muduo::net::TcpConnectionPtr& conn,
	               muduo::net::Buffer* buf,
	               muduo::Timestamp time);
				   
	void fillVehicle(commweb::VehicleInfo &vh);
				   
	muduo::net::TcpServer server_;
};

EchoServer::EchoServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
	: server_(loop, listenAddr, "EchoServer")
{
	server_.setConnectionCallback(
	    boost::bind(&EchoServer::onConnection, this, _1));
	server_.setMessageCallback(
	    boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start()
{
	server_.start();
}

void EchoServer::fillVehicle(commweb::VehicleInfo &vh)
{
	vh.set_caxlecount(2);
	vh.set_ntype(12);
	vh.set_fw(3600);
	vh.set_fv(3.6);
	for(int i = 0; i < 2; i++)
	{
		commweb::AxleInfo *ai = vh.add_ainfo();
		ai->set_fw(1800);
		ai->set_fv(3.6);
	}
}

void EchoServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
	         << conn->localAddress().toIpPort() << " is "
	         << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time)
{
	muduo::string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
	         << "data received at " << time.toString();
	
	//对接收到的数据进行反序列化
	commweb::CmdInfo ci;
	ci.ParseFromString(msg.c_str());
	
	if (ci.type() == '@')
	{
		//使用protobuf序列化要发送的数据
		commweb::VehicleInfo vi;
		fillVehicle(vi);
		
		//序列化到string中
		std::string sendData;
		vi.SerializeToString(&sendData);
		
		conn->send(sendData);
	}
	else
	{
		conn->send(msg);
	}
}


int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	LOG_INFO << "pid = " << getpid();
	muduo::net::EventLoop loop;
	muduo::net::InetAddress listenAddr(8888);
	EchoServer server(&loop, listenAddr);
	server.start();
	loop.loop();
	
	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();
}

