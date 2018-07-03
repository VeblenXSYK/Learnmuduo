#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "datahandle.h"
#include "common.h"
#include "protocol.h"
#include "dataanalysis.h"
#include "recvdata_ad.h"

#include <boost/bind.hpp>

#include <muduo/base/Logging.h>
#include <muduo/net/Socket.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>

using namespace alglib;
using namespace muduo;
using namespace muduo::net;

/*
	初始化输入文件
*/
void DataHandle::initInputData(const std::string filename)
{
	FILE *fp = fopen(filename.c_str(), "r");
	if(fp == NULL)
	{
		LOG_ERROR << "fopen input file error：" << strerror(errno) << filename;
		return;
	}
	
	bool first = true;
	int datacount = 0;
	uint16_t downaxlebase = 0, cur[15] = {0};
	double sensor[2048] = {0};
	char buf[128] = {0};
	while(fgets(buf, 128, fp) != NULL)
	{
		sscanf((const char *)buf, "%hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu", \
					(int16_t *)&cur[0],(int16_t *)&cur[1],(int16_t *)&cur[2],(int16_t *)&cur[3],\
					(int16_t *)&cur[4],(int16_t *)&cur[5],(int16_t *)&cur[6],(int16_t *)&cur[7],\
					(int16_t *)&cur[8],(int16_t *)&cur[9],(int16_t *)&cur[10],(int16_t *)&cur[11],\
					(int16_t *)&cur[12],(int16_t *)&cur[13],(int16_t *)&cur[14]);

		if(first)
		{
			downaxlebase = cur[14];
			first = false;
		}
		else
		{
			int addvalue = cur[14] - downaxlebase;
			if(addvalue < 0)
				addvalue = 0;
			sensor[datacount++] = addvalue;
		}	
	}
	
	fclose(fp);
	
	//根据文件中的数据设置origindatainfo_
	SensorInfo *si = getSensorInfo(0, 0);
	memcpy(&si->sensordata, sensor, sizeof(sensor));
	si->datacount = datacount;
	
	LOG_INFO << "datacount: " << datacount;
}

/*
	SSA平滑滤波
*/
int DataHandle::ssaSmooth1(double *origindata, double *smoothdata, int size)
{	
	real_1d_array input;
	input.setcontent(size, origindata);

    //
    // First, we create SSA model, set its properties and add dataset.
    //
    // We use window with width=3 and configure model to use direct SSA
    // algorithm - one which runs exact O(N*W^2) analysis - to extract
    // one top singular vector. Well, it is toy problem :)
    //
    // NOTE: SSA model may store and analyze more than one sequence
    //       (say, different sequences may correspond to data collected
    //       from different devices)
    //
	ssamodel s;
    ssacreate(s);
    ssasetwindow(s, 10);
    ssaaddsequence(s, input);
    ssasetalgotopkdirect(s, 1);

    //
    // Now we begin analysis. Internally SSA model stores everything it needs:
    // data, settings, solvers and so on. Right after first call to analysis-
    // related function it will analyze dataset, build basis and perform analysis.
    //
    // Subsequent calls to analysis functions will reuse previously computed
    // basis, unless you invalidate it by changing model settings (or dataset).
    //
    real_1d_array trend;
    real_1d_array noise;
    ssaanalyzesequence(s, input, trend, noise);
	
	//填充平滑后的数据
	double *trendptr = trend.getcontent();
	printf("[");
	for(int i = 0; i < size; i++)
	{
		smoothdata[i] = trendptr[i];
		printf("%.2lf ", smoothdata[i]);
	}
	printf("]\n");
    //printf("%s\n", trend.tostring(2).c_str());
	
    return 0;
}

