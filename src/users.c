#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "user_agence.h"
#include <unistd.h>
#include <string.h>

int keyMutex2 = 150;
int keyPresence = 350;
int keyMqueue = 400;
int semid_presence;
int semid_mutex2;

void replySignal(int sig);
void stop();
int users();

int main() {
    users();
    return (0);
}

int users() {

    signal(SIGINT, stop);
    signal(SIGHUP, stop);
    signal(SIGQUIT, stop);

    semid_mutex2 = open_semaphore(keyMutex2);
    semid_presence = open_semaphore(keyPresence);
    int mq400;

    int msgflg = IPC_CREAT | 0666;

    Flight flight;
    Message message;
    message_buf sbuf;

    if ((mq400 = msgget(keyMqueue, msgflg)) < 0) {
        perror("msgget\n");
        exit(1);
    }

    while (1) {

        char destination[21] = "";
        int placeNumber = 0;
        int myPid = getpid();


        fprintf(stdout, "Entrer la destination de vos rêve SVP:\n");
        scanf("%s", destination);
        fprintf(stdout, "la destination est :%s\n", destination);
        fprintf(stdout, "Entrer le nombre de places SVP:\n");
        scanf("%d", &placeNumber);
        strcpy(flight.destination, destination);
        flight.number = placeNumber;

        message.flight = flight;
        message.pid = myPid;

        sbuf.mtype = 1;
        sbuf.msg = message;

        down(semid_mutex2);
        if (msgsnd(mq400, &sbuf, sizeof(sbuf.msg), IPC_NOWAIT) < 0) {
            perror("msgsnd\n");
            exit(1);
        }

        up(semid_mutex2);

        signal(SIGUSR1, replySignal);
        signal(SIGUSR2, replySignal);
        pause();
    }

    return (0);
}

void replySignal(int sig) {
    if (sig == SIGUSR1)
        fprintf(stdout, "Réservation confirmée\n");

    if (sig == SIGUSR2)
        fprintf(stdout, "Probème lors de la réservation\n");
}

void stop() {
    up(semid_presence);
    fprintf(stdout, "Processus Utilisateur arrété\n");
    exit(0);
}

/*void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}*/
