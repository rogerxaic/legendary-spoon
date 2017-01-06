#ifndef TRAVELLER_MESSAGES_H
#define TRAVELLER_MESSAGES_H

#define SIZE 400
#define MESSAGES 20

typedef struct msgbufout {
    long mtype;
    char msg[SIZE];
} MessageOut;

typedef struct messageEntry {
    char message[SIZE];
} MessageEntry;

#endif //TRAVELLER_MESSAGES_H
