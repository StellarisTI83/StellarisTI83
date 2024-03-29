/**
 * @file empire.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief All empire-creation and management, without ai things
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright GNU General Public License v3.0
 * 
 */

#include <debug.h>
#include <string.h>
#include <assert.h>

#include "ai.h"
#include "empire.h"

/* struct ============================================================== */

struct EmpireStruct {
    char    name[31];
    char    color; // Color on the map
    int     species;
    int     clothes;
    Governement governement;

    EmpireAi* empireAi;

    EmpirePlanetList*  empirePlanetList;
    int     capitalSystemIndex;

    Ethic   ethic1;
    Ethic   ethic2;
    Ethic   ethic3;

    int     credits;
    int     creditVariation;
    int     minerals;
    int     mineraisChange;
    int     foods;
    int     nourritureChange;
    int     acier;
    int     acierChange;
    int     biensDeConsommation;
    int     biensDeConsommationChange;

    int     PuissanceMilitaire;
    int     PuissanceScientifique;
    int     PuissanceEconomique;


    FleetList*      fleet;
    DiplomacyList*  relationsListe;
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

// Empire list

EmpireList* empire_ListCreate() {
    return (EmpireList*)GenericList_Create();
}

void empire_ListFree(EmpireList* empireList) {
    Empire *empire = NULL;
    int index = 0;
    assert(empireList);

    empire = GenericCell_Get((GenericList*)empireList, index);

    while(empire) {
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free empire %d", index);
        #endif

        if(empire->fleet)
            fleet_ListFree(empire->fleet);
        
        if(empire->relationsListe)
            diplomacy_ListFree(empire->relationsListe);

        free_count(empire);
        index++;
        empire = GenericCell_Get((GenericList*)empireList, index);
    }
    GenericList_Free((GenericList*)empireList);
}

int empire_ArraySize(const EmpireList* empireList){
    assert(empireList);

    return GenericList_ArraySize((GenericList*)empireList);
}

// Empire creation and destruction

Empire* empire_Get(const EmpireList* empireList, const int index) {
    assert(empireList);

    return (Empire*)GenericCell_Get((GenericList*)empireList, index);
}

Empire* empire_Add(EmpireList* empireList) {
    Empire *empire;

    assert(empireList);

    empire = calloc_count(1, sizeof(Empire));

    assert(empire);

    GenericCell_Add((GenericList*)empireList, empire);
    return empire;
}

void empire_Free(EmpireList* empireList, int numero) {
    Empire *empire;

    assert(empireList);

    empire = GenericCell_Get((GenericList*)empireList, numero);

    assert(empire);

    #ifdef DEBUG_VERSION
    dbg_sprintf(dbgout, "Free empire %d", numero);
    #endif

    if(empire->fleet)
        fleet_ListFree(empire->fleet);

    if(empire->relationsListe)
        diplomacy_ListFree(empire->relationsListe);

    free_count(empire);
    GenericCell_Free((GenericList*)empireList, numero);
}

void empire_Generate(   Empire *empire, 
                        const int empireIndex,
                        StarSystem *empireStarSystem, 
                        const int systemIndex,
                        const int color){
    Station *station = starSystem_StationGet(empireStarSystem);
    int planetIndex = randInt(0, starSystem_NumberOfPlanetGet(empireStarSystem) - 1);
    Planet *planet = starSystem_PlanetGet( empireStarSystem, 
                                planetIndex);
    City *city = planet_CityCreate(planet);

    assert(empire);

    empire_CreditSet(empire, 100);
    empire_MineralsSet(empire, 100);
    empire_FoodSet(empire, 200);
    empire_AlloysSet(empire, 100);
    empire_ConsumerSet(empire, 100);

    empire_ColorSet(empire, color);
    empire_NameGenerate(empire);
    empire_SpeciesSet(empire, randInt(0, 3));
    empire_SpeciesClothesSet(empire, randInt(0, 2));

    empire_SystemCapitalSet(empire, systemIndex);
    starSystem_StarTypeSet(empireStarSystem, STAR_TYPE_K);
    starSystem_EmpireSet(empireStarSystem, empireIndex);

    station_LevelSet(station, STATION_STARPORT);
    station_ModuleSet(station, 0, STATION_MODULE_SHIPYARD);
    station_ModuleSet(station, 1, STATION_MODULE_TRADE_HUB);

    starSystem_PlanetHabitabilitySet(empireStarSystem, planetIndex, true);
    planet_TypeSet(planet, HABITABLE_CONTINENTAL);
    planet_NameGenerate(planet);
    empire_PlanetListCreate();
    empire_PlanetAdd(empire, planet);

    city_PopulationSet(city, 27);
    city_DistrictSet(city, 4, 3, 3, 3);
    city_JobUpdate(city);

    city_BuildingSet(city, BUILDING_CAPITAL, 1, 3);
    city_BuildingSet(city, BUILDING_CIVILIAN_INDUSTRIES, 2, 1);
    city_BuildingSet(city, BUILDING_FOUNDRIES, 3, 1);

    empire_FleetListCreate(empire);
    fleet_New(empire->fleet, systemIndex, FLEET_MILITARY, 3, 0, 0, 0);
    fleet_New(empire->fleet, systemIndex, FLEET_CONSTRUCTION, 0, 0, 0, 0);
    fleet_New(empire->fleet, systemIndex, FLEET_SCIENTIFIC, 0, 0, 0, 0);

    empire_PowerUpdate(empire);

    ai_EmpireGenerate(empire);
    
    empire->empirePlanetList = empire_PlanetListCreate();
    empire_PlanetAdd(empire, planet);
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Generate Empire: %s (%d-%p)\n   -System: %d\n   -Color: %d\n   -Planet: %d\n", empire->name, empireIndex, empire, systemIndex, color, planetIndex);
    #endif
}

// Fleet

void empire_FleetListCreate(Empire *empire){
    assert(empire);

    empire->fleet = fleet_ListCreate();

    assert(empire->fleet);
}
FleetList *empire_FleetListGet(Empire *empire){
    return empire->fleet;
}

// Governement

void empire_GovernementSet(Empire *empire, Governement governement){
    assert(empire);
    empire->governement = governement;
}
Governement empire_GovernementGet(Empire *empire){
    assert(empire);
    return empire->governement;
}

// Color and system

void empire_SystemCapitalSet(Empire *empire, int system){
    assert(empire);
    empire->capitalSystemIndex = system;
}
int empire_SystemCapitalGet(Empire *empire){
    assert(empire);
    return empire->capitalSystemIndex;
}

void empire_ColorSet(Empire *empire, Governement couleur){
    assert(empire);
    empire->color = couleur;
}
int empire_ColorGet(Empire *empire){
    assert(empire);
    return empire->color;
}

// Ethic

void empire_EthicSet(Empire *empire, Ethic ethic1, Ethic ethic2, Ethic ethic3){
    assert(empire);
    empire->ethic1 = ethic1;
    empire->ethic2 = ethic2;
    empire->ethic3 = ethic3;
}

Ethic empire_EthicGet(Empire *empire, int index){
    assert(empire);
    switch(index){
    case 1:
        return empire->ethic1;
        break;
    case 2:
        return empire->ethic2;
        break;
    case 3:
        return empire->ethic3;
        break;
    }
    return 0;
}

// Species

void empire_SpeciesSet(Empire *empire, int espece){
    assert(empire);
    empire->species = espece;
}

int empire_SpeciesGet(Empire *empire){
    assert(empire);
    return empire->species;
}


void empire_SpeciesClothesSet(Empire *empire, int clothes){
    assert(empire);
    empire->clothes = clothes;
}

int empire_SpeciesClothesGet(Empire *empire){
    assert(empire);
    return empire->clothes;
}

// Name

void empire_NameGenerate(Empire *empire){
    char name[31] = {0};
    int nomInt = 0;
    
    assert(empire);

    nomInt = randInt(0, (sizeof(empireNamePrefix)/sizeof(empireNamePrefix[0])) - 1);
    strcpy(name, empireNamePrefix[nomInt]);
    switch(empire->governement){
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
    strcpy(empire->name, name);
}

char* empire_NameStringGet(Empire *empire){
    assert(empire);
    return empire->name;
}

// Habitated planets

EmpirePlanetList* empire_PlanetListCreate() {
    return (EmpirePlanetList*)GenericList_Create();
}

void empire_PlanetListFree(EmpirePlanetList* empirePlanetList) {
    Empire *planetListCell = NULL;
    int index = 0;
    assert(empirePlanetList);

    planetListCell = GenericCell_Get((GenericList*)empirePlanetList, index);

    while(planetListCell) {
        free_count(planetListCell);
        index++;
        planetListCell = GenericCell_Get((GenericList*)empirePlanetList, index);
    }
    GenericList_Free((GenericList*)empirePlanetList);
}

Planet* empire_PlanetGet(const Empire* empire, const int index) {
    assert(empire);

    return (Planet*)GenericCell_Get((GenericList*)empire->empirePlanetList, index);
}

Planet* empire_PlanetAdd(const Empire* empire, Planet *planet) {
    assert(empire);

    GenericCell_Add((GenericList*)empire->empirePlanetList, planet);
    return planet;
} 

// Ressources

void empire_CreditSet(Empire *empire, int credits){
    assert(empire);
    empire->credits = credits;
}

void empire_CreditAdd(Empire *empire, int credits){
    assert(empire);
    empire->credits += credits;
}

int empire_CreditGet(Empire *empire){
    assert(empire);
    return empire->credits;
}

void empire_CreditVariationSet(Empire *empire, int variation){
    assert(empire);
    empire->creditVariation = variation;
}
void empire_CreditVariationAdd(Empire *empire, int variation){
    assert(empire);
    empire->creditVariation += variation;
}
int empire_CreditVariationGet(Empire *empire){
    assert(empire);
    return empire->creditVariation;
}



void empire_MineralsSet(Empire *empire, int minerals){
    assert(empire);
    empire->minerals = minerals;
}
void empire_MineralsAdd(Empire *empire, int minerals){
    assert(empire);
    empire->minerals += minerals;
}
int empire_MineralsGet(Empire *empire){
    assert(empire);
    return empire->minerals;
}

void empire_MineralsVariationSet(Empire *empire, int variation){
    assert(empire);
    empire->mineraisChange = variation;
}
void empire_MineralsVariationAdd(Empire *empire, int variation){
    assert(empire);
    empire->mineraisChange += variation;
}
int empire_MineralsVariationGet(Empire *empire){
    assert(empire);
    return empire->mineraisChange;
}



void empire_FoodSet(Empire *empire, int foods){
    assert(empire);
    empire->foods = foods;
}
void empire_FoodAdd(Empire *empire, int foods){
    assert(empire);
    empire->foods += foods;
}
int empire_FoodGet(Empire *empire){
    assert(empire);
    return empire->foods;
}

void empire_FoodVariationSet(Empire *empire, int variation){
    assert(empire);
    empire->nourritureChange = variation;
}
void empire_FoodVariationAdd(Empire *empire, int variation){
    assert(empire);
    empire->nourritureChange += variation;
}
int empire_FoodVariationGet(Empire *empire){
    assert(empire);
    return empire->nourritureChange;
}



void empire_AlloysSet(Empire *empire, int alloy){
    assert(empire);
    empire->acier = alloy;
}
void empire_AlloysAdd(Empire *empire, int alloy){
    assert(empire);
    empire->acier += alloy;
}
int empire_AlloysGet(Empire *empire){
    assert(empire);
    return empire->acier;
}

void empire_AlloysVariationSet(Empire *empire, int variation){
    assert(empire);
    empire->acierChange = variation;
}
void empire_AlloysVariationAdd(Empire *empire, int variation){
    assert(empire);
    empire->acierChange += variation;
}
int empire_AlloysVariationGet(Empire *empire){
    assert(empire);
    return empire->acierChange;
}



void empire_ConsumerSet(Empire *empire, int consumerGoods){
    assert(empire);
    empire->biensDeConsommation = consumerGoods;
}
void empire_ConsumerAdd(Empire *empire, int consumerGoods){
    assert(empire);
    empire->biensDeConsommation += consumerGoods;
}
int empire_ConsumerGet(Empire *empire){
    assert(empire);
    return empire->biensDeConsommation;
}

void empire_ConsumerVariationSet(Empire *empire, int variation){
    assert(empire);
    empire->biensDeConsommation = variation;
}
void empire_ConsumerVariationAdd(Empire *empire, int variation){
    assert(empire);
    empire->biensDeConsommation += variation;
}
int empire_ConsumerVariationGet(Empire *empire){
    assert(empire);
    return empire->biensDeConsommation;
}

// Power

void empire_PowerUpdate(Empire *empire){
    assert(empire);
    empire->PuissanceMilitaire = CalculateFleetPower(empire->fleet);
    empire->PuissanceScientifique = 0;
    empire->PuissanceEconomique = empire->credits + empire->creditVariation * 3 + empire->minerals  + empire->mineraisChange * 3 + empire->acier + empire->acierChange * 3;
}

int empire_PowerMilitaryGet(Empire *empire){
    assert(empire);
    return empire->PuissanceMilitaire;
}
int empire_PowerScienceGet(Empire *empire){
    assert(empire);
    return empire->PuissanceScientifique;
}
int empire_PowerEconomicGet(Empire *empire){
    assert(empire);
    return empire->PuissanceEconomique;
}

// Diplomacy
void empire_DiplomacyCreate(Empire *empire){
    assert(empire);
    empire->relationsListe = diplomacy_ListCreate();
    assert(empire->relationsListe);
}
void empire_DiplomacyNew(Empire *empire){
    assert(empire);
    diplomacy_RelationsAdd(empire->relationsListe);
}
DiplomacyList *empire_DiplomacyListGet(Empire *empire){
    assert(empire);
    return empire->relationsListe;
}


DiplomacyList* diplomacy_ListCreate() {
    return (DiplomacyList*)GenericList_Create();
}

void diplomacy_AllUpdate(EmpireList* empireList) {
    Empire* empire;
    int index = 0;
    assert(empireList);
    empire = empire_Get(empireList, index);
    while(empire) {
        empire->relationsListe = diplomacy_ListCreate();
        diplomacy_Update(empire->relationsListe, empireList);
        #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "Empire list %d updated : %p \n", index, empire->relationsListe);
        #endif
        index++;
        empire = empire_Get(empireList, index);
    }
}

void diplomacy_Update(DiplomacyList* relationsListe, EmpireList* empireList) {
    Empire* empire;
    Diplomacy* relations;
    int index = 0;
    assert(empireList);
    assert(relationsListe);
    empire = empire_Get(empireList, 0);
    while(empire) {
        relations = diplomacy_RelationsAdd(relationsListe);
        relations->empire = empire;
        relations->opinion = 0;
        index++;
        empire = empire_Get(empireList, index);
    }
}

void diplomacy_ListFree(DiplomacyList* relationsListe) {
    GenericList_Free((DiplomacyList*)relationsListe);
}

int diplomacy_ArraySize(DiplomacyList* relationsListe){
    return GenericList_ArraySize((GenericList*)relationsListe);
}

Diplomacy* diplomacy_RelationsGet(DiplomacyList* relationsListe, int numero) {
    return (Diplomacy*)GenericCell_Get((GenericList*)relationsListe, numero);
}

Diplomacy* diplomacy_RelationsAdd(DiplomacyList* relationsListe) {
    Diplomacy *pointeur = NULL;
    pointeur = calloc_count(1, sizeof(Diplomacy));
    GenericCell_Add((GenericList*)relationsListe, pointeur);
    return pointeur;
}

void diplomacy_RelationsFree(DiplomacyList* relationsListe, int numero) {
    GenericCell_Free((GenericList*)relationsListe, numero);
}

int diplomacy_OpinionGet(Diplomacy* relations) {
    return relations->opinion;
}
Behavior diplomacy_AttitudeGet(Diplomacy* relations) {
    return relations->attitude;
}

void diplomacy_RelationsUpgrade(Diplomacy* relations) {
    relations->opinionChange += 10;
}
void diplomacy_RelationDowngrade(Diplomacy* relations) {
    relations->opinionChange -= 10;
}
void diplomacy_WarDeclare(Diplomacy* relations) {
    relations->opinion -= 50;
}
void diplomacy_Insult(Diplomacy* relations) {
    relations->opinion -= 200;
}
void diplomacy_AgreementSign(Diplomacy* relations, Agreements pacte) {
    relations->accords = (relations->accords | pacte);
}
Agreements diplomacy_AgreementStatus(Diplomacy* relations, Agreements pacte) {
    if(relations->accords & pacte) {
        return true;
    }
    else {
        return false;
    }
}

EmpireAi* empire_AiGet(const Empire* empire){
    assert(empire);
    return empire->empireAi;
}

void empire_AiSet(Empire* empire, EmpireAi* empireAi){
    assert(empire);
    assert(empireAi);
    empire->empireAi = empireAi;
}