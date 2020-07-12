/*
	多播服务器的程序设计，不需要服务器加入多播组，可以直接向某个多播组发送数据
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MCAST_PORT 9000
#define MCAST_ADDR "239.0.0.2"				// 一个局部连接多播地址，路由器不进行转发
#define MCAST_DATA "BROADCAST TEST DATA\n"	// 多播发送的数据
#define MCAST_INTERVAL 2					// 发送间隔时间

int main(int argc, char *argv[])
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1)
	{
		perror("socket()");
		return -1;
	}

	struct sockaddr_in mcast_addr;
	memset(&mcast_addr, 0, sizeof(mcast_addr));
	mcast_addr.sin_family = AF_INET;
	mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	mcast_addr.sin_port = htons(MCAST_PORT);

	while(1) 
	{
		// 向多播地址发送数据
		int n = sendto(s, MCAST_DATA, sizeof(MCAST_DATA), 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
		if( n < 0)
		{
			perror("sendto()");
			return -2;
		}
		sleep(MCAST_INTERVAL);
	}
	return 0;
}