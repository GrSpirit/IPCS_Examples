#ifndef MESSAGE_H
#define MESSAGE_H

#define MTEXT_SIZE 1024

#define TMSG 1
#define TEXIT 2

#define SNEW 1
#define SDONE 2

typedef struct 
{
  int type;
  int status;
  char text[MTEXT_SIZE];
} Message;

#endif