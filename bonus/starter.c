/* starter.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "queue.h"
#include "starter.h"

int main()
{
  int bpid;
  int queueid;
  char ifile_name[] = "input.txt";
  FILE *ifd;
  char line[MTEXT_SIZE];
  Message msg;

  queueid = msg_init();
  bpid = exec_balancer(queueid);
  printf("Started balancer (pid %d)\n", bpid);

  if (!(ifd = fopen(ifile_name, "r"))) {
    perror("Cannot open input file");
    exit(-1);
  }

  while (fgets(line, sizeof(line), ifd)) {
    fill_msg(&msg, TMSG, 0, line);
    msg_send(queueid, &msg);
    usleep(10);
  }
  fclose(ifd);

  // Send exit message
  fill_msg(&msg, TEXIT, 1, NULL);
  msg_send(queueid, &msg);

  wait(NULL);
  msg_free(queueid);
  return 0;
}

int exec_balancer(int queueid)
{
  char squeueid[20];
  int pid = fork();
  if (pid) return pid;

  sprintf(squeueid, "%d", queueid);
  if ((execl("multi", "multi", "-q", squeueid, NULL)) != 0) {
    fprintf(stderr, "Failed to execute balancer\n");
    exit(-1);
  }
  exit(0);
}

// Fill message structure
void fill_msg(Message *msg, int type, int status, char *text)
{
  bzero(msg, sizeof(Message));
  msg->type = type;
  msg->status = status;
  if (text) strcpy(msg->text, text);
}