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

	void initInputData(const std::string filename);
	int findAereStartEnd(int threshold);
	int ssaSmooth1(double *origindata, double *smoothdata, int size);
	int ssaSmooth2(double *addr, int size);
	int sumArea();
	
	void startFileDataComputeHandle(const std::string filename);
	
	void startNetDataComputeHandle();
	
	void startSendVehicleHandle();
	
	SensorInfo *getSensorInfo(int carlane, int senum)
	{
		return static_cast<SensorInfo *>(&origindatainfo_.sensorinfo[carlane][senum]);
	}
	
private:
	typedef boost::shared_ptr<EZ_VEHICLE> Ez_VehiclePtr;
	
	OriginDataInfo origindatainfo_;
	boost::shared_ptr<CommServer> commserv_;
	std::deque<Ez_VehiclePtr> m_queue_vehicle;			//通过石英传感器后的车信息
	muduo::MutexLock mutex_;
};

#endif
