#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void cleanStop(int sig);

int main(int argc, char* argv[]) {
	signal(SIGINT, cleanStop);
	signal(SIGHUP, cleanStop);
	signal(SIGQUIT, cleanStop);

	//mutex 100 
	//mutex2 150
	//db 200
	//presence 350

	
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
