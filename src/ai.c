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
#include "galaxy.h"

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
EmpireList* empire_ListCreate() {
    return (EmpireList*)GenericList_Create();
}

/**
 * Supprime une liste d'empire
 */
void empire_ListFree(EmpireList* empireListe) {
    Empire *empire = NULL;
    int i = 0;
    if(!empireListe)
        return;
    empire = GenericCell_Get((GenericList*)empireListe, i);
    while(empire != NULL) {
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free empire %d", i);
        #endif
        if(empire->flotte)
            SupprimerFlotteListe(empire->flotte);
        free(empire);
        i++;
        empire = GenericCell_Get((GenericList*)empireListe, i);
    }
    GenericList_Free((GenericList*)empireListe);
}

/**
 * Renvoi nombre d'empires
 */
int empire_ArraySize(EmpireList* empireListe){
    return GenericList_ArraySize((GenericList*)empireListe);
}

/**
 * Renvoi un pointeur vers l'empire numero x
 */
Empire* empire_Get(EmpireList* empireListe, int numero) {
    return (Empire*)GenericCell_Get((GenericList*)empireListe, numero);
}

/**
 * Rajoute un empire à la liste des empire
 */
Empire* empire_Add(EmpireList* empireListe) {
    Empire *pointeur = NULL;
    pointeur = calloc(1, sizeof(Empire));
    GenericCell_Add((GenericList*)empireListe, pointeur);
    return pointeur;
}

/**
 *Supprime l'empire numero x à la liste des empires
 */
void empire_Free(EmpireList* empireListe, int numero) {
    Empire *empire = GenericCell_Get((GenericList*)empireListe, numero);
    #ifdef DEBUG_VERSION
    dbg_sprintf(dbgout, "Free empire %d", numero);
    #endif
    SupprimerFlotteListe(empire->flotte);
    free(empire);
    GenericCell_Free((GenericList*)empireListe, numero);
}

/**
 * Crèe une liste de flotte d'empire
 */
void empire_FleetListCreate(Empire *empire){
    empire->flotte = CreerFlotteListe();
    if(empire->flotte == NULL){
        exit(EXIT_FAILURE);
    }
}
void empire_FleetAdd(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses){
    NouvelleFlotte(empire->flotte, systeme, type, nombreDeCorvettes, nombreDeDestroyers, nombreDeCroiseurs, nombreDeCuirasses);
}
FlotteListe *empire_FleetListGet(Empire *empire){
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

void empire_CapitalSystemSet(Empire *empire, int system){
    empire->systemeCapitale = system;
}
int GetEmpireSystemCapital(Empire *empire){
    return empire->systemeCapitale;
}

/**
 * Change la couleur
 */
void empire_ColorSet(Empire *empire, Gouvernement couleur){
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
    return (RelationsListe*)GenericList_Create();
}

/**
 * Met à jour toutes les listes de relations
 */
void ai_RelationsUpdate(EmpireList* empireListe) {
    Empire* empire;
    int index = 0;
    empire = empire_Get(empireListe, index);
    while(empire != NULL) {
        empire->relationsListe = RelationListeCreer();
        RelationListeUpdate(empire->relationsListe, empireListe);
        #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "Empire list %d updated : %p \n", index, empire->relationsListe);
        #endif
        index++;
        empire = empire_Get(empireListe, index);
    }
}

/**
 * Met à jour une liste de relations
 */
void RelationListeUpdate(RelationsListe* relationsListe, EmpireList* empireListe) {
    Empire* empire;
    Relations* relations;
    int index = 0;
    empire = empire_Get(empireListe, 0);
    while(empire != NULL) {
        relations = RelationAjouter(relationsListe);
        relations->empire = empire;
        relations->opinion = 0;
        index++;
        empire = empire_Get(empireListe, index);
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

static void PlanetaryAI(EmpireList *empireListe, StarSystem **systemeStellaires){
    int systemeNumero = 0;
    int planeteNumero = 0;
    int taille = 0;
    Empire *empire;
    Planet *planete = NULL;
    while(systemeNumero < GALAXY_WIDTH * GALAXY_WIDTH){
        if(starSystem_EmpireGet(systemeStellaires[systemeNumero]) != 0){
            planeteNumero = 0;
            empire = empire_Get(empireListe, starSystem_EmpireGet(systemeStellaires[systemeNumero]));
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

static void EmpireAIEconomy(int numeroEmpire, Empire *empire, EmpireList *empireListe, StarSystem **systemeStellaires, Time *date){
    if(GetTimeYear(date) < 2300){
        //constuire flotte scientifique
        if(GetEmpireAlloys(empire) >= 100){
            OrdreFile *ordreQueue;
            Station *station;
            AddEmpireAlloys(empire, -100);
            station = starSystem_StationGet(systemeStellaires[GetEmpireSystemCapital(empire)]);
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

static void EmpireAICivilianFleet(Empire *empire, EmpireList *empireListe, Flotte *flotte, StarSystem **systemeStellaires){
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

static void EmpireAIMilitaryFleet(Empire *empire, EmpireList *empireListe, Flotte *flotte, StarSystem **systemeStellaires){

}

static void EmpireAIFleetManager(Empire *empire, EmpireList *empireListe, StarSystem **systemeStellaires){
    FlotteListe *flotteListe = empire_FleetListGet(empire);
    int tailleFlotte = FleetArraySize(flotteListe);
    if(tailleFlotte > 0){
        int compteurFlotte = 0;
        Flotte *flotte = NULL;

        while(compteurFlotte < tailleFlotte){
            flotte = FlotteNumero(flotteListe, compteurFlotte);
            if(GetFleetType(flotte) == FLOTTE_MILITAIRE)
                EmpireAIMilitaryFleet(empire, empireListe, flotte, systemeStellaires);
            
            else
                EmpireAICivilianFleet(empire, empireListe, flotte, systemeStellaires);

            compteurFlotte++;
        }
    }
}

void EmpireAI(EmpireList *empireListe, StarSystem **systemeStellaires, Time *date){
    Empire *empire = NULL;
    int empireCounter = 1;
    int empireTotalNumber = 0;

    PlanetaryAI(empireListe, systemeStellaires);

    empireTotalNumber = empire_ArraySize(empireListe);
    while(empireCounter < empireTotalNumber){
        empire = empire_Get(empireListe, empireCounter);

        EmpireAIEconomy(empireCounter, empire, empireListe, systemeStellaires, date);

        EmpireAIFleetManager(empire, empireListe, systemeStellaires);

        empireCounter++;
    }
}

void empire_Generate(   Empire *empire, 
                        int empireIndex,
                        StarSystem *empireStarSystem, 
                        int systemIndex,
                        int color){
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

    starSystem_StationLevelSet(empireStarSystem, PORT_STELLAIRE);
    starSystem_StationModuleSet(empireStarSystem, 0, CHANTIER_SPATIAL);
    starSystem_StationModuleSet(empireStarSystem, 1, CARREFOUR_COMMERCIAL);

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
    empire_FleetAdd(empire, systemIndex, FLOTTE_MILITAIRE, 3, 0, 0, 0);
    empire_FleetAdd(empire, systemIndex, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);
    empire_FleetAdd(empire, systemIndex, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);

    CalculateEmpirePower(empire);
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Generate Empire: %d (%p)\n   -System: %d\n   -Color: %d\n   -Planet: %d\n", empireIndex, empire, systemIndex, color, planetIndex);
    #endif
}