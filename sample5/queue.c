#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/msg.h>

// Определение структуры сообщения
typedef struct 
{
  long mtype;
  char mtext[1024];
} mymsgbuf;

int main()
{
  mymsgbuf msg;
  
  int qid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL);
  if (qid < 0) {
    perror("Couldnot create queue");
    exit(-1);
  }

  if (fork()) {           // Ответвление потомка
    msg.mtype = 1;                               // Формирование сообщения
    sprintf(msg.mtext, "Hello queue!");
    printf("Sending message: %s\n", msg.mtext);
    
    if (msgsnd(qid, &msg, sizeof(mymsgbuf) - sizeof(long), 0) < 0) {        // Отправка сообщения
      perror("Couldnot send message");
      exit(-1);
    }

    wait(0);

    if (msgctl(qid, IPC_RMID, NULL) < 0) {        // Освобождение очереди
      perror("Couldnot free queue");
      exit(-1);
    }
  }
  /* if (fork()) */
  else {                  // Потомок
    if (msgrcv(qid, &msg, sizeof(mymsgbuf) - sizeof(long), 0, 0) < 0) {    // Получение сообщения
      perror("Couldnot receive message");
      exit(-1);
    }

    printf("Received message: %s\n", msg.mtext);
  }
  
  return 0;
}
