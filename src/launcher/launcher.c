#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void helloFork(char *__command) {
    printf("[%s]\tLaunching (PID=%d)...\n",__command, getpid());
    for (int i = 0; i < 20; ++i) {
        sleep(1);
        printf("[%s]\t%2d\n", __command,i);
    }
}

int makeChild(char *__command) {
    int pid;
    if((pid=fork()) == 0) { //child code
        helloFork(__command);
        exit(0);
    } else if (pid == -1) { //error
        return -1;
    } else { //parent
        sleep(1);
        return pid;
    }
}



int main(int argc, char* argv[]) {
    printf("PARENT PID %d\n", getpid());
    printf("--------------------\n");

    makeChild("tirage");
    makeChild("display");
    makeChild("agence");

    //system("users");
    return 0;
}
