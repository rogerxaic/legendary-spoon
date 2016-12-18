#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "user_agence.h"
#include <time.h>

int rand_flight(int a, int b){

	srand(time(NULL));
	int number = (rand()%(b-a))+a;
	return number;
}

int ecrivain();

int tirage(){

//	signal(SIGINT,stopTirage);
//	signal(SIGHUP,stopTirage);
//	signal(SIGQUIT,stopTirage);

	Flight newflight;

	int pid, descripteur[2], destination, nb_places;
	int a = 1;
	int b = 20;
	int c = 100;
	int d = 500;

	if(pipe(descripteur) < 0){
		printf("erreur création pipe \n");
		exit(-1);
	}

	if((pid = fork())<0){
		printf("fork failed\n");
		return -1;
	}
	if(pid == 0){ //child
		ecrivain(descripteur);
	}
	else{  //parents
		close(descripteur[0]);
		while(1){

			printf("Je suis Tirage et je tire un vol aléatoire\n");
			destination = rand_flight(a,b);
			nb_places = rand_flight(c,d);
			newflight.destination = destination;
			newflight.number = nb_places;
			printf("%d \n",newflight.destination);
			printf("%d \n",newflight.number);

			write(descripteur[1],&newflight,sizeof(Flight));
			sleep(10);
		}
		close(descripteur[1]);
	}
}



int ecrivain(int descripteur[2]){

//	signal(SIGINT, stop);
//      signal(SIGHUP,stop);
//    signal(SIGQUIT,stop);
Flight newflight;
//newflight = (Flight*)malloc(sizeof(Flight));

	close(descripteur[1]);
	while(1){
		read(descripteur[0],&newflight,sizeof(Flight));
		printf("La destination est %d\n",newflight.destination);
		printf("Le nombre de place disponible est %d\n",newflight.number);
	}
	close(descripteur[0]);

}

int main(){
tirage();
return 0;
}