int DataHandle::ssaSmooth2(double *addr, int size)
{	
	real_1d_array input;
	input.setcontent(size, addr);

    //
    // First, we create SSA model, set its properties and add dataset.
    //
    // We use window with width=3 and configure model to use direct SSA
    // algorithm - one which runs exact O(N*W^2) analysis - to extract
    // one top singular vector. Well, it is toy problem :)
    //
    // NOTE: SSA model may store and analyze more than one sequence
    //       (say, different sequences may correspond to data collected
    //       from different devices)
    //
	ssamodel s;
    ssacreate(s);
    ssasetwindow(s, 10);
    ssaaddsequence(s, input);
    ssasetalgotopkdirect(s, 1);

    //
    // Now we begin analysis. Internally SSA model stores everything it needs:
    // data, settings, solvers and so on. Right after first call to analysis-
    // related function it will analyze dataset, build basis and perform analysis.
    //
    // Subsequent calls to analysis functions will reuse previously computed
    // basis, unless you invalidate it by changing model settings (or dataset).
    //
    real_1d_array trend;
    ssaforecastlast(s, size, trend);
	
    printf("%s\n", trend.tostring(2).c_str());
	
    return 0;
}

/*
	根据阈值查找开始点与结束点
*/
int DataHandle::findAereStartEnd(int threshold)
{	
	int i, hasstart = 0, hasend = 0;
	
	SensorInfo *si = getSensorInfo(0, 0);
	
	int axlenum = 0;
	for(i = 0; i < si->datacount; i++)
	{
		if(hasstart == 0 && si->sensordata[i] > threshold)
		{
			hasstart = 1;
			si->startendindex[axlenum][0] = i;
		}
		if(hasstart == 1 && si->sensordata[i] < threshold - 5)
		{
			hasstart = 0;
			si->startendindex[axlenum++][1] = i;
		}
	}
	//波峰数即为轴数
	si->peaknum = axlenum;				
	
	//show
	for(i = 0; i < si->peaknum; i++)
	{
		LOG_INFO << "axle" << i << " startindex:" << si->startendindex[i][0] << " endindex:" << si->startendindex[i][1];
	}
	
	return 0;
}

/*
	求波形面积
*/
int DataHandle::sumArea()
{
	int i, j;
	
	SensorInfo *si = getSensorInfo(0, 0);
	for(i = 0; i < si->peaknum; i++)
	{
		int size = si->startendindex[i][1] - si->startendindex[i][0];
		for(j = 0; j < size; j++)
		{
			si->sumarea[i] += si->smoothdata[i][j];
		}
	}
	
	//show
	for(i = 0; i < si->peaknum; i++)
	{
		LOG_INFO << "axle" << i << " sumArea:" << si->sumarea[i];
	}
	
	return 0;
}

void DataHandle::recvFiledataComputeHandle(const std::string filename)
{
	initInputData(filename);
	
	findAereStartEnd(100);
	
	SensorInfo *si = getSensorInfo(0, 0);
	for(int i = 0; i < si->peaknum; i++)
	{
		int start = si->startendindex[i][0];
		int end = si->startendindex[i][1];
		ssaSmooth1(&(si->sensordata[start]), si->smoothdata[i], end - start);
		//ssaSmooth2(&(si->sensordata[start]), end - start);
	}
	
	sumArea();
}

/*
	有数据时则回调
*/
void DataHandle::netDataReadCallback(int sockfd, muduo::Timestamp receiveTime)
{
	size_t frameLen = 1385;
	unsigned char message[frameLen];
	
	ssize_t nr = sockets::read(sockfd, message, sizeof message);
	if (nr < 0) 
	{
		LOG_SYSERR << "::read";
	} 
	else if (implicit_cast<size_t>(nr) == frameLen) 
	{
		LOG_INFO << "Recv valid data";
	} 
	else 
	{
		LOG_ERROR << "Expect " << frameLen << " bytes, received " << nr << " bytes.";
	}
}

/*
	接收AD板网络数据并处理的线程
*/
void DataHandle::recvNetdataComputeHandle()
{
	EventLoop loop;
	InetAddress serverAddr("168.1.5.95", 7);
	
	RecvAD recvad(&loop, serverAddr, this);
	recvad.start();

	loop.loop();
}

/*
	发送车辆信息给上位机的线程
*/
void DataHandle::sendVehicleInfoHandle()
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
				LOG_INFO << "sendVehicleInfoHandle "
					<< "Axlecount:" << static_cast<int>(ez->cAxleCount)
					<< " Weight:" << ez->fWeight
					<< " Speed:" << ez->fV;
			}
			
			commserv_->sendVehicleData(get_pointer(ez));
		}
		
		sleep(4);
	}
}
