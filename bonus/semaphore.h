#ifndef SEMAPHORE_H
#define SEMAPHORE_H

int sem_init(int sem_count);
void sem_down(int semid, int index, int len);
int sem_down_nowait(int semid, int index, int len);
void sem_up(int semid, int num, int len);
void sem_free(int semid);

#endif