#!/usr/bin/env python
# -*- coding: utf8 -*-
'''
WebServer程序Main函数入口。
'''

import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import os
import argparse
import logging
import WWSGC
from logging.handlers import RotatingFileHandler
from ServerVHMain import CServerVHMain


def ParseArgs():

	"""处理系统命令行"""
	
	# 创建一个argparse对象
	arg_parser = argparse.ArgumentParser(description="This program is used for WebServer.")
	
	# 使用add_agument可以定义参数的选项名称和类型及存储的目的位置。
	# 其中前面的如参数“-h”, "--help"都是unix风格的选项类型说明；choices则是选项参数的选择范围；
	# dest是存储的位置，default是缺省时的默认值。如果action是store，则保存为字符串；如果action
	# 为store_true，则有选项时将目的变量置为True，否则False。	  
	arg_parser.add_argument('-l', '--loglevel', action="store", \
		choices=['20', '30', '40', '50'], dest='loglevel', \
		default="20", help='Specify program log level, which can be:'\
		'20--info(default),30--warning,40--error,50--critical')
	arg_parser.add_argument('-t', action="store_true", dest='test', \
		default=False, help='Add test function to LocalDevice or RemoteServer')
		
	# parse_args()返回一个字典，key为add_augument中的dest，value为存储的值。
	args = arg_parser.parse_args()
	
	return args

def InitLog(file_name, log_level=logging.WARNING):

	"""初始化日志为多个文件循环的方式"""
	
	log_formatter = logging.Formatter(fmt='%(asctime)s %(levelname)s %(thread)d '\
		'[%(filename)s(%(lineno)d)] %(message)s', datefmt='%Y%m%d-%H%M%S')
	my_handler = RotatingFileHandler(file_name, mode='a', maxBytes=5*1024*1024, \
		backupCount=5, encoding=None, delay=0)
	my_handler.setFormatter(log_formatter)
	my_handler.setLevel(log_level)
	app_log = logging.getLogger()
	app_log.setLevel(log_level)
	app_log.addHandler(my_handler)

if __name__ == '__main__':
	args = ParseArgs()
	#设置日志文件名称
	logfile = WWSGC.FILE_OutLogPath()
	loglevel = int(args.loglevel)
	InitLog(logfile, loglevel)
	DaemonObj = CServerVHMain(args.test)
	DaemonObj.Run()
