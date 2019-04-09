#ifndef COMM_H
#define COMM_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <zmq.h>

namespace comm
{
inline int defsend(void *socket, const char *msg)
{
    return zmq_send(socket, msg, strlen(msg), 0);
}

inline int defsendmore(void *socket, const char *msg)
{
    return zmq_send(socket, msg, strlen(msg), ZMQ_SNDMORE);
}

inline int randof(int num)
{
	return (int)((float)(num) * random() / (RAND_MAX + 1.0));
}
	
inline int64_t getcurtimestamp(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

inline void defsleep(int ms)
{
	struct timespec t;
	t.tv_sec = ms/1000;
	t.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&t, NULL);
}
}

#endif