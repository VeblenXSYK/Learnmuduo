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

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	// 设置发送信息
	char message[] = "hello veblen";
	struct iovec io;
	io.iov_base = message;
	io.iov_len = sizeof(message);
	
	struct msghdr msg;
	msg.msg_name = &servaddr;
	msg.msg_namelen = sizeof(struct sockaddr_in);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;

	sendmsg(sockfd, &msg, 0);
	
	close(sockfd);
	exit(0);
}