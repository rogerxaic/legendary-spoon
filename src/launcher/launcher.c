#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void helloFork(char *__command) {
    printf("[%s]\tLaunching (PID=%d)...\n",__command, getpid());
    system(__command);
}

int makeChild(char *__command) {
    int pid;
    if((pid=fork()) == 0) { //child code
        helloFork(__command);
        exit(0);
    } else if (pid == -1) { //error
        return -1;
    } else { //parent
        return pid;
    }
}



int main(int argc, char* argv[]) {
    printf("PARENT PID %d\n", getpid());
    printf("--------------------\n");

    makeChild("./tirage");
    sleep(2);
    makeChild("./receiver");
    makeChild("./agence");
    system("./display");
    return 0;
}
