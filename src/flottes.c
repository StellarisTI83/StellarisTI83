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

#include "main.h"

#include "nouvelle_partie.h"
#include "flottes.h"
#include "pathfinding.h"

static Vecteur CaclulerVecteur(double x1, double y1, double x2, double y2);

typedef struct NoeudStruct Noeud;
typedef struct ListeNoeds Liste;

struct VecteurStruct {
	double xVecteur;
	double yVecteur;
} ;

struct FlotteStruct {
	int x;
	int y;
	Vecteur vecteur;

	FlotteType type;

	int puissance;
	int coqueTotal;
	int coqueVie;
	int blindageTotal;
	int blindageVie;
	int bouclierTotal;
	int bouclierVie;

	char systeme;
	char action;
	char destination; // planete ou etoile plus prècisement
	char avancementTrajet;
	char systemeArrive;
	char avancement;

	int chemin[50];

	char nombreVaisseaux;
	char nombreDeCorvette;
	char nombreDeDestroyer;
	char nombreDeCroiseur;
	char nombreDeCuirasse;
};

struct NoeudStruct {
	int numero;
	Noeud* parent;
	int x;
	int y;
	int g; //distance depuis le début
	int h; //heuristique
	int f; //g+h
} ;

struct ListeNoeds {
	Noeud* noeud;
};

struct FleetTemplateStruct {
	int puissance;
	FlotteType type;

	int hull;
	int armor;
	int shield;
};

/* entry points ======================================================== */

/**
 *Crée une liste de flottes
 */
FlotteListe* CreerFlotteListe() {
	return (FlotteListe*)CreateGenericList();
}

/**
 *Supprime une liste des flottes
 */
void SupprimerFlotteListe(FlotteListe* flotteliste) {
    Flotte *flotte = NULL;
	int i = 1;
    flotte = GenericCellGet((GenericList*)flotteliste, i);
    while(flotte != NULL) {
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Free fleet %d, ", i);
		#endif
        free(flotte);
		i++;
        flotte = GenericCellGet((GenericList*)flotteliste, i);
    }
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "\n");
	#endif
	FreeGenericList((GenericList*)flotteliste);
}

/**
 * Renvoi nombre de flottes
 */
int FleetArraySize(FlotteListe* flotteListe){
	return GenericListArraySize((GenericList*)flotteListe);
}

/**
 *Renvoi un pointeur vers la flotte numero x, commence à 1
 */
Flotte* FlotteNumero(FlotteListe* flotteliste, int numero) {
	return GenericCellGet((GenericList*)flotteliste, numero);
}

/**
 *Renvoi le numéro de la flotte suivant son pointeur
 */
int RecupererFlotteNumero(FlotteListe* flotteliste, Flotte* flotte) {
	return GenericCellGetNumber((GenericList*)flotteliste, flotte);
}

/**
 *Rajoute une flotte à la liste des flotte envoyée
 */
Flotte* AjouterFlotte(FlotteListe* flotteliste) {
	Flotte *pointeur = NULL;
	pointeur = calloc(1, sizeof(Flotte));
	if(!pointeur){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating fleet");
		#endif
		exit(EXIT_FAILURE);
	}
	GenericCellAdd((GenericList*)flotteliste, pointeur);
	return pointeur;
}

/**
 *Supprime la flotte numero x à la liste de flottes envoyée
 */
void SupprimerFlotte(FlotteListe* flotteliste, int numero) {
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "Free fleet %d\n", numero);
	#endif
	free(GenericCellGet((GenericList*)flotteliste, numero));
	FreeGenericCell((GenericList*)flotteliste, numero);
}

/**
 *Crée une nouvelle flotte
 */
