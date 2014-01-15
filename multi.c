/* multi.c */

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "multi.h"

void worker_run(Worker*);

int main(int argc, char *argv[])
{
  int i;
  int pid;
  int semid;
  int shmid;
  void *buffer;
  int proc_count = (argc < 2) ? 2 : atoi(argv[1]);
  Worker *childs;

  if (proc_count < 1 || proc_count > 10) {
    fprintf(stderr, "ERROR: Bad process count: %d\n", proc_count);
    exit(-1);
  }
  printf("Process count = %d\n", proc_count);

  childs = (Worker*)calloc(proc_count, sizeof(Worker));

  semid = sem_init(proc_count);
  shmid = shm_init(proc_count * sizeof(Message));
  buffer = shm_attach(shmid);

  for (i = 0; i < proc_count; ++i) {
    childs[i].semid = semid;
    childs[i].index = i;
    childs[i].message = (Message*)buffer + i;
    bzero(childs[i].message, sizeof(childs[i].message));
    if ((pid = fork()) == 0) {
      childs[i].pid = getpid();
      worker_run(&childs[i]);
    }
    else {
      printf("Created child %d\n", pid);
      childs[i].pid = pid;
    }
  }

  for (i = 0; i < proc_count; ++i) {
    if (!sem_down_nowait(semid, i, 1)) {
      // Message processed
      if (childs[i].message->status == SDONE) {
        bzero(childs[i].message, sizeof(childs[i].message));
      }
      sprintf(childs[i].message->text, "Test %d", i);
      childs[i].message->type = TMSG;
      childs[i].message->status = SNEW;
      sem_up(semid, i, 2);
    }
  }

  for (i = 0; i < proc_count; ++i) {
    waitpid(childs[i].pid, NULL, 0);
  }

  shm_free(shmid, buffer);
  sem_free(semid);
  free(childs);
  return 0;
}

// Create semaphore
int sem_init(int sem_count)
{
  int i;
  int semid;
  if ((semid = semget(IPC_PRIVATE, sem_count, 0660 | IPC_CREAT)) < 0) {
    perror("Couldn\'t create semaphore: ");
    exit(-1);
  }

  // Set initial semaphore value
  for (i = 0; i < sem_count; ++i) {
    if (semctl(semid, i, SETVAL, 1) < 0)  {
      perror("Failed to set semaphore value: ");
      exit(-1);
    }
  }
  return semid;  
}
// Lock semaphore
void sem_down(int semid, int index, int len)
{
  struct sembuf op;
  op.sem_num = index;
  op.sem_op = -1 * len;
  op.sem_flg = 0;

  if (semop(semid, &op, 1) < 0) {
    perror("Failed to down semaphore: ");
    exit(-1);
  }
}

// Lock semaphore
int sem_down_nowait(int semid, int index, int len)
{
  int errno;
  struct sembuf op;
  op.sem_num = index;
  op.sem_op = -1 * len;
  op.sem_flg = IPC_NOWAIT;

  if (semop(semid, &op, 1) < 0) {
    if (errno == EAGAIN) return 1;
    perror("Failed to down semaphore: ");
    exit(-1);
  }
  return 0;
}

// Unlock semaphore
void sem_up(int semid, int num, int len)
{
  struct sembuf op;
  op.sem_num = num;
  op.sem_op = len;
  op.sem_flg = 0;

  if (semop(semid, &op, 1) < 0) {
    perror("Failed to up semaphore: ");
    exit(-1);
  }
}

// Release semaphore
void sem_free(int semid)
{
  if (semctl(semid, 0, IPC_RMID) < 0) {
    perror("Failed to delete semaphore: ");
    exit(-1);
  }
}

// Create shared memory
int shm_init(size_t size)
{
  int shmid;
  if ((shmid = shmget(IPC_PRIVATE, size, 0660 | IPC_CREAT)) < 0) {
    perror("Failed to initiate shared memory: ");
    exit(-1);
  }
  return shmid;
}

// Attach to shared memory
void *shm_attach(int shmid)
{
  void *buffer;
  if ((buffer = shmat(shmid, NULL, 0)) == (void*)(-1)) {
    perror("Failed to attach to shared memory: ");
    exit(-1);
  }
  return buffer;
}

// Free shared memory
void shm_free(int shmid, void *buffer)
{
  if (shmdt(buffer) < 0) {
    perror("Failed to detach shared memory: ");
    exit(-1);
  }

  if (shmctl(shmid, IPC_RMID, NULL) < 0) {
    perror("Failed to delete shared memory: ");
    exit(-1);
  }
}

void worker_run(Worker *worker)
{
  FILE *fd;
  char file_name[255];
  Message *msg = worker->message;
  sprintf(file_name, "worker%d.dmp", worker->index);
  while(1) {
    sem_down(worker->semid, worker->index, 2);
    switch (msg->type) {
      case TEXIT:  // Exit
        exit(0);
      case TMSG:   // Process message
        if (!(fd = fopen(file_name, "a"))) {
          perror("Can\'t open file: ");
          exit(-1);
        }
        if (fprintf(fd, "Received message: %s\n", msg->text) < 0) {
          perror("Can\'t write to file: ");
          exit(-1);
        }
        msg->status = SDONE;
        if (fclose(fd) != 0) {
          perror("Can\'t close file: ");
          exit(-1);
        }
    }
    sem_up(worker->semid, worker->index, 1);
  }
}