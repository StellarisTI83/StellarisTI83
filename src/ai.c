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

#include "main.h"

#include "ai.h"

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
	int creditsChange;
	int minerais;
	int mineraisChange;
	int nourriture;
	int nourritureChange;
	int acier;
	int acierChange;
	int biensDeConsommation;
	int biensDeConsommationChange;

	int PuissanceMilitaire;
	int PuissanceScientifique;
	int PuissanceEconomique;

	int systemeCapitale;

	FlotteListe* flotte;
	RelationsListe* relationsListe;
};

struct RelationsStruct {
	Empire* empire;
	int opinion;
	char opinionChange;
	Attitude attitude;
	char accords;
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
	return (EmpireListe*)CreateGenericList();
}

/**
 * Supprime une liste d'empire
 */
void EmpireListeSupprimer(EmpireListe* empireListe) {
	Empire *empire = NULL;
	int i = 1;
    empire = GenericCellGet((GenericList*)empireListe, i);
    while(empire != NULL) {
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Free empire %d, ", i);
		#endif
		if(empire->flotte)
			SupprimerFlotteListe(empire->flotte);
        free(empire);
		i++;
        empire = GenericCellGet((GenericList*)empireListe, i);
    }
	FreeGenericList((GenericList*)empireListe);
}

/**
 * Renvoi nombre d'empires
 */
int EmpireArraySize(EmpireListe* empireListe){
	return GenericListArraySize((GenericList*)empireListe);
}

/**
 * Renvoi un pointeur vers l'empire numero x
 */
Empire* EmpireNumero(EmpireListe* empireListe, int numero) {
	return (Empire*)GenericCellGet((GenericList*)empireListe, numero);
}

/**
 * Rajoute un empire à la liste des empire
 */
Empire* EmpireAjouter(EmpireListe* empireListe) {
	Empire *pointeur = NULL;
	pointeur = calloc(1, sizeof(Empire));
	GenericCellAdd((GenericList*)empireListe, pointeur);
	return pointeur;
}

/**
 *Supprime l'empire numero x à la liste des empires
 */
void EmpireSupprimer(EmpireListe* empireListe, int numero) {
	Empire *empire = GenericCellGet((GenericList*)empireListe, numero);
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "Free empire %d, ", numero);
	#endif
	SupprimerFlotteListe(empire->flotte);
	free(empire);
	FreeGenericCell((GenericList*)empireListe, numero);
}

/**
 * Crèe une liste de flotte d'empire
 */
void EmpireFlotteCreer(Empire *empire){
	empire->flotte = CreerFlotteListe();
	if(empire->flotte == NULL){
		exit(EXIT_FAILURE);
	}
}
void EmpireFlotteNouvelle(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
	NouvelleFlotte(empire->flotte, systeme, type, nombreDeCorvettes, nombreDeDestroyers, nombreDeCroiseurs, nombreDeCuirasses);
}
FlotteListe *EmpireFleetGetArray(Empire *empire){
	return empire->flotte;
}

/**
 * Crèe une liste de relations d'empire
 */
