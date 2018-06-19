# -*- coding: utf-8 -*-
import sys
import os
import time
import logging
import threading
import WWSGC
from Utility import GO_VHUtility
		

class CRecordHandler(threading.Thread):

	def __init__(self):
	
		threading.Thread.__init__(self, name = 'thread_record')
		
	def GetRecordPath(self):
	
		if WWSGC.GAOSU_OUTPUT_HOME != '':
			os.chdir(WWSGC.GAOSU_OUTPUT_HOME)
		
		lst = os.listdir(os.getcwd())
		for filename in lst:
			if os.path.isfile(filename) and filename.endswith('.dat'): 	 
				return os.path.join(WWSGC.GAOSU_OUTPUT_HOME, filename)
		return None
		
	def Stop(self):
	
		try:
			if self.is_alive():
				self.join()
		except:
			pass
			
	def run(self):
	
		while True:
		
			time.sleep(8)
			
			#获取设备ID
			devid = GO_VHUtility.SysStat.GetDevID()
			
			checkinfo = {}
			checkinfo["DevID"] = devid
		
			#检测是否需要记录传感器信息
			retinfo = GO_VHUtility.HttpCli.CheckRecordCmd(checkinfo)
			if retinfo is not None:
				if int(retinfo.get("result", 0)) == 1:
					#开启传感器信息记录功能
					logging.info('Start Record Sensordata of Weighting Lately')
					WWSGC.SendUnixCmd(WWSGC.RECORD_SENSOR, 0.0, 0.0, 0, 0, 0.0)
			
			#检测输出目录中是否存在*.dat文件，有则POST给服务端
			recordpath = self.GetRecordPath()
			if recordpath is not None:
				logging.info('Exist Record Sensordata:%s' % (recordpath))
				retinfo = GO_VHUtility.HttpCli.SendSensorFile(recordpath, devid)
				if retinfo is not None:
					if int(retinfo.get("result", 0)) == 0:
						logging.info('Upload Sensordata Success')
						os.remove(recordpath)
			