
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
    int         planetNumber;
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

void starSystem_SetXY(StarSystem *starSystem, int x, int y){
    starSystem->x = x;
    starSystem->y = y;
}
int starSystem_GetX(StarSystem *starSystem){
    return starSystem->x;
}
int starSystem_GetY(StarSystem *starSystem){
    return starSystem->y;
}

//station
void starSystem_StationCreate(StarSystem *starSystem){
    starSystem->station = AllocStation();
}

void starSystem_StationModuleSet(StarSystem *starSystem, int moduleNumber, Module module){
    SetStationModule(starSystem->station, moduleNumber, module);
}
Module starSystem_StationModuleGet(StarSystem *starSystem, int moduleNumber){
    return GetStationModule(starSystem->station, moduleNumber);
}

void starSystem_IntelLevelSet(StarSystem *starSystem, IntelLevel niveau){
    starSystem->intelLevel = niveau;
}
IntelLevel starSystem_IntelLevelGet(StarSystem *starSystem){
    return starSystem->intelLevel;
}

void starSystem_StarTypeSet(StarSystem *starSystem, StarType type){
    starSystem->starType = type;
}
StarType starSystem_StarTypeGet(StarSystem *starSystem){
    return starSystem->starType;
}

//stations
Station *starSystem_StationGet(StarSystem *starSystem){
    return starSystem->station;
}
void starSystem_StationLevelSet(StarSystem *starSystem, Stationlevel level){
    SetStationLevel(starSystem->station, level);
}
Stationlevel starSystem_StationLevelGet(StarSystem *starSystem){
    return GetStationLevel(starSystem->station);
}

OrdreStation starSystem_StationOrderGet(StarSystem *starSystem){
    return GetStationOrder(starSystem->station);
}
void starSystem_StationOrderEnd(StarSystem *starSystem){
    EndStationOrder(starSystem->station);
}

int starSystem_StationOrderProgressGet(StarSystem *starSystem){
    return GetStationOrderProgress(starSystem->station);
}
void starSystem_StationOrderProgressIncrement(StarSystem *starSystem){
    UnincrementStationOrderProgress(starSystem->station);
}

int starSystem_StationInfo1Get(StarSystem *starSystem){
    return GetStationOrderInfo1(starSystem->station);
}
int starSystem_StationInfo2Get(StarSystem *starSystem){
    return GetStationOrderInfo2(starSystem->station);
}

void starSystem_EmpireSet(StarSystem *starSystem, int empire){
    starSystem->empire = empire;
}
int starSystem_EmpireGet(StarSystem *starSystem){
    return starSystem->empire;
}

void starSystem_NameSet(StarSystem *starSystem, char* string){
    if(strlen(string) <= 9)
        strcpy(starSystem->name, string);
    #ifdef DEBUG_VERSION
    else
        dbg_sprintf(dbgerr, "Name %s too long\n", string);
    #endif
}
char* starSystem_NameGet(StarSystem *starSystem){
    return starSystem->name;
}



//hyperlanes
void hyperlane_DestinationSet(StarSystem *starSystem, int numeroHyperlane, int destination){
    starSystem->hyperlane[numeroHyperlane].destination = destination;
}
int hyperlane_DestinationGet(StarSystem *starSystem, int numeroHyperlane){
    return starSystem->hyperlane[numeroHyperlane].destination;
}

void hyperlane_XYSet(StarSystem *starSystem, int numeroHyperlane, int x, int y){
    starSystem->hyperlane[numeroHyperlane].x = x;
    starSystem->hyperlane[numeroHyperlane].y = y;
}
int hyperlane_XGet(StarSystem *starSystem, int numeroHyperlane){
    return starSystem->hyperlane[numeroHyperlane].x;
}
int hyperlane_YGet(StarSystem *starSystem, int numeroHyperlane){
    return starSystem->hyperlane[numeroHyperlane].y;
}

// planets
void starSystem_PlanetAlloc(StarSystem *starSystem){
    starSystem->planets[starSystem->planetNumber] = AllocPlanet();
    starSystem->planetNumber++;
}
Planete *starSystem_PlanetGet(StarSystem *starSystem, int position){
    return starSystem->planets[position];
}

void starSystem_PlanetHabitabilitySet(StarSystem *starSystem, int number, int habitability){
    SetPlanetHabitability(starSystem->planets[number], habitability);
    starSystem->habitablePlanetNumber++;
}
int starSystem_PlanetHabitabilityGet(StarSystem *starSystem, int number){
    return GetPlanetHabitability(starSystem->planets[number]);
}

void starSystem_PlanetRadiusOrbitSet(StarSystem *starSystem, int number, int orbitRadius){
    SetPlanetOrbitRadius(starSystem->planets[number], orbitRadius);
}
int starSystem_PlanetRadiusOrbitGet(StarSystem *starSystem, int number){
    return GetPlanetOrbitRadius(starSystem->planets[number]);
}

void starSystem_PlanetXYSet(StarSystem *starSystem, int number, int x, int y){
    SetPlanetXY(starSystem->planets[number], x, y);
}
int starSystem_PlanetXGet(StarSystem *starSystem, int number){
    return GetPlanetX(starSystem->planets[number]);
}
int starSystem_PlanetYGet(StarSystem *starSystem, int number){
    return GetPlanetY(starSystem->planets[number]);
}

