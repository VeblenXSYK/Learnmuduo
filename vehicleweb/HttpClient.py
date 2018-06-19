#-*- coding: utf-8 -*-
import json
import logging
import urllib
import urllib2  #urllib2不支持长链接
from poster.encode import multipart_encode, MultipartParam
from poster.streaminghttp import register_openers


class CHttpClient:

	def __init__(self, ipaddr = '', dataport = 0, checkport = 0):

		#生成与服务端通信的URL
		self.GenerateURL(ipaddr, dataport, checkport)
		
		#在urllib2上注册http流处理句柄
		register_openers()
		
	def GenerateURL(self, ipaddr = '', dataport = 0, checkport = 0):
	
		self.headers = {'Content-Type': 'application/json'}
		self.dataddr = 'http://' + ipaddr + ':' + str(dataport)
		self.checkaddr = 'http://' + ipaddr + ':' + str(checkport)
		
		#向服务端POST数据的URL
		self.sensorbaseurl = self.dataddr + '/sensorbase'
		self.weighturl = self.dataddr + '/weightdata'
		self.devstaturl = self.dataddr + '/devstatus'
		self.sensorurl = self.dataddr + '/sensorfile'
		self.checkcmdurl = self.checkaddr + '/gaosuWeb/doPaintRequestAccess'
		
	def HandleRequest(self, request = '', url = '', time = 2):
	
		"""处理请求"""
	
		try:
			response = urllib2.urlopen(request, timeout = time)
			result = json.loads(response.read())
			response.close()
			return result
		except Exception,e:
			logging.error('Request "%s" Error: "%s"' % (url, str(e)))
			return None
			
	def SendSensorBase(self, data = ''): 
	
		"""发送传感器基准值给远程服务端"""
	
		request = urllib2.Request(self.sensorbaseurl, headers=self.headers, data=json.dumps(data))
		self.HandleRequest(request, self.sensorbaseurl)
		
	def SendWeightData(self, data = ''): 
	
		"""发送过称数据给远程服务端"""
	
		request = urllib2.Request(self.weighturl, headers=self.headers, data=json.dumps(data))
		self.HandleRequest(request, self.weighturl)
		
	def SendDevStatus(self, data = ''):
	
		"""发送设备状态信息给远程服务端"""
		
		request = urllib2.Request(self.devstaturl, headers=self.headers, data=json.dumps(data))
		self.HandleRequest(request, self.devstaturl)
		
	def CheckRecordCmd(self, data = ''):
	
		"""检测服务端是否下达记录文件的命令"""
		
		headers = { 'User-Agent' : 'user_agent' }
		checkdata = urllib.urlencode(data)
		request = urllib2.Request(self.checkcmdurl, checkdata, headers)
		return self.HandleRequest(request, self.checkcmdurl)
		
	def SendSensorFile(self, filename = '', devid = ''):
	
		"""
		开始对*.dat进行multiart/form-data编码
		headers：包含必须的 Content-Type 和 Content-Length
		datagen：是一个生成器对象，返回编码过后的参数
		"""
		datagen, headers = multipart_encode({"sensorfile": open(filename, "rb")})
		
		#创建请求对象
		request = urllib2.Request(self.sensorurl, headers=headers, data=datagen)
		#添加键值对
		request.add_header("devid", devid)
		
		#实际执行请求并取得返回
		return self.HandleRequest(request, self.sensorurl, 5)
            
#if __name__ == '__main__':

	#httpclient = CHttpClient('192.168.4.109', dataport = 6060, checkport = 8080)
	#httpclient.SendSensorFile("20170706080208.dat", "YM_guangxi_raocheng_nanningdong_01")