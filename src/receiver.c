#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include "semaphore.h"
#include "messages.h"

int messages_id;
MessageEntry *messages;

void cleanStop(int sig) {
    shmdt(messages);
    printf("[RECEIVER] Processus arrêté\n");
    exit(0);
}

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

    //messages 500
    key_t messages_key = 500;
    if ((messages_id = shmget(messages_key, MESSAGES * sizeof(MessageEntry), IPC_CREAT | 0666)) < 0) {
        perror("[RECEIVER][ERROR] shmget\n");
        exit(1);
    }
    if ((messages = shmat(messages_id, (void *) 0, 0)) == (MessageEntry *) (-1)) {
        perror("[RECEIVER][ERROR] shmat\n");
        exit(1);
    }

    down(semid_550);
    for (int i = 0; i < MESSAGES; i++) {
        strcpy((messages + i)->message, "");
    }
    up(semid_550);


    while (1) {

        if (msgrcv(mq450, &rbuf, sizeof(rbuf.msg), 1, 0) < 0) {
            perror("msgrcv\n");
            exit(1);
        }
        //printf("coucou2\n");

        //printf("[RECEIVER] %s\n", rbuf.msg);

        down(semid_550);
        //addMessage(rbuf.msg);

        printf("\033[25;0H\n");
        for (int i = 0; i < MESSAGES - 1; i++) {
            strcpy((messages + i)->message, (messages + i + 1)->message);
            //strcpy((messages +i)->message, "haha");
        }
        strcpy((messages + MESSAGES - 1)->message, rbuf.msg);


        for (int i = 0; i < MESSAGES; i++) {
            printf("%s\033[K\n", (messages + i)->message);
        }

        up(semid_550);
    }
}