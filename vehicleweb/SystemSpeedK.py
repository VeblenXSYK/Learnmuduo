# -*- coding: utf-8 -*-

from xml.dom import minidom
import os
import logging
import traceback

class CSysSpeedK:

	"""系统速度补偿系数"""

	def __init__(self, path_raw = '', path_meter = ''):

		self.path_raw = path_raw               #原始速度补偿系数文件路径
		self.path_meter = path_meter           #标定后速度补偿系数文件路径
		self.rawspeedk = [[0 for i in range(30)] for i in range(10)]	#生成rawspeedk[10][30]
		self.meterspeedk = [[0 for i in range(30)] for i in range(10)]	#生成meterspeedk[10][30]
		
	def LoadSpeedK(self, file_path = '', key = []):

		"""加载速度补偿文件。
		返回：成功返回True，否则False。"""
		
		try:
			XmlFile = open(file_path, 'rb')
			XmlData = XmlFile.read()
			XmlFile.close()
			Doc = minidom.parseString(XmlData)
			RootElem = Doc.getElementsByTagName('SPEEDK')[0]
			Keys = RootElem.getElementsByTagName('KEY')
			for KeyElem in Keys:
				Axlecount = int(KeyElem.getAttribute('axlecount'))
				data = ''
				
				GroupElem1 = KeyElem.getElementsByTagName("GROUP1")[0]
				TextNode1 = GroupElem1.childNodes[0].nodeValue
				data = data + TextNode1 + ','
				
				GroupElem2 = KeyElem.getElementsByTagName("GROUP2")[0]
				TextNode2 = GroupElem2.childNodes[0].nodeValue
				data = data + TextNode2 + ','
				
				GroupElem3 = KeyElem.getElementsByTagName("GROUP3")[0]
				TextNode3 = GroupElem3.childNodes[0].nodeValue
				data = data + TextNode3 + ','
				
				GroupElem4 = KeyElem.getElementsByTagName("GROUP4")[0]
				TextNode4 = GroupElem4.childNodes[0].nodeValue
				data = data + TextNode4
				
				data = data.split(',')
				data = map(float, data)
				#生成某一轴对应的补偿系数数组
				key[Axlecount] = data				
		except:
			logging.error('解析配置文件异常 : %s' % traceback.format_exc())
			return False

		return True
		
		
	def GetRawSpeedK(self, axlecount, speed):
	
		"""返回轴数、速度对应的原始补偿系数"""

		return self.rawspeedk[axlecount][(speed - 1) % 20]
		
		
	def GetMeterSpeedK(self, axlecount):
	
		"""返回轴数对应的速度补偿系数数组"""
		
		return self.meterspeedk[axlecount]
		
		
	def SetMeterSpeedK(self, axlecount, speed, value = '1.0'):
		
		"""修正补偿系数。
		返回：成功返回True，否则False。"""

		#修改内存中指定轴对应的补偿系数
		self.meterspeedk[axlecount][(speed - 1) % 20] = float(value)
		
		try:
			XmlFile = open(self.path_meter, 'rb')
			XmlData = XmlFile.read()
			XmlFile.close()
			Doc = minidom.parseString(XmlData)
			RootElem = Doc.getElementsByTagName('SPEEDK')[0]
			Keys = RootElem.getElementsByTagName('KEY')
			for KeyElem in Keys:
				Axlecount = int(KeyElem.getAttribute('axlecount'))
				#修改配置文件中指定轴对应的补偿系数
				if Axlecount == axlecount:
					num = (speed - 1) / 5 + 1
					index = (speed - 1) % 5
					GroupElem = KeyElem.getElementsByTagName("GROUP" + str(num))[0]
					TextNode = GroupElem.childNodes[0].nodeValue.split(',')
					TextNode[index] = value
					NewText = ','.join(TextNode)
					GroupElem.childNodes[0].nodeValue = NewText
					break
			
			#写入xml
			fp = open(self.path_meter, 'w')
			Doc.writexml(fp, addindent = '' , newl = '' ,encoding = 'utf-8')
			fp.close()
		except:
			logging.error('解析配置文件异常 : %s' % traceback.format_exc())
			return False

		return True
		
# if __name__ == '__main__':
	
	# speedkey = CSysSpeedK("/.gaosu/usrcnf/RawSpeedK.xml", "/.gaosu/usrcnf/MeterSpeedK.xml")
	# #speedkey.LoadSpeedK(speedkey.path_raw, speedkey.rawspeedk)
	# #speedkey.LoadSpeedK(speedkey.path_meter, speedkey.meterspeedk)
	# #print speedkey.meterspeedk
	# speedkey.SetSpeedK(2, 7, str(1.0234))
