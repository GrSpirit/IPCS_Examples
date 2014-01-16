/* multi.h */
#ifndef MULTI
#define MULTI

#include "message.h"

typedef union semun
{
  int val;
  struct semid_ds *buf;
  ushort * array;
} semun_t;

typedef struct 
{
  int semid;
  int index;
  int pid;
  Message *message;
} Worker;

#endif
