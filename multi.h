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

void worker_run(Worker*);
int parse_params(int argc, char *argv[], int *queueid, int *proc_count);

#endif
