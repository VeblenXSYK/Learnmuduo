# -*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import os
import time
import logging
import socket
import select
import struct
import threading
import WWSGC
from Utility import GO_VHUtility
		

class CUnixDomainClient(threading.Thread):

	def __init__(self, test = False):
	
		threading.Thread.__init__(self, name='UnixDomain-Cli')
		self.server_address = '/tmp/.unix_socket'
		self.unixsock = None
		self.__run = False
		self.__test = test
		
	def UnixComm(self):
	
		# Create a unixdomain socket
		self.unixsock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

		# Connect the unixsocket to server
		logging.info('Connecting to: "%s"' % (self.server_address))
		while True:
			try:
				self.unixsock.connect(self.server_address)
				break
			except socket.error, msg:
				logging.error('Connect UnixServer Fail: "%s"' % (msg))
				time.sleep(5)
		logging.info('Connect UnixServer Success')
		
		self.__run = True
		WWSGC.UNIX_SOCKET = self.unixsock

		try:

			inputs = [self.unixsock]
			if self.__test == True:
				inputs.append(sys.stdin)
			
			while self.__run:
				readable, writeable, exceptional = select.select(inputs, [], [])
				for fd in readable:
					if fd == self.unixsock:
						datahead = self.unixsock.recv(WWSGC.DATALEN_HEAD)
						if len(datahead) != 0:
							head, weightA, weightB, message = struct.unpack('iiii', datahead)
							
							if head == ord('@'):      #设备状态信息
								if message == WWSGC.ADRECV_ERROR:
									pass
								elif message == WWSGC.ADRECV_NORMAL:
									pass
								elif message == WWSGC.LIGHT_ERROR:
									logging.info('Light has problem')
									GO_VHUtility.SysStat.SetLightStat(WWSGC.DEVICE_ERROR)
								elif message == WWSGC.LIGHT_NORMAL:
									logging.info('Light has normal')
									GO_VHUtility.SysStat.SetLightStat(WWSGC.DEVICE_NORMAL)
								elif message == WWSGC.AXLEUP_ERROR:
									logging.info('Axleup has problem')
									GO_VHUtility.SysStat.SetAxleupStat(WWSGC.DEVICE_ERROR)
								elif message == WWSGC.AXLEUP_NORMAL:
									logging.info('Axleup has normal')
									GO_VHUtility.SysStat.SetAxleupStat(WWSGC.DEVICE_NORMAL)
								elif message == WWSGC.AXLEDOWN_ERROR:
									logging.info('Axledown has problem')
									GO_VHUtility.SysStat.SetAxledownStat(WWSGC.DEVICE_ERROR)
								elif message == WWSGC.AXLEDOWN_NORMAL:
									logging.info('Axledown has normal')
									GO_VHUtility.SysStat.SetAxledownStat(WWSGC.DEVICE_NORMAL)
									
								devinfo = GO_VHUtility.SysStat.GetDevinfo()
								logging.info('Devinfo: "%s"' % (devinfo))
								#发送设备信息给服务端
								GO_VHUtility.HttpCli.SendDevStatus(devinfo)

							elif head == ord('^'):    #设备合法性信息
								logging.info('DevLegal Message:%d ' % (message))
								if message != GO_VHUtility.SysStat.GetDevLegal():
									GO_VHUtility.SysStat.SetDevLegal(message)
									#修改配置文件
									GO_VHUtility.SysConf.ModSingleConfig("Common", "validity", str(message))
							
							elif head == ord('&'):	  #传感器基准值
								datainfo = self.unixsock.recv(message)
								
								sensorbase = GO_VHUtility.SysStat.GetSensorBase(datainfo)
								logging.info('sensorbase: "%s"' % (sensorbase))
								#发送车辆信息给服务端
								GO_VHUtility.HttpCli.SendSensorBase(sensorbase)
							
							elif head == ord('!'):    #静态标定信息
								GO_VHUtility.SysStat.SetWeightA(weightA)
								GO_VHUtility.SysStat.SetWeightB(weightB)	
								
							elif head == ord('#'):    #动态车辆信息
								datainfo = self.unixsock.recv(message)
								
								GO_VHUtility.SysStat.SetCarInfo(datainfo)
								carinfo = GO_VHUtility.SysStat.GetCarInfo()
								logging.info('Carinfo: "%s"' % (carinfo))
								#发送车辆信息给服务端
								GO_VHUtility.HttpCli.SendWeightData(carinfo)
								
							elif head == ord('$'):	  #传感器状态信息
								if message == WWSGC.ALLSENSOR_NORMAL:
									logging.info("All Sensor has Normal")
									GO_VHUtility.SysStat.SetAllSensorStat()
								else:
									logging.info("Sensor:%d has Problem" %(message + 1))
									GO_VHUtility.SysStat.SetSensorStat(message, WWSGC.DEVICE_ERROR)
								devinfo = GO_VHUtility.SysStat.GetDevinfo()
								logging.info('Devinfo: "%s"' % (devinfo))
								#发送设备信息给服务端
								GO_VHUtility.HttpCli.SendDevStatus(devinfo)
						else:
							logging.info('Remote UnixServer Close')
							self.__run = False
							break
							
					elif fd == sys.stdin:
						#test cmd
						data = sys.stdin.readline().strip('\n').split()
						if len(data):
							WWSGC.SendUnixCmd(int(data[0]), float(data[1]), float(data[2]),\
								int(data[3]), int(data[4]), float(data[5]))
		finally:
			logging.info('Closing Unixsocket')
			self.unixsock.close()
			self.unixsock = None
			WWSGC.UNIX_SOCKET = None			
		
	def Stop(self):
	
		if self.unixsock is not None:
			self.unixsock.close()
			self.unixsock = None
		try:
			if self.is_alive():
				self.join()
		except:
			pass
			
	def run(self):

		while True:
			self.UnixComm()
			time.sleep(1)
		
		
if __name__ == '__main__':

	unixtest = CUnixDomainClient()
	unixtest.setDaemon(True)
	unixtest.start()
	while True:
		if not unixtest.isAlive():
			break
		time.sleep(2)
		
			