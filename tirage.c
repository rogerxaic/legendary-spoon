#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int tirage(){

	signal(SIGINT, stop);
	signal(SIGHUP,stop);
	signal(SIGQUIT,stop);

	int pid, descripteur[2];
	if(pipe(descripteur[2])== -1);{
		printf("erreur création pipe\n",);
		exit(-1);
	}

	if((pid = fork()<0){
		printf("fork failed\n");
		return -1;
	}
	if(pid == 0){ //child
		close(descripteur[1]);
	}
	else{  //parents
		close(descripteur[0]);
		printf("Je suis Tirage et je tire un vol aléatoire\n");
		
	}
	close(descripteur[0]);

}
