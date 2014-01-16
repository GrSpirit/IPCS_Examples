#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

int shm_init(size_t size);
void *shm_attach(int shmid);
void shm_free(int semid, void *buffer);

#endif