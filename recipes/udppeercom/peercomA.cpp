#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>


int main()
{
	// 创建套接口
	int udp_fd = 0;
	if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("socket create failed\n");
		exit(EXIT_FAILURE);
	}
	printf("socket create success\n");
	
	// 对套接字初始化
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	
	// 绑定端口
	if (bind(udp_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("bind fail\n");
		close(udp_fd);
		exit(EXIT_SUCCESS);
	}
	printf("bind success\n");

	char buf[100];
	while (1) 
	{
		fd_set rset;
		FD_ZERO(&rset);
		FD_SET(0, &rset);
		FD_SET(udp_fd, &rset);
		
		if (select(udp_fd + 1, &rset, NULL, NULL, NULL) > 0) 
		{
			if (FD_ISSET(udp_fd, &rset)) 
			{
				memset(buf, 0, sizeof(buf));
				
				// 从对端接收信息
				struct sockaddr_in recvaddr;
				socklen_t addrlen = sizeof(recvaddr);
				recvfrom(udp_fd, buf, sizeof(buf), 0, (struct sockaddr*)&recvaddr, &addrlen);
				
				// 获取接收到的ip地址
				char ipbuf[32] = { 0 };
				inet_ntop(AF_INET, &recvaddr.sin_addr, ipbuf, sizeof(ipbuf));
				
				// 获取接收到的port
				int port = ntohs(recvaddr.sin_port);
				
				printf("from %s:%d recv message: %s\n", ipbuf, port, buf);
			}
			else if (FD_ISSET(0, &rset))
			{	
				// 读取标准输入中的所有数据，避免因数据没有读取完而再次进入造成死循环
				read(0, buf, sizeof(buf));
				
				// 设置对端地址为 127.0.0.1:9000
				struct sockaddr_in peeraddr;
				bzero(&peeraddr, sizeof(peeraddr));
				peeraddr.sin_family = AF_INET;
				peeraddr.sin_port = htons(9000);
				inet_pton(AF_INET, "127.0.0.1", &peeraddr.sin_addr);
				
				// 发送信息给对端
				const char *message = "test";
				sendto(udp_fd, message, strlen(message), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr));
			}
		}
	}
}
