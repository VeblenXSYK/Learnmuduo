# -*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import os, time
import socket, select
import logging
import threading
import WWSGC
		

class CTcpClient(threading.Thread):

	def __init__(self):
	
		threading.Thread.__init__(self, name='TcpClient')
		self.__seraddr = ('127.0.0.1', 8000)
		self.__clientsock = None
		self.__delaytime = 2
		self.__run = False
		
	def Connect(self):
	
		self.__clientsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		logging.info('Connecting to: "%s"' % (str(self.__seraddr)))
		try:
			self.__clientsock.connect(self.__seraddr)
			logging.info('Connect TcpServer Success')
			return True
		except socket.error, msg:
			logging.error('Connect TcpServer Fail: "%s"' % (msg))
			return False
			
	def Retry(self):

		# 关闭socket
		self.__clientsock.close()
		self.__clientsock = None
		
		# 延迟
		time.sleep(self.__delaytime)
		
		# 重新设置重连的时间
		self.__delaytime = self.__delaytime * 2
		if self.__delaytime > 30:
			self.__delaytime = 30
			
		return self.Connect()
			
	def RecvMessage(self):
		
		self.__run = True

		try:

			inputs = [self.__clientsock]
			
			while self.__run:
				readable, writeable, exceptional = select.select(inputs, [], [])
				for fd in readable:
					if fd == self.__clientsock:
						datahead = self.__clientsock.recv(WWSGC.DATALEN_HEAD)
						if len(datahead) != 0:
							pass
						else:
							logging.info('Remote TcpServer Close')
							self.__run = False
							break
		finally:
			logging.info('Closing client sock')
			self.__clientsock.close()
			self.__clientsock = None
		
	def Stop(self):
	
		if self.__clientsock is not None:
			self.__clientsock.close()
			self.__clientsock = None
		try:
			if self.is_alive():
				self.join()
		except:
			pass
			
	def run(self):

		while True:
		
			# 连接TCP服务端
			if self.Connect() == False:
				while self.Retry() == False:
					pass
		
			# 接收信息
			self.RecvMessage()
		
		
if __name__ == '__main__':

	tcpcli = CTcpClient()
	tcpcli.setDaemon(True)
	tcpcli.start()
	while True:
		if not tcpcli.isAlive():
			break
		time.sleep(2)
		
			