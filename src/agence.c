#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "user_agence.h"
#include "database.h"
#include "messages.h"
#include <string.h>

int message_id;
int semid_presence;
FlightEntry *db;

void cleanStop(int sig);

int prefix(const char *pre, const char *str);

void sendMessage(char *message) {
    MessageOut mout;
    mout.mtype = 1;
    strcpy(mout.msg, message);
    if (msgsnd(message_id, &mout, sizeof(mout.msg), IPC_NOWAIT) < 0) {
        perror("msgsnd\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, cleanStop);
    signal(SIGHUP, cleanStop);
    signal(SIGQUIT, cleanStop);

    //mutex 100 (AGENCE) <==> (DATABASE)
    int semid_mutex = create_semaphore(100);
    init_semaphore(semid_mutex, 1);

    //db 200
    int db200;
    FlightEntry *array;

    if ((db200 = shmget(200, 20 * sizeof(FlightEntry), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }

    if ((array = shmat(db200, (void *) 0, 0)) == (FlightEntry *) (-1)) {
        perror("shmat\n");
        exit(1);
    }
    db = array;


    //presence 350
    semid_presence = open_semaphore(350);

    //messagequeue 400
    int mq400;
    key_t key;
    message_buf rbuf;

    key = 400;

    if ((mq400 = msgget(key, 0666)) < 0) {
        perror("[AGENCE][ERROR] ***create mqueue***\n");
        exit(1);
    }

    message_id = msgget(450, IPC_CREAT|0666);

    //Agence
    Message message;
    Flight flight;
    while (1) {
        if (msgrcv(mq400, &rbuf, sizeof(Message), 1, 0) < 0) {
            perror("msgrcv\n");
            exit(1);
        }

        message = rbuf.msg;
        flight = message.flight;
        int placeNumber = flight.number;
        char destination[21] = "";
        strcpy(destination, flight.destination);

        down(semid_mutex);

        //lecture db
        int compteur = 0, found = 0;
        for (compteur = 0; compteur < 20; compteur++) {
            if (prefix(destination, (array + compteur)->name)) {
                found = 1;
                if (((array + compteur)->places) >= placeNumber) {
                    ((array + compteur)->places) = ((array + compteur)->places) - placeNumber;
                    kill(message.pid, SIGUSR1);
                    char str[100];
                    sprintf(str, "[AGENCE] Destination %s, # places %d", destination, placeNumber);
                    sendMessage(str);
                    break;
                } else {
                    kill(message.pid, SIGUSR2);
                    sendMessage("[AGENCE] Problème sur le nombre de places");
                    break;
                }
            }
        }

        if (!found) {
            kill(message.pid, SIGUSR2);
            sendMessage("[AGENCE] Problème pour la destination");
        }
        up(semid_mutex);
    }

    return 0;
}


void cleanStop(int sig) {
    //database detachment
    shmdt(db);

    //presence up
    up(semid_presence);

    fprintf(stdout, "[AGENCE] Processus arrêté\n");
    exit(0);
}

//compare les dest avec ce que l'on rentre, si correspondance retourne 1 sinon 0
int prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}
