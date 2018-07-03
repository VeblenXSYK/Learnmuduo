#ifndef COMMON_H
#define COMMON_H


namespace common
{
//定义一个函数指针
typedef int (*FunType)(unsigned char *);

//以当前系统时间创建保存数据的文件路径
void createFilePath(char *datname, char *logname);  
//测试函数运行时间
void funcRunTime(FunType fun, unsigned char *pdata);
//获取目录下的.bin文件
int getDataFile(const char *dir, char (*pfilename)[32]);
//创建非阻塞的UDP套接字
int createNonblockingUDP();
}

#endif