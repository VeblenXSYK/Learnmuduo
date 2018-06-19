# -*- coding: utf-8 -*-

"""
全局的常量、函数、类定义
"""

import os
import time
import datetime
import string
import psutil
import urllib2
import struct
import logging
from random import Random

"""通用的目录"""
GAOSU_ROOT = '/.gaosu'
GAOSU_PROG_HOME = '/.gaosu/bin/'
GAOSU_USRCNF_HOME = '/.gaosu/usrcnf/'
GAOSU_OUTPUT_HOME = '/.gaosu/output/'


"""称台类型"""
WEIGHTYPE_TRUCK = 1
WEIGHTYPE_AXLE = 2

"""系统工作状态"""
WORKSTAT_CHECKA = 0		#静态标定A
WORKSTAT_CHECKB = 1		#静态标定B
WORKSTAT_HASCAR = 2		#动态有车
WORKSTAT_NOCAR = 3		#动态无车

"""外设工作状态"""
DEVICE_NORMAL = 0
DEVICE_ERROR = 1
SENSOR_ABORT = 2

"""标定类型(A or B)"""
CHECKTYPEA = 0
CHECKTYPEB = 1

"""Unix域通信信息"""
UNIX_SOCKET = None
DATALEN_HEAD = 16
START_CHECK = 0x10
STOP_CHECKA = 0x11
STOP_CHECKB = 0x12
STRIP_WEIGHT = 0x13
SPEED_MAKEUP = 0x20
RECORD_SENSOR = 0x2A
VERIFY_DEVICE = 0x30
START_SYSTEMTEST = 0x40
STOP_SYSTEMTEST = 0x41

ADRECV_ERROR = 0x30
ADRECV_NORMAL = 0x31
LIGHT_ERROR = 0x41
LIGHT_NORMAL = 0x42
AXLEUP_ERROR = 0x51
AXLEUP_NORMAL = 0x52
AXLEDOWN_ERROR = 0x61
AXLEDOWN_NORMAL = 0x62
ALLSENSOR_NORMAL = 0x70
ONESENSOR_ERROR = 0x71
ONESENSOR_ABORT = 0x72

"""车型信息"""
MODEL2_11 = 0x21
MODEL2_12 = 0x22
MODEL3_15 = 0x31
MODEL4_125 = 0x41
MODEL5_155 = 0x51 
MODEL6_157 = 0x61


def DIR_WebSites():
	return os.path.join(GAOSU_ROOT, 'WebGUI')
	
	
def FILE_WebTruckData():
	return os.path.join(DIR_WebSites(), 'data', 'truckscale.dat')
	
	
def FILE_WebAxleData():
	return os.path.join(DIR_WebSites(), 'data', 'axlegroup.dat')

	
def FILE_OutLogPath():
	return os.path.join(GAOSU_OUTPUT_HOME, 'serverlz.log')
	

def FILE_SysCnf():
	return os.path.join(GAOSU_USRCNF_HOME, 'Config.xml')
	
	
def FILE_TruckSpeedK():
	return os.path.join(GAOSU_USRCNF_HOME, 'TruckSpeedK.xml')
	

def FILE_AxleSpeedK():
	return os.path.join(GAOSU_USRCNF_HOME, 'AxleSpeedK.xml')
	
	
def FILE_MeterSpeedK():
	return os.path.join(GAOSU_USRCNF_HOME, 'MeterSpeedK.xml')
	

def FILE_Certificate():
	return os.path.join(GAOSU_USRCNF_HOME, 'License.xml')
	
	
def PROG_CheckSensor():
	return os.path.join(GAOSU_PROG_HOME, 'TruckScale_check')
	
	
def GetProcPidsByName(name = ''):
	
	"""根据进程名称获取进程ID号，返回整型ID号的list，如果不存在，返回空list
	name:		   进程名称，字符串"""

	if not name:
		return []
	Pids = []
	for Proc in psutil.process_iter():
		if Proc.name() == name:
			Pids.append(Proc.pid)

	return Pids


def KillProcByName(name = '', sleepseconds = 0.1):
	
	"""根据进程名称杀死进程：
	name:		   进程名称，字符串
	sleepseconds:  向进程发送SIGTERM和SIGKILL信号的中间间隔时间，数值"""

	if not name:
		return None
	lst = GetProcPidsByName(name)
	for i in lst:
		KillProcByPID(i, sleepseconds)


def KillProcByPID(pid = 0, sleepseconds = 0.1):
	
	"""根据PID杀死进程：
	pid:		   进程ID整型值
	sleepseconds:  向进程发送SIGTERM和SIGKILL信号的中间间隔时间，数值"""

	if pid <= 0:
		return
	Proc = None
	try:
		Proc = psutil.Process(pid)
		Proc.terminate()
		Proc.wait(sleepseconds)
	except psutil.NoSuchProcess:
		return
	except psutil.TimeoutExpired:
		Proc.kill()
		return


def KillProcByPIDAndName(pid = 0, name = '', sleepseconds = 0.1):
	
	"""杀死PID和进程名同时匹配的进程：
	pid:		   进程ID整型值
	name:		   进程名称，字符串
	sleepseconds:  向进程发送SIGTERM和SIGKILL信号的中间间隔时间，数值
	"""

	if pid <= 0 or not name:
		return None
	try:
		Proc = psutil.Process(pid)
		if Proc.name() != name:
			return None
		Proc.terminate()
		Proc.wait(sleepseconds)
	except psutil.NoSuchProcess:
		return None
	except psutil.TimeoutExpired:
		Proc.kill()
		return None
		
		
def GetCurrenTime():

	"""获取系统当前的时间"""
	
	return datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	

def GetRandomStr(randomlength = 4):

	"""获取指定长度的随机字符串"""

	str = ''

	chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789'

	length = len(chars) - 1

	random = Random()

	for i in range(randomlength):
		str += chars[random.randint(0, length)]

	return str
	

def GetCarModelStr(model):

	"""获取车型字符串"""
	
	modelstr = ""
	if model == MODEL2_11:
		modelstr = "11"
	elif model == MODEL2_12:
		modelstr = "12"
	elif model == MODEL3_15:
		modelstr = "15"
	elif model == MODEL4_125:
		modelstr = "125"
	elif model == MODEL5_155:
		modelstr = "155"
	elif model == MODEL6_157:
		modelstr = "157"
	return modelstr


def TestConnection(httphost = 'http://www.baidu.com'):

	"""测试当前公网联通状态。
	返回：如果可以访问公网地址，返回True，否则Flase"""

	for _ in range(0, 1):
		try:
			urllib2.urlopen(httphost, timeout=1)
			return True
		except urllib2.URLError:
			time.sleep(2)

	return False
	
	
def SendUnixCmd(cmd = '', gska = 0.0, gskb = 0.0, axlecount = 0, speed = 0, key = 0.0):

	"""发送Unix域命令给服务端"""
	
	if UNIX_SOCKET is not None:
		packed = struct.pack('iffiif', cmd, gska, gskb, axlecount, speed, key)
		UNIX_SOCKET.sendall(packed)
	
