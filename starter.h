/* starter.h */
#ifndef STARTER_H
#define STARTER_H
#include "message.h"

int exec_balancer(int queueid);
void fill_msg(Message*, int, int, char*);

#endif