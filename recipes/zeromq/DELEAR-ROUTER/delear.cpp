#include <zmq.h>

int main(void)
{
    void *context = zmq_ctx_new();
    void *socket_for_client = zmq_socket(context, ZMQ_ROUTER);
    void *socket_for_server = zmq_socket(context, ZMQ_DEALER);
    zmq_bind(socket_for_client, "tcp://*:5559");
    zmq_bind(socket_for_server, "tcp://*:5560");

    zmq_pollitem_t items[] = {
        {   socket_for_client,  0,  ZMQ_POLLIN, 0   },
        {   socket_for_server,  0,  ZMQ_POLLIN, 0   },
    };

    while(1)
    {
        zmq_msg_t message;
        zmq_poll(items, 2, -1);

        if(items[0].revents & ZMQ_POLLIN)
        {
            while(1)
            {
				// 创建消息对象
                zmq_msg_init(&message);
				// 接收消息
                zmq_msg_recv(&message, socket_for_client, 0);
				// 检查消息是否完整
                int more = zmq_msg_more(&message);
				// 发送消息 ZMQ_SNDMORE表示后面还有其它消息
                zmq_msg_send(&message, socket_for_server, more ? ZMQ_SNDMORE : 0);
				// 释放消息
                zmq_msg_close(&message);

                if(!more)
                {
                    break;
                }
            }
        }

        if(items[1].revents & ZMQ_POLLIN)
        {
            while(1)
            {
                zmq_msg_init(&message);
                zmq_msg_recv(&message, socket_for_server, 0);
                int more = zmq_msg_more(&message);
                zmq_msg_send(&message, socket_for_client, more ? ZMQ_SNDMORE : 0);
                zmq_msg_close(&message);

                if(!more)
                {
                    break;
                }
            }
        }
    }

    zmq_close(socket_for_client);
    zmq_close(socket_for_server);
    zmq_ctx_destroy(context);

    return 0;
}
