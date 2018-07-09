# -*- coding: utf-8 -*-

"""
	系统通用组件对象集合。
"""

import logging
import datetime
import WWSGC
import SystemConfig
import SystemStatus
import DBHandle

class CVHUtility():


	def __init__(self):
	
		self.SYS_START_TIME = datetime.datetime.now()
		self.SysConf = SystemConfig.CSysConfig()
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
	 
		return True		

GO_VHUtility = CVHUtility()
