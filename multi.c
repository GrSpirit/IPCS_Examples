/* multi.c */

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "multi.h"
#include "semaphore.h"
#include "shared_memory.h"
#include "queue.h"

int main(int argc, char *argv[])
{
  int i;
  int pid;
  int semid;
  int shmid;
  int queueid = -1;
  void *buffer;
  int proc_count = 2;
  Worker *childs;
  Message qmsg;

  if (parse_params(argc, argv, &queueid, &proc_count)) {
    exit(0);
  }

  if (proc_count < 1 || proc_count > 10) {
    fprintf(stderr, "ERROR: Bad process count: %d\n", proc_count);
    exit(-1);
  }
  printf("Process count = %d\n", proc_count);

  childs = (Worker*)calloc(proc_count, sizeof(Worker));

  semid = sem_init(proc_count);
  shmid = shm_init(proc_count * sizeof(Message));
  buffer = shm_attach(shmid);

  for (i = 0; i < proc_count; ++i) {
    childs[i].semid = semid;
    childs[i].index = i;
    childs[i].message = (Message*)buffer + i;
    bzero(childs[i].message, sizeof(Message));
    if ((pid = fork()) == 0) {
      childs[i].pid = getpid();
      worker_run(&childs[i]);
    }
    else {
      printf("Created child %d\n", pid);
      childs[i].pid = pid;
    }
  }

  while(1) {
    msg_recv(queueid, &qmsg);
    for (i = 0; i < proc_count; ++i) {
      if (!sem_down_nowait(semid, i, 1)) {
        // Message processed
        if (childs[i].message->status == SDONE) {
          bzero(childs[i].message, sizeof(Message));
        }
        memcpy(childs[i].message, &qmsg, sizeof(Message));
        //sprintf(childs[i].message->text, "Test %d", i);
        //childs[i].message->type = TMSG;
        //childs[i].message->status = SNEW;
        sem_up(semid, i, 2);
      }
    }
  }

  for (i = 0; i < proc_count; ++i) {
    waitpid(childs[i].pid, NULL, 0);
  }

  shm_free(shmid, buffer);
  sem_free(semid);
  free(childs);
  return 0;
}

void worker_run(Worker *worker)
{
  FILE *fd;
  char file_name[255];
  Message *msg = worker->message;
  sprintf(file_name, "worker%d.dmp", worker->index);
  while(1) {
    sem_down(worker->semid, worker->index, 2);
    switch (msg->type) {
      case TEXIT:  // Exit
        exit(0);
      case TMSG:   // Process message
        if (!(fd = fopen(file_name, "a"))) {
          perror("Can\'t open file: ");
          exit(-1);
        }
        if (fprintf(fd, "Received message: %s\n", msg->text) < 0) {
          perror("Can\'t write to file: ");
          exit(-1);
        }
        msg->status = SDONE;
        if (fclose(fd) != 0) {
          perror("Can\'t close file: ");
          exit(-1);
        }
    }
    sem_up(worker->semid, worker->index, 1);
  }
}

int parse_params(int argc, char *argv[], int *queueid, int *proc_count)
{
  int opt;
  int qfnd = 0;
  const char usage_str[] = "Usage: multi -q queue_id [-p process_count]\n";
  while ((opt = getopt(argc, argv, "qp")) != -1) {
    switch(opt) {
      case 'q':
        *queueid = atoi(optarg);
        if (*queueid <= 0) {
          fprintf(stderr, "Wrong queue identifier: %s\n", optarg);
          return -1;
        }
        qfnd = 1;
        break;
      case 'p':
        *proc_count = atoi(optarg);
        if (*proc_count < 1 || *proc_count > 10) {
          fprintf(stderr, "Wrong process count: %s\n", optarg);
          return -1;
        }
        break;
      default: /* '?' */
       fprintf(stderr, usage_str);
       return -2;
    }
  }

  if (qfnd == 0) {
    fprintf(stderr, usage_str);
    return -1;
  }
  return 0;
}