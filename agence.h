#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#ifndef __AGENCE_H__
#define __AGENCE_H__

typedef struct StructureMessage{
  int destination;
  int nombre;
  int pid;
}Message;

void cleanStop(int sig);

#endif
