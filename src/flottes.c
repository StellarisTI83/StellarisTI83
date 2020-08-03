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

static Vecteur CaclulerVecteur(double x1, double y1, double x2, double y2);

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
	memset(&flotte->vecteur, 0, sizeof(Vecteur));
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
	int path[50];
	int error;
	int index = 0;
	double norme = 0;

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

		camera->mapType = SYSTEME;
		camera->x = systemeStellaires[flotte->systeme].x;
		camera->y = systemeStellaires[flotte->systeme].y;
		
		error = PathFinding(systemeStellaires, flotte->chemin, flotte->systeme, systeme);
		if(error != 0){
			flotte->action = FLOTTE_AUCUNE_ACTION;
		}


		while((index < 4) && (systemeStellaires[flotte->systeme].hyperlane[index].destination != flotte->chemin[flotte->avancementTrajet])){
			index++;
		}
		flotte->vecteur = CaclulerVecteur(flotte->x,  flotte->y, systemeStellaires[flotte->systeme].hyperlane[index].x, systemeStellaires[flotte->systeme].hyperlane[index].y);
	}
}

/**
 * Fait effectuer les action des flottes
 */
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires){
	Empire* empire = NULL; 
	Flotte* flotte = NULL;
	int index = 0;
	int norme = 0;
	empire = empireListe->premier;
	while(empire != NULL){
		flotte = empire->flotte->premier;
		while(flotte != NULL){
			
			//bouger la flotte
			if(flotte->action == FLOTTE_BOUGER) {
				flotte->x += flotte->vecteur.xVecteur;
				flotte->y += flotte->vecteur.yVecteur;

				//calculer si la flotte sort du systeme
				if(pow((double)(flotte->x - X_CENTRE_SYSTEME), 2.0) + pow((double)(flotte->y - Y_CENTRE_SYSTEME), 2.0) > pow((double)RAYON_DE_VUE_SYSTEME, 2.0)) {
					if(flotte->avancement >= 1){
						if(flotte->chemin[flotte->avancementTrajet] == flotte->systemeArrive) {
							flotte->vecteur.xVecteur = 0;
							flotte->vecteur.yVecteur = 0;
							flotte->action = FLOTTE_AUCUNE_ACTION;
						}

						index = 0;
						while((index < 4) && (systemeStellaires[flotte->chemin[flotte->avancementTrajet]].hyperlane[index].destination != flotte->systeme)){
							index++;
						}
						flotte->x = systemeStellaires[flotte->chemin[flotte->avancementTrajet]].hyperlane[index].x;
						flotte->y = systemeStellaires[flotte->chemin[flotte->avancementTrajet]].hyperlane[index].y;

						flotte->avancement = 0;
						flotte->systeme = flotte->chemin[flotte->avancementTrajet];
						flotte->avancementTrajet++;

						index = 0;
						while((index < 4) && (systemeStellaires[flotte->systeme].hyperlane[index].destination != flotte->chemin[flotte->avancementTrajet])){
							index++;
						}
						flotte->vecteur = CaclulerVecteur(flotte->x, flotte->y, systemeStellaires[flotte->systeme].hyperlane[index].x, systemeStellaires[flotte->systeme].hyperlane[index].y);
					} else {
						flotte->avancement = 1;
					}

					systemeStellaires[flotte->systeme].niveauDeConnaissance = TOTAL;
				}
			}
			flotte = flotte->suivant;
		}
		empire = empire->suivant;
	}
}

/**
 * Calculer Vecteur
 * */
Vecteur CaclulerVecteur(double x1, double y1, double x2, double y2){
	Vecteur vecteur;
	double norme = 0;
	int index = 0;
	if(norme == 0){
		norme = 1;
	}
	//calcule du vecteur
	norme = sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
	vecteur.xVecteur = ((x2 - x1) / norme) * 20.0;
	vecteur.yVecteur = ((y2 - y1) / norme) * 20.0;
	return vecteur;
}