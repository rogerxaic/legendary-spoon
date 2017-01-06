#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "semaphore.h"
#include "database.h"
#include "messages.h"


FlightEntry *db_200;
MessageEntry *messages;
int semid_350;
int messages_id;

void cleanStop(int sig);

void printTableSeparator();

int main(int argc, char *argv[]) {
    signal(SIGINT, cleanStop);
    signal(SIGHUP, cleanStop);
    signal(SIGQUIT, cleanStop);

    //mutex 100 (DISPLAY) <==> (DATABASE)
    int semid_100 = open_semaphore(100);

    //db 200
    key_t db = 200;
    int db200;
    FlightEntry *array;
    MessageEntry *messages_db;

    if ((db200 = shmget(db, 20 * sizeof(FlightEntry), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }
    if ((array = shmat(db200, (void *) 0, 0)) == (FlightEntry *) (-1)) {
        perror("shmat\n");
        exit(1);
    }
    db_200 = array;

    if ((messages_id = shmget(db, 20 * sizeof(FlightEntry), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }
    if ((messages_db = shmat(messages_id, (void *) 0, 0)) == (MessageEntry *) (-1)) {
        perror("shmat\n");
        exit(1);
    }
    messages = messages_db;

    //semaphore flight 300
    int semid_300 = open_semaphore(300);

    //presence 350
    semid_350 = open_semaphore(350);

    int semid_550 = open_semaphore(550);

    while (1) {
        down(semid_550);
        printf("\033[H");
        printTableSeparator();
        printf("| NAME");
        for (int j = 0; j < 17; j++) printf(" ");
        printf("| PLACES | \033[K\n");
        printTableSeparator();

        down(semid_100);
        int i;
        for (i = 0; i < 20; i++) {
            if ((array + i)->places != 0) {
                printf("| %s |", (*(array + i)).name);
                printf(" %3d    | \033[K\n", (*(array + i)).places);
            } else { //emptying the empty flight
                strcpy((array + i)->name, "");
                if (i != 19) {
                    strcpy((array + i)->name, (array + i + 1)->name);
                    (array + i)->places = (array + i + 1)->places;
                    (array + i + 1)->places = 0;
                }
            }
        }
        up(semid_100);
        printTableSeparator();
        printf("\n");


        up(semid_550);

        printf("\033[46;0H\n");
        sleep(1);
    }

    return 0;
}

void cleanStop(int sig) {

    //database detachment
    shmdt(db_200);

    //messages detachment
    shmdt(messages);

    //presence up
    up(semid_350);

    fprintf(stdout, "[DISPLAY] Processus arrêté\n");
    exit(0);
}

void printTableSeparator() {
    printf("+");
    for (int j = 0; j < 22; j++) printf("-");
    printf("+");
    for (int j = 0; j < 8; j++) printf("-");
    printf("+ \033[K\n");
}