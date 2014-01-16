#ifndef QUEUE_H
#define QUEUE_H

#include "message.h"

int msg_init();
void msg_send(int msgid, Message *msg);
void msg_free(int msgid);

#endif