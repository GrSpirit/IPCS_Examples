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

typedef struct 
{
  int semid;
  int shmid;
  void *shm_buffer;
} Globals;

void worker_run(Worker*);
int parse_params(int argc, char *argv[], int *queueid, int *proc_count);

void handler_exit(Message *msg);
void handler_default(Message *msg);

#endif
