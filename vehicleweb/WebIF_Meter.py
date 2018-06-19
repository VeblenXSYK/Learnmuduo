# -*- coding: utf-8 -*-

import os
from bottle import request
import WWSGC
from Utility import GO_VHUtility
from WebIF_Main import GO_VHWebSrv, WEB_JSON_Suc, WEB_JSON_Err


@GO_VHWebSrv.route('/Meter/StartStatic')
def WR_METER_StartStatic():

	"""开始静态标定"""
	
	checktype = request.query.get('weightype', default=0, type=int)
	
	WWSGC.SendUnixCmd(WWSGC.START_CHECK, 0.0, 0.0, 0, 0, 0.0)
	
	if checktype == WWSGC.CHECKTYPEA:
		GO_VHUtility.SysStat.SetWorkStat(WWSGC.WORKSTAT_CHECKA)
	else:
		GO_VHUtility.SysStat.SetWorkStat(WWSGC.WORKSTAT_CHECKB)
		
	return WEB_JSON_Suc
	
	
@GO_VHWebSrv.route('/Meter/StopStatic')
def WR_METER_StopStatic():

	"""结束静态标定"""
	
	checktype = request.query.get('weightype', default=0, type=int)
	realweight = request.query.get('realweight', default=1, type=int)
	weight = request.query.get('weight', default=1, type=int)
	
	if realweight <= 0 or weight <= 0:
		return WEB_JSON_Err
	
	gsk = float(realweight*1.0/weight)
	if checktype == WWSGC.CHECKTYPEA:
		WWSGC.SendUnixCmd(WWSGC.STOP_CHECKA, gsk, 0.0, 0, 0, 0.0)
		GO_VHUtility.SysStat.Setgska(gsk)
		GO_VHUtility.SysConf.ModSingleConfig("Gauge", "gska", str(gsk))
	else:
		WWSGC.SendUnixCmd(WWSGC.STOP_CHECKB, 0.0, gsk, 0, 0, 0.0)
		GO_VHUtility.SysStat.Setgskb(gsk)
		GO_VHUtility.SysConf.ModSingleConfig("Gauge", "gskb", str(gsk))
	GO_VHUtility.SysStat.SetWorkStat(WWSGC.WORKSTAT_NOCAR)
	
	return WEB_JSON_Suc
	
	
@GO_VHWebSrv.route('/Meter/SetZero')
def WR_METER_SetZero():

	"""称台置零操作"""
	
	workstat = GO_VHUtility.SysStat.GetWorkStat()
	if workstat == WWSGC.WORKSTAT_HASCAR or workstat == WWSGC.WORKSTAT_NOCAR:
		return WEB_JSON_Err
	
	WWSGC.SendUnixCmd(WWSGC.STRIP_WEIGHT, 0.0, 0.0, 0, 0, 0.0)
	return WEB_JSON_Suc
	

@GO_VHWebSrv.route('/Meter/GetSpeedKey')
def WR_METER_GetSpeedKey():

	"""获取速度补偿系数"""
	
	axlecount = request.query.get('axlecount', default=2, type=int)
	speedK = GO_VHUtility.SysSpeedK.GetMeterSpeedK(axlecount)
	
	Ret = '{'
	num = 1
	BeFirst = True
	for key in speedK:
		if not BeFirst:
			Ret += ',"%d":"%f"' %(num, key)
		else:
			BeFirst = False
			Ret += '"%d":"%f"' %(num, key)
		num += 1
	Ret += '}'
	
	return Ret
	

@GO_VHWebSrv.route('/Meter/SetSpeedKey')
def WR_METER_SetSpeedKey():

	"""设置速度补偿系数"""
	
	axlecount = request.query.get('axlecount', default=2, type=int)
	speed = request.query.get('speed', default=5, type=int)
	meterkey = request.query.get('key', default=1.0, type=float)
	
	rawkey = GO_VHUtility.SysSpeedK.GetRawSpeedK(axlecount, speed)
	key = rawkey * meterkey
	
	GO_VHUtility.SysSpeedK.SetMeterSpeedK(axlecount, speed, str(meterkey))
	WWSGC.SendUnixCmd(WWSGC.SPEED_MAKEUP, 0.0, 0.0, axlecount, speed, key)	
	
	return WEB_JSON_Suc
	