void EmpireCreerRelations(Empire *empire){
	empire->relationsListe = RelationListeCreer();
	if(empire->relationsListe == NULL){
		exit(EXIT_FAILURE);
	}
}
void EmpireRelationNouvelle(Empire *empire){
	RelationAjouter(empire->relationsListe);
}
RelationsListe *EmpireRelationGetArray(Empire *empire){
	return empire->relationsListe;
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
		empire->nom[curseur] = lettre;

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

void SetEmpireCreditChange(Empire *empire, int change){
	empire->creditsChange = change;
}
void AddEmpireCreditChange(Empire *empire, int change){
	empire->creditsChange += change;
}
int GetEmpireCreditChange(Empire *empire){
	return empire->creditsChange;
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

void SetEmpireMineralsChange(Empire *empire, int change){
	empire->mineraisChange = change;
}
void AddEmpireMineralsChange(Empire *empire, int change){
	empire->mineraisChange += change;
}
int GetEmpireMineralsChange(Empire *empire){
	return empire->mineraisChange;
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

void SetEmpireFoodChange(Empire *empire, int change){
	empire->nourritureChange = change;
}
void AddEmpireFoodChange(Empire *empire, int change){
	empire->nourritureChange += change;
}
int GetEmpireFoodChange(Empire *empire){
	return empire->nourritureChange;
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

void SetEmpireAlloysChange(Empire *empire, int change){
	empire->acierChange = change;
}
void AddEmpireAlloysChange(Empire *empire, int change){
	empire->acierChange += change;
}
int GetEmpireAlloysChange(Empire *empire){
	return empire->acierChange;
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

void SetEmpireConsumerGoodsChange(Empire *empire, int change){
	empire->biensDeConsommation = change;
}
void AddEmpireConsumerGoodsChange(Empire *empire, int change){
	empire->biensDeConsommation += change;
}
int GetEmpireConsumerGoodsChange(Empire *empire){
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

void CalculateEmpirePower(Empire *empire){
	empire->PuissanceMilitaire = CalculateFleetPower(empire->flotte);
	empire->PuissanceScientifique = 0;
	empire->PuissanceEconomique = empire->credits + empire->creditsChange * 3 + empire->minerais  + empire->mineraisChange * 3 + empire->acier + empire->acierChange * 3;
}

int GetEmpirePowerMilitary(Empire *empire){
	return empire->PuissanceMilitaire;
}
int GetEmpirePowerScientific(Empire *empire){
	return empire->PuissanceScientifique;
}
int GetEmpirePowerEconomic(Empire *empire){
	return empire->PuissanceEconomique;
}


/**
 * Crée une liste de relations
 */
RelationsListe* RelationListeCreer() {
	return (RelationsListe*)CreateGenericList();
}

/**
 * Met à jour toutes les listes de relations
 */
void RelationAllListeUpdate(EmpireListe* empireListe) {
	Empire* empire;
	Relations* relations;
	int index = 1;
	empire = EmpireNumero(empireListe, index);
	while(empire != NULL) {
		empire->relationsListe = RelationListeCreer();
		RelationListeUpdate(empire->relationsListe, empireListe);
		#ifdef DEBUG_VERSION
			dbg_sprintf(dbgout, "Empire liste %d updated : %p \n", index, empire->relationsListe);
		#endif
		index++;
		empire = EmpireNumero(empireListe, index);
	}
}

/**
 * Met à jour une liste de relations
 */
void RelationListeUpdate(RelationsListe* relationsListe, EmpireListe* empireListe) {
	Empire* empire;
	Relations* relations;
	int index = 1;
	empire = EmpireNumero(empireListe, 1);
	while(empire != NULL) {
		relations = RelationAjouter(relationsListe);
		relations->empire = empire;
		relations->opinion = 0;
		index++;
		empire = EmpireNumero(empireListe, index);
	}
}

/**
 * Supprime une liste de relations
 */
void RelationListeSupprimer(RelationsListe* relationsListe) {
	FreeGenericList((RelationsListe*)relationsListe);
}

/**
 * Renvoi nombre de relations
 */
int RelationArraySize(RelationsListe* relationsListe){
	return GenericListArraySize((GenericList*)relationsListe);
}

/**
 * Renvoi un pointeur vers les relations de l'empire numero x
 */
Relations* RelationNumero(RelationsListe* relationsListe, int numero) {
	return (Relations*)GenericCellGet((GenericList*)relationsListe, numero);
}
/**
 * Rajoute la relation d'un empire à la liste des relations
 */
Relations* RelationAjouter(RelationsListe* relationsListe) {
	Relations *pointeur = NULL;
	pointeur = calloc(1, sizeof(Relations));
	GenericCellAdd((GenericList*)relationsListe, pointeur);
	return pointeur;
}
/**
 *Supprime la relation numero x à la liste des relations
 */
void RelationSupprimer(RelationsListe* relationsListe, int numero) {
	FreeGenericCell((GenericList*)relationsListe, numero);
}

int RelationGetOpinion(Relations* relations) {
	return relations->opinion;
}
Attitude RelationGetAttitude(Relations* relations) {
	return relations->attitude;
}

void RelationAmeliorer(Relations* relations) {
	relations->opinionChange += 10;
}
void RelationDegrader(Relations* relations) {
	relations->opinionChange -= 10;
}
void RelationGuerreDeclarer(Relations* relations) {
	relations->opinion -= 50;
}
void RelationInsulter(Relations* relations) {
	relations->opinion -= 200;
}
void RelationSetPacte(Relations* relations, Pacte pacte) {
	relations->accords = (relations->accords | pacte);
}
Pacte RelationGetPacteStatus(Relations* relations, Pacte pacte) {
	if(relations->accords & pacte) {
		return true;
	}
	else {
		return false;
	}
}




/*Empires AI*/

static void PlanetaryAI(EmpireListe *empireListe, SystemeStellaire **systemeStellaires){
	int systemeNumero = 0;
	int planeteNumero = 0;
	int taille = 0;
	Empire *empire;
	Planete *planete = NULL;
	while(systemeNumero < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
		if(GetSystemEmpire(systemeStellaires) != 0){
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
		//constuire flotte scientifique
		if(GetEmpireAlloys(empire) >= 100){
			OrdreFile *ordreQueue;
			Station *station;
			AddEmpireAlloys(empire, -100);
			station = GetSystemStation(systemeStellaires[GetEmpireSystemCapital(empire)]);
			ordreQueue = GetStationOrderQueue(station);
			#ifdef DEBUG_VERSION
				dbg_sprintf(dbgout, "Empire %d create a science fleet in system %d\n", numeroEmpire, GetEmpireSystemCapital(empire));
			#endif
			NouvelOrdre(ordreQueue,
						CONSTRUIRE_VAISSEAU,
						numeroEmpire,
						3,
						FLOTTE_SCIENTIFIQUE,
						1,
						100);
		}
	}
}

static void EmpireAICivilianFleet(Empire *empire, EmpireListe *empireListe, Flotte *flotte, SystemeStellaire **systemeStellaires){
	if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
		// int systeme = GetFleetSystem(flotte);
		if(GetFleetType(flotte) == FLOTTE_SCIENTIFIQUE){
			if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
				int systemIndex = 0;
				int systemeDestination = 0;
				for(systemIndex = 0; systemIndex < 4; systemIndex++){
					systemeDestination = GetHyperlaneDestination(systemeStellaires[GetFleetSystem(flotte)], systemIndex);
					if(!GetSystemEmpire(systemeStellaires[systemeDestination]) && systemeDestination != 255){
						MoveFleet(flotte, GetHyperlaneDestination(systemeStellaires[GetFleetSystem(flotte)], systemIndex), systemeStellaires);
						#ifdef DEBUG_VERSION
							dbg_sprintf(dbgout, "Empire %p move science fleet from %d to %d\n", empire, GetFleetSystem(flotte), GetHyperlaneDestination(systemeStellaires[GetFleetSystem(flotte)], systemIndex));
						#endif
						systemIndex = 4;
					}
				}
			}
		}
	}
}

static void EmpireAIMilitaryFleet(Empire *empire, EmpireListe *empireListe, Flotte *flotte, SystemeStellaire **systemeStellaires){

}

static void EmpireAIFleetManager(Empire *empire, EmpireListe *empireListe, SystemeStellaire **systemeStellaires){
	FlotteListe *flotteListe = EmpireFleetGetArray(empire);
	int tailleFlotte = FleetArraySize(flotteListe);
	if(tailleFlotte > 1){
		int compteurFlotte = 1;
		Flotte *flotte = NULL;

		while(compteurFlotte <= tailleFlotte){
			flotte = FlotteNumero(flotteListe, compteurFlotte);
			if(GetFleetType(flotte) == FLOTTE_MILITAIRE)
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