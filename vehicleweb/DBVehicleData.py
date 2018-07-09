# -*- coding: utf-8 -*-

"""
	主要处理过称信息的数据表。
"""

class CDBVehicleData:

	def __init__(self, dbconnect):

		"""构造"""

		self.__DBConnect = dbconnect

	def GetPasswdByUser(self, username = ''):

		"""根据用户名获取其密码"""

		Sql = 'select passwd from userinfo where username=%s'
		Args = (username,)
		Rslt = self.__DBConnect.SelectFromDB(Sql, Args)
		if Rslt is None or not Rslt:
			return None
		return Rslt[0][0]

	def AddVehicleInfo(self, column, speed, weight, axlenum, time):

		"""增加过称信息
		返回：成功返回True，否则False。"""

		Sql = 'insert into carinfo(`column`,speed,weight,axlenum,`time`) values(%s,%s,%s,%s,%s)'
		Args = (column,speed,weight,axlenum,time)
		return self.__DBConnect.ExecuteSqlCmd(Sql, Args)
		
	def GetVehicleInfoCnt(self, starttm = '', stoptm = ''):
		
		"""获取一个时间段内的过称信息。
		starttm:   起始时间
		stoptm:	   截至时间
		返回：返回获取到的符合条件的数据记录总数。"""

		Sql = "select count(*) from carinfo where "
		ArgList = []
		if starttm:
			Sql += '`Time`>=%s '
			ArgList.append(starttm)
		if stoptm:
			Sql += 'and `Time`<=%s '
			ArgList.append(stoptm)
		Args = tuple(ArgList)
		Rslt = self.__DBConnect.SelectFromDB(Sql, Args)
		if Rslt is None or not Rslt:
			return 0
		return int(Rslt[0][0])

	def GetVehicleInfo(self, pageid = 1, pagesize = 15, starttm = '', stoptm = ''):
		
		"""获取符合条件的过称信息，分页查询时有效。
		pageid:	   当前显示的页码，从1开始
		pagesize:  单页数据量
		starttm:   起始时间
		stoptm:	   截至时间
		返回：成功返回 column,speed,weight,axlenum,time这几个字段的Tuple的Tuple。失败返回None"""

		Sql = 'select `column`,speed,weight,axlenum,`time` from carinfo '
		ArgList = []
		if starttm:
			Sql += 'where `Time`>=%s '
			ArgList.append(starttm)
		if stoptm:
			Sql += 'and `Time`<=%s '
			ArgList.append(stoptm)
		Sql += 'order by `Time` DESC limit %s offset %s'
		ArgList.append(pagesize)
		ArgList.append((pageid - 1) * pagesize)
		Args = tuple(ArgList)
		return self.__DBConnect.SelectFromDB(Sql, Args)
