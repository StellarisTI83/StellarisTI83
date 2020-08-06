#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include "ordres.h"
#include "main.h"

/* structures ========================================================== */
struct OrdreElement{
	int ordre;
	int info1; //numero du module / type de flotte / numero du batiment
	int info2; //type du module / nombre de vaisseaux dans la flotte
	int prix;
	int tempsTotal;
	int tempsActuel;
	Ordre* ordreSuivant;
};

struct OrdreFileStruct{
	Ordre* premierOrdre;
};

/* entry points ======================================================== */
OrdreFile *CreerFileOrdres(){
	OrdreFile *ordreFile = NULL;
	ordreFile = calloc(1, sizeof(OrdreFile));
	if(ordreFile == NULL)
		exit(EXIT_FAILURE);

	ordreFile->premierOrdre = NULL;
	return ordreFile;
}

void SupprimerFileOrdres(OrdreFile *ordreFile){
	Ordre *ordre = NULL, *ordreSuivant = NULL;
	ordre = ordreFile->premierOrdre;
	while(ordre != NULL){
		ordreSuivant = ordre->ordreSuivant;
		free(ordre);
		ordre = ordreSuivant;
	}
	free(ordreFile);
}

void NouvelOrdre(OrdreFile* ordreFile, int ordre, int tempsTotal, int info1, int info2, int prix){
	Ordre *ordreElement = NULL, *ordrePrecedent = NULL;

	if(ordreFile->premierOrdre == NULL){
		ordreFile->premierOrdre = calloc(1, sizeof(Ordre));
		ordreElement = ordreFile->premierOrdre;
	}
	else{
		ordreElement = ordreFile->premierOrdre;
		while(ordreElement->ordreSuivant != NULL){
			ordreElement = ordreElement->ordreSuivant;
		}
		ordreElement->ordreSuivant = calloc(1, sizeof(Ordre));
		ordreElement = ordreElement->ordreSuivant;
	}

	ordreElement->ordre = ordre;
	ordreElement->tempsTotal = tempsTotal;
	ordreElement->tempsActuel = tempsTotal;
	ordreElement->info1 = info1;
	ordreElement->info2 = info2;
	ordreElement->prix = prix;
	ordreElement->ordreSuivant = NULL;
}

void FinirOrdre(OrdreFile *ordreFile){
	Ordre *ordre;
	ordre = ordreFile->premierOrdre;
	if(ordre != NULL){
		ordreFile->premierOrdre = ordre->ordreSuivant;
		free(ordre);
	}
}

Ordre* RecupererOrdre(OrdreFile *ordreFile){
	Ordre *ordre = NULL;
	ordre = ordreFile->premierOrdre;
	return ordre;
}

int NombredOrdres(OrdreFile *ordreFile){
	int nombre = 0;
	Ordre *ordre = NULL;
	ordre = ordreFile->premierOrdre;
	while(ordre != NULL){
		ordre = ordre->ordreSuivant;
		nombre++;
	}
	return(nombre);
}