# -*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import os, time
import socket, select
import struct
import logging
import threading
import WWSGC
import commweb_pb2
		
		
"""
	struct TransportFormat
	{
		int32_t  headLen;
		int32_t  nameLen;
		char     typeName[nameLen];
		char     protobufData[headLen-nameLen-8];
		int32_t  checkSum;
	}
"""

class CTcpClient(threading.Thread):

	def __init__(self):
	
		threading.Thread.__init__(self, name='TcpClient')
		self.__seraddr = ('127.0.0.1', 8000)
		self.__clientsock = None
		self.__delaytime = 2
		self.__run = False
		
	# 填充网络传输信息
	def FillTransferInfo(self, proto, data):
		
		namelen = 0; namesize = 4 
		messagename = proto.DESCRIPTOR.full_name; messagesize = len(messagename) + 1	#messagename以'\0'结尾
		protosize = len(data)
		checksum = 0; checksize = 4
		
		headlen = namesize + messagesize + protosize + checksize				
		namelen = messagesize
		
		# 打包为TransportFormat
		fmt = '!ii' + str(namelen) + 's' + str(protosize) + 'si'
		packed = struct.pack(fmt, headlen, namelen, messagename, data, checksum)
		
		return packed
		
	def SendData(self, data):
	
		if self.__clientsock != None:
			self.__clientsock.sendall(data)
			
	def TestTcpConn(self):
	
		# 序列化protobuf
		proto = commweb_pb2.CmdInfo()
		proto.type = ord('@')
		data = proto.SerializeToString()

		# 填充TransportFormat
		packed = self.FillTransferInfo(proto, data)
		
		# 发送出去
		self.SendData(packed)
		
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
		recvbuffer = bytes()		# 接收缓存区，用于处理TCP粘包与分包问题
		headsize = 4				# 头部占用的空间大小
		minbodylen = 4*2+2			# nameLen + typeName + checkSum
		maxmessageLen = 1*1024*1024	# 1M

		try:

			inputs = [self.__clientsock]
			
			while self.__run:
			
				readable, writeable, exceptional = select.select(inputs, [], [])
				for fd in readable:
					if fd == self.__clientsock:
						data = self.__clientsock.recv(1024)
						
						if len(data) != 0:
						
							# 把数据存入缓冲区
							recvbuffer += data
							
							while True:
							
								# 接收的数据量少于门限值
								if len(recvbuffer) < minbodylen:
									logging.info('datapacket size less then minmessageLen')
									break
									
								# 获取头部长度的值与消息名长度的值
								bodylen, messagenamelen = struct.unpack('!ii', recvbuffer[:headsize*2])
								
								# 分包处理
								if len(recvbuffer) < bodylen + headsize:
									logging.info('datapacket size less then bodylen')
									break
									
								# 读取消息正文内容
								protolen = bodylen - messagenamelen - headsize * 2
								fmt = '!' + str(messagenamelen) + 's' + str(protolen) + 'si'
								messagename, protodata, checksum = struct.unpack(fmt, recvbuffer[headsize*2:headsize+bodylen])
								
								# 根据消息名进行相应的处理
								if messagename[:-1] == 'commweb.VehicleInfo':
									vi = commweb_pb2.VehicleInfo()
									vi.ParseFromString(protodata)
									print vi.fW, vi.fV
									print vi.aInfo[0].fW, vi.aInfo[0].fV
								
								# 粘包处理
								recvbuffer = recvbuffer[headsize+bodylen:]
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
					
			# 测试TCP通信
			self.TestTcpConn()
		
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
		
			