Flotte* NouvelleFlotte(FlotteListe *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
	Flotte* flotte = NULL;
	flotte = AjouterFlotte(flotteListe);
	memset(flotte, 0, sizeof(Flotte));

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

int CalculateFleetPower(FlotteListe *flotteListe){
	Flotte *flotte = NULL;
	int compteur = 1;
	int arraySize = FleetArraySize(flotteListe);
	int puissance = 0;
	while(compteur < arraySize){
		flotte = GenericCellGet((GenericList*)flotteListe, compteur);
		compteur++;
		puissance += flotte->puissance;
	}
	return puissance;
}

/**
 * Get the system of the designated flotte
 */
int GetFleetSystem(Flotte *flotte){
	return flotte->systeme;
}

/**
 * Get the path of the designated flotte
 */
int GetFleetPath(Flotte *flotte, int index){
	if(index < 50)
		return flotte->chemin[index];
	else
		return 0;
}

/**
 * Get the type of the designated flotte
 */
FlotteType GetFleetType(Flotte *flotte){
	return flotte->type;
}

/**
 * Get the power of the designated flotte
 */
int GetFleetPower(Flotte *flotte){
	return flotte->puissance;
}

/**
 * Get the progress of the designated flotte
 */
int GetFleetPathProgress(Flotte *flotte){
	return flotte->avancementTrajet;
}
void IncrementFleetPathProgress(Flotte *flotte){
	flotte->avancementTrajet++;
}
void SetFleetPathProgress(Flotte *flotte, int progress){
	flotte->avancementTrajet = progress;
}

int GetFleetProgress(Flotte *flotte){
	return flotte->avancement;
}
void IncrementFleetProgress(Flotte *flotte){
	flotte->avancement++;
}
void SetFleetProgress(Flotte *flotte, int progress){
	flotte->avancement = progress;
}

int GetFleetX(Flotte *flotte){
	return flotte->x;
}
int GetFleetY(Flotte *flotte){
	return flotte->y;
}
int GetFleetXVector(Flotte *flotte){
	return flotte->vecteur.xVecteur;
}
int GetFleetYVector(Flotte *flotte){
	return flotte->vecteur.yVecteur;
}

int GetFleetHullPourcent(Flotte *flotte){
	return (flotte->coqueVie * 100) / flotte->coqueTotal;
}
int GetFleetArmorPourcent(Flotte *flotte){
	return (flotte->blindageVie * 100) / flotte->blindageTotal;
}
int GetFleetShieldPourcent(Flotte *flotte){
	return (flotte->bouclierVie * 100) / flotte->bouclierTotal;
}

int GetFleetCorvetteNumber(Flotte *flotte){
	return flotte->nombreDeCorvette;
}
int GetFleetDestroyerNumber(Flotte *flotte){
	return flotte->nombreDeDestroyer;
}
int GetFleetCruiserNumber(Flotte *flotte){
	return flotte->nombreDeCroiseur;
}
int GetFleetBattleshipNumber(Flotte *flotte){
	return flotte->nombreDeCuirasse;
}

char GetFleetAction(Flotte *flotte){
	return flotte->action;
}
void SetFleetAction(Flotte *flotte, char action){
	flotte->action = action;
}
int GetFleetArriveSystem(Flotte *flotte){
	return flotte->systemeArrive;
}

/**
 *Donne l'ordre de faire bouger la flotte numero x
 */
void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Fenetre *fenetre, Camera *camera, EmpireListe *empireListe, SystemeStellaire **systemeStellaires){
	Empire* empire;
	Flotte* flotte;
	int error;

	empire = EmpireNumero(empireListe, numeroDeEmpire);
	flotte = FlotteNumero(EmpireFleetGetArray(empire), numeroDeFlotte);
	if(flotte == NULL) {
		#ifdef DEBUG_VERSION
			dbg_sprintf(dbgerr, "Error fleet pointer NULL in function 'BougerFlotte'");
		#endif
		return;
	}
	
	if(IsCameraMoveFleet(camera) == false){
		SetCameraMoveFleet(camera, true);
		SetCameraLock(camera, false);
		SetCameraMapType(camera, NORMAL);
		CloseMenu(fenetre, camera);
		SetCameraEmpire(camera, numeroDeEmpire);
		SetCameraFleet(camera, numeroDeFlotte);
	} else {
		if(systeme == flotte->systeme){
			SetCameraMoveFleet(camera, false);
			flotte->action = FLOTTE_AUCUNE_ACTION;
		} else if(((flotte->type == FLOTTE_SCIENTIFIQUE) && (GetSystemIntelLevel(systemeStellaires[systeme]) == INCONNU)) || (GetSystemIntelLevel(systemeStellaires[systeme]) != INCONNU)){
			SetCameraMoveFleet(camera, false);
			
			SetCameraEmpire(camera, 0);
			SetCameraFleet(camera, 0);

			SetCameraMapType(camera, SYSTEME);
			SetCameraX(camera, GetSystemX(systemeStellaires[(int)flotte->systeme]) * GetCameraZoom(camera));
			SetCameraY(camera, GetSystemY(systemeStellaires[(int)flotte->systeme]) * GetCameraZoom(camera));
			
			error = MoveFleet(flotte, systeme, systemeStellaires);
			
			if(error != 0) { // si il n'y a pas de chemin
				flotte->action = FLOTTE_AUCUNE_ACTION;
			}

			
			switch(flotte->action){
			case FLOTTE_CONSTRUIRE_BASE:
				
				break;
			}
		}
	}
}

