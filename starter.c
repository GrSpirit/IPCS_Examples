/* starter.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "queue.h"

int exec_balancer(int queueid);

int main()
{
  int queueid;
  char ifile_name[] = "input.txt";
  FILE *ifd;
  char line[MTEXT_SIZE];
  Message msg;

  queueid = msg_init();
  exec_balancer(queueid);

  if (!(ifd = fopen(ifile_name, "r"))) {
    perror("Cannot open input file");
    exit(-1);
  }

  while (fgets(line, sizeof(line), ifd)) {
    bzero(&msg, sizeof(Message));
    msg.type = 1;
    msg.status = 1;
    strcpy(msg.text, line);
    msg_send(queueid, &msg);
    usleep(10);
  }
  fclose(ifd);

  msg_free(queueid);
  return 0;
}

int exec_balancer(int queueid)
{
  char squeueid[20];
  sprintf(squeueid, "%d", queueid);
  if (fork())
    return 0;
  if ((execl("multi", "multi", "-q", squeueid, NULL)) != 0) {
    fprintf(stderr, "Failed to execute balancer\n");
    exit(-1);
  }
  exit(0);
}