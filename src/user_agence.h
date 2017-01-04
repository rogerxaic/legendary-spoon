#ifndef __USER_AGENCE_H__
#define __USER_AGENCE_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "semaphore.h"

typedef struct flight{
	char destination[21];
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