int MoveFleet(Flotte *flotte, int systeme, SystemeStellaire **systemeStellaires){
	int error = 0;
	int index = 0;

	if(flotte == NULL){
		#ifdef DEBUG_VERSION
			dbg_sprintf(dbgerr, "Error fleet pointer NULL in function 'MoveFleet'");
		#endif
		return -1;
	}

	flotte->systemeArrive = systeme;
	flotte->avancement = 0;
	flotte->avancementTrajet = 1;
	flotte->action = FLOTTE_BOUGER;
	error = PathFinding(systemeStellaires, flotte->chemin, flotte->systeme, systeme, sizeof(flotte->chemin)/sizeof(flotte->chemin[0]));
	flotte->vecteur = CaclulerVecteur(flotte->x,  flotte->y, GetHyperlaneX(systemeStellaires[(int)flotte->systeme], index), GetHyperlaneY(systemeStellaires[(int)flotte->systeme], index));
	while((index < 4) && (GetHyperlaneDestination(systemeStellaires[(int)flotte->systeme], index) != flotte->chemin[(int)flotte->avancementTrajet])){
		index++;
	}
	return error;
}

/**
 * Fait effectuer les action des flottes
 */
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire **systemeStellaires){
	Empire* empire = NULL; 
	Flotte* flotte = NULL;
	int index = 0, numeroEmpire = 1;
	int fleetSize = 1;
	int fleetIndex = 1;
	int empireSize = 1;
	empire = EmpireNumero(empireListe, 1);
	empireSize = EmpireArraySize(empireListe);
	while(numeroEmpire <= empireSize){
		flotte = FlotteNumero(EmpireFleetGetArray(empire), 1);
		fleetIndex = 1;
		fleetSize = FleetArraySize(EmpireFleetGetArray(empire));
		while(fleetIndex <= fleetSize){
			
			//bouger la flotte
			if(flotte->action != FLOTTE_AUCUNE_ACTION) {
				flotte->x += flotte->vecteur.xVecteur;
				flotte->y += flotte->vecteur.yVecteur;

				if(flotte->systeme == flotte->systemeArrive) {
					flotte->vecteur = CaclulerVecteur(flotte->x, flotte->y, X_CENTRE_SYSTEME, Y_CENTRE_SYSTEME);
					flotte->avancement = 0;
					if(pow((double)(flotte->x - X_CENTRE_SYSTEME), 2.0) + pow((double)(flotte->y - Y_CENTRE_SYSTEME), 2.0) < pow((double)10, 2.0)) {
						//arrivé au centre du systeme
						if(flotte->action == FLOTTE_CONSTRUIRE_BASE) {
							SetStationLevel(GetSystemStation(systemeStellaires[(int)flotte->systeme]), AVANT_POSTE);
							SetSystemEmpire(systemeStellaires[(int)flotte->systeme], numeroEmpire);
						}
						flotte->action = FLOTTE_AUCUNE_ACTION;
					}
				}

				//calculer si la flotte sort du systeme
				if(pow((double)(flotte->x - X_CENTRE_SYSTEME), 2.0) + pow((double)(flotte->y - Y_CENTRE_SYSTEME), 2.0) > pow((double)RAYON_DE_VUE_SYSTEME, 2.0)) {
					if(flotte->avancement >= 1){

						index = 0;
						while((index < 4) && (GetHyperlaneDestination(systemeStellaires[flotte->chemin[(int)flotte->avancementTrajet]], index) != flotte->systeme)){
							index++;
						}
						
						flotte->x = GetHyperlaneX(systemeStellaires[flotte->chemin[(int)flotte->avancementTrajet]], index);
						flotte->y = GetHyperlaneY(systemeStellaires[flotte->chemin[(int)flotte->avancementTrajet]], index);

						flotte->avancement = 0;
						flotte->systeme = flotte->chemin[(int)flotte->avancementTrajet];
						flotte->avancementTrajet++;

						index = 0;
						while((index < 4) && (GetHyperlaneDestination(systemeStellaires[(int)flotte->systeme], index) != flotte->chemin[(int)flotte->avancementTrajet])){
							index++;
						}
						flotte->vecteur = CaclulerVecteur(flotte->x, flotte->y, GetHyperlaneX(systemeStellaires[(int)flotte->systeme], index), GetHyperlaneY(systemeStellaires[(int)flotte->systeme], index));
					} else {
						flotte->avancement = 1;
					}
				}
			}
			fleetIndex++;
			flotte = FlotteNumero(EmpireFleetGetArray(empire), fleetIndex);
		}
		numeroEmpire++;
		empire = EmpireNumero(empireListe, numeroEmpire);
	}
}

