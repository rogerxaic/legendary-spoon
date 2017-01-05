#include <stdio.h>
#include <stdlib.h>

typedef struct flight{
        char destination[21];
        int number;
	struct flight *suivant;
} Flight;

typedef Flight *LISTE;

void affiche(LISTE liste){
	if(liste==NULL) return;
	printf("%s\n", liste->destination);
	printf("%d\n",liste->number);
	affiche(liste->suivant);
}


void insererListe(LISTE* root, struct Flight){
	LISTE newListe;
	newListe=(LISTE) malloc(sizeof(Flight));
	newListe->destination=Flight->destination;
	newListe->number=Flight->number
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

void detruitListe(LISTE root){
	if(root == NULL) return;
	detruitListe(root->suivant);
	free(root);
}
