#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "user_agence.h"
#include "database.h"
#include <string.h>

void cleanStop(int sig);
int prefix(const char *pre, const char *str);

int main(int argc, char* argv[]) {
	signal(SIGINT, cleanStop);
	signal(SIGHUP, cleanStop);
	signal(SIGQUIT, cleanStop);

	//mutex 100 (AGENCE) <==> (DATABASE)
	int semid_mutex = create_semaphore(100);
	init_semaphore(semid_mutex,1);

	//mutex2 150 (USERS) <==> (AGENCE)
	int semid_mutex2 = create_semaphore(150);
	init_semaphore(semid_mutex2,1);

	//db 200
    int db200;
    FlightEntry *array;

    if ((db200 = shmget(200, 20 * sizeof(FlightEntry), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }

    if ((array = shmat(db200, (void *)0, 0)) == (FlightEntry *) (-1)) {
        perror("shmat\n");
        exit(1);
    }


	//presence 350
	int semid_presence = open_semaphore(350);

	//messagequeue 400
	int mq400;
	key_t key;
	message_buf rbuf;

	key = 400;

	if((mq400=msgget(key, 0666)) < 0) {
		perror("[ERROR] ***create mqueue***\n");
		exit(1);
	}

	//Agence
	Message message;
	Flight flight;
	while(1) {
		down(semid_mutex2);
		if (msgrcv(mq400, &rbuf, sizeof(Message), 1, 0) < 0) {
			perror("msgrcv");
			exit(1);
		}
		up(semid_mutex2);

		message = rbuf.msg;
		flight = message.flight;
		int placeNumber = flight.number;
		char destination[21]="";
		strcpy(destination,flight.destination);

		printf("Msg recu\n");

		printf("*** AGENCE *** je fais down\n");
		down(semid_mutex);

		//lecture db
		int compteur=0;
		for(compteur=0; compteur<20;compteur++){
			if(prefix(destination,(array+compteur)->name)==0){
				if(((array+compteur)->places)>=placeNumber){
					((array+compteur)->places)=((array+compteur)->places)-placeNumber;
					kill(message.pid, SIGUSR1);
					printf("Destination %s, # places %d\n", destination, placeNumber);
				}else {
					kill(message.pid, SIGUSR2);
					printf("Problème sur le nombre de places");
				}
			} else {
				kill(message.pid, SIGUSR2);
				printf("Problème pour la destination");
			}
			printf("*** AGENCE *** je fais up\n");
			up(semid_mutex);
		}
	}

	return 0;
}


void cleanStop(int sig) {
	//database detachment

	//message queue detachment


	//presence up
	up(350);

	//presence detachment
//	remove_semaphore(350);

	fprintf(stdout, "[AGENCE]\t***Agence process stopped ***\n");
	exit(0);
}

//compare les dest avec ce que l'on rentre, si correspondance retourne 0 sinon 1
int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}
