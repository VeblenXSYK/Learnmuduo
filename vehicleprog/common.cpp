#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "common.h"

void Usleep(int us)
{  
    struct timeval delay;  
    delay.tv_sec = 0;  
    delay.tv_usec = us;
    select(0, NULL, NULL, NULL, &delay);
}

void CreateFilePath(char *datname, char *logname)
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

void FuncRunTime(FunType fun, unsigned char *pdata)
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
	
	pr_debug("Exec fun cost time:%lluus\n", end_time - start_time);
}

int GetDataFile(const char *dir, char (*pfilename)[32])
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

ssize_t recvn(int fd, void *buf, size_t len, int flags)
{
	size_t nleft;
	ssize_t nrecv;
	char *ptr;

	ptr = (char *)buf;
	nleft = len;
	while(nleft > 0)
	{
		if((nrecv = recv(fd, ptr, nleft, flags)) <= 0)
		{
			if(errno == EINTR)
				continue;
			return nrecv;
		}

		nleft -= nrecv;
		ptr += nrecv;
	}

	return (len - nleft);
}

ssize_t sendn(int fd, const void *buf, size_t len, int flags)
{
	size_t nleft;
	ssize_t nsend;
	const char *ptr;
	
	ptr = (const char *)buf;
	nleft = len;
	while(nleft > 0)
	{
		if((nsend = send(fd, ptr, nleft, flags)) <= 0)
		{
			if(errno == EINTR)
				continue;
			return nsend;
		}
		
		nleft -= nsend;
		ptr += nsend;
	}
	
	return (len - nleft);
}

