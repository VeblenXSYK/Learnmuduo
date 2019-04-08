#include <stdio.h>
#include <unistd.h>
#include <zmq.h>

int main(void)
{
    void *context = zmq_ctx_new();
    void *receiver = zmq_socket(context, ZMQ_PULL);
    zmq_connect(receiver, "tcp://localhost:5557");

    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5556");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "10001", 5);

    while(1)
    {
        char msg[256];
		
		while(1)
        {
            int size = zmq_recv(subscriber, msg, 255, ZMQ_DONTWAIT);	// 非阻塞模式
            if(size != -1)
            {
                printf("ZMQ_SUB\n");		// 接收数据成功
            }
            else
            {
                break;
            }
        }
		
        while(1)
        {
            int size = zmq_recv(receiver, msg, 255, ZMQ_DONTWAIT);
            if(size != -1)
            {
                printf("ZMQ_PULL\n");		// 接收数据成功
            }
            else
            {
                break;
            }
        }
		
        sleep(1);   						// 休息一下, 避免疯狂循环
    }

    zmq_close(receiver);
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
