#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#define SERVER_PORT 	8000
#define CLIENT_PORT 	9000
#define GROUP_ADDR 		"239.0.0.2"
//239.0.0.0～239.255.255.255


int main(void)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);            /* 构造用于UDP通信的套接字 */
    
	struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;                        /* IPv4 */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);         /*本地任意IP INADDR_ANY = 0 */
    serveraddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	struct ip_mreqn group;
    inet_pton(AF_INET, GROUP_ADDR, &group.imr_multiaddr);   /* 设置组播组的地址 */
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);      /* 本地任意IP自动分配有效IP */
    group.imr_ifindex = if_nametoindex("ens33");             /* 给出网卡名，转换为对应编号：eth0 --> 编号         ，，  命令:ip ad */

    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));  /*获取组播权限*/

	struct sockaddr_in clientaddr;
    bzero(&clientaddr, sizeof(clientaddr));                 /* 构造client地址 IP+端口号 */
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET, GROUP_ADDR, &clientaddr.sin_addr.s_addr); /* IPv4  239.0.0.2+9000 */
    clientaddr.sin_port = htons(CLIENT_PORT);

	int i = 0;
	char buf[1024];
    while (1) 
	{
		memset(buf, 0, sizeof(buf));
        sprintf(buf, "multicast %d\n", i++);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        sleep(1);
    }
    close(sockfd);
    return 0;
}
