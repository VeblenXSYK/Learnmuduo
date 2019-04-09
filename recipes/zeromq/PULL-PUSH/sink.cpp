#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
	assert(context);
    void *socket_to_worker_and_ventilator = zmq_socket(context, ZMQ_PULL);
	assert(socket_to_worker_and_ventilator);
	void *socket_to_worker_of_control = zmq_socket(context, ZMQ_PUB); 
	assert(socket_to_worker_of_control);
	
    if(zmq_bind(socket_to_worker_and_ventilator, "tcp://*:5558") == -1)
	{
		printf("zmq_bind failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}
	
	if(zmq_bind(socket_to_worker_of_control, "tcp://*:5559") == -1)
    {
        printf("zmq_bind failed: %s\n", zmq_strerror(zmq_errno()));
        return -1;
    }

	// 接收来自包工头的开始干活的消息
	char buffer[256] = { 0 };
	int len = zmq_recv(socket_to_worker_and_ventilator, buffer, sizeof(buffer)-1, 0);
	if(len == -1)
		return -1;
    printf("Received msg: %s", buffer);

    int64_t start_time = getcurtimestamp();

    for(int i = 0; i < 100; ++i)
    {
        // 接收100个worker干完活的消息(接收多个工程队的消息，使用的是公平队列策略)
		memset(buffer, 0, sizeof(buffer));
        len = zmq_recv(socket_to_worker_and_ventilator, buffer, sizeof(buffer)-1, 0);
        if(len == -1)
			return -1;

        if(i / 10 * 10 == i)
            printf(":");
        else
            printf(".");
        fflush(stdout);
    }

    printf("Total elapsed time: %d ms]\n", (int)(getcurtimestamp() - start_time));
	
	if(defsend(socket_to_worker_of_control, "STOP") == -1)
    {
        printf("defsend failed: %s\n", zmq_strerror(zmq_errno()));
        return -1;
    }

    zmq_close(socket_to_worker_and_ventilator);
	zmq_close(socket_to_worker_of_control);
    zmq_ctx_destroy(context);

    return 0;
}