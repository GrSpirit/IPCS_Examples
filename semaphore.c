#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>

#include "semaphore.h"

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
  if (semid && semctl(semid, 0, IPC_RMID) < 0) {
    perror("Failed to delete semaphore: ");
    exit(-1);
  }
}
