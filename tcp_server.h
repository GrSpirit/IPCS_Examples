/* tcp_server.h */
#ifndef TCP_SERVER
#define TCP_SERVER

#define BUF_SIZE 1024

typedef struct
{
	long type;
	int status;
	char text[BUF_SIZE];
} Message;

int msg_init();
void msg_send(int msgid, Message *msg);
void mgs_free(int msgid);

#endif