# -*- coding: utf-8 -*-

import re
import os
import WWSGC
from flask import Flask, render_template
from beaker.middleware import SessionMiddleware
from Utility import GO_VHUtility


#设置session参数
session_opts = {
	'session.type':'file',
	'session.cookei_expires':3600,
	'session.data_dir':'/tmp/sessions',
	'session.auto':True
}

GO_VHWebSrv = Flask(__name__)
WEB_JSON_Suc = u'{"result":"0"}'
WEB_JSON_Err = u'{"result":"1"}'


def RunFlask():

	"""开启Flask服务器"""

	return SessionMiddleware(GO_VHWebSrv, session_opts)
		

@GO_VHWebSrv.route('/')
@GO_VHWebSrv.route('/index.htm')
@GO_VHWebSrv.route('/index.html')
def WEB_HomePage():

	"""返回主页面"""

	return render_template('index.html')
	
	
@GO_VHWebSrv.route('/Control.html')
def WEB_ControlPage():

	"""返回控制页面"""

	return render_template('Control.html')
	
	
@GO_VHWebSrv.route('/Setting.html')
def WEB_SetPage():

	"""返回设置页面"""

	return render_template('Setting.html')
	
	
@GO_VHWebSrv.route('/Test.html')
def WEB_TestPage():

	"""返回测试页面"""

	return render_template('Test.html')
	

@GO_VHWebSrv.route('/CurrentStatus')
def WEB_GetCurrentStatus():

	"""获取当前工作状态"""

	workstat = GO_VHUtility.SysStat.GetWorkStat()
	
	if workstat == WWSGC.WORKSTAT_CHECKA:
		return '{"type":"%d", "weightA":"%d", "weightB":"%d"}' %(workstat, \
				GO_VHUtility.SysStat.GetWeightA(), GO_VHUtility.SysStat.GetWeightB())
	elif workstat == WWSGC.WORKSTAT_CHECKB:
		return '{"type":"%d", "weightA":"%d", "weightB":"%d"}' %(workstat, \
				GO_VHUtility.SysStat.GetWeightA(), GO_VHUtility.SysStat.GetWeightB())
	else:
		Ret = '{"type":"%d", ' %(workstat)
		
		if workstat == WWSGC.WORKSTAT_HASCAR:
			carinfo = GO_VHUtility.SysStat.GetCarInfo()
			
			#基本信息
			Ret += '"carmodel":"%s", "weight":"%d", "modeweight":"%d", "chargeweight":"%d", "speed":"%.2f", "axleinfo":[' \
			%(carinfo["carmodel"], carinfo["weight"], carinfo["modeweight"], carinfo["chargeweight"], carinfo["speed"])
			
			#轴信息
			BeFirst = True
			for axleinfo in carinfo["axleinfo"]:
				if not BeFirst:
					Ret += ',{"aw":"%d", "as":"%.2f"}' %(axleinfo["aw"], axleinfo["as"])
				else:
					BeFirst = False
					Ret += '{"aw":"%d", "as":"%.2f"}' %(axleinfo["aw"], axleinfo["as"])
			
			#传感器信息
			BeFirst = True
			Ret += '],"seninfo":['
			seninfo = GO_VHUtility.SysStat.GetSensorStat()
			for senstat in seninfo:
				if not BeFirst:
					Ret += ',{"s":"%d"}' %(senstat)
				else:
					BeFirst = False
					Ret += '{"s":"%d"}' %(senstat)
			Ret += ']}'	
			#将工作状态改变为动态无车
			GO_VHUtility.SysStat.SetWorkStat(WWSGC.WORKSTAT_NOCAR)
		else:	
			#称台信息
			Ret += '"weightA":"%d", "weightB":"%d",' %(GO_VHUtility.SysStat.GetWeightA(), GO_VHUtility.SysStat.GetWeightB())
			Ret += '"gska":"%f", "gskb":"%f", "light":"%d", ' %(float(GO_VHUtility.SysStat.Getgska()), \
			float(GO_VHUtility.SysStat.Getgskb()), GO_VHUtility.SysStat.GetLightStat())
			Ret += '"seninfo":['
			seninfo = GO_VHUtility.SysStat.GetSensorStat()
			BeFirst = True
			for senstat in seninfo:
				if not BeFirst:
					Ret += ',{"s":"%d"}' %(senstat)
				else:
					BeFirst = False
					Ret += '{"s":"%d"}' %(senstat)
				
			Ret += ']}'
		return Ret
		