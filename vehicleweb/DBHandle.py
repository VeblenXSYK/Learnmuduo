# -*- coding: utf-8 -*-
"""
	系统数据库处理，以成员方式提供不同表的处理接口。
"""

import DBConnect
from DBVehicleData import CDBVehicleData

class CDBHanlde:

	def __init__(self):

		"""构造函数"""

		self.__DbConnect = None
		self.VehicleData = None

	def InitDB(self, dbhost = '127.0.0.1', dbport = 3306, dbuser = 'root', dbpwd = '', dbname = ''):

		"""启动数据库操作对象。
		返回：成功True，否则False"""

		self.__DbConnect = DBConnect.CConnector(dbhost, dbport, dbuser, dbpwd, dbname)
		self.__DbConnect.setDaemon(True)
		if not self.__DbConnect.Start():
			return False
		self.VehicleData = CDBVehicleData(self.__DbConnect)
		return True

	def StopDB(self):

		"""停止数据库操作对象。"""

		if self.__DbConnect is not None:
			self.__DbConnect.Stop()

if __name__ == '__main__':

	DB = CDBHanlde()
	if not DB.InitDB('127.0.0.1', 3306, 'root', '123456', 'vehicle'):
		print u'初始化数据库失败！'
	
	DB.VehicleData.AddVehicleInfo('A1B1', 25, 33000, 6, '2018-07-09 14:34:33')	
	# for i in range(10):
		# DB.VehicleData.AddVehicleInfo('A1B1', 25, 33000, 6, '2018-07-09 14:34:33')
