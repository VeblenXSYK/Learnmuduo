#include <stdio.h>

#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket_to_ventilator = zmq_socket(context, ZMQ_PULL);
    void *socket_to_sink = zmq_socket(context, ZMQ_PUSH);
    zmq_connect(socket_to_ventilator, "tcp://localhost:5557");
    zmq_connect(socket_to_sink, "tcp://localhost:5558");

	char buffer[256];
    while(1)
    {
		memset(buffer, 0, sizeof(buffer));
        int len = zmq_recv(socket_to_ventilator, buffer, sizeof(buffer)-1, 0);
		if(len == -1)
			return -1;
        printf("Received msg: %s\n", buffer);
        fflush(stdout);
		
        defsleep(atoi(buffer));     		// 干活(睡眠指定毫秒)
        defsend(socket_to_sink, "DONE"); 	// 活干完了通知监理
    }

    zmq_close(socket_to_ventilator);
    zmq_close(socket_to_sink);
    zmq_ctx_destroy(context);

    return 0;
}