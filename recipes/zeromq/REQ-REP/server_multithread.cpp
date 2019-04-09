/*
 * 使用一个掮客外加5个服务端线程来响应客户端请求，用于演示在多线程环境中使用ZMQ
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <zmq.h>

typedef struct _DataInfo
{
	void *context;
	int id;
}DataInfo;

static void *worker_routine(void *data)
{
	DataInfo *dataInfo = (DataInfo *)data;
	
	// 连接掮客(即主线程)
    void *socket_to_main_thread = zmq_socket(dataInfo->context, ZMQ_REP);
    assert(socket_to_main_thread);
    zmq_connect(socket_to_main_thread, "inproc://workers");
	
	char buffer[10];
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes = zmq_recv(socket_to_main_thread, buffer, sizeof(buffer) - 1, 0);
        printf("[Server%d] Recevied Request Message: %d bytes, content == \"%s\"\n", dataInfo->id, bytes, buffer);

        sleep(1);

        const char *replyMsg = "World";
        bytes = zmq_send(socket_to_main_thread, replyMsg, strlen(replyMsg), 0);
        printf("[Server%d] Sended Reply Message: %d bytes, content == \"%s\"\n", dataInfo->id, bytes, replyMsg);
    }

    zmq_close(socket_to_main_thread);
	
    return NULL;
}

int main(void)
{
    void *context = zmq_ctx_new();
	assert(context);
	
	// 创建用于掮客与客户端的连接
	void *socket_to_client = zmq_socket(context, ZMQ_ROUTER);
    assert(socket_to_client);
    zmq_bind(socket_to_client, "tcp://*:5555");
	
	// 创建用于掮客与服务端的连接
	void *socket_to_worker_thread = zmq_socket(context, ZMQ_DEALER);
    assert(socket_to_worker_thread);
    zmq_bind(socket_to_worker_thread, "inproc://workers");				// 线程间的数据交互(父线程bind, 子线程connect)
	
	// 创建服务端线程
	DataInfo dataInfo[5];
	for(int i = 0; i < 5; ++i)
    {
		dataInfo[i].context = context;
		dataInfo[i].id = i;
		
        pthread_t worker;
        pthread_create(&worker, NULL, worker_routine, (void *)&dataInfo[i]);
    }
	
	// 开启ZMQ掮客
	zmq_proxy(socket_to_client, socket_to_worker_thread, NULL);

    zmq_close(socket_to_client);
	zmq_close(socket_to_worker_thread);
    zmq_ctx_destroy(context);

    return 0;
}