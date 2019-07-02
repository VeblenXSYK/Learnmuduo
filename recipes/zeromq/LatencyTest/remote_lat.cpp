#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

int main (int argc, char *argv[])
{
    if (argc != 4) {
        printf ("usage: remote_lat <connect-to> <message-size> <roundtrip-count>\n");
        return 1;
    }    

    const char *connect_to = argv[1];
    size_t message_size = atoi (argv[2]);
    int roundtrip_count = atoi (argv[3]);

    void *ctx = zmq_init (1);
    if (!ctx) {
        printf ("error in zmq_init: %s\n", zmq_strerror (errno));
        return -1;
    }

    void *s = zmq_socket (ctx, ZMQ_REQ);
    if (!s) {
        printf ("error in zmq_socket: %s\n", zmq_strerror (errno));
        return -1;
    }

    int rc = zmq_connect (s, connect_to);
    if (rc != 0) {
        printf ("error in zmq_connect: %s\n", zmq_strerror (errno));
        return -1;
    }

    zmq_msg_t msg;
    rc = zmq_msg_init_size (&msg, message_size);
    if (rc != 0) {
        printf ("error in zmq_msg_init_size: %s\n", zmq_strerror (errno));
        return -1;
    }
    memset (zmq_msg_data (&msg), 0, message_size);

    void *watch = zmq_stopwatch_start ();   // 开启stopwatch，用于计算运行时间

    for (int i = 0; i != roundtrip_count; i++) {
        rc = zmq_sendmsg (s, &msg, 0);
        if (rc < 0) {
            printf ("error in zmq_sendmsg: %s\n", zmq_strerror (errno));
            return -1;
        }
        rc = zmq_recvmsg (s, &msg, 0);
        if (rc < 0) {
            printf ("error in zmq_recvmsg: %s\n", zmq_strerror (errno));
            return -1;
        }
        if (zmq_msg_size (&msg) != message_size) {
            printf ("message of incorrect size received\n");
            return -1;
        }
    }

    unsigned long elapsed = zmq_stopwatch_stop (watch);

    rc = zmq_msg_close (&msg);
    if (rc != 0) {
        printf ("error in zmq_msg_close: %s\n", zmq_strerror (errno));
        return -1;
    }

    double latency = (double) elapsed / (roundtrip_count * 2);

    printf ("message size: %d [B]\n", (int) message_size);
    printf ("roundtrip count: %d\n", (int) roundtrip_count);
    printf ("average latency: %.3f [us]\n", (double) latency);

    rc = zmq_close (s);
    if (rc != 0) {
        printf ("error in zmq_close: %s\n", zmq_strerror (errno));
        return -1;
    }

    rc = zmq_ctx_term (ctx);
    if (rc != 0) {
        printf ("error in zmq_ctx_term: %s\n", zmq_strerror (errno));
        return -1;
    }

    return 0;
}
