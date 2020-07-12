#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#define GROUP_PORT 	9000
#define GROUP_ADDR 	"239.0.0.2"

int main(int argc, char *argv[])
{   
    int confd = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in recvaddr;
    bzero(&recvaddr, sizeof(recvaddr));                                   	/* 初始化接收地址 */
    recvaddr.sin_family = AF_INET;
    inet_pton(AF_INET, GROUP_ADDR, &recvaddr.sin_addr.s_addr);				/* 接收地址必须是组播地址+端口 */
    recvaddr.sin_port = htons(GROUP_PORT);

    bind(confd, (struct sockaddr *)&recvaddr, sizeof(recvaddr));

	struct ip_mreqn group;                                                  /* 组播结构体 */
    inet_pton(AF_INET, GROUP_ADDR, &group.imr_multiaddr);                   /* 设置组播组地址 */
	inet_pton(AF_INET, "192.168.5.108", &group.imr_address);                /* 设置本地地址添加到组播组 */
    group.imr_ifindex = if_nametoindex("ens33");                            /* 设置本地地址的设备编号 ip a */
	
    setsockopt(confd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));/* 加入组播组(注意：不需要服务器加入多播组) */
	
	char buf[1024];
	int count = 5;
    while (--count) 
	{
		memset(buf, 0, sizeof(buf));
        ssize_t len = recvfrom(confd, buf, sizeof(buf), 0, NULL, 0);
        write(STDOUT_FILENO, buf, len);
    }
	
	setsockopt(confd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &group, sizeof(group));/* 退出组播组 */
    close(confd);
    return 0;
}
