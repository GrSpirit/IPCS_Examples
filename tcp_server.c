/* tcp_server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tcp_server.h"
#include "socket.h"
#include "queue.h"

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    int msgid;
    Message msg;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) {
        perror("Couldn\'t create socket");
        exit(-1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Couldn\'t bind socket");
        exit(-1);
    }

    msgid = msg_init();

    listen(listener, 1);
    
    while(1) {
        sock = accept(listener, NULL, NULL);
        if(sock < 0) {
            perror("Failed to accept data");
            exit(-1);
        }

        bzero(&msg, sizeof(msg));
        recvall(sock, msg.text, sizeof(msg.text));

        printf("Received: %s\n", msg.text);

        msg.type = 1;
        msg.status = 0;
        msg_send(msgid, &msg);

        strcpy(buf, "OK");
        sendall(sock, buf, strlen(buf) + 1);

        close(sock);
    }
    
    msg_free(msgid);
    return 0;
}
