#ifndef _COMMON_H_
#define _COMMON_H_
 
#define SM_BUF_SIZE 1024
 
struct sm_msg
{
	pthread_mutex_t sm_mutex;
	char buf[SM_BUF_SIZE];
};

#endif
