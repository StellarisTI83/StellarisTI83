
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include "main.h"

#include "systemes.h"
#include "stations.h"

/* structures ========================================================== */
struct HyperlaneStruct{
    int         destination;
    int         x;
    int         y;
};

struct StarSystemStruct{
    char        name[10];
    int         x;
    int         y;

    StarType    starType;
    char        planetNumber;
    char        inhabitedPlanetNumber;
    char        habitablePlanetNumber;
    char        empire;

    Hyperlane   hyperlane[4];

    IntelLevel  intelLevel;

    Planete     *planets[5];
    Station     *station;
};

/* entry points ======================================================== */

StarSystem* starSystem_Create(){
    return calloc(1, sizeof(StarSystem));
}

void starSystem_SetXY(StarSystem *systemeStellaire, int x, int y){
    systemeStellaire->x = x;
    systemeStellaire->y = y;
}
int starSystem_GetX(StarSystem *systemeStellaire){
    return systemeStellaire->x;
}
int starSystem_GetY(StarSystem *systemeStellaire){
    return systemeStellaire->y;
}

//station
void CreateSystemStation(StarSystem *systemeStellaire){
    systemeStellaire->station = AllocStation();
}

void SetSystemStationModule(StarSystem *systemeStellaire, int moduleNumber, Module module){
    SetStationModule(systemeStellaire->station, moduleNumber, module);
}
Module GetSystemStationModule(StarSystem *systemeStellaire, int moduleNumber){
    return GetStationModule(systemeStellaire->station, moduleNumber);
}

void SetSystemIntelLevel(StarSystem *systemeStellaire, IntelLevel niveau){
    systemeStellaire->intelLevel = niveau;
}
IntelLevel GetSystemIntelLevel(StarSystem *systemeStellaire){
    return systemeStellaire->intelLevel;
}

void SetSystemStarType(StarSystem *systemeStellaire, StarType type){
    systemeStellaire->starType = type;
}
StarType GetSystemStarType(StarSystem *systemeStellaire){
    return systemeStellaire->starType;
}

//stations
Station *GetSystemStation(StarSystem *systemeStellaire){
    return systemeStellaire->station;
}
void SetSystemStationLevel(StarSystem *systemeStellaire, Stationlevel level){
    SetStationLevel(systemeStellaire->station, level);
}
Stationlevel GetSystemStationLevel(StarSystem *systemeStellaire){
    return GetStationLevel(systemeStellaire->station);
}

OrdreStation GetSystemStationOrder(StarSystem *systemeStellaire){
    return GetStationOrder(systemeStellaire->station);
}
void EndSystemStationOrder(StarSystem *systemeStellaire){
    EndStationOrder(systemeStellaire->station);
}

int GetSystemStationOrderProgress(StarSystem *systemeStellaire){
    return GetStationOrderProgress(systemeStellaire->station);
}
void UnincrementSystemStationOrderProgress(StarSystem *systemeStellaire){
    UnincrementStationOrderProgress(systemeStellaire->station);
}

int GetSystemStationInfo1(StarSystem *systemeStellaire){
    return GetStationOrderInfo1(systemeStellaire->station);
}
int GetSystemStationInfo2(StarSystem *systemeStellaire){
    return GetStationOrderInfo2(systemeStellaire->station);
}

void SetSystemEmpire(StarSystem *systemeStellaire, int empire){
    systemeStellaire->empire = empire;
}
int GetSystemEmpire(StarSystem *systemeStellaire){
    return systemeStellaire->empire;
}

void SetSystemName(StarSystem *systemeStellaire, char* string){
    if(strlen(string) <= 9)
        strcpy(systemeStellaire->name, string);
    #ifdef DEBUG_VERSION
    else
        dbg_sprintf(dbgerr, "Name %s too long\n", string);
    #endif
}
char* GetSystemName(StarSystem *systemeStellaire){
    return systemeStellaire->name;
}



