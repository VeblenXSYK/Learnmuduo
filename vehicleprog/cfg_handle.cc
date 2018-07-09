#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>
#include "cfg_handle.h"

#include <boost/lexical_cast.hpp>
#include <muduo/base/Logging.h>

using namespace libconfig;
using namespace muduo;

std::string CfgHandle::CFG_PATH = "/.vehicle/usrcnf/vehicle.cfg";

int CfgHandle::loadFileInfo(std::string filename)
{
	Config cfg;

	try
	{
		cfg.readFile(filename.c_str());
	} 
	catch(const FileIOException &fioex) 
	{
		LOG_ERROR << "Config::readFile error";
		return(EXIT_FAILURE);
	} 
	catch(ParseException &pex) 
	{
		LOG_ERROR << "Parse error at " << filename << ":" << pex.getLine()
		          << " - " << pex.getError();
		return(EXIT_FAILURE);
	}
	
	const Setting &root = cfg.getRoot();
	
	//获取系统信息
	try
	{
		const Setting &sysinfo = root["sysinfo"];
		sysinfo.lookupValue("saverawdata", saverawdata_);
	}	
	catch(const SettingNotFoundException &nfex)
	{
		// Ignore.
	}
	LOG_INFO << "saverawdata:" << saverawdata_;
	
	//获取Web信息
	try
	{
		const Setting &webserinfo = root["webserinfo"];
		webserinfo.lookupValue("ipaddr", webserip_);
		webserinfo.lookupValue("port", webport_);
	}	
	catch(const SettingNotFoundException &nfex)
	{
		// Ignore.
	}
	LOG_INFO << webserip_ << ":" << webport_;
	
	//获取标定信息
	try
	{
		const Setting &value = root["calibinfo"]["value"];
		int axlecount = value.getLength();							//车轴数
		for(int i = 0; i < axlecount; i++)
		{
			string key = "axle" + boost::lexical_cast<string>(i);
			const Setting &axlex = value[key.c_str()];
			int lanecount = axlex.getLength();						//车道数
			for(int j = 0; j < lanecount; j++)
			{
				calibinfo_[i][j] = axlex[j];
			}
		}
		
	}	
	catch(const SettingNotFoundException &nfex)
	{
		// Ignore.
	}
	LOG_INFO << "calibinfo_[axlenum=2][lane=2]:" << calibinfo_[2][2];
	
	return(EXIT_SUCCESS);
}