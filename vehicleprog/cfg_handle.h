#ifndef CFG_HANDLE_H
#define CFG_HANDLE_H

#include <string>
#include "protocol.h"


class CfgHandle
{
public:
	static std::string CFG_PATH;
	int loadFileInfo(std::string filename);
	
private:
	int saverawdata_;									//保存原始数据的标志
	float calibinfo_[MAX_AXLE_NUM+1][MAX_LANE_NUM+1];	//保存标定信息
	int webport_;										//Web服务port
	std::string webserip_;								//Web服务IP
};

#endif