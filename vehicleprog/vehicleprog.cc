#include <unistd.h>

#include <muduo/base/Singleton.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "commserver.h"
#include "datahandle.h"
#include "cfg_handle.h"

int main(int argc, char *argv[])
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	LOG_INFO << argv[0] << " run";
	
	//加载配置文件
	muduo::Singleton<CfgHandle>::instance().loadFileInfo(CfgHandle::CFG_PATH);
	
	//启动Tcp服务端为主线程
	muduo::net::EventLoop loop;
	muduo::net::InetAddress listenAddr(8000);
	boost::shared_ptr<CommServer> server(new CommServer(&loop, listenAddr));
	server->start();
	
	boost::shared_ptr<DataHandle> dh(new DataHandle(server));
	
	if (argc == 2) 
	{	
		//开启接收文件数据处理
		dh->recvFiledataComputeHandle(argv[1]);
		exit(0);
	} 
	else 
	{
		//开启接收网络数据计算线程
		muduo::Thread recvNetdataComputeThread(boost::bind(&DataHandle::recvNetdataComputeHandle, get_pointer(dh)), "recvNetdataThread");
		recvNetdataComputeThread.start();
	}
	
	//开启发送车辆信息线程
	muduo::Thread sendVehicleInfoThread(boost::bind(&DataHandle::sendVehicleInfoHandle, get_pointer(dh)), "sendVehicleThread");
	sendVehicleInfoThread.start();

	loop.loop();
	
	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();
}

