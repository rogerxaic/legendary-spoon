#ifndef __USER_AGENCE_H__
#define __USER_AGENCE_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lib/semaphore.h"

typedef struct flight{
	int destination;
	int number;
} Flight;

typedef struct message {
	Flight flight;
	int pid;
} Message;

typedef struct msgbuf {
	long mtype;
	Message msg;
} message_buf;

#endif
