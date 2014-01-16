/* multi.h */
#ifndef MULTI
#define MULTI
#define MESSAGE_SIZE 1024

#define TMSG 1
#define TEXIT 2

#define SNEW 1
#define SDONE 2

typedef union semun
{
  int val;
  struct semid_ds *buf;
  ushort * array;
} semun_t;

typedef struct 
{
  int type;
  int status;
  char text[MESSAGE_SIZE];
} Message;

typedef struct 
{
  int semid;
  int index;
  int pid;
  Message *message;
} Worker;

#endif
