#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 100

int main(int argc, char ** argv) 
{
	int sockfd;
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	char buf[MAXSIZE + 1];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999);
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	// 设置接收缓冲区
	struct iovec io;
	io.iov_base = buf;
	io.iov_len = MAXSIZE;
	
	struct msghdr msg;
	msg.msg_name = &cliaddr;
	msg.msg_namelen = sizeof(struct sockaddr_in);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;

	// 接收数据
	ssize_t len = recvmsg(sockfd, &msg, 0);
	
	// 解析客户端IP+PORT
	char ip[16] = { 0 };
	inet_ntop(AF_INET, &(cliaddr.sin_addr), ip, sizeof(ip));
	int port = ntohs(cliaddr.sin_port);
	
	// 解析客户端信息
	char *temp = (char *)msg.msg_iov[0].iov_base;
	temp[len] = '\0';
	
	printf("get message from %s[%d]: %s\n", ip, port, temp);
	
	close(sockfd);
}