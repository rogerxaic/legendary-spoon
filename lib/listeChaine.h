#ifndef __LISTE_CHAINEE_H__
#define __LISTE_CHAINEE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/user_agence.h"

typedef struct flightNode {
    char destination[21];
    int number;
    struct flightNode *suivant;
} Node;

typedef Node *LISTE;

void affiche(LISTE liste);

void insererListe(LISTE *root, Flight v);

void detruitListe(LISTE root);

void affiche(LISTE liste) {
    if (liste == NULL) return;
    printf("%s\n", liste->destination);
    printf("%d\n", liste->number);
    affiche(liste->suivant);
}


void insererListe(LISTE *root, Flight v) {
    LISTE newListe;
    newListe = (LISTE) malloc(sizeof(Node));
    strcpy(newListe->destination, v.destination);
    newListe->number = v.number;
    newListe->suivant = NULL;

    if (*root == NULL) {
        *root = newListe;
    } else {

        LISTE cur = *root;
        while (cur->suivant != NULL)
            cur = cur->suivant;

        cur->suivant = newListe;
    }
    return;
}

LISTE enleverListe(LISTE *root) {
    LISTE cur = *root;
    *root = (*root)->suivant;
    return cur;
}

void detruitListe(LISTE root) {
    if (root == NULL) return;
    detruitListe(root->suivant);
    free(root);
}


#endif
