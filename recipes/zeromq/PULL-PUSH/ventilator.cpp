#include <stdio.h>
#include <time.h>

#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket_to_sink = zmq_socket(context, ZMQ_PUSH);
    void *socket_to_worker = zmq_socket(context, ZMQ_PUSH);
    zmq_connect(socket_to_sink, "tcp://localhost:5558");
    zmq_bind(socket_to_worker, "tcp://*:5557");

    printf("Press Enter when all workers get ready:");
    getchar();
    printf("Sending tasks to workers...\n");

    defsend(socket_to_sink, "Start check work");	// 通知监理, 干活了

    srandom((unsigned)time(NULL));

	char msgbuf[10];
    int total_ms = 0;
    for(int i = 0; i < 100; ++i)
    {
        int workload = randof(100) + 1;     	// 工作需要的耗时, 单位ms
        total_ms += workload;
        
		memset(msgbuf, 0, sizeof(msgbuf));
        snprintf(msgbuf, sizeof(msgbuf), "%d", workload);
        defsend(socket_to_worker, msgbuf);   	// 将工作分派给工程队
    }

    printf("Total expected cost: %d ms\n", total_ms);

    zmq_close(socket_to_sink);
    zmq_close(socket_to_worker);
    zmq_ctx_destroy(context);

    return 0;
}