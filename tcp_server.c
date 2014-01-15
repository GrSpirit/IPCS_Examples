/* tcp_server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <netinet/in.h>

#include "tcp_server.h"
#include "socket.h"

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    int bytes_read;
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

int msg_init()
{
	int msgid;

	if ((msgid = msgget(IPC_PRIVATE, 0660 | IPC_CREAT)) < 0) {
		perror("Couldn\'t create message queue: ");
		exit(-1);
	}

	return msgid;
}

void mgs_free(int msgid)
{
	if (msgctl(msgid, IPC_RMID, NULL) < 0) {
		perror("Can\'t delete queue: ");
		exit(-1);
	}
}

void msg_send(int msgid, Message *msg)
{
	if ((msgsnd(msgid, msg, sizeof(Message) - sizeof(long), 0)) < 0) {
		perror("Failed to send message in queue: ");
		exit(-1);
	}
}