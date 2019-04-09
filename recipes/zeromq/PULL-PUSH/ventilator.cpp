#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
	assert(context);
    void *socket_to_sink = zmq_socket(context, ZMQ_PUSH);
	assert(socket_to_sink);
    void *socket_to_worker = zmq_socket(context, ZMQ_PUSH);
	assert(socket_to_worker);
	
    if(zmq_connect(socket_to_sink, "tcp://localhost:5558") == -1)
	{
		printf("zmq_connect failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}
	
    if(zmq_bind(socket_to_worker, "tcp://*:5557") == -1)
	{
		printf("zmq_bind failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}

	// 手动输入enter来启动任务分发的方式, 手动同步了工程队/包工头/监理，避免最先建立连接的工程队把所有任务都接收处理了
    printf("Press Enter when all workers get ready:");
    getchar();
    printf("Sending tasks to workers...\n");

	// 通知监理, 干活了
    if(defsend(socket_to_sink, "Start check work") == -1)
	{
		printf("defsend failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}

    srandom((unsigned)time(NULL));

	char msgbuf[10];
    int total_ms = 0;
    for(int i = 0; i < 100; ++i)
    {
        int workload = randof(100) + 1;     			// 工作需要的耗时, 单位ms
        total_ms += workload;
        
		memset(msgbuf, 0, sizeof(msgbuf));
        snprintf(msgbuf, sizeof(msgbuf), "%d", workload);
        if(defsend(socket_to_worker, msgbuf) == -1)   	// 将工作分派给工程队(采用的是轮流/平均分配的方式，这是简单的负载均衡)
		{
			printf("defsend failed: %s\n", zmq_strerror(zmq_errno()));
			return -1;
		}
    }

    printf("Total expected cost: %d ms\n", total_ms);

    zmq_close(socket_to_sink);
    zmq_close(socket_to_worker);
    zmq_ctx_destroy(context);

    return 0;
}