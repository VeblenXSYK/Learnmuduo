#! /usr/bin/env python
# -*- coding: utf-8 -*-
import socket
import sys
import time
import commweb_pb2

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('127.0.0.1', 8888)
sock.connect(server_address)

vi = commweb_pb2.VehicleInfo()
ci = commweb_pb2.CmdInfo()
while True:

	# 序列化cmdinfo并发送出去
	ci.type = ord('@')
	data = ci.SerializeToString()
	sock.sendall(data)
	
	# 对接收到的数据进行反序列化
	data = sock.recv(1024)
	vi.ParseFromString(data)
	print vi.fW, vi.fV
	print vi.aInfo[0].fW, vi.aInfo[0].fV
	
	time.sleep(2)
	
sock.close()

