#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "user_agence.h"

void cleanStop(int sig);

int main(int argc, char* argv[]) {
	signal(SIGINT, cleanStop);
	signal(SIGHUP, cleanStop);
	signal(SIGQUIT, cleanStop);

	//mutex 100 
	create_semaphore(100);
	init_semaphore(100,1);

	//mutex2 150
	create_semaphore(150);
	init_semaphore(150,1);

	//db 200


	//presence 350

	//messagequeue 400
	int mq400;
	if((mq400=create_mqueue(400))<0) {
		perror("[ERROR] ***create mqueue***\n");
		exit(1);
	}

	Message msg;
	while(1) {
		rcv_mqueue(mq400, &msg, sizeof(Message));
		printf("%d\n", (int)sizeof(Message));
		//if(msgrcv(mq400, &msg, sizeof(Message), 1, 0) < 0) {
			//perror("[ERROR] ***msgrcv***\n");
			//exit(1);
		//}*/
		printf("Msg recu\n");
		printf("msg.pid=%d\n", msg.pid);

		down(100);
			//lecture db
		if(1) {
			kill(msg.pid, SIGUSR1);
		} else {
			kill(msg.pid, SIGUSR2);
		}
		up(100);
	}
	
	return 0;
}


void cleanStop(int sig) {
	//database detachement
	

	//message queue detachement
	

	//presence up
	

	//presence detachement
	

	fprintf(stdout, "[AGENCE]\t***Agence process stopped ***\n");
	exit(0);
}
