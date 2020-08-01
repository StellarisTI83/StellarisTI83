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
#include "boucle.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"
#include "flottes.h"
#include "pathfinding.h"

#include "locale/locale.h"

/**
 *Crée une list de flottes
 */
FlotteListe* FlotteListeCreer() {
	FlotteListe* flotteliste;
	flotteliste = malloc(sizeof(FlotteListe));
	flotteliste->premier = NULL;
	return flotteliste;
}

/**
 *Supprime une liste des flottes
 */
void FlotteListeSupprimer(FlotteListe* flotteliste) {
	Flotte *pointeur = 0, *pointeursuivant = 0;
	int compteur = 0;
	pointeur = flotteliste->premier;
	while(pointeur->suivant != NULL) {
		pointeursuivant = pointeur->suivant;
		free(pointeur);
		pointeur = pointeursuivant;
	}
	free(flotteliste);
}

/**
 *Renvoi un pointeur vers l'empire numero x
 */
Flotte* FlotteNumero(FlotteListe* flotteliste, int numero) {
	Flotte *pointeur = 0;
	int compteur = 1;
	pointeur = flotteliste->premier;
	while(compteur < numero) {
		if(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	return pointeur;
}

/**
 *Renvoi le numéro de la flotte suivant son pointeur
 */
int FlotteNumeroRecuperer(FlotteListe* flotteliste, Flotte* flotte) {
	Flotte *pointeur = 0;
	int compteur = 1;
	pointeur = flotteliste->premier;
	while((pointeur != flotte) && (pointeur != NULL)) {
		pointeur = pointeur->suivant;
		compteur++;
	}
	compteur--;
	return compteur;
}

/**
 *Rajoute une flotte à la liste des flotte envoyée
 */
Flotte* FlotteAjouter(FlotteListe* flotteliste) {
	Flotte *pointeur = 0;
	pointeur = flotteliste->premier;
	if(flotteliste->premier != NULL) {
		while(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		pointeur->suivant = malloc(sizeof(Flotte));
		pointeur = pointeur->suivant;
		pointeur->suivant = NULL;
	}
	else {
		flotteliste->premier = malloc(sizeof(Flotte));
		pointeur = flotteliste->premier;
	}
	return pointeur;
}

/**
 *Supprime la flotte numero x à la liste de flottes envoyée
 */
void FlotteSupprimer(FlotteListe* flotteliste, int numero) {
	Flotte *pointeur = 0, *pointeurPrecedent = 0;
	int compteur = 0;
	pointeur = flotteliste->premier;
	while(compteur <= numero) {
		if(pointeur->suivant != NULL) {
			pointeurPrecedent = pointeur;
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	if(pointeur == flotteliste->premier){
		flotteliste->premier = pointeur->suivant;
	}
	else {
		pointeurPrecedent->suivant = pointeur->suivant;
	}
	free(pointeur);
}

/**
 *Crée une nouvelle flotte
 */
Flotte* NouvelleFlotte(FlotteListe *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
	Flotte* flotte = NULL;
	flotte = FlotteAjouter(flotteListe);
	flotte->puissance = 0;
	flotte->coqueVie = 0;
	flotte->coqueTotal = 0;
	flotte->blindageVie = 0;
	flotte->blindageTotal = 0;
	flotte->bouclierVie = 0;
	flotte->bouclierTotal = 0;
	flotte->nombreDeCorvette = 0;
	flotte->nombreDeDestroyer = 0;
	flotte->nombreDeCroiseur = 0;
	flotte->nombreDeCuirasse = 0;
	flotte->nombreVaisseaux = 0;

	flotte->systeme = systeme;
	flotte->x = X_CENTRE_SYSTEME - 10;
	flotte->y = Y_CENTRE_SYSTEME + 10;
	flotte->vitesse = Y_CENTRE_SYSTEME + 10;
	flotte->action = FLOTTE_AUCUNE_ACTION;
	flotte->avancementTrajet = 0;
	flotte->systemeArrive = 0;
	flotte->avancement = 0;
	switch(type){
		case FLOTTE_SCIENTIFIQUE:
			flotte->nombreVaisseaux = 1;
			flotte->puissance = 0;
			flotte->type = FLOTTE_SCIENTIFIQUE;
			flotte->coqueVie = 300;
			flotte->coqueTotal = 300;
			flotte->blindageVie = 50;
			flotte->blindageTotal = 50;
			flotte->bouclierVie = 100;
			flotte->bouclierTotal = 100;
			break;
		case FLOTTE_DE_CONSTRUCTION:
			flotte->nombreVaisseaux = 1;
			flotte->puissance = 0;
			flotte->type = FLOTTE_DE_CONSTRUCTION;
			flotte->coqueVie = 300;
			flotte->coqueTotal = 300;
			flotte->blindageVie = 50;
			flotte->blindageTotal = 50;
			flotte->bouclierVie = 100;
			flotte->bouclierTotal = 100;
			break;
		case FLOTTE_MILITAIRE:
			flotte->type = FLOTTE_MILITAIRE;

			flotte->nombreVaisseaux += nombreDeCorvettes;
			flotte->puissance += 150 * nombreDeCorvettes;
			flotte->coqueVie += 300 * nombreDeCorvettes;
			flotte->coqueTotal += 300 * nombreDeCorvettes;
			flotte->blindageVie += 50 * nombreDeCorvettes;
			flotte->blindageTotal += 50 * nombreDeCorvettes;
			flotte->bouclierVie += 100 * nombreDeCorvettes;
			flotte->bouclierTotal += 100 * nombreDeCorvettes;
			flotte->nombreDeCorvette = nombreDeCorvettes;

			flotte->nombreVaisseaux += nombreDeDestroyers;
			flotte->puissance += 500 * nombreDeDestroyers;
			flotte->coqueVie += 800 * nombreDeDestroyers;
			flotte->coqueTotal += 800 * nombreDeDestroyers;
			flotte->blindageVie += 150 * nombreDeDestroyers;
			flotte->blindageTotal += 150 * nombreDeDestroyers;
			flotte->bouclierVie += 300 * nombreDeDestroyers;
			flotte->bouclierTotal += 300 * nombreDeDestroyers;
			flotte->nombreDeDestroyer = nombreDeDestroyers;

			flotte->nombreVaisseaux += nombreDeCroiseurs;
			flotte->puissance += 1000 * nombreDeCroiseurs;
			flotte->coqueVie += 1800 * nombreDeCroiseurs;
			flotte->coqueTotal += 1800 * nombreDeCroiseurs;
			flotte->blindageVie += 500 * nombreDeCroiseurs;
			flotte->blindageTotal += 500 * nombreDeCroiseurs;
			flotte->bouclierVie += 800 * nombreDeCroiseurs;
			flotte->bouclierTotal += 800 * nombreDeCroiseurs;
			flotte->nombreDeCroiseur = nombreDeCroiseurs;

			flotte->nombreVaisseaux += nombreDeCuirasses;
			flotte->puissance += 2000 * nombreDeCuirasses;
			flotte->coqueVie += 3000 * nombreDeCuirasses;
			flotte->coqueTotal += 3000 * nombreDeCuirasses;
			flotte->blindageVie += 1000 * nombreDeCuirasses;
			flotte->blindageTotal += 1000 * nombreDeCuirasses;
			flotte->bouclierVie += 1800 * nombreDeCuirasses;
			flotte->bouclierTotal += 1800 * nombreDeCuirasses;
			flotte->nombreDeCuirasse = nombreDeCuirasses;
			break;
	}
	return flotte;
}

/**
 *Donne l'ordre de faire bouger la flotte numero x
 */
void FlotteBouger(int numeroDeFlotte, int numeroDeEmpire, int systeme, Camera *camera, EmpireListe *empireListe, SystemeStellaire* systemeStellaires){
	Empire* empire;
	Flotte* flotte;
	int path[100];
	empire = EmpireNumero(empireListe, numeroDeEmpire);
	flotte = FlotteNumero(empire->flotte, numeroDeFlotte);
	if(camera->bougerFlotte == FALSE){
		camera->bougerFlotte = TRUE;
		camera->bloque = FALSE;
		camera->mapType = NORMAL;
		camera->fenetre = MENU_AUCUN;
		camera->empire = numeroDeEmpire;
		camera->flotte = numeroDeFlotte;
	}
	else if(camera->bougerFlotte == TRUE){
		camera->bougerFlotte = FALSE;
		flotte->systemeArrive = systeme;
		flotte->action = FLOTTE_BOUGER;
		flotte->avancement = 0;
		flotte->avancementTrajet = 1;
		camera->flotte = 0;
		camera->empire = 0;
		flotte->vitesse = 20;
		flotte->angle = atan((double)(systemeStellaires[flotte->systeme].y - systemeStellaires[systeme].y) / (systemeStellaires[flotte->systeme].x - systemeStellaires[systeme].x));
		EcrireConsoleInt(systemeStellaires[systeme].y);
		EcrireConsoleInt(systemeStellaires[flotte->systeme].y);
		EcrireConsoleInt(systemeStellaires[systeme].x);
		EcrireConsoleInt(systemeStellaires[flotte->systeme].x);
		EcrireConsoleInt(flotte->angle);
		camera->mapType = SYSTEME;
		camera->x = systemeStellaires[flotte->systeme].x;
		camera->y = systemeStellaires[flotte->systeme].y;
		PathFinding(systemeStellaires, flotte->chemin, flotte->systeme, systeme);
	}
}

/**
 *Fait effectuer les action des flottes
 */
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires){
	Empire* empire = NULL; 
	Flotte* flotte = NULL;
	empire = empireListe->premier;
	while(empire != NULL){
		flotte = empire->flotte->premier;
		while(flotte != NULL){
			if(flotte->action == FLOTTE_BOUGER){
				//bouger la flotte
				if(systemeStellaires[flotte->systeme].x < systemeStellaires[flotte->systemeArrive].x){
					flotte->x += cos(flotte->angle) * flotte->vitesse;
				}
				else{
					flotte->x -= cos(flotte->angle) * flotte->vitesse;
				}
				if(systemeStellaires[flotte->systeme].y < systemeStellaires[flotte->systemeArrive].y){
					flotte->y += sin(flotte->angle) * flotte->vitesse;
				}
				else{
					flotte->y -= sin(flotte->angle) * flotte->vitesse;
				}

				//calculer si la flotte sort du systeme
				if(pow((double)(flotte->x - 480), 2.0) + pow((double)(flotte->y - 360), 2.0) > pow((double)RAYON_DE_VUE_SYSTEME, 2.0)){
					if(flotte->avancement >= 1){
						flotte->avancement = 0;
						flotte->x = 470; 
						flotte->y = 380;
						flotte->systeme = flotte->chemin[flotte->avancementTrajet];
						flotte->avancementTrajet++;
						flotte->avancement = 0;
					} else{
						flotte->avancement = 1;
					}
					if(flotte->chemin[flotte->avancementTrajet] == flotte->systemeArrive){
						flotte->vitesse = 0;
						flotte->action = FLOTTE_AUCUNE_ACTION;
					}
					systemeStellaires[flotte->systeme].niveauDeConnaissance = TOTAL;
				}
			}
			flotte = flotte->suivant;
		}
		empire = empire->suivant;
	}
}