//hyperlanes
void SetHyperlaneDestination(StarSystem *systemeStellaire, int numeroHyperlane, int destination){
    systemeStellaire->hyperlane[numeroHyperlane].destination = destination;
}
int GetHyperlaneDestination(StarSystem *systemeStellaire, int numeroHyperlane){
    return systemeStellaire->hyperlane[numeroHyperlane].destination;
}

void SetHyperlaneXY(StarSystem *systemeStellaire, int numeroHyperlane, int x, int y){
    systemeStellaire->hyperlane[numeroHyperlane].x = x;
    systemeStellaire->hyperlane[numeroHyperlane].y = y;
}
int GetHyperlaneX(StarSystem *systemeStellaire, int numeroHyperlane){
    return systemeStellaire->hyperlane[numeroHyperlane].x;
}
int GetHyperlaneY(StarSystem *systemeStellaire, int numeroHyperlane){
    return systemeStellaire->hyperlane[numeroHyperlane].y;
}

// planetes
void CreateSystemPlanet(StarSystem *systemeStellaire, int position){
    systemeStellaire->planets[position] = AllocPlanet();
}
Planete *GetSystemPlanet(StarSystem *systemeStellaire, int position){
    return systemeStellaire->planets[position];
}

void SetSystemPlanetHabitability(StarSystem *systemeStellaire, int number, int habitability){
    SetPlanetHabitability(systemeStellaire->planets[number], habitability);
}
int GetSystemPlanetHabitability(StarSystem *systemeStellaire, int number){
    return GetPlanetHabitability(systemeStellaire->planets[number]);
}

void SetSystemPlanetOrbitRadius(StarSystem *systemeStellaire, int number, int orbitRadius){
    SetPlanetOrbitRadius(systemeStellaire->planets[number], orbitRadius);
}
int GetSystemPlanetOrbitRadius(StarSystem *systemeStellaire, int number){
    return GetPlanetOrbitRadius(systemeStellaire->planets[number]);
}

void SetSystemPlanetXY(StarSystem *systemeStellaire, int number, int x, int y){
    SetPlanetXY(systemeStellaire->planets[number], x, y);
}
int GetSystemPlanetX(StarSystem *systemeStellaire, int number){
    return GetPlanetX(systemeStellaire->planets[number]);
}
int GetSystemPlanetY(StarSystem *systemeStellaire, int number){
    return GetPlanetY(systemeStellaire->planets[number]);
}

void SetSystemPlanetType(StarSystem *systemeStellaire, int number, PlanetType type){
    SetPlanetType(systemeStellaire->planets[number], type);
}
PlanetType GetSystemPlanetType(StarSystem *systemeStellaire, int number){
    return GetPlanetType(systemeStellaire->planets[number]);
}

void SetSystemPlanetRadius(StarSystem *systemeStellaire, int number, int radius){
    SetPlanetRadius(systemeStellaire->planets[number], radius);
}
int GetSystemPlanetRadius(StarSystem *systemeStellaire, int number){
    return GetPlanetRadius(systemeStellaire->planets[number]);
}

void SetSystemPlanetName(StarSystem *systemeStellaire, int number, char* string){
    SetPlanetName(systemeStellaire->planets[number], string);
}
char* GetSystemPlanetName(StarSystem *systemeStellaire, int number){
    return GetPlanetName(systemeStellaire->planets[number]);
}

void CreateSystemPlanetCity(StarSystem *systemeStellaire, int number){
    CreatePlanetCity(systemeStellaire->planets[number]);
}


void SetSystemPlanetCityPopulation(StarSystem *systemeStellaire, int number, int population){
    SetPlanetCityPopulation(systemeStellaire->planets[number], population);
}
int GetSystemPlanetCityPopulation(StarSystem *systemeStellaire, int number){
    return GetPlanetCityPopulation(systemeStellaire->planets[number]);
}

