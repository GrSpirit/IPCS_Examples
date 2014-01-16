#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "shared_memory.h"

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

