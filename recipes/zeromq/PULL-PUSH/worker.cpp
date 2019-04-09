#include <stdio.h>
#include <assert.h>
#include <zmq.h>

#include "comm.h"

using namespace comm;

int main(void)
{
    void *context = zmq_ctx_new();
	assert(context);
    void *socket_to_ventilator = zmq_socket(context, ZMQ_PULL);			// 接收包工头发来的任务
	assert(socket_to_ventilator);
    void *socket_to_sink = zmq_socket(context, ZMQ_PUSH);				// 发送完成任务的信息给监理
	assert(socket_to_sink);
	void *socket_to_sink_of_control = zmq_socket(context, ZMQ_SUB);		// 订阅监理的发布消息
    assert(socket_to_sink_of_control);
	
    if(zmq_connect(socket_to_ventilator, "tcp://localhost:5557") == -1)
	{
		printf("zmq_connect failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}
	
    if(zmq_connect(socket_to_sink, "tcp://localhost:5558") == -1)
	{
		printf("zmq_connect failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}
	
	if(zmq_connect(socket_to_sink_of_control, "tcp://localhost:5559") == -1)
	{
		printf("zmq_connect failed: %s\n", zmq_strerror(zmq_errno()));
		return -1;
	}
	if(zmq_setsockopt(socket_to_sink_of_control, ZMQ_SUBSCRIBE, "STOP", 4) == -1)	// 过滤器为空，则接收全部的消息
    {
        printf("zmq_setsockopt failed: %s\n", zmq_strerror(zmq_errno()));
    }

	zmq_pollitem_t items [] = {
        {   socket_to_ventilator,   0,  ZMQ_POLLIN, 0   },
        {   socket_to_sink_of_control,  0,  ZMQ_POLLIN, 0   },
    };
	
	char buffer[256];
    while(1)
    {
		if(zmq_poll(items, 2, -1) == -1)
        {
            printf("zmq_poll failed: %s\n", zmq_strerror(zmq_errno()));
			return -1;
        }
		
		if(items[1].revents & ZMQ_POLLIN)
        {
			// 接收到监理的发布消息
            break;
        }
		
		if(items[0].revents & ZMQ_POLLIN)
		{
			memset(buffer, 0, sizeof(buffer));
			int len = zmq_recv(socket_to_ventilator, buffer, sizeof(buffer)-1, 0);
			if(len == -1)
				return -1;
			
			printf("Received msg: %s\n", buffer);
			fflush(stdout);
			
			// 干活(睡眠指定毫秒)
			defsleep(atoi(buffer));
			
			// 活干完了通知监理
			if(defsend(socket_to_sink, "DONE") == -1)
			{
				printf("defsend failed: %s\n", zmq_strerror(zmq_errno()));
				return -1;
			}
		}
    }

    zmq_close(socket_to_ventilator);
    zmq_close(socket_to_sink);
	zmq_close(socket_to_sink_of_control);
    zmq_ctx_destroy(context);				// 主线程中的zmq_ctx_destroy会阻塞，直到所有sockets安全关闭

    return 0;
}