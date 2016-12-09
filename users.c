#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


int main(){



  return(0);
}

int user{

  signal(SIGINT,arret);
  signal(SIGHUP,arret);
  signal(SIGQUIT,arret);

  while(1){
    int destination=0;
    int nombrePlaces=0;

    fprintf(stdout,"Entrer la destination de vos rêve SVP:\n");
    scanf("%d\n",&destination);
    fprintf(stdout,"Entrer le nombre de places SVP:\n");
    scanf("%d\n",&nombrePlaces);
    
  }

  return(0);
}
