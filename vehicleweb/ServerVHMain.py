# -*- coding: utf-8 -*-

"""
定义系统主功能类。
"""

import signal
import os
import logging
import sys
import cherrypy
import WebIF_Main
import WebIF_System
from wsgigzip import GzipMiddleware
from Utility import GO_VHUtility
from TcpClient import CTcpClient
from RecordHandler import CRecordHandler


class CServerVHMain:

	def __init__(self, test = False):

		"""构造函数"""

		self.__Inited = False
		self.__HasRcvSigTerm = False
		self.__TcpClient = CTcpClient()
		self.__RecordHandler = None

	def __RunWebServer(self, host = u'0.0.0.0', port = 80):

		"""启动web服务器"""

		app_gz = GzipMiddleware(WebIF_Main.RunFlask())
		cherrypy.tree.graft(app_gz, '/')
		cherrypy.config.update({'engine.autoreload.on': False,
		 'log.screen': True,
		 'server.socket_port': port,
		 'server.socket_host': host.encode('ascii')})
		cherrypy.engine.start()
		cherrypy.engine.block()

	def __sigterm_handler(self, signum, frame):

		"""信号处理函数"""

		if signum != signal.SIGTERM or self.__HasRcvSigTerm or not self.__Inited:
			return
		self.__HasRcvSigTerm = True
		self.Stop()

	def __killProg(self):

		"""初始化过程出错，退出执行"""

		pid = os.getpid()
		logging.critical('初始化出错，即将退出程序！PID: %d!' % pid)
		self.Stop()

	def Run(self):
	
		"""启动系统功能"""

		signal.signal(signal.SIGTERM, self.__sigterm_handler)
		if not GO_VHUtility.InitUtility():
			return self.__killProg()
		
		#开启Unix域客户端线程，用于与底层C++通信
		self.__TcpClient.setDaemon(True)
		self.__TcpClient.start()
		
		#开启RecordHandler线程，用于处理传感器记录
		self.__RecordHandler = CRecordHandler()
		self.__RecordHandler.setDaemon(True)
		self.__RecordHandler.start()
		
		#运行Web服务
		self.__Inited = True
		self.__RunWebServer(GO_VHUtility.SysConf.WebIP(), GO_VHUtility.SysConf.WebPort())

	def Stop(self):

		"""停止系统功能"""

		cherrypy.engine.stop()
		self.__TcpClient.Stop()
		if self.__RecordHandler is not None:
			self.__RecordHandler.Stop()
		sys.exit(0)
