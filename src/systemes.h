#ifndef H_SYSTEMES
#define H_SYSTEMES

#include <stdbool.h>
#include "planet.h"
#include "stations.h"

#define NO_SYSTEM -1

/* types =============================================================== */
/**
 * @brief The different intel levels
 * 
 */
typedef enum {  INTEL_UNKNOWN, 
                INTEL_LOW, 
                INTEL_MEDIUM, 
                INTEL_HIGH, 
                INTEL_FULL} IntelLevel;


/**
 * @brief The different star types
 * 
 */
typedef enum {  STAR_NONE,
                STAR_TYPE_B = 1, 
                STAR_TYPE_A, 
                STAR_TYPE_F, 
                STAR_TYPE_G, 
                STAR_TYPE_K, 
                STAR_TYPE_M, 
                STAR_TYPE_BLACKHOLE, 
                STAR_TYPE_PULSAR, 
                STAR_TYPE_NEUTRON} StarType;

/* structures ========================================================== */

typedef struct HyperlaneStruct Hyperlane;

typedef struct StarSystemStruct StarSystem;

/* internal public functions =========================================== */
/**
 * @brief To alloc a star system
 * 
 * @return StarSystem* 
 */
StarSystem* starSystem_Create();


/**
 * @brief Set the coordinates of a star system
 * 
 * @param starSystem 
 * @param x 
 * @param y 
 */
void starSystem_SetXY(StarSystem *starSystem, int x, int y);

/**
 * @brief To get the X position of a star system
 * 
 * @param starSystem 
 * @return int 
 */
int starSystem_GetX(StarSystem *starSystem);

/**
 * @brief To get the Y position of a star system
 * 
 * @param starSystem 
 * @return int 
 */
int starSystem_GetY(StarSystem *starSystem);


/**
 * @brief Set the empire owning the star system
 * 
 * @param starSystem 
 * @param number 
 */
void starSystem_EmpireSet(StarSystem *starSystem, int number);

/**
 * @brief Get the empire owning the star system
 * 
 * @param starSystem 
 * @return int 
 */
int starSystem_EmpireGet(StarSystem *starSystem);

/**
 * @brief Set the name of the star system
 * 
 * @param starSystem 
 * @param string 
 */
void starSystem_NameSet(StarSystem *starSystem, char* string);

/**
 * @brief Get the name of a star system
 * 
 * @param starSystem 
 * @return char* 
 */
char* starSystem_NameGet(StarSystem *starSystem);

void starSystem_IntelLevelSet(StarSystem *starSystem, IntelLevel niveau);
IntelLevel starSystem_IntelLevelGet(StarSystem *starSystem);

void starSystem_StarTypeSet(StarSystem *starSystem, StarType type);
StarType starSystem_StarTypeGet(StarSystem *starSystem);

Station *starSystem_StationGet(StarSystem *starSystem);

void starSystem_StationCreate(StarSystem *starSystem);
void starSystem_StationModuleSet(StarSystem *starSystem, int moduleNumber, Module module);
Module starSystem_StationModuleGet(StarSystem *starSystem, int moduleNumber);

void starSystem_StationLevelSet(StarSystem *starSystem, Stationlevel level);
Stationlevel starSystem_StationLevelGet(StarSystem *starSystem);

OrdreStation starSystem_StationOrderGet(StarSystem *starSystem);
void starSystem_StationOrderEnd(StarSystem *starSystem);

int starSystem_StationOrderProgressGet(StarSystem *starSystem);
void starSystem_StationOrderProgressIncrement(StarSystem *starSystem);

int starSystem_StationInfo1Get(StarSystem *starSystem);
int starSystem_StationInfo2Get(StarSystem *starSystem);


void hyperlane_DestinationSet(StarSystem *starSystem, int numeroHyperlane, int destination);
int hyperlane_DestinationGet(StarSystem *starSystem, int numeroHyperlane);

void hyperlane_XYSet(StarSystem *starSystem, int numeroHyperlane, int x, int y);
int hyperlane_XGet(StarSystem *starSystem, int numeroHyperlane);
int hyperlane_YGet(StarSystem *starSystem, int numeroHyperlane);

Planet *starSystem_PlanetAlloc(StarSystem *starSystem);
Planet *starSystem_PlanetGet(StarSystem *starSystem, int position);

void starSystem_PlanetHabitabilitySet(StarSystem *starSystem, int number, int habitability);
int starSystem_PlanetHabitabilityGet(StarSystem *starSystem, int number);

void starSystem_PlanetRadiusOrbitSet(StarSystem *starSystem, int number, int);
int starSystem_PlanetRadiusOrbitGet(StarSystem *starSystem, int number);

void starSystem_PlanetXYSet(StarSystem *starSystem, int number, int x, int y);
int starSystem_PlanetXGet(StarSystem *starSystem, int number);
int starSystem_PlanetYGet(StarSystem *starSystem, int number);

void starSystem_PlanetTypeSet(StarSystem *starSystem, int number, PlanetType type);
PlanetType starSystem_PlanetTypeGet(StarSystem *starSystem, int number);

void starSystem_PlanetRadiusSet(StarSystem *starSystem, int number, int radius);
int starSystem_PlanetRadiusGet(StarSystem *starSystem, int number);

char* starSystem_PlanetNameGet(StarSystem *starSystem, int number);

void starSystem_PlanetCityCreate(StarSystem *starSystem, int number);

void starSystem_PlanetCityPopulationSet(StarSystem *starSystem, int number, int population);
int starSystem_PlanetCityPopulationGet(StarSystem *starSystem, int number);

void starSystem_PlanetCityDistrictSet(StarSystem *starSystem, int number, int urban, int generator, int mining, int agriculture);

void AddSystemPlanetCityUrbanDistrict(StarSystem *starSystem, int number, int urban);
int GetSystemPlanetCityUrbanDistrict(StarSystem *starSystem, int number);

void AddSystemPlanetCityGeneratorDistrict(StarSystem *starSystem, int number, int generator);
int GetSystemPlanetCityGeneratorDistrict(StarSystem *starSystem, int number);

void AddSystemPlanetCityMiningDistrict(StarSystem *starSystem, int number, int mining);
int GetSystemPlanetCityMiningDistrict(StarSystem *starSystem, int number);

void AddSystemPlanetCityAgricultureDistrict(StarSystem *starSystem, int number, int agriculture);
int GetSystemPlanetCityAgricultureDistrict(StarSystem *starSystem, int number);

void SetSystemPlanetCityBuilding(StarSystem *starSystem, int number, int buildingNumber, Building batiment, int level);
Building GetSystemPlanetCityBuildingNumber(StarSystem *starSystem, int number, int buildingNumber);
int GetSystemPlanetCityBuildingLevel(StarSystem *starSystem, int number, int buildingNumber);

int CalculateSystemPlanetCityJob(StarSystem *starSystem, int number);
int GetSystemPlanetCityJob(StarSystem *starSystem, int number);

Planet *GetPlanet(StarSystem *starSystem, int number);

void SetSystemPlanetNumber(StarSystem *starSystem, int number);
int GetSystemPlanetNumber(StarSystem *starSystem);

void SetSystemPlanetHabitableNumber(StarSystem *starSystem, int number);
int GetSystemPlanetHabitableNumber(StarSystem *starSystem);

void SetSystemPlanetInhabitedNumber(StarSystem *starSystem, int number);
int GetSystemPlanetInhabitedNumber(StarSystem *starSystem);

City *GetSystemPlanetCity(StarSystem *starSystem, int number);

#endif