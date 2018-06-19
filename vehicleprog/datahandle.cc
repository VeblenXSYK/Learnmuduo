#include <muduo/base/Logging.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "datahandle.h"
#include "common.h"
#include "protocol.h"

void DataHandle::fileRecvHandle(const std::string filename)
{
	FILE * pf = fopen(filename.c_str(), "rb");
	if(!pf)
	{
		LOG_ERROR << "UserLog open file error：" << strerror(errno) << filename;
		return;
	}

	unsigned char rbuf[RECV_TotalLen];
	memset(rbuf, 0, sizeof(rbuf));
	int rlen = fread(rbuf, sizeof(EZ_VEHICLE), 1, pf);
	if (rlen == 1)
	{
		Ez_VehiclePtr ez(new EZ_VEHICLE());
		memcpy(get_pointer(ez), rbuf, sizeof(EZ_VEHICLE));
		{
			muduo::MutexLockGuard lock(mutex_);
			m_queue_vehicle.push_back(ez);
			
			int queuelen = m_queue_vehicle.size();
			LOG_INFO << "startDataComputeHandle: m_queue_vehicle size is " << queuelen;
		}
	}
	
	fclose(pf);
}

void DataHandle::startDataComputeHandle()
{
	while(1)
	{
		fileRecvHandle("GaugeData/154328-787_2");
		sleep(2);
	}
}

void DataHandle::startSendVehicleHandle()
{
	while(1)
	{
		{
			Ez_VehiclePtr ez;
			{
				muduo::MutexLockGuard lock(mutex_);
				if(!m_queue_vehicle.empty())
				{
					ez = m_queue_vehicle.front();
					m_queue_vehicle.pop_front();
				}
			}
			
			if(ez.use_count() > 0)
			{
				LOG_INFO << "startSendVehicleHandle "
					<< "Axlecount:" << static_cast<int>(ez->cAxleCount)
					<< " Weight:" << ez->fWeight
					<< " Speed:" << ez->fV;
			}
			
			commserv_->sendVehicleData(get_pointer(ez));
		}
		
		sleep(4);
	}
}


