#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <string.h>
#include "semaphore.h"
#include "database.h"


FlightEntry *db_200;
int semid_350;

void cleanStop(int sig);

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

    if ((db200 = shmget(db, 20 * sizeof(FlightEntry), IPC_CREAT | 0666)) < 0) {
        perror("shmget\n");
        exit(1);
    }

    if ((array = shmat(db200, (void *) 0, 0)) == (FlightEntry *) (-1)) {
        perror("shmat\n");
        exit(1);
    }
    db_200 = array;

    //semaphore flight 300
    int semid_300 = open_semaphore(300);

    //presence 350
    semid_350 = open_semaphore(350);

    while (1) {
        down(semid_100);
        int i;
        for (i = 0; i < 20; i++) {
            if ((array + i)->places != 0) {
                printf("name: %s\t", (*(array + i)).name);
                printf("places: %d\n", (*(array + i)).places);
            } else { //emptying the empty flight
                strcpy((array+i)->name, "");
                if(i!=19) {
                    strcpy((array+i)->name, (array+i+1)->name);
                    (array+i)->places = (array+i+1)->places;
                }
            }
            //
        }
        up(semid_100);
        printf("*****************\n");
        sleep(10);
    }

    return 0;
}

void cleanStop(int sig) {

    //database detachment
    shmdt(db_200);

    //presence up
    up(semid_350);

    fprintf(stdout, "[DISPLAY] \t*** Display process stopped ***\n");
    exit(0);
}
