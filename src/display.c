#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include "semaphore.h"
#include "database.h"

void cleanStop(int sig);

int main (int argc, char* argv[]) {
	signal(SIGINT, cleanStop);
	signal(SIGHUP, cleanStop);
	signal(SIGQUIT, cleanStop);

	//mutex 100 (DISPLAY) <==> (DATABASE)
	open_semaphore(100);

	//db 200
	key_t db = 200;
	int db200;
	FlightEntry *array;

	if ((db200 = shmget(db, 20 * sizeof(FlightEntry), IPC_CREAT | 0666)) < 0) {
		perror("shmget\n");
		exit(1);
	}

	if ((array = shmat(db200, (void *)0, 0)) == (FlightEntry *) (-1)) {
		perror("shmat\n");
		exit(1);
	}

	//semaphore flight 300
	open_semaphore(300);

	//presence 350
	open_semaphore(350);

	while(1) {
		down(100);
		int i;
		for(i=0;i<20;i++){
			printf("name: %s\t",(*(array+i)).name);
            printf("places: %d\n", (*(array+i)).places);
            //
		}
		up(100);
        printf("*****************\n");
		sleep(10);
	}
	return 0;
}

void cleanStop(int sig) {
	//
	

	//database detachment
	

	//presence up
	up(350);

	//presence detachment
//	remove_semaphore(350);

	fprintf(stdout, "[DISPLAY] \t*** Display process stopped ***\n");
	exit(0);
}
