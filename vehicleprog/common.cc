#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <muduo/base/Logging.h>

#include "common.h"

	
void common::createFilePath(char *datname, char *logname)
{
	struct tm *t_now;
	time_t t;
	
	time(&t);
	t_now = localtime(&t);
	
	sprintf(datname, "/tmp/%4d%02d%02d_%02d%02d%02d.dat", t_now->tm_year + 1900, t_now->tm_mon + 1, \
			t_now->tm_mday, t_now->tm_hour, t_now->tm_min, t_now->tm_sec);
	sprintf(logname, "/tmp/%4d%02d%02d_%02d%02d%02d.bin", t_now->tm_year + 1900, t_now->tm_mon + 1, \
			t_now->tm_mday, t_now->tm_hour, t_now->tm_min, t_now->tm_sec);
}

void common::funcRunTime(FunType fun, unsigned char *pdata)
{
	/* 测试函数执行时间 */
	uint64_t start_time;    /* 起始时间 */
	uint64_t end_time;      /* 结束时间 */ 
	struct timespec ltv;
	clock_gettime(CLOCK_REALTIME, &ltv);
	start_time = (uint64_t)ltv.tv_sec * 1000 * 1000 + (uint64_t)ltv.tv_nsec / 1000; /* (us) */

	(*fun)(pdata);

	clock_gettime(CLOCK_REALTIME, &ltv);
	end_time = (uint64_t)ltv.tv_sec * 1000 * 1000 + (uint64_t)ltv.tv_nsec / 1000;   /* (us) */
	
	LOG_INFO << "Exec fun cost time:" << end_time - start_time;
}

int common::getDataFile(const char *dir, char (*pfilename)[32])
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	
	if((dp = opendir(dir)) == NULL)
		return 0;
	
	if(chdir(dir) != 0)
		return 0;
	
	int num = 0;
	while((entry = readdir(dp)) != NULL)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		    continue;
		
		if(strstr(entry->d_name, ".bin") != NULL)
		{
			memcpy(pfilename[num], entry->d_name, strlen(entry->d_name));
			num++;
		}
	}
	
	closedir(dp);
	
	return num;
}

int common::createNonblockingUDP()
{
	//SOCK_CLOEXEC:fork子进程时关闭父进程打开的文件描述符
	int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
	if (sockfd < 0) {
		LOG_SYSFATAL << "::socket";
	}
	return sockfd;
}

