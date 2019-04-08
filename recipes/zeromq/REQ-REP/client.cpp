#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <zmq.h>

int main(void)
{
    printf("Connecting to server...\n");

    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");

	char buffer[10];
    for(int i = 0; i < 3; ++i)
    {
        
        const char *requestMsg = "Hello";
        int bytes = zmq_send(socket, requestMsg, strlen(requestMsg), 0);
        printf("[Client][%d] Sended Request Message: %d bytes, content == \"%s\"\n", i, bytes, requestMsg);

		memset(buffer, 0, sizeof(buffer));
        bytes = zmq_recv(socket, buffer, sizeof(buffer) - 1, 0);
        printf("[Client][%d] Received Reply Message: %d bytes, content == \"%s\"\n", i, bytes, buffer);

    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}