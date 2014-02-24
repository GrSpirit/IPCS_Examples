#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>

int main()
{
  const int memsize = 1024;
  char *buf;
  key_t shmkey;
  int shmid;

  shmkey = ftok("shm2.c", 0);    // Получение ключа
  if (shmkey < 0) {
    perror("Couldnot get file token");
    exit(-1);
  }
  
  shmid = shmget(shmkey, memsize, 0666); // Получения идентификатора общей памяти
  if (shmid < 0) { 
    perror("Couldnot create shared memory");
    exit(-1);
  }

  buf = (char*)shmat(shmid, NULL, 0);  // Получение адреса буфера
  if (buf == (char *)(-1)) { 
    perror("Couldnot attach to shared memory");
    exit(-1);
  }

  printf("Message: %s\n", buf);        // Использование памяти
  
  if (shmdt(buf) < 0) {               // Освобождение буфера
    perror("Couldnot detach shared memory");
    exit(-1);
  }

  if (shmctl(shmid, IPC_RMID, NULL) < 0) { // Удаление общей памяти
    perror("Couldnot free shared memory");
    exit(-1);    
  }
  
  return 0;
}