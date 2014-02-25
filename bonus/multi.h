/* multi.h */
#ifndef MULTI
#define MULTI

#include "message.h"
#define HANDLERS_COUNT 2

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
  int count;
  Worker *worker;
} ChildList;

typedef struct 
{
  int semid;
  int shmid;
  void *shm_buffer;
} Globals;

typedef void (*handler_fun)(ChildList*, Message*);
typedef struct 
{
  int id;
  handler_fun func;
} Handler;

void worker_run(Worker*);
int parse_params(int argc, char *argv[], int *queueid, int *proc_count);
void free_resources();

void handler_exit(ChildList *childs, Message *msg);
void handler_default(ChildList *childs, Message *msg);

#endif