/**
 * Calculer Vecteur
 */
Vecteur CaclulerVecteur(double x1, double y1, double x2, double y2){
	Vecteur vecteur;
	double norme = 0;
	if(norme == 0){
		norme = 1;
	}
	//calcule du vecteur
	norme = sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
	vecteur.xVecteur = ((x2 - x1) / norme) * 20.0;
	vecteur.yVecteur = ((y2 - y1) / norme) * 20.0;
	return vecteur;
}



/**
 *Crée une liste de templates de flottes
 */
FleetTemplateListe* fleet_TemplateListCreate() {
	return (FlotteListe*)CreateGenericList();
}

/**
 *Supprime une liste de templates de flottes
 */
void fleet_TemplateListFree(FleetTemplateListe* flotteliste) {
	FleetTemplate *fleetTemplate = NULL;
	int i = 0;
    fleetTemplate = GenericCellGet((GenericList*)flotteliste, i);
    while(fleetTemplate != NULL) {
        free(fleetTemplate);
		i++;
        fleetTemplate = GenericCellGet((GenericList*)flotteliste, i);
    }
	FreeGenericList((GenericList*)flotteliste);
}

/**
 * Renvoi nombre de templates de flottes
 */
int fleet_TemplateListSize(FleetTemplateListe* flotteListe){
	return GenericListArraySize((GenericList*)flotteListe);
}

/**
 * Renvoi un pointeur vers le template flotte numero x, commence à 1
 */
FleetTemplate* fleet_TemplateGet(FleetTemplateListe* flotteliste, int numero) {
	return GenericCellGet((GenericList*)flotteliste, numero);
}

/**
 * Rajoute une flotte à la liste des flotte envoyée
 */
FleetTemplate* fleet_TemplateAdd(FleetTemplateListe* flotteliste) {
	FleetTemplate *pointeur = NULL;
	pointeur = calloc(1, sizeof(FleetTemplate));
	if(!pointeur){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating fleet template");
		#endif
		exit(EXIT_FAILURE);
	}
	GenericCellAdd((GenericList*)flotteliste, pointeur);
	return pointeur;
}

/**
 * Supprime le template de flotte numero x à la liste de templates de flottes envoyée
 */
void fleet_TemplateDestroy(FleetTemplateListe* flotteliste, int numero) {
	free(GenericCellGet((GenericList*)flotteliste, numero));
	FreeGenericCell((GenericList*)flotteliste, numero);
}

/**
 * Set fleet template
 */
void fleet_TemplateSetType(FleetTemplate* fleetTemplate, FlotteType type){
	fleetTemplate->type = type;
}
void fleet_TemplateSetPower(FleetTemplate* fleetTemplate, int puissance){
	fleetTemplate->puissance = puissance;
}
void fleet_TemplateSetHull(FleetTemplate* fleetTemplate, int hull){
	fleetTemplate->hull = hull;
}
void fleet_TemplateSetArmor(FleetTemplate* fleetTemplate, int armor){
	fleetTemplate->armor = armor;
}
void fleet_TemplateSetShield(FleetTemplate* fleetTemplate, int shield){
	fleetTemplate->shield = shield;
}

/**
 * Get fleet template
 */
FlotteType fleet_TemplateGetType(FleetTemplate* fleetTemplate){
	return fleetTemplate->type;
}
int fleet_TemplateGetPower(FleetTemplate* fleetTemplate){
	return fleetTemplate->puissance;
}
int fleet_TemplateGetHull(FleetTemplate* fleetTemplate){
	return fleetTemplate->hull;
}
int fleet_TemplateGetArmor(FleetTemplate* fleetTemplate){
	return fleetTemplate->armor;
}
int fleet_TemplateGetShield(FleetTemplate* fleetTemplate){
	return fleetTemplate->shield;
}