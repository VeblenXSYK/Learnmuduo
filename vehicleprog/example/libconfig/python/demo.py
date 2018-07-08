#! /usr/bin/env python
# -*- coding: utf-8 -*-
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')  
import io, os
import logging, traceback
import libconf


class CSysConfig:

	"""系统全部配置信息"""

	def __init__(self):

		self.File_Path = ''
		self.CfgInfo = ''
		self.Devid = ''
		self.DevLegal = 0
		self.WebIp = ''
		self.WebPort = 0
		self.DBHost = ''
		self.DBPort = 0
		self.DBUser = ''
		self.DBPwd = ''
		self.DBName = ''

	def LoadConfig(self, file_path = ''):
	
		"""加载系统配置信息：
		成功返回True，否则False。"""

		self.File_Path = file_path
		
		try:
			f = io.open(file_path, 'r', encoding='utf-8')
		except:
			logging.error(u'解析配置文件异常 : %s' % traceback.format_exc())
			return False
		else:
			with f:
				self.CfgInfo = libconf.load(f)
				self.WebIp = self.CfgInfo['webserinfo']['ipaddr']
				self.WebPort = self.CfgInfo['webserinfo']['port']
				self.DBHost = self.CfgInfo['dbinfo']['host']
				self.DBPort = self.CfgInfo['dbinfo']['port']
				self.DBUser = self.CfgInfo['dbinfo']['user']
				self.DBPwd = self.CfgInfo['dbinfo']['passwd']
				self.DBName = self.CfgInfo['dbinfo']['name']
		
		return True
		
	def ModConfig(self):
	
		"""修改系统配置信息：
		成功返回True，否则False。"""
		
		try:
			f = io.open(self.File_Path, 'w', encoding='utf-8')
		except:
			logging.error(u'解析配置文件异常 : %s' % traceback.format_exc())
			return False
		else:
			with f:
				libconf.dump(self.CfgInfo, f)
		return True
		
	def SetDBUser(self, user = ''):
	
		"""设置数据库用户名"""
		
		self.DBUser = user
		self.CfgInfo['dbinfo']['user'] = user
		
	def SetDBPwd(self, pwd = ''):
	
		"""设置数据库密码"""
		
		self.DBPwd = pwd
		self.CfgInfo['dbinfo']['passwd'] = pwd

		
if __name__ == '__main__':
	
	sysconf = CSysConfig()
	
	#加载配置
	if sysconf.LoadConfig("demo.cfg") == True:
		print sysconf.WebIp
		print sysconf.DBPwd
	
		#修改配置
		sysconf.SetDBUser('veblen')
		sysconf.SetDBPwd('123456')
		sysconf.ModConfig()
		