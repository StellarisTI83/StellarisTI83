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

#include "ai.h"

#include "locale/locale.h"

/* structures ========================================================== */

struct EmpireStruct {
	char nom[31];
	char couleur; //couleur sur la map
	int espece;
	int vetements;
	Gouvernement gouvernement;

	Principe principe1;
	Principe principe2;
	Principe principe3;

	int credits;
	int minerais;
	int nourriture;
	int acier;
	int biensDeConsommation;

	int PuissanceMilitaire;
	int PuissanceScientifique;
	int PuissanceEconomique;

	int systemeCapitale;

	FlotteListe* flotte;
	Empire* suivant;
};

struct EmpireListeStruct{
	Empire* premier;
};

/* private variables =================================================== */

char * empireNamePostFixDemocratic[] = {
	"Republic",
	"Alliance",
	"Commonwealth",
	"Confederacy"
};

char * empireNamePostFixOligarchic[] = {
	"Confederation",
	"Union",
	"League",
	"Directorate"
};

char * empireNamePostFixDictatorial[] = {
	"Hegemony",
	"Empire",
	"Autocracy"
};

char * empireNamePrefix[] = {
	"Star's ",
	"Galaxy's ",
	"Federal "
};

/* entry points ======================================================== */

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
 * Renvoi nombre d'empires
 */
