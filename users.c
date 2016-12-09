#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "agence.h"


int main(){



  return(0);
}

int users(){

  signal(SIGINT,arret);
  signal(SIGHUP,arret);
  signal(SIGQUIT,arret);

  while(1){
    int destination=0;
    int nombrePlaces=0;
    Message newMessage=0;

    fprintf(stdout,"Entrer la destination de vos rêve SVP:\n");
    scanf("%d\n",&destination);
    fprintf(stdout,"Entrer le nombre de places SVP:\n");
    scanf("%d\n",&nombrePlaces);
        
  }

  return(0);
}

 creerMessage(){
       
       }


