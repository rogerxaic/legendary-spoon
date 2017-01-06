#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include "semaphore.h"
#include "messages.h"

int messages_id;
char messages[MESSAGES][SIZE];

void cleanStop(int sig) {
    shmdt(messages);
    printf("[RECEIVER] Processus arrêté\n");
    exit(0);
}

/**
 * Receives messages through a message queue and prints them to the screen
 * when it has the rights to do so (mutex).
 * @return
 */
int main() {
    signal(SIGINT, cleanStop);
    signal(SIGHUP, cleanStop);
    signal(SIGQUIT, cleanStop);

    int semid_550 = open_semaphore(550);

    //messagequeue 450
    int mq450;
    key_t key = 450;
    MessageOut rbuf;

    if ((mq450 = msgget(key, 0666)) < 0) {
        perror("[RECEIVER][ERROR] ***create mqueue***\n");
        exit(1);
    }

    for (int i = 0; i < MESSAGES; i++) {
        strcpy(messages[i], "");
    }

    while (1) {

        if (msgrcv(mq450, &rbuf, sizeof(rbuf.msg), 1, 0) < 0) {
            perror("msgrcv\n");
            exit(1);
        }

        down(semid_550);

        printf("\033[25;0H\n");
        for (int i = 0; i < MESSAGES - 1; i++) {
            strcpy((messages + i), (messages + i + 1));
        }
        strcpy((messages + MESSAGES - 1), rbuf.msg);


        for (int i = 0; i < MESSAGES; i++) {
            printf("%s\033[K\n", (messages + i));
        }

        up(semid_550);
    }
}