/**
 * @file ai.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief All ai related files, like the creation of an empire or updating the
 *        actions he takes
 * @version 0.1
 * @date 2021-08-26
 * 
 * @copyright GNU General Public License v3.0
 * 
 */

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
#include "galaxy.h"

/* structures ========================================================== */

struct EmpireStruct {
    char nom[31];
    char couleur; //couleur sur la map
    int espece;
    int vetements;
    Governement gouvernement;

    Ethic principe1;
    Ethic principe2;
    Ethic principe3;

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

    FleetList* fleet;
    RelationsListe* relationsListe;
};

struct RelationsStruct {
    Empire* empire;
    int opinion;
    char opinionChange;
    Behavior attitude;
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

EmpireList* empire_ListCreate() {
    return (EmpireList*)GenericList_Create();
}

void empire_ListFree(EmpireList* empireList) {
    Empire *empire = NULL;
    int index = 0;
    if(empireList)
        empire = GenericCell_Get((GenericList*)empireList, index);
    else{
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying free empire list : no empireList\n");
        #endif
        return;
    }

    while(empire) {
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free empire %d", index);
        #endif

        if(empire->fleet)
            fleet_ListFree(empire->fleet);
        
        if(empire->relationsListe)
            RelationListeSupprimer(empire->relationsListe);

        free(empire);
        index++;
        empire = GenericCell_Get((GenericList*)empireList, index);
    }
    GenericList_Free((GenericList*)empireList);
}

int empire_ArraySize(const EmpireList* empireList){
    if(empireList)
        return GenericList_ArraySize((GenericList*)empireList);
    else {
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying array size empire list : no empireList\n");
        #endif
        return 0;
    }
}

Empire* empire_Get(const EmpireList* empireList, const int numero) {
    if(empireList)
        return (Empire*)GenericCell_Get((GenericList*)empireList, numero);
    else {
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get empire in empire list : no empireList\n");
        #endif
        return NULL;
    }
}

Empire* empire_Add(EmpireList* empireList) {
    Empire *empire;
    if(!empireList){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying add empire in empire list : no empireList\n");
        #endif
        return NULL;
    }
    empire = calloc(1, sizeof(Empire));
    if(!empire){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Cannot malloc\n");
        #endif
        exit(EXIT_FAILURE);
    }
    GenericCell_Add((GenericList*)empireList, empire);
    return empire;
}

void empire_Free(EmpireList* empireList, int numero) {
    Empire *empire;

    if(empireList)
        empire = GenericCell_Get((GenericList*)empireList, numero);
    else{
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying free empire : no empireList\n");
        #endif
        return;
    }

    if(!empire){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying free empire : no empire number %d\n", numero);
        #endif
        return;
    }

    #ifdef DEBUG_VERSION
    dbg_sprintf(dbgout, "Free empire %d", numero);
    #endif

    if(empire->fleet)
        fleet_ListFree(empire->fleet);

    if(empire->relationsListe)
        RelationListeSupprimer(empire->relationsListe);

    free(empire);
    GenericCell_Free((GenericList*)empireList, numero);
}

void empire_FleetListCreate(Empire *empire){
    if(!empire){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying create empire fleet : empire NULL\n");
        #endif
        return;
    }
    empire->fleet = fleet_ListCreate();
    if(!empire->fleet)
        exit(EXIT_FAILURE);
}
void empire_FleetAdd(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
    fleet_New(empire->fleet, systeme, type, nombreDeCorvettes, nombreDeDestroyers, nombreDeCroiseurs, nombreDeCuirasses);
}
FleetList *empire_FleetListGet(Empire *empire){
    return empire->fleet;
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
void SetEmpireGouvernement(Empire *empire, Governement gouvernement){
    empire->gouvernement = gouvernement;
}
/**
 * Recuperer l'espèce
 */
Governement GetEmpireGouvernement(Empire *empire){
    return empire->gouvernement;
}

void empire_CapitalSystemSet(Empire *empire, int system){
    empire->systemeCapitale = system;
}
int GetEmpireSystemCapital(Empire *empire){
    return empire->systemeCapitale;
}

/**
 * Change la couleur
 */
void empire_ColorSet(Empire *empire, Governement couleur){
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
void SetEmpirePrincipes(Empire *empire, Ethic principe1, Ethic principe2, Ethic principe3){
    empire->principe1 = principe1;
    empire->principe2 = principe2;
    empire->principe3 = principe3;
}
/**
 * Recuperer l'espèce
 */
Ethic GetEmpirePrincipes(Empire *empire, int numero){
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
void empire_SpeciesSet(Empire *empire, int espece){
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
void empire_SpeciesClothesSet(Empire *empire, int clothes){
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

void empire_NameGenerate(Empire *empire){
    char name[31] = "";
    int nomInt = 0;
    nomInt = randInt(0, (sizeof(empireNamePrefix)/sizeof(empireNamePrefix[0])) - 1);
    strcpy(name, empireNamePrefix[nomInt]);
    switch(empire->gouvernement){
        case GOVENEMENT_DEMOCRATIC:
            nomInt = randInt(0, (sizeof(empireNamePostFixDemocratic)/sizeof(empireNamePostFixDemocratic[0])) - 1);
            strcat(name, empireNamePostFixDemocratic[nomInt]);
            break;
        case GOVERNEMENT_OLIGARCHIC:
            nomInt = randInt(0, (sizeof(empireNamePostFixOligarchic)/sizeof(empireNamePostFixOligarchic[0])) - 1);
            strcat(name, empireNamePostFixOligarchic[nomInt]);
            break;
        case GOVERNEMENT_DICTATORIAL:
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
    empire->PuissanceMilitaire = CalculateFleetPower(empire->fleet);
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
    return (RelationsListe*)GenericList_Create();
}

/**
 * Met à jour toutes les listes de relations
 */
void ai_RelationsUpdate(EmpireList* empireList) {
    Empire* empire;
    int index = 0;
    empire = empire_Get(empireList, index);
    while(empire != NULL) {
        empire->relationsListe = RelationListeCreer();
        RelationListeUpdate(empire->relationsListe, empireList);
        #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "Empire list %d updated : %p \n", index, empire->relationsListe);
        #endif
        index++;
        empire = empire_Get(empireList, index);
    }
}

/**
 * Met à jour une liste de relations
 */
void RelationListeUpdate(RelationsListe* relationsListe, EmpireList* empireList) {
    Empire* empire;
    Relations* relations;
    int index = 0;
    empire = empire_Get(empireList, 0);
    while(empire != NULL) {
        relations = RelationAjouter(relationsListe);
        relations->empire = empire;
        relations->opinion = 0;
        index++;
        empire = empire_Get(empireList, index);
    }
}

/**
 * Supprime une liste de relations
 */
void RelationListeSupprimer(RelationsListe* relationsListe) {
    GenericList_Free((RelationsListe*)relationsListe);
}

/**
 * Renvoi nombre de relations
 */
int RelationArraySize(RelationsListe* relationsListe){
    return GenericList_ArraySize((GenericList*)relationsListe);
}

/**
 * Renvoi un pointeur vers les relations de l'empire numero x
 */
Relations* RelationNumero(RelationsListe* relationsListe, int numero) {
    return (Relations*)GenericCell_Get((GenericList*)relationsListe, numero);
}
/**
 * Rajoute la relation d'un empire à la liste des relations
 */
Relations* RelationAjouter(RelationsListe* relationsListe) {
    Relations *pointeur = NULL;
    pointeur = calloc(1, sizeof(Relations));
    GenericCell_Add((GenericList*)relationsListe, pointeur);
    return pointeur;
}
/**
 *Supprime la relation numero x à la liste des relations
 */
void RelationSupprimer(RelationsListe* relationsListe, int numero) {
    GenericCell_Free((GenericList*)relationsListe, numero);
}

int RelationGetOpinion(Relations* relations) {
    return relations->opinion;
}
Behavior RelationGetAttitude(Relations* relations) {
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
void RelationSetPacte(Relations* relations, Agreements pacte) {
    relations->accords = (relations->accords | pacte);
}
Agreements RelationGetPacteStatus(Relations* relations, Agreements pacte) {
    if(relations->accords & pacte) {
        return true;
    }
    else {
        return false;
    }
}




/*Empires AI*/

static void PlanetaryAI(EmpireList *empireList, StarSystem **systemeStellaires){
    int systemeNumero = 0;
    int planeteNumero = 0;
    int taille = 0;
    Empire *empire;
    Planet *planete = NULL;
    while(systemeNumero < GALAXY_WIDTH * GALAXY_WIDTH){
        if(starSystem_EmpireGet(systemeStellaires[systemeNumero]) != 0){
            planeteNumero = 0;
            empire = empire_Get(empireList, starSystem_EmpireGet(systemeStellaires[systemeNumero]));
            taille = starSystem_NumberOfPlanetGet(systemeStellaires[systemeNumero]);
            while(planeteNumero < taille){
                if(planet_CityGet(starSystem_PlanetGet(systemeStellaires[systemeNumero], planeteNumero))){
                    planete = starSystem_PlanetGet(systemeStellaires[systemeNumero], planeteNumero);
                    /*La "planete" est habitée et "empire" contient son empire*/
                    
                }
                planeteNumero++;
            }
        }
        systemeNumero++;
    }
}

static void EmpireAIEconomy(int numeroEmpire, Empire *empire, StarSystem **systemeStellaires, Time *date){
    if(time_YearGet(date) < 2300){
        //constuire flotte scientifique
        if(GetEmpireAlloys(empire) >= 100){
            OrdreFile *ordreQueue;
            Station *station;
            AddEmpireAlloys(empire, -100);
            station = starSystem_StationGet(systemeStellaires[GetEmpireSystemCapital(empire)]);
            ordreQueue = station_OrderQueueGet(station);
            #ifdef DEBUG_VERSION
                dbg_sprintf(dbgout, "Empire %d create a science fleet in system %d\n", numeroEmpire, GetEmpireSystemCapital(empire));
            #endif
            NouvelOrdre(ordreQueue,
                        STATION_ORDER_BUILD_SHIP,
                        numeroEmpire,
                        3,
                        FLOTTE_SCIENTIFIQUE,
                        1,
                        100);
        }
    }
}

static void EmpireAICivilianFleet(Empire *empire, Fleet *flotte, StarSystem **systemeStellaires){
    if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
        // int systeme = GetFleetSystem(flotte);
        if(GetFleetType(flotte) == FLOTTE_SCIENTIFIQUE){
            if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
                int systemIndex = 0;
                int systemeDestination = 0;
                for(systemIndex = 0; systemIndex < 4; systemIndex++){
                    systemeDestination = hyperlane_DestinationGet(systemeStellaires[GetFleetSystem(flotte)], systemIndex);
                    if(!starSystem_EmpireGet(systemeStellaires[systemeDestination]) && systemeDestination != 255){
                        MoveFleet(flotte, hyperlane_DestinationGet(systemeStellaires[GetFleetSystem(flotte)], systemIndex), systemeStellaires);
                        #ifdef DEBUG_VERSION
                            dbg_sprintf(dbgout, "Empire %p move science fleet from %d to %d\n", empire, GetFleetSystem(flotte), hyperlane_DestinationGet(systemeStellaires[GetFleetSystem(flotte)], systemIndex));
                        #endif
                        systemIndex = 4;
                    }
                }
            }
        }
    }
}

static void EmpireAIMilitaryFleet(){

}

static void EmpireAIFleetManager(Empire *empire, StarSystem **systemeStellaires){
    FleetList *flotteListe = empire_FleetListGet(empire);
    int tailleFlotte = FleetArraySize(flotteListe);
    if(tailleFlotte > 0){
        int compteurFlotte = 0;
        Fleet *flotte = NULL;

        while(compteurFlotte < tailleFlotte){
            flotte = FlotteNumero(flotteListe, compteurFlotte);
            if(GetFleetType(flotte) == FLOTTE_MILITAIRE)
                EmpireAIMilitaryFleet();
            
            else
                EmpireAICivilianFleet(empire, flotte, systemeStellaires);

            compteurFlotte++;
        }
    }
}

void EmpireAI(EmpireList *empireList, StarSystem **systemeStellaires, Time *date){
    Empire *empire = NULL;
    int empireCounter = 1;
    int empireTotalNumber = 0;

    PlanetaryAI(empireList, systemeStellaires);

    empireTotalNumber = empire_ArraySize(empireList);
    while(empireCounter < empireTotalNumber){
        empire = empire_Get(empireList, empireCounter);

        EmpireAIEconomy(empireCounter, empire, systemeStellaires, date);

        EmpireAIFleetManager(empire, systemeStellaires);

        empireCounter++;
    }
}

void empire_Generate(   Empire *empire, 
                        int empireIndex,
                        StarSystem *empireStarSystem, 
                        int systemIndex,
                        int color){
    Station *station = starSystem_StationGet(empireStarSystem);
    int planetIndex = randInt(0, starSystem_NumberOfPlanetGet(empireStarSystem) - 1);
    Planet *planet = starSystem_PlanetGet( empireStarSystem, 
                                planetIndex);
    City *city = planet_CityCreate(planet);

    SetEmpireCredit(empire, 100);
    SetEmpireMinerals(empire, 100);
    SetEmpireFood(empire, 200);
    SetEmpireAlloys(empire, 100);
    SetEmpireConsumerGoods(empire, 100);

    empire_ColorSet(empire, color);
    empire_NameGenerate(empire);
    empire_SpeciesSet(empire, randInt(0, 3));
    empire_SpeciesClothesSet(empire, randInt(0, 2));

    empire_CapitalSystemSet(empire, systemIndex);
    starSystem_StarTypeSet(empireStarSystem, STAR_TYPE_K);
    starSystem_EmpireSet(empireStarSystem, empireIndex);

    station_LevelSet(station, STATION_STARPORT);
    station_ModuleSet(station, 0, STATION_MODULE_SHIPYARD);
    station_ModuleSet(station, 1, STATION_MODULE_TRADE_HUB);

    starSystem_PlanetHabitabilitySet(empireStarSystem, planetIndex, true);
    planet_TypeSet(planet, HABITABLE_CONTINENTAL);
    planet_NameGenerate(planet);

    city_PopulationSet(city, 27);
    city_DistrictSet(city, 4, 3, 3, 3);
    city_JobUpdate(city);

    city_BuildingSet(city, BUILDING_CAPITAL, 1, 3);
    city_BuildingSet(city, BUILDING_CIVILIAN_INDUSTRIES, 2, 1);
    city_BuildingSet(city, BUILDING_FOUNDRIES, 3, 1);

    empire_FleetListCreate(empire);
    fleet_New(empire->fleet, systemIndex, FLOTTE_MILITAIRE, 3, 0, 0, 0);
    fleet_New(empire->fleet, systemIndex, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);
    fleet_New(empire->fleet, systemIndex, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);

    CalculateEmpirePower(empire);
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Generate Empire: %d (%p)\n   -System: %d\n   -Color: %d\n   -Planet: %d\n", empireIndex, empire, systemIndex, color, planetIndex);
    #endif
}