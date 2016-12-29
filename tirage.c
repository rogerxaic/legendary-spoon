#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "user_agence.h"
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "lib/semaphore.h"
#include "lib/shmem.h"
#include "mqueue.h"

int key_mutex = 100;
int key_mutex2 = 150;
int key_database = 200;
int key_vols = 300;
int key_presence = 350;
int key_mqueue = 400;
int size_shmem = 20;

int rand_flight(int a, int b);
int tirage();
void stopTirage();
int ecrivain(int descripteur[2]);
void stopEcrivain();

int tirage(){
	signal(SIGINT,stopTirage);
	signal(SIGHUP,stopTirage);
	signal(SIGQUIT,stopTirage);

	Flight newflight;

	int pid, descripteur[2], destination, nb_places;
	int destMin = 1;
	int destMax = 20;
	int placeMin = 100;
	int placeMax = 500;

	//creation et initialisation sémaphore Mutex à 1
	int semid_Mutex = create_semaphore(key_mutex);
	if (semid_Mutex == -1)
		exit(1);
        init_semaphore(semid_Mutex, 1);

	//création et initialisation sémaphore Mutex2 à 1
	int semid_Mutex2 = create_semaphore(key_mutex2);
        if (semid_Mutex2 == -1)
                exit(1);
        init_semaphore(semid_Mutex2, 1);

	//Création de la BD
	int shmem_id = create_shmem(key_database, size_shmem);
	if (shmem_id == -1)
		exit(1);

	//création et initialisation sémaphore vols à 20
	int semid_vols = create_semaphore(key_presence);
        if (semid_vols == -1)
                exit(1);
        init_semaphore(semid_vols, 20);

	//création et initialisation sémaphore présence à -3
	int semid_presence = create_semaphore(key_presence);
        if (semid_presence == -1)
                exit(1);
	init_semaphore(semid_presence, -3);

	//création de la file de message
	int mqueue_id = create_mqueue(key_mqueue);
	if (mqueue_id == -1)
		exit(1);

	/*création du pipe avec prise en compte erreur création ou erreur fork
	 cas du fils et du père
	 si c'est le père on tire un vol, le met dans la struct, l'envoie a travers le pipe
	 puis attend 10 secondes et recommence */

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
			destination = rand_flight(destMin,destMax);
			nb_places = rand_flight(placeMin,placeMax);
			newflight.destination = destination;
			newflight.number = nb_places;
			write(descripteur[1],&newflight,sizeof(Flight));
			sleep(10);
		}
		close(descripteur[1]);
	}
}

void stopTirage(){
	//envoie un sigint aux autres processus
	//attend la mort de son fils
	down(key_presence);
	//Est ce qu'il faut fermer le pipe malgré qu'on ferme les accès dans les fcts
	//Est ce qu'on détruit un pipe ??
	remove_semaphore(key_mutex);
	remove_semaphore(key_mutex2);
	remove_shmem(key_database);
	remove_semaphore(key_vols);
	remove_mqueue(key_mqueue);
	remove_semaphore(key_presence);
	printf("Processus Tirage arrêté\n");
	exit(0);
}

int ecrivain(int descripteur[2]){

	signal(SIGINT, stopEcrivain);
	signal(SIGHUP,stopEcrivain);
	signal(SIGQUIT,stopEcrivain);

	Flight newflight;

	open_semaphore(key_mutex);
	open_shmem(key_database,size_shmem);
	open_semaphore(key_vols);
	open_semaphore(key_presence);

	close(descripteur[1]);
	while(1){
		read(descripteur[0],&newflight,sizeof(Flight));
		printf("La destination est %d\n",newflight.destination);
		printf("Le nombre de place disponible est %d\n",newflight.number);
	}
	close(descripteur[0]);
}

void stopEcrivain(){
	remove_semaphore(key_mutex);
	remove_shmem(key_database);
	remove_semaphore(key_vols);
	//même question pour le pipe en lecture
	up(key_presence);
	remove_semaphore(key_presence);
	printf("Processus Ecrivain arrêté\n");
	exit(0);
}

int rand_flight(int a, int b){

        srand(time(NULL));
        int number = (rand()%(b-a))+a;
        return number;
}

int main(){
tirage();
return 0;
}

