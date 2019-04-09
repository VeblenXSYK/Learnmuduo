#include <stdio.h>
#include <string.h>
#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_SUB);
    zmq_connect(socket, "tcp://localhost:5563");

	// 设置订阅的主题信息
    const char *topicinfo = "B";
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, topicinfo, strlen(topicinfo));

	char msgtype[16], msgcontent[64];
    for(int i = 0; i < 50; ++i)
    {
		memset(msgtype, 0, sizeof(msgtype));
        int len = zmq_recv(socket, msgtype, sizeof(msgtype)-1, 0);
		if(len == -1)
			return -1;
		
		memset(msgcontent, 0, sizeof(msgcontent));
        len = zmq_recv(socket, msgcontent, sizeof(msgcontent)-1, 0);
		if(len == -1)
			return -1;
		
        printf("[Subscriber] Received msg: %s %s\n", msgtype, msgcontent);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);
    
    return 0;
}
