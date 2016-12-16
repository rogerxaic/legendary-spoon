#ifndef __USER_AGENCE_H__
#define __USER_AGENCE_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "semaphore.h"
#include "mqueue.h"

#define MSGSZ 128

typedef struct StructureMessage{
  int destination;
  int number;
  int pid;
} Message;

typedef struct msgbuf {
	long mtype;
	char mtext[MSGSZ];
} message_buf;

#endif
