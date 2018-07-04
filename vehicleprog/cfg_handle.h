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
	int saverawdata_;									//����ԭʼ���ݵı�־
	float calibinfo_[MAX_AXLE_NUM+1][MAX_LANE_NUM+1];	//����궨��Ϣ
	int webport_;										//Web����port
	std::string webserip_;								//Web����IP
};

#endif