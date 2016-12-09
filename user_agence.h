#ifndef __USER_AGENCE_H__
#define __USER_AGENCE_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "semaphore.h"
#include "mqueue.h"

typedef struct StructureMessage{
  int destination;
  int nombre;
  int pid;
} Message;

#endif
