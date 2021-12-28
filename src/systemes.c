/**
 * @file systemes.c
 * @author Cocheril Dimitri
 * @brief File containing all system functions
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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
#include "ai.h"

/* struct ============================================================== */

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

    Planet     *planets[5];
    Station     *station;
};

/* entry points ======================================================== */

StarSystem* starSystem_Create(){
    return calloc_count(1, sizeof(StarSystem));
}

void starSystem_SetXY(StarSystem *starSystem, const int x, const int y){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying set systemXY : no starSystem\n");
        #endif
        return;
    }
    starSystem->x = x;
    starSystem->y = y;
}
int starSystem_XGet(const StarSystem *starSystem){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get systemX : no starSystem\n");
        #endif
        return 0;
    }
    return starSystem->x;
}
int starSystem_YGet(const StarSystem *starSystem){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get systemY : no starSystem\n");
        #endif
        return 0;
    }
    return starSystem->y;
}

void starSystem_EmpireSet(StarSystem *starSystem, const int empire){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get empire system : no starSystem\n");
        #endif
        return;
    }
    starSystem->empire = empire;
}
int starSystem_EmpireGet(const StarSystem *starSystem){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get empire system : no starSystem\n");
        #endif
        return NO_EMPIRE;
    }
    return starSystem->empire;
}

void starSystem_NameSet(StarSystem *starSystem, const char* string){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get empire system : no starSystem\n");
        #endif
        return;
    }
    if(strlen(string) <= 9)
        strcpy(starSystem->name, string);
    #ifdef DEBUG_VERSION
    else
        dbg_sprintf(dbgerr, "Name %s too long\n", string);
    #endif
}
char* starSystem_NameGet(StarSystem *starSystem){
    if(!starSystem){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get empire system : no starSystem\n");
        #endif
        return NULL;
    }
    return starSystem->name;
}

void starSystem_IntelLevelSet(StarSystem *starSystem, IntelLevel intelLevel){
    starSystem->intelLevel = intelLevel;
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

//station
void starSystem_StationCreate(StarSystem *starSystem){
    starSystem->station = station_Create();
}
Station *starSystem_StationGet(StarSystem *starSystem){
    return starSystem->station;
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
Planet *starSystem_PlanetAlloc(StarSystem *starSystem){
    starSystem->planets[starSystem->planetNumber] = planet_Alloc();
    starSystem->planetNumber++;
    return starSystem->planets[starSystem->planetNumber - 1];
}
Planet *starSystem_PlanetGet(StarSystem *starSystem, int position){
    return starSystem->planets[position];
}

void starSystem_PlanetHabitabilitySet(StarSystem *starSystem, int number, int habitability){
    planet_HabitabilitySet(starSystem->planets[number], habitability);
    starSystem->habitablePlanetNumber++;
}
int starSystem_PlanetHabitabilityGet(StarSystem *starSystem, int number){
    return planet_HabitabilityGet(starSystem->planets[number]);
}

void starSystem_PlanetRadiusOrbitSet(StarSystem *starSystem, int number, int orbitRadius){
    planet_OrbitRadiusSet(starSystem->planets[number], orbitRadius);
}
int starSystem_PlanetRadiusOrbitGet(StarSystem *starSystem, int number){
    return planet_OrbitRadiusGet(starSystem->planets[number]);
}

void starSystem_PlanetXYSet(StarSystem *starSystem, int number, int x, int y){
    planet_PositionSet(starSystem->planets[number], x, y);
}
int starSystem_PlanetXGet(StarSystem *starSystem, int number){
    return planet_XGet(starSystem->planets[number]);
}
int starSystem_PlanetYGet(StarSystem *starSystem, int number){
    return planet_YGet(starSystem->planets[number]);
}

void starSystem_PlanetTypeSet(StarSystem *starSystem, int number, PlanetType type){
    planet_TypeSet(starSystem->planets[number], type);
}
PlanetType starSystem_PlanetTypeGet(StarSystem *starSystem, int number){
    return planet_TypeGet(starSystem->planets[number]);
}

void starSystem_PlanetRadiusSet(StarSystem *starSystem, int number, int radius){
    planet_SizeSet(starSystem->planets[number], radius);
}
int starSystem_PlanetRadiusGet(StarSystem *starSystem, int number){
    return planet_SizeGet(starSystem->planets[number]);
}
char* starSystem_PlanetNameGet(StarSystem *starSystem, int number){
    return planet_NameGet(starSystem->planets[number]);
}

void starSystem_PlanetCityCreate(StarSystem *starSystem, int number){
    planet_CityCreate(starSystem->planets[number]);
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

void SetSystemPlanetCityBuilding(StarSystem *starSystem, int number, int buildingNumber, Building batiment, int level){
    SetPlanetCityBuilding(starSystem->planets[number], buildingNumber, batiment, level);
}
Building GetSystemPlanetCityBuildingNumber(StarSystem *starSystem, int number, int buildingNumber){
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

void SetSystemPlanetNumber(StarSystem *starSystem, int number){
    starSystem->planetNumber = number;
}
int starSystem_NumberOfPlanetGet(StarSystem *starSystem){
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
    return planet_CityGet(starSystem->planets[number]);
}