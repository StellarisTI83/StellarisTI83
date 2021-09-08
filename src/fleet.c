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

#include "ai.h"
#include "fleet.h"
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
FleetList* fleet_ListCreate() {
	return (FleetList*)GenericList_Create();
}

/**
 *Supprime une liste des flottes
 */
void fleet_ListFree(FleetList* flotteliste) {
    Fleet *flotte = NULL;
	int i = 0;
    flotte = GenericCell_Get((GenericList*)flotteliste, i);
    while(flotte != NULL) {
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, ", Free fleet %d", i);
		#endif
        free(flotte);
		i++;
        flotte = GenericCell_Get((GenericList*)flotteliste, i);
    }
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "\n");
	#endif
	GenericList_Free((GenericList*)flotteliste);
}

/**
 * Renvoi nombre de flottes
 */
int FleetArraySize(const FleetList* flotteListe){
	return GenericList_ArraySize((GenericList*)flotteListe);
}

/**
 *Renvoi un pointeur vers la flotte numero x, commence à 1
 */
Fleet* FlotteNumero(const FleetList* flotteliste, const int numero) {
	return GenericCell_Get((GenericList*)flotteliste, numero);
}

/**
 *Renvoi le numéro de la flotte suivant son pointeur
 */
int RecupererFlotteNumero(const FleetList* flotteliste, const Fleet* flotte) {
	return GenericCell_GetNumber((GenericList*)flotteliste, flotte);
}

/**
 *Rajoute une flotte à la liste des flotte envoyée
 */
Fleet* AjouterFlotte(FleetList* flotteliste) {
	Fleet *pointeur = NULL;
	pointeur = calloc(1, sizeof(Fleet));
	if(!pointeur){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating fleet");
		#endif
		exit(EXIT_FAILURE);
	}
	GenericCell_Add((GenericList*)flotteliste, pointeur);
	return pointeur;
}

/**
 *Supprime la flotte numero x à la liste de flottes envoyée
 */
void SupprimerFlotte(FleetList* flotteliste, int numero) {
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "Free fleet %d\n", numero);
	#endif
	free(GenericCell_Get((GenericList*)flotteliste, numero));
	GenericCell_Free((GenericList*)flotteliste, numero);
}

/**
 *Crée une nouvelle flotte
 */
