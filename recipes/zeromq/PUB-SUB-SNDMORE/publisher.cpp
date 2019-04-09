#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_PUB);
    zmq_bind(socket, "tcp://*:5563");

    while(1)
    {
		defsendmore(socket, "A");
        defsend(socket, "We don't want to see this");
        defsendmore(socket, "B");
        defsend(socket, "We would like to see this");
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;

}
