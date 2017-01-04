#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "user_agence.h"
#include "database.h"

void cleanStop(int sig);

int main(int argc, char* argv[]) {
	signal(SIGINT, cleanStop);
	signal(SIGHUP, cleanStop);
	signal(SIGQUIT, cleanStop);

	//mutex 100 (AGENCE) <==> (DATABASE)
	create_semaphore(100);
	init_semaphore(100,1);

	//mutex2 150 (USERS) <==> (AGENCE)
	create_semaphore(150);
	init_semaphore(150,1);

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
	open_semaphore(350);

	//messagequeue 400
	int mq400;
	key_t key;
	message_buf rbuf;

	key = 400;

	if((mq400=msgget(key, 0666)) < 0) {
		perror("[ERROR] ***create mqueue***\n");
		exit(1);
	}

	Message message;
	Flight flight;
	while(1) {
		down(150);
		if (msgrcv(mq400, &rbuf, sizeof(Message), 1, 0) < 0) {
			perror("msgrcv");
			exit(1);
		}
		up(150);
//		rcv_mqueue(mq400, &msg, sizeof(Message));
//		printf("%d\n", (int)sizeof(Message));
		//if(msgrcv(mq400, &msg, sizeof(Message), 1, 0) < 0) {
			//perror("[ERROR] ***msgrcv***\n");
			//exit(1);
		//}*/
		message = rbuf.msg;
		flight = message.flight;
		printf("Msg recu\n");
//		printf("msg.text=%s\n", rbuf.mtext);



		printf("*** AGENCE *** je fais down\n");
		down(100);
			//lecture db
		if(1) {
			kill(message.pid, SIGUSR1);
			printf("Destination %d, # places %d\n", flight.destination, flight.number);
		} else {
			kill(message.pid, SIGUSR2);
		}
		printf("*** AGENCE *** je fais up\n");
		up(100);
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
