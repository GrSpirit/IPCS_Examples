#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
  mymsgbuf msg1, msg2;
  
  int qid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL);
  if (qid < 0) {
    perror("Couldnot create queue");
    exit(-1);
  }

  msg1.mtype = 1;                               // Формирование сообщения
  sprintf(msg1.mtext, "Hello queue!");
  
  if (msgsnd(qid, &msg1, sizeof(mymsgbuf) - sizeof(long), 0) < 0) {        // Отправка
    perror("Couldnot send message");
    exit(-1);
  }

  if (msgrcv(qid, &msg2, sizeof(mymsgbuf) - sizeof(long), 0, 0) < 0) {    // Получение
    perror("Couldnot send message");
    exit(-1);
  }

  printf("Message: %s\n", msg2.mtext);        
  
  if (msgctl(qid, IPC_RMID, NULL) < 0) {        // Освобождение очереди
    perror("Couldnot free queue");
    exit(-1);
  }        // Освобождение очереди

  return 0;
}
