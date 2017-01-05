#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "user_agence.h"
#include <time.h>
#include <string.h>
#include "shmem.h"
#include "mqueue.h"
#include "database.h"

int key_mutex = 100;
int key_mutex2 = 150;
int key_database = 200;
int key_vols = 300;
int key_presence = 350;
int key_mqueue = 400;
int size_shmem = 20 * sizeof(FlightEntry);

int semid_Mutex;
int semid_Mutex2;
int shmem_id;
int semid_vols;
int semid_presence;
int mqueue_id;
FlightEntry *dbp;

int rand_flight(int a, int b);
int tirage();
void stopTirage(int sig);
int ecrivain(int descripteur[2]);
void stopEcrivain(int sig);
char *process(char array[]);

int tirage() {
    signal(SIGINT, stopTirage);
    signal(SIGHUP, stopTirage);
    signal(SIGQUIT, stopTirage);

    Flight newflight;

    int pid, descripteur[2], destination, nb_places;
    int placeMin = 100;
    int placeMax = 500;

    //creation et initialisation sémaphore Mutex à 1
    semid_Mutex = create_semaphore(key_mutex);
    if (semid_Mutex == -1)
        exit(1);
    init_semaphore(semid_Mutex, 1);

    //création et initialisation sémaphore Mutex2 à 1
    semid_Mutex2 = create_semaphore(key_mutex2);
    if (semid_Mutex2 == -1)
        exit(1);
    init_semaphore(semid_Mutex2, 1);

    //Création de la BD
    shmem_id = create_shmem(key_database, size_shmem);
    if (shmem_id == -1)
        exit(1);

    //création et initialisation sémaphore vols à 20
    semid_vols = create_semaphore(key_vols);
    if (semid_vols == -1)
        exit(1);
    init_semaphore(semid_vols, 20);

    //création et initialisation sémaphore présence à -3
    semid_presence = create_semaphore(key_presence);
    if (semid_presence == -1)
        exit(1);
    init_semaphore(semid_presence, -3);

    //création de la file de message
    mqueue_id = create_mqueue(key_mqueue);
    if (mqueue_id == -1)
        exit(1);

    /*création du pipe avec prise en compte erreur création ou erreur fork
     cas du fils et du père
     si c'est le père on tire un vol, le met dans la struct, l'envoie a travers le pipe
     puis attend 10 secondes et recommence */

    if (pipe(descripteur) < 0) {
        printf("erreur création pipe \n");
        exit(-1);
    }

    if ((pid = fork()) < 0) {
        printf("fork failed\n");
        return -1;
    }
    if (pid == 0) { //child
        ecrivain(descripteur);
    } else {  //parents
        FILE *p = fopen("../data/destinations.txt", "r");
        int lineCount = 0;
        if (p != NULL) {
            char line[256]; /* or other suitable maximum line size */
            while (fgets(line, sizeof line, p) != NULL) /* read a line */
            {
                lineCount++;
            }
            fclose(p);
        }

        close(descripteur[0]);
        while (1) {

            printf("Je suis Tirage et je tire un vol aléatoire\n");
            destination = rand_flight(1, lineCount);
            int count = 0;
            p = fopen("../data/destinations.txt", "r");
            if (p != NULL) {
                char line[256]; /* or other suitable maximum line size */
                while (fgets(line, sizeof line, p) != NULL) /* read a line */
                {
                    if (++count == destination) strcpy(newflight.destination, process(line));
                }
                fclose(p);
            }
            nb_places = rand_flight(placeMin, placeMax);

            newflight.number = nb_places;
            write(descripteur[1], &newflight, sizeof(Flight));
            sleep(10);
        }
        close(descripteur[1]);
    }
}

void stopTirage(int sig) {
    //envoie un sigint aux autres processus qui sont tous de son groupe
    kill(0, SIGINT);
    //attend la mort de son fils

    int status;
    wait(&status);
    down(semid_presence);
    remove_semaphore(semid_Mutex);
    remove_semaphore(semid_Mutex2);
    remove_shmem(shmem_id);
    remove_semaphore(semid_vols);
    remove_mqueue(mqueue_id);
    remove_semaphore(semid_presence);
    printf("Processus Tirage arrêté\n");
    exit(0);
}

int ecrivain(int descripteur[2]) {

    signal(SIGINT, stopEcrivain);
    signal(SIGHUP, stopEcrivain);
    signal(SIGQUIT, stopEcrivain);

    Flight newflight;

    int semid_mutex = open_semaphore(key_mutex);
    //open_shmem(key_database, size_shmem);
    open_semaphore(key_vols);
    open_semaphore(key_presence);
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
    dbp = array;

    close(descripteur[1]);
    while (1) {
        read(descripteur[0], &newflight, sizeof(Flight));
        int i, put = 0;
        printf("[ECRIVAIN] down (MUTEX2)\n");
        down(semid_mutex);
        for (i = 0; i < 20; i++) {
            if ((array + i)->places == 0) {
                put = 1;
                printf("[ECRIVAIN] There's enough room in the DB\n");
                (array + i)->places = newflight.number;
                strcpy((array + i)->name, newflight.destination);
                down(key_vols);
                break;
            }
        }

        printf("[ECRIVAIN] up (MUTEX2)\n");
        up(semid_mutex);
        if (put == 0) {
            //add flight to wating list
        }

        printf("La destination est %s\n", newflight.destination);
        printf("Le nombre de place disponible est %d\n", newflight.number);
    }
    close(descripteur[0]);
}

void stopEcrivain(int sig) {
    remove_shmem(dbp);
    up(semid_presence);
    printf("Processus Ecrivain arrêté\n");
    exit(0);
}

int rand_flight(int a, int b) {

    srand((unsigned int) time(NULL));
    int number = (rand() % (b - a)) + a;
    return number;
}

int main() {
    tirage();
    return 0;
}

char *process(char array[]) {
    char *str = (char *) malloc(sizeof(char) * 21);

    for (size_t i = 0; i < 21; i++) { str[i] = ' '; }
    str[20] = '\0';

    for (size_t i = 0; i < 20; i++) {
        if (array[i] == '\n') {
            if (array[i - 1] == ' ' || array[i - 1] == ',') {
                str[i - 1] = ' ';
            }
            break;
        } else if (i == 19) {
            if (array[i] == ' ' || array[i] == ',') {
                str[i] = ' ';
                if (array[i - 1] == ' ' || array[i - 1] == ',') {
                    str[i - 1] = ' ';
                }
                break;
            }
        }
        str[i] = array[i];
    }

    return str;
}
