# -*- coding: utf-8 -*-
import time
import threading
import MySQLdb
import logging
import traceback

class CConnector(threading.Thread):

	"""管理MySQL数据库连接的线程对象：
	初始化时，依次传入数据库服务器地址，连接端口，用户名，密码，数据库名称；
	然后，可以通过 CheckInterval 成员设置超时管理线程检查超时的时间间隔，单位秒，默认 300；
	然后，可以通过 Timeout 成员设置数据库闲置断开时长，单位秒，默认 14400；
	然后，调用 Start()函数初始化数据库连接，并启用连接超时管理线程；
	需要使用连接者通过调用 GetDbConnectObj()获取数据库连接对象；
	使用连接对象完毕后，调用 Stop()函数停止数据库连接超时管理。
	示例：
	Obj = CConnector('127.0.0.1', 3306, 'root', '123456', 'testdb')
	if not Obj.Start():
		# error handle here...
	Conn = Obj.GetDbConnectObj()
	try:
		Cursor = Conn.cursor()
		Cursor.execute('select * ...')
		Cursor.commit()
	except:
		# error handle here...
	Obj.Stop()
	"""

	def __init__(self, host = '', port = 3360, user = '', passwd = '', dbname = ''):

		""" 数据库连接对象构造函数"""

		threading.Thread.__init__(self, name='MySQL-Connect')
		self.CheckInterval = 300
		self.Timeout = 14400
		self.__Host = host
		self.__User = user
		self.__Passwd = passwd
		self.__DbName = dbname
		self.__Port = port
		self.__Conn = None
		self.__Mutex = threading.Lock()
		self.__IsConncted = False
		self.__LastUseTime = 0
		self.__NeedRun = False

	def Start(self):

		"""启动数据库连接管理功能"""

		if self.__NeedRun:
			return True
		if self.GetDbConnectObj() is None:
			return False
		self.__NeedRun = True
		self.start()
		return True

	def Stop(self):

		"""停止数据库连接管理功能"""

		self.__NeedRun = False
		try:
			if self.is_alive():
				self.join()
		except:
			logging.debug('%s' % traceback.format_exc())

	def GetDbConnectObj(self):

		"""获取数据库连接对象：成功返回连接对象，否则返回None
		1.如果当前状态为已连接，则更新最后使用时间，并返回当前连接
		2.如果当前状态为未连接，则重新连接
		"""

		self.AcquireLock()
		self.__EnsureDBConnected()
		self.ReleaseLock()
		return self.__Conn

	def AcquireLock(self):

		"""手动获取互斥锁。
		注意：函数会阻塞直到当前线程可以获取互斥锁再返回。
			 使用后，必须使用ReleaseLock释放锁，否则会导致后续获取无法成功。"""

		return self.__Mutex.acquire()

	def ReleaseLock(self):
		
		"""手动释放互斥锁。
		注意：函数必须与AcquireLock配对使用。"""

		return self.__Mutex.release()

	def SelectFromDB(self, sql_statment = '', arg_tuple = (), need_lock = True):
		
		"""从数据库中读取数据。
		sql_statment:	select的语句，可以包含格式化字符串
		arg_tuple:		select格式化字符串对应参数列表
		need_lock:		是否需要加锁，True则会在内部加锁，否则不会
		返回：成功返回自组织SQL语句与相应格式化参数执行后的数据的tuple的tuple；失败返回None。"""

		Rslt = None
		if need_lock:
			self.__Mutex.acquire()
		while True:
			DBConnObj = self.__EnsureDBConnected()
			if DBConnObj is None:
				break
			Cursor = DBConnObj.cursor()
			try:
				if logging.root.level <= logging.DEBUG:
					logging.debug('execute SQL: %s' % sql_statment % arg_tuple)
				Cursor.execute(sql_statment, arg_tuple)
				DBConnObj.commit()
				Rslt = Cursor.fetchall()
			except:
				logging.error('数据库操作异常	%s' % traceback.format_exc())

			break

		if need_lock:
			self.__Mutex.release()
		return Rslt

	def ExecuteSqlCmd(self, sql_statment = '', arg_tuple = (), need_lock = True):
		
		"""执行数据库操作（操作完成之后，不需要读取数据库中的数据）。
		sql_statment:	需要执行的语句，可以包含格式化字符串
		arg_tuple:		需要执行的语句格式化字符串对应参数列表
		need_lock:		是否需要加锁，True则会在内部加锁，否则不会
		返回：成功返回True，否则False。"""

		Rslt = False
		if need_lock:
			self.__Mutex.acquire()
		while True:
			DBConnObj = self.__EnsureDBConnected()
			if DBConnObj is None:
				if need_lock:
					self.__Mutex.release()
				break
			Cursor = DBConnObj.cursor()
			if arg_tuple is None:
				arg_tuple = ()
			try:
				if logging.root.level <= logging.DEBUG:
					logging.debug('execute SQL: %s' % sql_statment % arg_tuple)
				Cursor.execute(sql_statment, arg_tuple)
				DBConnObj.commit()
				Rslt = True
			except:
				logging.error('数据库操作异常	%s' % traceback.format_exc())

			break

		if need_lock:
			self.__Mutex.release()
		return Rslt

	def ConstructSetString(self, data):

		"""构造select中 where xx in (xx) 语句中带括号的部分，如：('data1', 'data2')。
		data:  可以遍历的序列，如list、set、tuple等。
		返回：带括号的字符串。
		注意：需要确保"""

		IsFirst = True
		Ret = '('
		for Item in data:
			if not Item:
				continue
			if IsFirst:
				Ret += "'%s'" % MySQLdb.escape_string(Item)
				IsFirst = False
			else:
				Ret += ",'%s'" % MySQLdb.escape_string(Item)

		Ret += ')'
		return Ret

	def __EnsureDBConnected(self):
		if self.__IsConncted:
			self.__LastUseTime = time.time()
		else:
			try:
				MySQLdb.threadsafety = 3
				self.__Conn = MySQLdb.connect(host=self.__Host, user=self.__User, passwd=self.__Passwd, db=self.__DbName, port=self.__Port, charset='utf8')
				self.__IsConncted = True
				self.__LastUseTime = time.time()
				logging.debug('数据库重新连接成功！')

			except:
				logging.error('数据库连接异常! %s' % traceback.format_exc())
				return None

		return self.__Conn

	def __SleepAndCheckQuit(self, seconds = 100):
		
		"""睡眠一定时间，并每半秒时间醒来判断是否需要退出：
		返回：需要退出返回True，否则False"""

		for i in range(0, seconds * 2):
			if not self.__NeedRun:
				return True
			time.sleep(0.5)

		return False

	def run(self):
		
		"""检测连接使用时间是否超时的线程函数"""

		logging.info('线程【数据库连接保持】启动')

		while True:
			if self.__SleepAndCheckQuit(self.CheckInterval):
				break
			Now = time.time()
			self.__Mutex.acquire()
			if self.__IsConncted and Now - self.__LastUseTime > self.Timeout:
				try:
					self.__Conn.close()
				except:
					logging.error('数据库关闭连接出现异常 %s' % traceback.format_exc())

				self.__IsConncted = False
				logging.debug('数据库长时间没有使用，已经关闭!')

			self.__Mutex.release()

		logging.info('线程【数据库连接保持】停止')

