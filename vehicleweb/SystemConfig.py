# -*- coding: utf-8 -*-
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')  
from xml.dom import minidom
import os
import logging
import traceback


class CSysConfig:

	"""系统全部配置信息"""

	def __init__(self):

		self.file_path = ''
		self.weightype = ''
		self.devid = ''
		self.devlegal = 0
		self.gska = 0
		self.gskb = 0
		self.senum = 0
		self.webip = ''
		self.webport = 0
		self.remoteip = ''
		self.remotedataport = 0
		self.remotecheckport = 0
		
	def WebIP(self):
	
		return self.webip
		
	def WebPort(self):
	
		return self.webport
		
	def SetRemoteIP(self, ip):
	
		self.remoteip = ip
		
	def RemoteIP(self):
	
		return self.remoteip
		
	def SetRemoteDataPort(self, port):
	
		self.remotedataport = port
		
	def RemoteDataPort(self):
	
		return self.remotedataport

	def SetRemoteCheckPort(self, port):
	
		self.remotecheckport = port
	
	def RemoteCheckPort(self):
	
		return self.remotecheckport

	def LoadConfig(self, file_path = ''):

		"""加载配置文件。
		返回：成功返回True，否则False。"""

		self.file_path = file_path
		
		try:
			XmlFile = open(file_path, 'rb')
			XmlData = XmlFile.read()
			XmlFile.close()
			
			Doc = minidom.parseString(XmlData)
			RootElem = Doc.getElementsByTagName('CHECKSENSOR')[0]
			
			CommElem = RootElem.getElementsByTagName('Common')[0]
			self.devid = CommElem.getAttribute('devid')
			self.weightype = int(CommElem.getAttribute('type'))
			self.devlegal = int(CommElem.getAttribute('validity'))
			
			GaugeElem = RootElem.getElementsByTagName('Gauge')[0]
			self.senum = int(GaugeElem.getAttribute('senum'))
			self.gska = float(GaugeElem.getAttribute('gska'))
			self.gskb = float(GaugeElem.getAttribute('gskb'))
			
			WebsrvElem = RootElem.getElementsByTagName('WebServer')[0]
			self.webip = WebsrvElem.getAttribute('ip')
			self.webport = int(WebsrvElem.getAttribute('port'))
			
			RemotesrvElem = RootElem.getElementsByTagName('RemoteServer')[0]
			self.remoteip = RemotesrvElem.getAttribute('ip')
			self.remotedataport = int(RemotesrvElem.getAttribute('dataport'))
			self.remotecheckport = int(RemotesrvElem.getAttribute('checkport'))
		except:
			logging.error('解析配置文件异常 : %s' % traceback.format_exc())
			return False

		return True
		
	def ModSingleConfig(self, node= '', attr = '', value = ''):
		
		"""修改配置文件中某个属性值。
		返回：成功返回True，否则False。"""

		try:
			XmlFile = open(self.file_path, 'rb')
			XmlData = XmlFile.read()
			XmlFile.close()
			Doc = minidom.parseString(XmlData)
			RootElem = Doc.getElementsByTagName('CHECKSENSOR')[0]
			
			for NodeElem in RootElem.childNodes:
				if NodeElem.nodeName == node:
					NodeElem.setAttribute(attr, value)
					break
			
			#写入xml
			fp = open(self.file_path, 'w')
			Doc.writexml(fp, addindent = '' , newl = '' ,encoding = 'utf-8')
			fp.close()
		except:
			logging.error('解析配置文件异常 : %s' % traceback.format_exc())
			return False

		return True
		
	def ModSysConfig(self, sysconfiginfo):
	
		"""修改系统配置信息返回：
		成功返回True，否则False。"""
		
		try:
			XmlFile = open(self.file_path, 'rb')
			XmlData = XmlFile.read()
			XmlFile.close()
			Doc = minidom.parseString(XmlData)
			RootElem = Doc.getElementsByTagName('CHECKSENSOR')[0]
			
			CommElem = RootElem.getElementsByTagName('Common')[0]
			CommElem.setAttribute('devid', sysconfiginfo["devid"])
			CommElem.setAttribute('type', sysconfiginfo["weightype"])
			
			GaugeElem = RootElem.getElementsByTagName('Gauge')[0]
			GaugeElem.setAttribute('senum', sysconfiginfo["senum"])
			
			RemotesrvElem = RootElem.getElementsByTagName('RemoteServer')[0]
			RemotesrvElem.setAttribute('ip', sysconfiginfo["remoteip"])
			RemotesrvElem.setAttribute('dataport', sysconfiginfo["jsonport"])
			RemotesrvElem.setAttribute('checkport', sysconfiginfo["recordport"])
			
			#写入xml
			fp = open(self.file_path, 'w')
			Doc.writexml(fp, addindent = '' , newl = '' ,encoding = 'utf-8')
			fp.close()
			
		except:
			logging.error('解析配置文件异常 : %s' % traceback.format_exc())
			return False

		return True
		
# if __name__ == '__main__':
	
	# sysconf = CSysConfig()
	# sysconf.LoadConfig("/.gaosu/usrcnf/Config.xml")
	# sysconf.ModSingleConfig("Common", "type", "2")
	
	# sysconf = CSysConfig()
	# sysconf.LoadConfig("/.gaosu/usrcnf/Config.xml")
	# sysconfiginfo = {}
	# sysconfiginfo["devid"] = "guangxi-nanlinggaosu-nanlingzhan-1"
	# sysconfiginfo["weightype"] = '1'
	# sysconfiginfo["senum"] = '12'
	# sysconfiginfo["remoteip"] = '45.75.149.90'
	# sysconfiginfo["jsonport"] = '6060'
	# sysconfiginfo["recordport"] = '8080'
	
	# sysconf.ModSysConfig(sysconfiginfo)
		