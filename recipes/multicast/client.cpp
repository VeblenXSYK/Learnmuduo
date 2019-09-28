#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#define SERVER_PORT 	8000
#define CLIENT_PORT 	9000
#define GROUP_ADDR 		"239.0.0.2"

int main(int argc, char *argv[])
{   
    int confd = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof(localaddr));                                   /* 初始化*/
    localaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0" , &localaddr.sin_addr.s_addr);
    localaddr.sin_port = htons(CLIENT_PORT);

    bind(confd, (struct sockaddr *)&localaddr, sizeof(localaddr));

	struct ip_mreqn group;                                                  /* 组播结构体 */
    inet_pton(AF_INET, GROUP_ADDR, &group.imr_multiaddr);                   /* 设置组播组地址 */
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);                      /* 使用本地任意IP添加到组播组 */
    group.imr_ifindex = if_nametoindex("ens33");                             /* 设置网卡名 编号 ip ad */
	printf("%d\n", group.imr_ifindex);
	
    setsockopt(confd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));/* 将client加入组播组(注意：不需要服务器加入多播组) */
	
	char buf[1024];
    while (1) 
	{
		memset(buf, 0, sizeof(buf));
        ssize_t len = recvfrom(confd, buf, sizeof(buf), 0, NULL, 0);
        write(STDOUT_FILENO, buf, len);
    }
    close(confd);
    return 0;
}
