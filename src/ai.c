/**
 * To create an empire and make artificial intelligence
 * */

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

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include "gfx/gfx.h"

#include "main.h"
#include "nouvelle_partie.h"
#include "flottes.h"
#include "ai.h"

#include "locale/locale.h"

struct EmpireStruct {
	char nom[31];
	char couleur; //couleur sur la map
	int espece;
	Gouvernement gouvernement;

	Principe principe1;
	Principe principe2;
	Principe principe3;

	int credits;
	int minerais;
	int nourriture;
	int acier;
	int biensDeConsommation;

	FlotteListe* flotte;
	Empire* suivant;
};

struct EmpireListeStruct{
	Empire* premier;
};

/**
 * Crée une liste d'empires
 */
EmpireListe* EmpireListeCreer() {
	EmpireListe* empireListe;
	empireListe = malloc(sizeof(EmpireListe));
	empireListe->premier = NULL;
	return empireListe;
}

/**
 * Supprime une liste d'empire
 */
void EmpireListeSupprimer(EmpireListe* empireListe) {
	Empire *pointeur = 0, *pointeursuivant = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	while(pointeur->suivant != NULL) {
		pointeursuivant = pointeur->suivant;
		SupprimerFlotteListe(pointeur->flotte);
		free(pointeur);
		pointeur = pointeursuivant;
	}
	free(empireListe);
}

/**
 * Renvoi un pointeur vers l'empire numero x
 */
Empire* EmpireNumero(EmpireListe* empireListe, int numero) {
	Empire *pointeur = 0;
	int compteur = 1;
	pointeur = empireListe->premier;
	while(compteur < numero) {
		if(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	return pointeur;
}

/**
 * Rajoute un empire à la liste des empire
 */
Empire* EmpireAjouter(EmpireListe* empireListe) {
	Empire *pointeur = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	if(empireListe->premier != NULL) {
		while(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		pointeur->suivant = malloc(sizeof(Empire));
		pointeur = pointeur->suivant;
		pointeur->suivant = NULL;
	}
	else {
		empireListe->premier = malloc(sizeof(Empire));
		pointeur = empireListe->premier;
		pointeur->suivant = NULL;
	}
	return pointeur;
}

/**
 *Supprime l'empire numero x à la liste des empires
 */
void EmpireSupprimer(EmpireListe* empireListe, int numero) {
	Empire *pointeur = 0, *pointeurPrecedent = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	while(compteur <= numero) {
		if(pointeur->suivant != NULL) {
			pointeurPrecedent = pointeur;
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	if(pointeur == empireListe->premier){
		empireListe->premier = pointeur->suivant;
	}
	else {
		pointeurPrecedent->suivant = pointeur->suivant;
	}
	SupprimerFlotteListe(pointeur->flotte);
	free(pointeur);
}

/**
 * Crèe une liste de flotte d'empire
 */
void CreerEmpireFlotte(Empire *empire){
	empire->flotte = CreerFlotteListe();
	if(empire->flotte == NULL){
		exit(EXIT_FAILURE);
	}
}

/**
 * Change le gouvernement
 */
void SetGouvernement(Empire *empire, Gouvernement gouvernement){
	empire->gouvernement = gouvernement;
}
/**
 * Recuperer l'espèce
 */
Gouvernement GetGouvernement(Empire *empire){
	return empire->gouvernement;
}

/**
 * Change la couleur
 */
void SetColor(Empire *empire, Gouvernement couleur){
	empire->couleur = couleur;
}
/**
 * Recuperer la couleur
 */
int GetColor(Empire *empire){
	return empire->couleur;
}

/**
 * Changer les principes
 */
void SetPrincipes(Empire *empire, Principe principe1, Principe principe2, Principe principe3){
	empire->principe1 = principe1;
	empire->principe2 = principe2;
	empire->principe3 = principe3;
}
/**
 * Recuperer l'espèce
 */
Principe GetPrincipes(Empire *empire, int numero){
	switch(numero){
	case 1:
		return empire->principe1;
		break;
	case 2:
		return empire->principe2;
		break;
	case 3:
		return empire->principe3;
		break;
	}
	return 0;
}

/**
 * Changer nom lettre par lettre
 */
void SetName(Empire *empire, int curseur ,char lettre){
	if(lettre <= 29)
		empire->nom[lettre] = lettre;

	empire->nom[30] = '\0';
}
/**
 * Recuper Char
 */
char GetnameChar(Empire *empire, int cursor){
	return empire->nom[cursor];
}
/**
 * Recuper string
 */
char* GetnameString(Empire *empire){
	return empire->nom;
}

/**
 * Changer l'espèce
 */
void SetSpecies(Empire *empire, int espece){
	empire->espece = espece;
}
/**
 * Recuperer l'espèce
 */
int GetSpecies(Empire *empire){
	return empire->espece;
}

/**
 * Rajouter des credits
 */
void AddCredit(Empire *empire, int argent){
	empire->credits += argent;
}
/**
 * Recuperer nombre de credits
 */
int GetCredit(Empire *empire){
	return empire->credits;
}

/**
 * Rajouter des minerais
 */
void AddMinerals(Empire *empire, int minerais){
	empire->minerais += minerais;
}
/**
 * Recuperer nombre de minerais
 */
int GetMinerals(Empire *empire){
	return empire->minerais;
}

/**
 * Rajouter de la nourriture
 */
void AddFood(Empire *empire, int nourriture){
	empire->nourriture += nourriture;
}
/**
 * Recuperer nombre de nourriture
 */
int GetFood(Empire *empire){
	return empire->nourriture;
}

/**
 * Rajouter de l'acier
 */
void AddAlloys(Empire *empire, int alloy){
	empire->acier += alloy;
}
/**
 * Recuperer nombre d'acier
 */
int GetAlloys(Empire *empire){
	return empire->acier;
}

/**
 * Rajouter des biens de consommation
 */
void AddConsumerGoods(Empire *empire, int consumerGoods){
	empire->biensDeConsommation += consumerGoods;
}
/**
 * Recuperer nombre de biens de consommation
 */
int GetConsumerGoods(Empire *empire){
	return empire->biensDeConsommation;
}
