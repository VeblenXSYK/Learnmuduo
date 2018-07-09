# -*- coding: utf-8 -*-
import struct
import WWSGC
		

class CSystemStatus():

	def __init__(self, type = 1, devid = '', devlegal = 0, senum = 10, workstat = WWSGC.WORKSTAT_NOCAR):
	
		self.devid = devid              #设备ID
		self.devlegal = devlegal		#设备合法性
		self.weightype = type           #称台类型
		self.gska = 0                   #称台系数A
		self.gskb = 0                   #称台系数B
		self.weightA = 0                #称台A的重量
		self.weightB = 0                #称台B的重量
		self.senum = senum              #传感器数量
		self.lightstat = 0              #0:光幕正常 1:光幕损坏
		self.axleupstat = 0				#0:轴检上正常 1:轴检上损坏
		self.axledownstat = 0			#0:轴检下正常 1:轴检下损坏
		self.seninfo = [0] * senum      #0:传感器正常 1:传感器异常 2:传感器损坏
		self.workstat = workstat        #0:静态标定(A) 1:静态标定(B) 2:动态有车 3:动态无车
		self.systest = 0				#系统测试的标志
		self.devinfo = {}               #系统设备状态信息
		self.carinfo = {}               #最近一次过称的车信息
		
	def SetDevID(self, devid):
	
		"""设置设备id"""
		
		self.devid = devid
		
	def GetDevID(self):
	
		"""获取设备id"""
		
		return self.devid
		
	def SetDevLegal(self, devlegal):
	
		"""设置设备合法性"""
		
		self.devlegal = devlegal
		
	def GetDevLegal(self):
	
		"""获取设备合法性"""
		
		return self.devlegal
		
	def SetWeighType(self, type):
	
		"""设置称台类型"""
		
		self.weightype = type
		
	def GetWeighType(self):
	
		"""获取称台类型"""
		
		return self.weightype
		
	def Setgska(self, gska):
	
		"""设置gska系数"""
		
		self.gska = gska
		
	def Getgska(self):
	
		"""获取gska系数"""
		
		return self.gska
		
	def Setgskb(self, gskb):
	
		"""设置gskb系数"""
		
		self.gskb = gskb
		
	def Getgskb(self):
	
		"""获取gskb系数"""
		
		return self.gskb
		
	def SetSenum(self, num):
	
		"""设置传感器数目"""
		
		self.senum = num
		#重新分配传感器信息数组
		self.seninfo = [0] * num
		
	def GetSenum(self):
	
		"""获取传感器数目"""
		
		return self.senum
		
	def SetWeightA(self, weight):
	
		"""设置称台A的重量"""
		
		self.weightA = weight
		
	def GetWeightA(self):
	
		"""获取称台A的重量"""
		
		return self.weightA
		
	def SetWeightB(self, weight):
	
		"""设置称台B的重量"""
		
		self.weightB = weight
		
	def GetWeightB(self):
	
		"""获取称台B的重量"""
		
		return self.weightB
		
	def SetLightStat(self, stat):
	
		"""设置光幕状态"""
		
		self.lightstat = stat
		
	def GetLightStat(self):
	
		"""获取光幕状态"""
		
		return self.lightstat
		
	def SetAxleupStat(self, stat):
	
		"""设置轴检上状态"""
		
		self.axleupstat = stat
		
	def GetAxleupStat(self):
	
		"""获取轴检上状态"""
		
		return self.axleupstat
		
	def SetAxledownStat(self, stat):
	
		"""设置轴检下状态"""
		
		self.axledownstat = stat
		
	def GetAxledownStat(self):
	
		"""获取轴检下状态"""
		
		return self.axledownstat
		
	def SetAllSensorStat(self):
		
		"""设置所有的传感器正常"""
		
		for index in range(self.senum):
			#除了标记为异常的部分
			if self.seninfo[index] != WWSGC.SENSOR_ABORT:
				self.seninfo[index] = WWSGC.DEVICE_NORMAL
	
	def SetSensorStat(self, index, stat):
	
		"""设置传感器状态"""
		
		self.seninfo[index] = stat
		
	def GetSensorStat(self):
	
		"""获取传感器状态"""
		
		return self.seninfo
		
	def SetWorkStat(self, stat):
	
		"""设置工作状态"""
		
		self.workstat = stat
		
	def GetWorkStat(self):
	
		"""获取工作状态"""
		
		return self.workstat
		
	def GetTestStatus(self):
	
		"""获取系统测试状态"""
		
		return self.systest
		
	def StartSysTest(self):
	
		"""开始系统测试"""
		
		self.systest = 1
		
	def StopSysTest(self):
	
		"""结束系统测试"""
		
		self.systest = 0
		
	def GetDevinfo(self):
	
		"""发送设备状态"""
		
		self.devinfo["weightype"] = self.weightype
		self.devinfo["devid"] = self.devid
		self.devinfo["light"] = self.lightstat
		self.devinfo["axlecheckup"] = self.axleupstat
		self.devinfo["axlecheckdown"] = self.axledownstat
		
		#添加传感器状态
		senlist = [0] * self.senum
		for index in range(self.senum):
			sendict = {}
			sendict["s"] = self.seninfo[index]
			senlist[index] = sendict
		self.devinfo['seninfo'] = senlist

		return self.devinfo
		
	def SetCarInfo(self, datainfo):
	
		"""设置车的信息"""
		
		sendflag, backflag, lightrigflag, carmodel, \
				weight, modweight, chargeweight, chargeaxlenum, \
				speed, axlenum, seninfo, senadd, axlew, axles, timestamp = \
				struct.unpack('4ciiiifi12s24s32s32si', datainfo)
		self.carinfo["carmodel"] = carmodel
		self.carinfo["weightype"] = self.weightype
		self.carinfo["devid"] = self.devid
		self.carinfo["weight"] = weight
		self.carinfo["modeweight"] = modweight
		self.carinfo['chargeweight'] = chargeweight
		self.carinfo['chargeaxlenum'] = chargeaxlenum
		self.carinfo["speed"] = round(speed, 1)
		
		#设置轴信息
		axlelist = [0] * axlenum
		for num in range(axlenum):
			axledict = {}
			axleweight = struct.unpack('i', axlew[num*4:(num+1)*4])
			axlespeed = struct.unpack('f', axles[num*4:(num+1)*4])
			axledict["aw"] = axleweight[0]
			axledict["as"] = round(axlespeed[0], 1)
			axlelist[num] = axledict
		self.carinfo["axleinfo"] = axlelist
		
		#设置传感器
		senlist = [0] * self.senum
		for index in range(self.senum):
			sendict = {}
			sendict["s"] = self.seninfo[index]
			senlist[index] = sendict
		self.carinfo['seninfo'] = senlist
		
		#设置传感器增量信息
		senaddlist = [0] * self.senum
		for index in range(self.senum):
			senaddict = {}
			senaddinfo = struct.unpack('H', senadd[index*2:(index+1)*2])
			senaddict["v"] = senaddinfo[0]
			senaddlist[index] = senaddict
		self.carinfo['sensorvalue'] = senaddlist
		
		#改变工作状态为动态有车
		self.workstat = WWSGC.WORKSTAT_HASCAR
		
	def GetCarInfo(self):
		
		"""获取车的信息"""
		
		return self.carinfo
		
	def GetSensorBase(self, datainfo):
		
		"""获取传感器基准值信息"""
		
		sensorbase = {}
		sensorbase["devid"] = self.devid
		
		list = [0] * self.senum
		for index in range(self.senum):
			sendict = {}
			info = struct.unpack('H', datainfo[index*2:(index+1)*2])
			sendict["v"] = info[0]
			list[index] = sendict
		sensorbase['sensorbase'] = list
		
		return sensorbase
	