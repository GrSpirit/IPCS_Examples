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

int sem_init(int sem_count);
void sem_down(int semid, int index, int len);
int sem_down_nowait(int semid, int index, int len);
void sem_up(int semid, int num, int len);
void sem_free(int semid);

int shm_init(size_t size);
void *shm_attach(int shmid);
void shm_free(int semid, void *buffer);

#endif
