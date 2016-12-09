#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "user_agence.h"


int main(){



  return(0);
}

int users(){

  signal(SIGINT,stop);
  signal(SIGHUP,stop);
  signal(SIGQUIT,stop);

  while(1){
    int destination=0;
    int placeNumber=0;
    int myPid=getpid();
    Message *newMessage;
    newMessage=(Message*)malloc(sizeof(Message));
    open_semaphore(key)

    fprintf(stdout,"Entrer la destination de vos rêve SVP:\n");
    scanf("%d\n",&destination);
    fprintf(stdout,"Entrer le nombre de places SVP:\n");
    scanf("%d\n",&placeNumber);
    newMessage->destination=destination;
    newMessage->number=placeNumber;
    newMessage->pid=myPid;
    

  }

  return(0);
}

 creerMessage(){
       
       }


