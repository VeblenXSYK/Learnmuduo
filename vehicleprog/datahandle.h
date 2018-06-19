#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <string>
#include <deque>
#include <muduo/base/Mutex.h>
#include <boost/shared_ptr.hpp>

#include "commserver.h"
#include "protocol.h"

class DataHandle
{
public:
	DataHandle(boost::shared_ptr<CommServer> &commserv) : commserv_(commserv)
	{	
	}

	void fileRecvHandle(const std::string filename);
	
	void startDataComputeHandle();
	
	void startSendVehicleHandle();
	
private:
	typedef boost::shared_ptr<EZ_VEHICLE> Ez_VehiclePtr;
	
	boost::shared_ptr<CommServer> commserv_;
	std::deque<Ez_VehiclePtr> m_queue_vehicle;			//通过石英传感器后的车信息
	muduo::MutexLock mutex_;
};

#endif
