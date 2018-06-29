#include <unistd.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "commserver.h"
#include "datahandle.h"

int main(int argc, char *argv[])
{
	
	//启动服务端线程
	muduo::net::EventLoop loop;
	muduo::net::InetAddress listenAddr(8000);
	boost::shared_ptr<CommServer> server(new CommServer(&loop, listenAddr, 1));
	server->start();
	
	boost::shared_ptr<DataHandle> dh(new DataHandle(server));
	
	if (argc == 2) 
	{	
		//开启文件数据处理
		dh->startFileDataComputeHandle(argv[1]);
		exit(0);
	} 
	else 
	{
		//开启网络数据处理线程
		muduo::Thread dataHandleThread(boost::bind(&DataHandle::startNetDataComputeHandle, get_pointer(dh)));
		dataHandleThread.start();
	}
	
	//开启数据发送线程
	muduo::Thread sendHandleThread(boost::bind(&DataHandle::startSendVehicleHandle, get_pointer(dh)));
	sendHandleThread.start();

	loop.loop();
}

