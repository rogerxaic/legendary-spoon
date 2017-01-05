#include <stdio.h>
#include <stdlib.h>

typedef struct flight{
        char destination[21];
        int number;
	struct flight *suivant;
}Vol;

typedef Vol *LISTE;

void affiche(LISTE liste){
	if(liste==NULL) return;
	printf("%s\n", liste->destination);
	printf("%d\n",liste->number);
	affiche(liste->suivant);
}


void insererListe(LISTE* root, Flight v){
	LISTE newListe;
	newListe=(LISTE) malloc(sizeof(Vol));
	strcpy(newListe->destination,v.destination);
	newListe->number=v.number;
	newListe->suivant=NULL;
	if(*root == NULL){
		*root=newListe;
	}else{
		LISTE cur=*root;
		while(cur->suivant!=NULL)
			cur=cur->suivant;

	cur->suivant = newListe;
	}
	return;
}
LISTE enleverListe(LISTE *root){
	LISTE cur=*root;
	root=root->suivant;

	return cur;
}

void detruitListe(LISTE root){
	if(root == NULL) return;
	detruitListe(root->suivant);
	free(root);
}
