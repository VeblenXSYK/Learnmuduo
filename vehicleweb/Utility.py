# -*- coding: utf-8 -*-

"""
	系统通用组件对象集合。
"""

import logging
import datetime
import WWSGC
import SystemConfig
import SystemSpeedK
import SystemStatus
import HttpClient
import DBHandle

class CVHUtility():


	def __init__(self):
	
		self.SYS_START_TIME = datetime.datetime.now()
		self.SysConf = SystemConfig.CSysConfig()
		if self.SysConf.weightype == WWSGC.WEIGHTYPE_TRUCK:
			self.SysSpeedK = SystemSpeedK.CSysSpeedK(WWSGC.FILE_TruckSpeedK(), WWSGC.FILE_MeterSpeedK())
		else:
			self.SysSpeedK = SystemSpeedK.CSysSpeedK(WWSGC.FILE_AxleSpeedK(), WWSGC.FILE_MeterSpeedK())
		self.DB = DBHandle.CDBHanlde()
		self.SysStat = None
		self.HttpCli = None

	def InitUtility(self):

		"""初始化系统通用组件对象。
		返回：成功返回True，否则False。"""

		if not self.SysConf.LoadConfig(WWSGC.FILE_SysCnf()):
			logging.error('加载配置文件失败！')
			return False
			
		if not self.DB.InitDB('127.0.0.1', 3306, 'root', '123456', 'vehicle'):
			logging.error('初始化数据库失败！')
			return False
			
		#创建系统状态的对象
		self.SysStat = SystemStatus.CSystemStatus(self.SysConf.weightype, self.SysConf.devid, \
												  self.SysConf.devlegal, self.SysConf.senum)
			
		#设置gska、gskb
		self.SysStat.Setgska(self.SysConf.gska)
		self.SysStat.Setgskb(self.SysConf.gskb)
		
		#获取速度补偿系数
		self.SysSpeedK.LoadSpeedK(self.SysSpeedK.path_raw, self.SysSpeedK.rawspeedk)
		self.SysSpeedK.LoadSpeedK(self.SysSpeedK.path_meter, self.SysSpeedK.meterspeedk)
		
		#创建Http客户端对象
		self.HttpCli = HttpClient.CHttpClient(self.SysConf.RemoteIP(), self.SysConf.RemoteDataPort(), self.SysConf.RemoteCheckPort())
	 
		return True		

GO_VHUtility = CVHUtility()
