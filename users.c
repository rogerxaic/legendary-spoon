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

int main(){
  users();
  return(0);
}

int users(){

  signal(SIGINT,stop);
  signal(SIGHUP,stop);
  signal(SIGQUIT,stop);
printf("1\n");
  open_semaphore(keyMutex2);
  open_semaphore(keyPresence);
  int mq400 = open_mqueue(keyMqueue);
printf("2\n");
  while(1){
printf("3\n");
    int destination=0;
    int placeNumber=0;
    int myPid=getpid();
    Message *newMessage;
    newMessage=(Message*)malloc(sizeof(Message));
    	printf("4\n");
    fprintf(stdout,"Entrer la destination de vos rêve SVP:\n");
    scanf("%d\n",&destination);
    fprintf(stdout,"Entrer le nombre de places SVP:\n");
    scanf("%d\n",&placeNumber);
    newMessage->destination=destination;
    newMessage->number=placeNumber;
    newMessage->pid=myPid;
	printf("avant\n");
    down(keyMutex2);
	printf("apres\n");
    send_mqueue(mq400,&newMessage,sizeof(Message));
    pause();
    signal(SIGUSR1,replySignal);
    signal(SIGUSR2,replySignal);
  }

  return(0);
  
}

void replySignal(int sig){
  if(sig == SIGUSR1)
    fprintf(stdout,"Réservation confirmée");

  if(sig == SIGUSR2)
    fprintf(stdout,"Probème lors de la réservation");
}

void stop(){
  remove_semaphore(keyMutex2);
  remove_mqueue(keyMqueue);
  up(keyPresence);
  remove_semaphore(keyPresence);
  fprintf(stdout,"Processus Utilisateur arrété");
  exit(0);
}
