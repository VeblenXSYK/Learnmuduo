#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define _DEBUG         //��ʾ��ӡ��������Ϣ                                              
#ifdef _DEBUG
    #define pr_debug(fmt, args...) fprintf(stderr, fmt, ##args)
#else
    #define pr_debug(fmt, args...)
#endif

#define V_DEBUG  	1	//VLOG��debug����
#define V_THRESHOLD 10  //VLOG���������

class Uncopyable
{
protected:
	Uncopyable(){}					//����derived�����������
	~Uncopyable(){}
private:
	Uncopyable(const Uncopyable &);	//����ֹcopying
	Uncopyable& operator=(const Uncopyable&);
};

typedef int (*FunType)(unsigned char *);
typedef unsigned short ushort;

//����һ������ָ��
extern void Usleep(int us);
//�Ե�ǰϵͳʱ�䴴���������ݵ��ļ�·��
extern void CreateFilePath(char *datname, char *logname);  
//���Ժ�������ʱ��
extern void FuncRunTime(FunType fun, unsigned char *pdata);
//��ȡĿ¼�µ�.bin�ļ�
extern int GetDataFile(const char *dir, char (*pfilename)[32]);

extern ssize_t recvn(int fd, void *buf, size_t len, int flags);
extern ssize_t sendn(int fd, const void *buf, size_t len, int flags);

#endif