int EmpireArraySize(EmpireListe* empireListe){
	Empire *empire;
	int index = 0;
	empire = empireListe->premier;
	while(empire != NULL){
		empire = empire->suivant;
		index++;
	}
	return index;
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
void EmpireNouvelleFlotte(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
	NouvelleFlotte(empire->flotte, systeme, type, nombreDeCorvettes, nombreDeDestroyers, nombreDeCroiseurs, nombreDeCuirasses);
}
FlotteListe *GetFleetArray(Empire *empire){
	return empire->flotte;
}

/**
 * Change le gouvernement
 */
void SetEmpireGouvernement(Empire *empire, Gouvernement gouvernement){
	empire->gouvernement = gouvernement;
}
/**
 * Recuperer l'espèce
 */
Gouvernement GetEmpireGouvernement(Empire *empire){
	return empire->gouvernement;
}

void SetEmpireSystemCapital(Empire *empire, int system){
	empire->systemeCapitale = system;
}
int GetEmpireSystemCapital(Empire *empire){
	return empire->systemeCapitale;
}

/**
 * Change la couleur
 */
void SetEmpireColor(Empire *empire, Gouvernement couleur){
	empire->couleur = couleur;
}
/**
 * Recuperer la couleur
 */
int GetEmpireColor(Empire *empire){
	return empire->couleur;
}

/**
 * Changer les principes
 */
void SetEmpirePrincipes(Empire *empire, Principe principe1, Principe principe2, Principe principe3){
	empire->principe1 = principe1;
	empire->principe2 = principe2;
	empire->principe3 = principe3;
}
/**
 * Recuperer l'espèce
 */
Principe GetEmpirePrincipes(Empire *empire, int numero){
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
void SetEmpireName(Empire *empire, int curseur ,char lettre){
	if(lettre <= 29)
		empire->nom[lettre] = lettre;

	empire->nom[30] = '\0';
}
/**
 * Recuper Char
 */
char GetEmpireNameChar(Empire *empire, int cursor){
	return empire->nom[cursor];
}
/**
 * Recuper string
 */
char* GetEmpireNameString(Empire *empire){
	return empire->nom;
}

/**
 * Changer l'espèce
 */
void SetEmpireSpecies(Empire *empire, int espece){
	empire->espece = espece;
}
/**
 * Recuperer l'espèce
 */
int GetEmpireSpecies(Empire *empire){
	return empire->espece;
}

/**
 * Changer les habits
 */
void SetEmpireClothes(Empire *empire, int clothes){
	empire->vetements = clothes;
}
/**
 * Recuperer les habits
 */
int GetEmpireClothes(Empire *empire){
	return empire->vetements;
}

void SetEmpireCredit(Empire *empire, int argent){
	empire->credits = argent;
}
/**
 * Rajouter des credits
 */
void AddEmpireCredit(Empire *empire, int argent){
	empire->credits += argent;
}
/**
 * Recuperer nombre de credits
 */
int GetEmpireCredit(Empire *empire){
	return empire->credits;
}

void SetEmpireMinerals(Empire *empire, int minerais){
	empire->minerais = minerais;
}
/**
 * Rajouter des minerais
 */
void AddEmpireMinerals(Empire *empire, int minerais){
	empire->minerais += minerais;
}
/**
 * Recuperer nombre de minerais
 */
int GetEmpireMinerals(Empire *empire){
	return empire->minerais;
}

void SetEmpireFood(Empire *empire, int nourriture){
	empire->nourriture = nourriture;
}
/**
 * Rajouter de la nourriture
 */
void AddEmpireFood(Empire *empire, int nourriture){
	empire->nourriture += nourriture;
}
/**
 * Recuperer nombre de nourriture
 */
int GetEmpireFood(Empire *empire){
	return empire->nourriture;
}

void SetEmpireAlloys(Empire *empire, int alloy){
	empire->acier = alloy;
}
/**
 * Rajouter de l'acier
 */
void AddEmpireAlloys(Empire *empire, int alloy){
	empire->acier += alloy;
}
/**
 * Recuperer nombre d'acier
 */
int GetEmpireAlloys(Empire *empire){
	return empire->acier;
}

void SetEmpireConsumerGoods(Empire *empire, int consumerGoods){
	empire->biensDeConsommation = consumerGoods;
}
/**
 * Rajouter des biens de consommation
 */
void AddEmpireConsumerGoods(Empire *empire, int consumerGoods){
	empire->biensDeConsommation += consumerGoods;
}
/**
 * Recuperer nombre de biens de consommation
 */
int GetEmpireConsumerGoods(Empire *empire){
	return empire->biensDeConsommation;
}

void EmpireGenerateRandomName(Empire *empire){
	char name[31] = "";
    int nomInt = 0;
	nomInt = randInt(0, (sizeof(empireNamePrefix)/sizeof(empireNamePrefix[0])) - 1);
	strcpy(name, empireNamePrefix[nomInt]);
	switch(empire->gouvernement){
		case DEMOCRATIE:
			nomInt = randInt(0, (sizeof(empireNamePostFixDemocratic)/sizeof(empireNamePostFixDemocratic[0])) - 1);
			strcat(name, empireNamePostFixDemocratic[nomInt]);
			break;
		case OLIGARCHIE:
			nomInt = randInt(0, (sizeof(empireNamePostFixOligarchic)/sizeof(empireNamePostFixOligarchic[0])) - 1);
			strcat(name, empireNamePostFixOligarchic[nomInt]);
			break;
		case DICTATORIALE:
			nomInt = randInt(0, (sizeof(empireNamePostFixDictatorial)/sizeof(empireNamePostFixDictatorial[0])) - 1);
			strcat(name, empireNamePostFixDictatorial[nomInt]);
			break;
		default:
			nomInt = randInt(0, (sizeof(empireNamePostFixDemocratic)/sizeof(empireNamePostFixDemocratic[0])) - 1);
			strcat(name, empireNamePostFixDemocratic[nomInt]);
			break;
	}
	strcpy(empire->nom, name);
}

void CalculateEmpireFleetPower(Empire *empire){
	empire->PuissanceMilitaire = CalculateFleetPower(empire->flotte);
}

int GetEmpireFleetPower(Empire *empire){
	return empire->PuissanceMilitaire;
}

/*Empires AI*/

static void PlanetaryAI(EmpireListe *empireListe, SystemeStellaire **systemeStellaires){
	int systemeNumero = 0;
	int planeteNumero = 0;
	int taille = 0;
	Empire *empire;
	Planete *planete = NULL;
	while(systemeNumero < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
		if(GetSystemEmpire != 0){
			planeteNumero = 0;
			empire = EmpireNumero(empireListe, GetSystemEmpire(systemeStellaires[systemeNumero]));
			taille = GetSystemPlanetNumber(systemeStellaires[systemeNumero]);
			while(planeteNumero < taille){
				if(GetPlanetCityStatus(GetSystemPlanet(systemeStellaires[systemeNumero], planeteNumero))){
					planete = GetSystemPlanet(systemeStellaires[systemeNumero], planeteNumero);
					/*La "planete" est habitée et "empire" contient son empire*/
					
				}
				planeteNumero++;
			}
		}
		systemeNumero++;
	}
}

static void EmpireAIEconomy(int numeroEmpire, Empire *empire, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date){
	if(GetTimeYear(date) < 2300){
		if(GetEmpireAlloys(empire) >= 100){
			OrdreFile *ordreQueue;
			Station *station;
			AddEmpireAlloys(empire, -100);
			station = GetSystemStation(systemeStellaires[GetEmpireSystemCapital(empire)]);
			ordreQueue = GetStationOrderQueue(station);
			NouvelOrdre(ordreQueue,
						1,
						CONSTRUIRE_VAISSEAU,
						3,
						FLOTTE_SCIENTIFIQUE,
						1,
						100
					);
		}
	}
}

static void EmpireAICivilianFleet(Empire *empire, EmpireListe *empireListe, Flotte *flotte, SystemeStellaire **systemeStellaires){
	if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
		int systeme = GetFleetSystem(flotte);
		if(GetFleetType(flotte) == FLOTTE_SCIENTIFIQUE){
			
		}
	}
}

static void EmpireAIMilitaryFleet(Empire *empire, EmpireListe *empireListe, Flotte *flotte, SystemeStellaire **systemeStellaires){

}

static void EmpireAIFleetManager(Empire *empire, EmpireListe *empireListe, SystemeStellaire **systemeStellaires){
	FlotteListe *flotteListe = GetFleetArray(empire);
	int tailleFlotte = FleetArraySize(flotteListe);
	if(tailleFlotte > 1){
		int compteurFlotte = 1;
		Flotte *flotte = NULL;

		while(compteurFlotte <= tailleFlotte){
			flotte = NumeroFlotte(flotteListe, compteurFlotte);
			if(GetFleetType == FLOTTE_MILITAIRE)
				EmpireAIMilitaryFleet(empire, empireListe, flotte, systemeStellaires);
			
			else
				EmpireAICivilianFleet(empire, empireListe, flotte, systemeStellaires);

			compteurFlotte++;
		}
	}
}

void EmpireAI(EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date){
	Empire *empire = NULL;
	int empireCounter = 2;
	int empireTotalNumber = 0;

	PlanetaryAI(empireListe, systemeStellaires);

	empireTotalNumber = EmpireArraySize(empireListe);
	while(empireCounter <= empireTotalNumber){
		empire = EmpireNumero(empireListe, empireCounter);

		EmpireAIEconomy(empireCounter, empire, empireListe, systemeStellaires, date);

		EmpireAIFleetManager(empire, empireListe, systemeStellaires);

		empireCounter++;
	}
}