Fleet* fleet_New(FleetList *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
	Fleet* flotte = NULL;
	flotte = AjouterFlotte(flotteListe);
	memset(flotte, 0, sizeof(Fleet));

	flotte->systeme = systeme;
	flotte->x = SYSTEM_SPECIAL_X - 10;
	flotte->y = SYSTEM_SPECIAL_Y + 10;
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

int CalculateFleetPower(FleetList *flotteListe){
	Fleet *flotte = NULL;
	int compteur = 0;
	int arraySize = FleetArraySize(flotteListe);
	int puissance = 0;
	while(compteur < arraySize){
		flotte = GenericCell_Get((GenericList*)flotteListe, compteur);
		compteur++;
		puissance += flotte->puissance;
	}
	return puissance;
}

/**
 * Get the system of the designated flotte
 */
int GetFleetSystem(Fleet *flotte){
	return flotte->systeme;
}

/**
 * Get the path of the designated flotte
 */
int GetFleetPath(Fleet *flotte, int index){
	if(index < 50)
		return flotte->chemin[index];
	else
		return 0;
}

/**
 * Get the type of the designated flotte
 */
FlotteType GetFleetType(Fleet *flotte){
	return flotte->type;
}

/**
 * Get the power of the designated flotte
 */
int GetFleetPower(Fleet *flotte){
	return flotte->puissance;
}

/**
 * Get the progress of the designated flotte
 */
int GetFleetPathProgress(Fleet *flotte){
	return flotte->avancementTrajet;
}
void IncrementFleetPathProgress(Fleet *flotte){
	flotte->avancementTrajet++;
}
void SetFleetPathProgress(Fleet *flotte, int progress){
	flotte->avancementTrajet = progress;
}

int GetFleetProgress(Fleet *flotte){
	return flotte->avancement;
}
void IncrementFleetProgress(Fleet *flotte){
	flotte->avancement++;
}
void SetFleetProgress(Fleet *flotte, int progress){
	flotte->avancement = progress;
}

int GetFleetX(Fleet *flotte){
	return flotte->x;
}
int GetFleetY(Fleet *flotte){
	return flotte->y;
}
int GetFleetXVector(Fleet *flotte){
	return flotte->vecteur.xVecteur;
}
int GetFleetYVector(Fleet *flotte){
	return flotte->vecteur.yVecteur;
}

int GetFleetHullPourcent(Fleet *flotte){
	return (flotte->coqueVie * 100) / flotte->coqueTotal;
}
int GetFleetArmorPourcent(Fleet *flotte){
	return (flotte->blindageVie * 100) / flotte->blindageTotal;
}
int GetFleetShieldPourcent(Fleet *flotte){
	return (flotte->bouclierVie * 100) / flotte->bouclierTotal;
}

int GetFleetCorvetteNumber(Fleet *flotte){
	return flotte->nombreDeCorvette;
}
int GetFleetDestroyerNumber(Fleet *flotte){
	return flotte->nombreDeDestroyer;
}
int GetFleetCruiserNumber(Fleet *flotte){
	return flotte->nombreDeCroiseur;
}
int GetFleetBattleshipNumber(Fleet *flotte){
	return flotte->nombreDeCuirasse;
}

char GetFleetAction(Fleet *flotte){
	return flotte->action;
}
void SetFleetAction(Fleet *flotte, char action){
	flotte->action = action;
}
int GetFleetArriveSystem(Fleet *flotte){
	return flotte->systemeArrive;
}

/**
 *Donne l'ordre de faire bouger la flotte numero x
 */
void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Window *fenetre, Camera *camera, EmpireList *empireListe, StarSystem **systemeStellaires){
	Empire* empire;
	Fleet* flotte;
	int error;

	empire = empire_Get(empireListe, numeroDeEmpire);
	flotte = FlotteNumero(empire_FleetListGet(empire), numeroDeFlotte);
	if(flotte == NULL) {
		#ifdef DEBUG_VERSION
			dbg_sprintf(dbgerr, "Error fleet pointer NULL in function 'BougerFlotte'");
		#endif
		return;
	}
	
	if(camera_FleetMoveGet(camera) == false){
		camera_FleetMoveSet(camera, true);
		camera_LockSet(camera, false);
		camera_MapTypeSet(camera, VUE_GALACTIC);
		menu_Close(fenetre, camera);
		camera_FleetSet(camera, numeroDeFlotte);
	} else {
		if(systeme == flotte->systeme){
			camera_FleetMoveSet(camera, false);
			flotte->action = FLOTTE_AUCUNE_ACTION;
		} else if(((flotte->type == FLOTTE_SCIENTIFIQUE) && (starSystem_IntelLevelGet(systemeStellaires[systeme]) == INTEL_UNKNOWN)) || (starSystem_IntelLevelGet(systemeStellaires[systeme]) != INTEL_UNKNOWN)){
			camera_FleetMoveSet(camera, false);
			
			camera_FleetSet(camera, 0);

			camera_MapTypeSet(camera, VUE_SYSTEM);
			camera_XSet(camera, starSystem_XGet(systemeStellaires[(int)flotte->systeme]) * camera_ZoomGet(camera));
			camera_YSet(camera, starSystem_YGet(systemeStellaires[(int)flotte->systeme]) * camera_ZoomGet(camera));
			
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

int MoveFleet(Fleet *flotte, int systeme, StarSystem **systemeStellaires){
	int error = 0;
	int index = 0;

	if(!flotte){
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
	flotte->vecteur = CaclulerVecteur(flotte->x,  flotte->y, hyperlane_XGet(systemeStellaires[(int)flotte->systeme], index), hyperlane_YGet(systemeStellaires[(int)flotte->systeme], index));
	while((index < 4) && (hyperlane_DestinationGet(systemeStellaires[(int)flotte->systeme], index) != flotte->chemin[(int)flotte->avancementTrajet])){
		index++;
	}
	return error;
}

/**
 * Fait effectuer les action des flottes
 */
void fleet_ActionsUpdate(StarSystem **systemeStellaires, EmpireList* empireListe){
	Empire* empire = NULL; 
	Fleet* flotte = NULL;
	int index = 0, numeroEmpire = 0;
	int fleetSize;
	int fleetIndex;
	int empireSize;
	empire = empire_Get(empireListe, 0);
	empireSize = empire_ArraySize(empireListe);
	while(numeroEmpire < empireSize){
		flotte = FlotteNumero(empire_FleetListGet(empire), 0);
		fleetIndex = 0;
		fleetSize = FleetArraySize(empire_FleetListGet(empire));
		while(fleetIndex < fleetSize){
			
			//bouger la flotte
			if(flotte->action != FLOTTE_AUCUNE_ACTION) {
				flotte->x += flotte->vecteur.xVecteur;
				flotte->y += flotte->vecteur.yVecteur;

				if(flotte->systeme == flotte->systemeArrive) {
					flotte->vecteur = CaclulerVecteur(flotte->x, flotte->y, SYSTEM_SPECIAL_X, SYSTEM_SPECIAL_Y);
					flotte->avancement = 0;
					if(pow((double)(flotte->x - SYSTEM_SPECIAL_X), 2.0) + pow((double)(flotte->y - SYSTEM_SPECIAL_Y), 2.0) < pow((double)10, 2.0)) {
						//arrivé au centre du systeme
						if(flotte->action == FLOTTE_CONSTRUIRE_BASE) {
							station_LevelSet(starSystem_StationGet(systemeStellaires[(int)flotte->systeme]), STATION_OUTPOST);
							starSystem_EmpireSet(systemeStellaires[(int)flotte->systeme], numeroEmpire);
						}
						flotte->action = FLOTTE_AUCUNE_ACTION;
					}
				}

				//calculer si la flotte sort du systeme
				if(pow((double)(flotte->x - SYSTEM_SPECIAL_X), 2.0) + pow((double)(flotte->y - SYSTEM_SPECIAL_Y), 2.0) > pow((double)SYSTEM_VIEW_RADIUS, 2.0)) {
					if(flotte->avancement >= 1){

						index = 0;
						while((index < 4) && (hyperlane_DestinationGet(systemeStellaires[flotte->chemin[(int)flotte->avancementTrajet]], index) != flotte->systeme)){
							index++;
						}
						
						flotte->x = hyperlane_XGet(systemeStellaires[flotte->chemin[(int)flotte->avancementTrajet]], index);
						flotte->y = hyperlane_YGet(systemeStellaires[flotte->chemin[(int)flotte->avancementTrajet]], index);

						flotte->avancement = 0;
						flotte->systeme = flotte->chemin[(int)flotte->avancementTrajet];
						flotte->avancementTrajet++;

						index = 0;
						while((index < 4) && (hyperlane_DestinationGet(systemeStellaires[(int)flotte->systeme], index) != flotte->chemin[(int)flotte->avancementTrajet])){
							index++;
						}
						flotte->vecteur = CaclulerVecteur(flotte->x, flotte->y, hyperlane_XGet(systemeStellaires[(int)flotte->systeme], index), hyperlane_YGet(systemeStellaires[(int)flotte->systeme], index));
					} else {
						flotte->avancement = 1;
					}
				}
			}
			fleetIndex++;
			flotte = FlotteNumero(empire_FleetListGet(empire), fleetIndex);
		}
		numeroEmpire++;
		empire = empire_Get(empireListe, numeroEmpire);
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
	return (FleetList*)GenericList_Create();
}

/**
 *Supprime une liste de templates de flottes
 */
void fleet_TemplateListFree(FleetTemplateListe* flotteliste) {
	FleetTemplate *fleetTemplate = NULL;
	int i = 0;
    fleetTemplate = GenericCell_Get((GenericList*)flotteliste, i);
    while(fleetTemplate != NULL) {
        free(fleetTemplate);
		i++;
        fleetTemplate = GenericCell_Get((GenericList*)flotteliste, i);
    }
	GenericList_Free((GenericList*)flotteliste);
}

/**
 * Renvoi nombre de templates de flottes
 */
int fleet_TemplateListSize(FleetTemplateListe* flotteListe){
	return GenericList_ArraySize((GenericList*)flotteListe);
}

/**
 * Renvoi un pointeur vers le template flotte numero x, commence à 1
 */
FleetTemplate* fleet_TemplateGet(FleetTemplateListe* flotteliste, int numero) {
	return GenericCell_Get((GenericList*)flotteliste, numero);
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
	GenericCell_Add((GenericList*)flotteliste, pointeur);
	return pointeur;
}

/**
 * Supprime le template de flotte numero x à la liste de templates de flottes envoyée
 */
void fleet_TemplateDestroy(FleetTemplateListe* flotteliste, int numero) {
	free(GenericCell_Get((GenericList*)flotteliste, numero));
	GenericCell_Free((GenericList*)flotteliste, numero);
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