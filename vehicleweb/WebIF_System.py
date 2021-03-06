# -*- coding: utf-8 -*-

import os
import time
import WWSGC
import json
from subprocess import Popen
from flask import request
from Utility import GO_VHUtility
from WebIF_Main import GO_VHWebSrv, WEB_JSON_Suc, WEB_JSON_Err


def GetTestDataPath():

	"""根据称台类型获取系统测试数据路径"""
	
	datapath = ""
	if GO_VHUtility.SysStat.GetWeighType() == WWSGC.WEIGHTYPE_TRUCK:
		datapath = WWSGC.FILE_WebTruckData()
	else:
		datapath = WWSGC.FILE_WebAxleData()
		
	return datapath


@GO_VHWebSrv.route('/System/GetSysConfig')
def WR_SYSTEM_GetSysConfig():

	"""获取系统配置信息"""
	
	sysconfiginfo = {}
	sysconfiginfo["devid"] = GO_VHUtility.SysStat.GetDevID()
	sysconfiginfo["weightype"] = GO_VHUtility.SysStat.GetWeighType()
	sysconfiginfo["senum"] = GO_VHUtility.SysStat.GetSenum()
	
	return json.dumps(sysconfiginfo)
	
	
@GO_VHWebSrv.route('/System/SetSysConfig')
def WR_SYSTEM_SetSysConfig():

	"""设置系统配置信息"""
	
	devid = request.query.get('devid', default='')
	weightype = request.query.get('weightype', default=1, type=int)
	senum = request.query.get('senum', default=1, type=int)

	modify_config = 0
	restart_checkprogram = 0
	
	#判断是否修改过
	if devid != GO_VHUtility.SysStat.GetDevID():
		GO_VHUtility.SysStat.SetDevID(devid)
		modify_config = 1
	if weightype != GO_VHUtility.SysStat.GetWeighType():
		GO_VHUtility.SysStat.SetWeighType(weightype)
		restart_checkprogram = 1
		modify_config = 1
	if senum != GO_VHUtility.SysStat.GetSenum():
		GO_VHUtility.SysStat.SetSenum(senum)
		restart_checkprogram = 1
		modify_config = 1
		
	if modify_config == 1:
		sysconfiginfo = {}
		sysconfiginfo["devid"] = devid
		sysconfiginfo["weightype"] = str(weightype)
		sysconfiginfo["senum"] = str(senum)
		
		#修改配置
		GO_VHUtility.SysConf.ModSysConfig(sysconfiginfo)
		
		#判断是否有必要重启后台检测程序
		if restart_checkprogram == 1:
			WWSGC.KillProcByName("TruckScale_check")
			Popen([WWSGC.PROG_CheckSensor()], close_fds = True)		
	
	return WEB_JSON_Suc

	
@GO_VHWebSrv.route('/System/GetLegalInfo')
def WR_SYSTEM_GetLegalInfo():

	"""获取合法性信息"""
	
	if GO_VHUtility.SysStat.GetDevLegal():
		return WEB_JSON_Suc
	else:
		return WEB_JSON_Err
	
	
@GO_VHWebSrv.route('/System/Authentication', methods=['GET', 'POST'])
def WR_SYSTEM_Authentication():

	"""验证系统合法性"""
	
	Data = request.files.get('filename')
	if Data.file:
		RawData = Data.file.read()
		FileWrite = open(WWSGC.FILE_Certificate(), 'w+')
		FileWrite.write(RawData)
		FileWrite.close()
		
		#通知底层去验证
		WWSGC.SendUnixCmd(WWSGC.VERIFY_DEVICE, 0.0, 0.0, 0, 0, 0.0)
		
		#等待验证结果并返回给用户
		timeout = 5
		while timeout > 0:
			if GO_VHUtility.SysStat.GetDevLegal():
				return WEB_JSON_Suc
			timeout = timeout - 1
			time.sleep(1)
		
	return WEB_JSON_Err
	

@GO_VHWebSrv.route('/System/TestStatus')
def WR_SYSTEM_GetTestStatus():	
	
	"""获取系统测试状态"""
	
	retinfo = {}
	retinfo["status"] = GO_VHUtility.SysStat.GetTestStatus()
	
	return json.dumps(retinfo)
	
	
@GO_VHWebSrv.route('/System/StartTest')
def WR_SYSTEM_StartTest():

	"""开始系统测试"""
	
	#先删除存在的数据文件
	datapath = GetTestDataPath()
	if os.path.exists(datapath):
		os.remove(datapath)
	
	WWSGC.SendUnixCmd(WWSGC.START_SYSTEMTEST, 0.0, 0.0, 0, 0, 0.0)
	
	GO_VHUtility.SysStat.StartSysTest()
	
	return WEB_JSON_Suc

	
@GO_VHWebSrv.route('/System/StopTest')
def WR_SYSTEM_StopTest():

	"""结束系统测试"""
	
	WWSGC.SendUnixCmd(WWSGC.STOP_SYSTEMTEST, 0.0, 0.0, 0, 0, 0.0)
	
	GO_VHUtility.SysStat.StopSysTest()
	
	#等待数据准备好
	timeout = 5
	while timeout > 0:
		if os.path.exists(GetTestDataPath()):
			break
		timeout = timeout - 1
		time.sleep(1)
	
	retinfo = {}
	retinfo["weightype"] = GO_VHUtility.SysStat.GetWeighType()
	
	return retinfo
	
	
#if __name__ == '__main__':
	
	#WWSGC.KillProcByName("TruckScale_check")
	#Popen([WWSGC.PROG_CheckSensor()], close_fds = True)
	