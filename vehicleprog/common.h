#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define _DEBUG         //表示打印出调试信息                                              
#ifdef _DEBUG
    #define pr_debug(fmt, args...) fprintf(stderr, fmt, ##args)
#else
    #define pr_debug(fmt, args...)
#endif

#define V_DEBUG  	1	//VLOG的debug级别
#define V_THRESHOLD 10  //VLOG的输出级别

class Uncopyable
{
protected:
	Uncopyable(){}					//允许derived对象构造和析构
	~Uncopyable(){}
private:
	Uncopyable(const Uncopyable &);	//但阻止copying
	Uncopyable& operator=(const Uncopyable&);
};

typedef int (*FunType)(unsigned char *);
typedef unsigned short ushort;

//定义一个函数指针
extern void Usleep(int us);
//以当前系统时间创建保存数据的文件路径
extern void CreateFilePath(char *datname, char *logname);  
//测试函数运行时间
extern void FuncRunTime(FunType fun, unsigned char *pdata);
//获取目录下的.bin文件
extern int GetDataFile(const char *dir, char (*pfilename)[32]);

extern ssize_t recvn(int fd, void *buf, size_t len, int flags);
extern ssize_t sendn(int fd, const void *buf, size_t len, int flags);

#endif