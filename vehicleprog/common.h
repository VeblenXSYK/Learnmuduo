#ifndef COMMON_H
#define COMMON_H


namespace common
{
//����һ������ָ��
typedef int (*FunType)(unsigned char *);

//�Ե�ǰϵͳʱ�䴴���������ݵ��ļ�·��
void createFilePath(char *datname, char *logname);  
//���Ժ�������ʱ��
void funcRunTime(FunType fun, unsigned char *pdata);
//��ȡĿ¼�µ�.bin�ļ�
int getDataFile(const char *dir, char (*pfilename)[32]);
//������������UDP�׽���
int createNonblockingUDP();
}

#endif