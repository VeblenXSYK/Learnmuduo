#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <zmq.h>

#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_PUB);
    zmq_bind(socket, "tcp://*:5556");

    srandom((unsigned)time(NULL));

	char msg[20];
    while(1)
    {
        int topicinfo = randof(100000);   	// 主题: 0 ~ 99999
        int temp = randof(84) - 42;     	// 温度: -42 ~ 41
        int relhumidity = randof(50) + 10;  // 相对湿度: 10 ~ 59

		memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "%5d %d %d", topicinfo, temp, relhumidity);
		/* 
		 *	1、如果没有任何sub，其所有消息将被简单的抛弃
		 *	2、当消息过多，而sub的消化能力比较低，则未发送的消息缓存在pub
		 */
		zmq_send(socket, msg, strlen(msg), 0);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;

}
