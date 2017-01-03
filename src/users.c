#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "user_agence.h"
#include <unistd.h>

int keyMutex2=150;
int keyPresence=350;
int keyMqueue=400;


void replySignal(int sig);
void stop();
int users();

int main(){
  users();
  return(0);
}

int users(){

  signal(SIGINT,stop);
  signal(SIGHUP,stop);
  signal(SIGQUIT,stop);

  open_semaphore(keyMutex2);
  open_semaphore(keyPresence);
  int mq400;

  int msgflg = IPC_CREAT | 0666;
  key_t key;
  Flight flight;
  Message message;
  message_buf sbuf;

  key = 400;

  if ((mq400 = msgget(key, msgflg)) < 0 ) {
    perror("msgget\n");
    exit(1);
  }

  while(1){

    int destination=0;
    int placeNumber=0;
    int myPid=getpid();

    fprintf(stdout,"Entrer la destination de vos rêve SVP:\n");
    scanf("%d",&destination);
    fprintf(stdout,"Entrer le nombre de places SVP:\n");
    scanf("%d",&placeNumber);
    flight.destination = destination;
    flight.number = placeNumber;

    message.flight = flight;
    message.pid = myPid;

    sbuf.mtype = 1;
    sbuf.msg = message;

    down(keyMutex2);
    if (msgsnd(mq400, &sbuf, sizeof(sbuf.msg), IPC_NOWAIT) < 0) {
        perror("msgsnd\n");
        exit(1);
    }

    up(keyMutex2);

    signal(SIGUSR1,replySignal);
    signal(SIGUSR2,replySignal);
    pause();
  }

  return(0);
}

void replySignal(int sig){
  if(sig == SIGUSR1)
    fprintf(stdout,"Réservation confirmée\n");

  if(sig == SIGUSR2)
    fprintf(stdout,"Probème lors de la réservation\n");
}

void stop(){
  remove_semaphore(keyMutex2);
  up(keyPresence);
  remove_semaphore(keyPresence);
  fprintf(stdout,"Processus Utilisateur arrété\n");
  exit(0);
}
