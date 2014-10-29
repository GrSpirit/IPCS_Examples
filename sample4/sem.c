#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>

int main()
{
  FILE *fd;
  struct sembuf op;
  int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT | IPC_EXCL);  // Создание семафора

  if (semctl(semid, 0, SETVAL, 1) < 0)  {                           // Установка начального значения семафора
      perror("Failed to set semaphore value");
      exit(-1);
  }

  int pid = fork();
  srandom((uint)time(NULL) + pid);
  usleep(random()%1000);

  if (pid) {    // Дочерний процесс
      op = (struct sembuf){.sem_num = 0, .sem_op = -1, .sem_flg = 0};
      semop(semid, &op, 1);    // Занять семафор

      fd = fopen("semout.txt", "a");
      fprintf(fd, "Child writes\n");
      fclose(fd);

      op = (struct sembuf){.sem_num = 0, .sem_op = 1, .sem_flg = 0};
      semop(semid, &op, 1);    // Освободить семафор
  } 
  else {         // Родительский процесс
      op = (struct sembuf){.sem_num = 0, .sem_op = -1, .sem_flg = 0};
      semop(semid, &op, 1);    // Занять семафор

      fd = fopen("semout.txt", "a");
      fprintf(fd, "Parent writes\n");
      fclose(fd);

      op = (struct sembuf){.sem_num = 0, .sem_op = 1, .sem_flg = 0};
      semop(semid, &op, 1);    // Освободить семафор
      sleep(2);
      semctl(semid, 1, IPC_RMID);    // Удалить семафор
  }

  return 0;
}