void starSystem_PlanetTypeSet(StarSystem *starSystem, int number, PlanetType type){
    SetPlanetType(starSystem->planets[number], type);
}
PlanetType starSystem_PlanetTypeGet(StarSystem *starSystem, int number){
    return GetPlanetType(starSystem->planets[number]);
}

void starSystem_PlanetRadiusSet(StarSystem *starSystem, int number, int radius){
    SetPlanetRadius(starSystem->planets[number], radius);
}
int starSystem_PlanetRadiusGet(StarSystem *starSystem, int number){
    return GetPlanetRadius(starSystem->planets[number]);
}

void starSystem_PlanetNameSet(StarSystem *starSystem, int number, char* string){
    SetPlanetName(starSystem->planets[number], string);
}
char* starSystem_PlanetNameGet(StarSystem *starSystem, int number){
    return GetPlanetName(starSystem->planets[number]);
}

void starSystem_PlanetCityCreate(StarSystem *starSystem, int number){
    CreatePlanetCity(starSystem->planets[number]);
}


void starSystem_PlanetCityPopulationSet(StarSystem *starSystem, int number, int population){
    SetPlanetCityPopulation(starSystem->planets[number], population);
}
int starSystem_PlanetCityPopulationGet(StarSystem *starSystem, int number){
    return GetPlanetCityPopulation(starSystem->planets[number]);
}

void starSystem_PlanetCityDistrictSet(StarSystem *starSystem, int number, int urban, int generator, int mining, int agriculture){
    SetPlanetCityDistrict(starSystem->planets[number], urban, generator, mining, agriculture);
}

void AddSystemPlanetCityUrbanDistrict(StarSystem *starSystem, int number, int urban){
    AddPlanetCityUrbanDistrict(starSystem->planets[number], urban);
}
int GetSystemPlanetCityUrbanDistrict(StarSystem *starSystem, int number){
    return GetPlanetCityUrbanDistrictNumber(starSystem->planets[number]);
}

void AddSystemPlanetCityGeneratorDistrict(StarSystem *starSystem, int number, int generator){
    AddPlanetCityGeneratorDistrict(starSystem->planets[number], generator);
}
int GetSystemPlanetCityGeneratorDistrict(StarSystem *starSystem, int number){
    return GetPlanetCityGeneratorDistrictNumber(starSystem->planets[number]);
}

void AddSystemPlanetCityMiningDistrict(StarSystem *starSystem, int number, int mining){
    AddPlanetCityMiningDistrict(starSystem->planets[number], mining);
}
int GetSystemPlanetCityMiningDistrict(StarSystem *starSystem, int number){
    return GetPlanetCityMiningDistrictNumber(starSystem->planets[number]);
}

void AddSystemPlanetCityAgricultureDistrict(StarSystem *starSystem, int number, int agriculture){
    AddPlanetCityAgricultureDistrict(starSystem->planets[number], agriculture);
}
int GetSystemPlanetCityAgricultureDistrict(StarSystem *starSystem, int number){
    return GetPlanetCityAgricultureDistrictNumber(starSystem->planets[number]);
}

void SetSystemPlanetCityBuilding(StarSystem *starSystem, int number, int buildingNumber, Batiment batiment, int level){
    SetPlanetCityBuilding(starSystem->planets[number], buildingNumber, batiment, level);
}
Batiment GetSystemPlanetCityBuildingNumber(StarSystem *starSystem, int number, int buildingNumber){
    return GetPlanetCityBuildingNumber(starSystem->planets[number], buildingNumber);
}
int GetSystemPlanetCityBuildingLevel(StarSystem *starSystem, int number, int buildingNumber){
    return GetPlanetCityBuildingNumber(starSystem->planets[number], buildingNumber);
}

int CalculateSystemPlanetCityJob(StarSystem *starSystem, int number){
    return CalculatePlanetCityJob(starSystem->planets[number]);
}
int GetSystemPlanetCityJob(StarSystem *starSystem, int number){
    return GetPlanetCityJob(starSystem->planets[number]);
}

Planete *GetPlanet(StarSystem *starSystem, int number){
    return starSystem->planets[number];
}

void SetSystemPlanetNumber(StarSystem *starSystem, int number){
    starSystem->planetNumber = number;
}
int GetSystemPlanetNumber(StarSystem *starSystem){
    return starSystem->planetNumber;
}

void SetSystemPlanetHabitableNumber(StarSystem *starSystem, int number){
    starSystem->habitablePlanetNumber = number;
}
int GetSystemPlanetHabitableNumber(StarSystem *starSystem){
    return starSystem->habitablePlanetNumber;
}

void SetSystemPlanetInhabitedNumber(StarSystem *starSystem, int number){
    starSystem->inhabitedPlanetNumber = number;
}
int GetSystemPlanetInhabitedNumber(StarSystem *starSystem){
    return starSystem->inhabitedPlanetNumber;
}

City *GetSystemPlanetCity(StarSystem *starSystem, int number){
    return GetPlanetCity(starSystem->planets[number]);
}