#include <stdio.h>
#include <zmq.h>

int main(void)
{
    void *context = zmq_ctx_new();
    void *receiver = zmq_socket(context, ZMQ_PULL);
    zmq_connect(receiver, "tcp://localhost:5557");

    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5556");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "10001", 5);

	char msg[256];
    while(1)
    {
        zmq_pollitem_t items[] = {
            {receiver,  0,  ZMQ_POLLIN,     0},
            {subscriber,0,  ZMQ_POLLIN,     0},
        };

        zmq_poll(items, 2, -1);

        if(items[0].revents & ZMQ_POLLIN)
        {
            int size = zmq_recv(receiver, msg, 255, 0);
            if(size != -1)
            {
                printf("ZMQ_PULL\n");
            }
        }

        if(items[1].revents & ZMQ_POLLIN)
        {
            int size = zmq_recv(subscriber, msg, 255, 0);
            if(size != -1)
            {
                printf("ZMQ_SUB\n");
            }
        }
    }

    zmq_close(receiver);
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
