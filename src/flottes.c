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
	flotte->systemeSuivant = 0;
	flotte->systemeArrive = 0;
	flotte->avancement = 0;
	flotte->chemin = NULL;
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
		camera->flotte = 0;
		camera->empire = 0;
		flotte->vitesse = 20;
		flotte->angle = atan((double)(systemeStellaires[systeme].y - systemeStellaires[flotte->systeme].y) / (systemeStellaires[systeme].x - systemeStellaires[flotte->systeme].x));
		camera->mapType = SYSTEME;
		camera->x = systemeStellaires[flotte->systeme].x;
		camera->x = systemeStellaires[flotte->systeme].y;
		// FlottePathFinding(flotte->systeme, systeme, systemeStellaires, path);
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
				flotte->x += cos(flotte->angle) * flotte->vitesse;
				flotte->y += sin(flotte->angle) * flotte->vitesse;
				if(pow((double)(flotte->x - 480), 2.0) + pow((double)(flotte->y - 360), 2.0) > pow((double)RAYON_DE_VUE_SYSTEME, 2.0)){
					flotte->avancement = 1;
					flotte->vitesse = 0;
					flotte->action = FLOTTE_AUCUNE_ACTION;
					systemeStellaires[flotte->systemeArrive].niveauDeConnaissance = TOTAL;
				}
			}
			flotte = flotte->suivant;
		}
		empire = empire->suivant;
	}
}

/*int* FlottePathFinding(int debut, int fin, SystemeStellaire *systemeStellaires, int* path){
	int i = 0, systemeActuel = 0, j = 0, compteur = 0; //index
	Noeud depart, arrive, actuel, noeudEnfant;
	Liste listeOuverte[LARGEUR_GALAXIE * LARGEUR_GALAXIE], listeFermee[LARGEUR_GALAXIE * LARGEUR_GALAXIE];
	Noeud listeEnfants[4];
	int numeroEnfantHyperlane = 255;
	depart.x = systemeStellaires[debut].x;
	depart.y = systemeStellaires[debut].y;
	depart.g = 0;
	depart.h = 0;
	depart.f = 0;
	systemeActuel = debut;

	arrive.x = systemeStellaires[fin].x;
	arrive.y = systemeStellaires[fin].y;

	memset(listeOuverte, 0, sizeof(Liste) * LARGEUR_GALAXIE * LARGEUR_GALAXIE);
	memset(listeFermee, 0, sizeof(Liste) * LARGEUR_GALAXIE * LARGEUR_GALAXIE);
	memset(listeEnfants, 0, sizeof(Liste) * 4);
	AjouterElementListe(listeOuverte, &actuel);

	gfx_SetTextXY(50, 50);
	while(compteur < 10){
		CopierNoeud(&actuel, listeOuverte[0].noeud);
		systemeActuel = actuel.numero;
		i = 0;
		while(i < LARGEUR_GALAXIE * LARGEUR_GALAXIE){//prends le noeud actuel
			if(listeOuverte[i].noeud->f < actuel.f){
				CopierNoeud(&actuel, listeOuverte[i].noeud);
			}
			i++;
		}

		SupprimerElementListe(listeOuverte, i);
		AjouterElementListe(listeFermee, &actuel);

		if(actuel.numero == fin){
			i = 0;
			while(actuel.parent != NULL){
				path[i] = actuel.numero;
				CopierNoeud(&actuel, actuel.parent);
				i++;
			}
			return path;
		}

		memset(listeEnfants, 0, sizeof(Liste) * 4);
		//genere les enfants
		i = 0;
		while(i < 4){
			switch(i){
				case 1:
					numeroEnfantHyperlane = systemeStellaires[systemeActuel].hyperlane1;
					break;
				case 2:
					numeroEnfantHyperlane = systemeStellaires[systemeActuel].hyperlane2;
					break;
				case 3:
					numeroEnfantHyperlane = systemeStellaires[systemeActuel].hyperlane3;
					break;
				case 4:
					numeroEnfantHyperlane = systemeStellaires[systemeActuel].hyperlane4;
					break;
			}
			if(numeroEnfantHyperlane != 255){
				listeEnfants[j].numero = numeroEnfantHyperlane;
				listeEnfants[j].x = systemeStellaires[numeroEnfantHyperlane].x;
				listeEnfants[j].y = systemeStellaires[numeroEnfantHyperlane].y;
				listeEnfants[j].parent = &actuel;
			}
			i++;
		}

		// regarde les enfants
		i = 0;
		while(i < 4){
			j = 0;
			//verifie que le noeuds n'est pas dans la liste des enfants
			while(j < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
				if((listeEnfants[i].x == listeFermee[j].noeud->x) && (listeEnfants[i].y == listeFermee[j].noeud->y)){
					i++;
					j = LARGEUR_GALAXIE * LARGEUR_GALAXIE;
				}
				j++;
			}

			// calcule f g et h
			listeEnfants[i].g = actuel.g + 1;
			listeEnfants[i].h = pow((double)listeEnfants[i].x - arrive.x, 2.0) + pow((double)listeEnfants[i].y - arrive.y, 2.0);
			listeEnfants[i].f = listeEnfants[i].g + listeEnfants[i].h;
			j = 0;
			while(j < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
				if((listeEnfants[i].x == listeOuverte[j].noeud->x) && (listeEnfants[i].y == listeOuverte[j].noeud->y)){
					if(listeEnfants[i].g > listeOuverte[j].noeud->g){
						i++;
						j = LARGEUR_GALAXIE * LARGEUR_GALAXIE;
					}
				}
				j++;
			}
			AjouterElementListe(listeFermee, &listeEnfants[i]);
			i++;
		}
		compteur++;
	}
	return path;
}

void SupprimerElementListe(Liste *liste, int numero){
	int index = numero;
	while(index < sizeof(*liste)){
		liste[index].noeud = liste[index + 1].noeud;
		index++;
	}
}
void AjouterElementListe(Liste *liste, Noeud *noeud){
	int index = 0;
	while(liste[index].noeud != NULL){
		index++;
	}
	liste[index].noeud = noeud;
}

void CopierNoeud(Noeud *noeud1, Noeud *noeud2){
	noeud1->numero = noeud2->numero;
	noeud1->parent = noeud2->parent;
	noeud1->x = noeud2->x;
	noeud1->y = noeud2->y;
	noeud1->g = noeud2->g;
	noeud1->h = noeud2->h;
	noeud1->f = noeud2->f;
}*/