void SetSystemPlanetCityDistrict(StarSystem *systemeStellaire, int number, int urban, int generator, int mining, int agriculture){
    SetPlanetCityDistrict(systemeStellaire->planets[number], urban, generator, mining, agriculture);
}

void AddSystemPlanetCityUrbanDistrict(StarSystem *systemeStellaire, int number, int urban){
    AddPlanetCityUrbanDistrict(systemeStellaire->planets[number], urban);
}
int GetSystemPlanetCityUrbanDistrict(StarSystem *systemeStellaire, int number){
    return GetPlanetCityUrbanDistrictNumber(systemeStellaire->planets[number]);
}

void AddSystemPlanetCityGeneratorDistrict(StarSystem *systemeStellaire, int number, int generator){
    AddPlanetCityGeneratorDistrict(systemeStellaire->planets[number], generator);
}
int GetSystemPlanetCityGeneratorDistrict(StarSystem *systemeStellaire, int number){
    return GetPlanetCityGeneratorDistrictNumber(systemeStellaire->planets[number]);
}

void AddSystemPlanetCityMiningDistrict(StarSystem *systemeStellaire, int number, int mining){
    AddPlanetCityMiningDistrict(systemeStellaire->planets[number], mining);
}
int GetSystemPlanetCityMiningDistrict(StarSystem *systemeStellaire, int number){
    return GetPlanetCityMiningDistrictNumber(systemeStellaire->planets[number]);
}

void AddSystemPlanetCityAgricultureDistrict(StarSystem *systemeStellaire, int number, int agriculture){
    AddPlanetCityAgricultureDistrict(systemeStellaire->planets[number], agriculture);
}
int GetSystemPlanetCityAgricultureDistrict(StarSystem *systemeStellaire, int number){
    return GetPlanetCityAgricultureDistrictNumber(systemeStellaire->planets[number]);
}

void SetSystemPlanetCityBuilding(StarSystem *systemeStellaire, int number, int buildingNumber, Batiment batiment, int level){
    SetPlanetCityBuilding(systemeStellaire->planets[number], buildingNumber, batiment, level);
}
Batiment GetSystemPlanetCityBuildingNumber(StarSystem *systemeStellaire, int number, int buildingNumber){
    return GetPlanetCityBuildingNumber(systemeStellaire->planets[number], buildingNumber);
}
int GetSystemPlanetCityBuildingLevel(StarSystem *systemeStellaire, int number, int buildingNumber){
    return GetPlanetCityBuildingNumber(systemeStellaire->planets[number], buildingNumber);
}

int CalculateSystemPlanetCityJob(StarSystem *systemeStellaire, int number){
    return CalculatePlanetCityJob(systemeStellaire->planets[number]);
}
int GetSystemPlanetCityJob(StarSystem *systemeStellaire, int number){
    return GetPlanetCityJob(systemeStellaire->planets[number]);
}

Planete *GetPlanet(StarSystem *systemeStellaire, int number){
    return systemeStellaire->planets[number];
}

void SetSystemPlanetNumber(StarSystem *systemeStellaire, int number){
    systemeStellaire->planetNumber = number;
}
int GetSystemPlanetNumber(StarSystem *systemeStellaire){
    return systemeStellaire->planetNumber;
}

void SetSystemPlanetHabitableNumber(StarSystem *systemeStellaire, int number){
    systemeStellaire->habitablePlanetNumber = number;
}
int GetSystemPlanetHabitableNumber(StarSystem *systemeStellaire){
    return systemeStellaire->habitablePlanetNumber;
}

void SetSystemPlanetInhabitedNumber(StarSystem *systemeStellaire, int number){
    systemeStellaire->inhabitedPlanetNumber = number;
}
int GetSystemPlanetInhabitedNumber(StarSystem *systemeStellaire){
    return systemeStellaire->inhabitedPlanetNumber;
}

Villes *GetSystemPlanetCity(StarSystem *systemeStellaire, int number){
    return GetPlanetCity(systemeStellaire->planets[number]);
}