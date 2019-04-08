#include <stdio.h>
#include <string.h>

#include <zmq.h>

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_SUB);
    zmq_connect(socket, "tcp://localhost:5556");

	// 设置订阅的主题信息
    char *topicinfo = "10001";
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, topicinfo, strlen(topicinfo));

	char buffer[256];
    for(int i = 0; i < 50; ++i)
    {
		memset(buffer, 0, sizeof(buffer));
        int len = zmq_recv(socket, buffer, sizeof(buffer)-1, 0);
		if(len == -1)
			return -1;
        printf("[Subscriber] Received weather report msg: %s\n", buffer);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);
    
    return 0;
}
