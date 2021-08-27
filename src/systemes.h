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
 *        use free() to free the star system
 * 
 * @return StarSystem* 
 */
StarSystem* starSystem_Create();


/**
 * @brief Set the coordinates of a star system
 * 
 * @param galaxy 
 * @param x 
 * @param y 
 */
void starSystem_SetXY(StarSystem *starSystem, const int x, const int y);

/**
 * @brief To get the X position of a star system
 * 
 * @param galaxy 
 * @return int 
 */
int starSystem_XGet(const StarSystem *starSystem);

/**
 * @brief To get the Y position of a star system
 * 
 * @param galaxy 
 * @return int 
 */
int starSystem_YGet(const StarSystem *starSystem);


/**
 * @brief Set the empire owning the star system
 * 
 * @param galaxy 
 * @param number 
 */
void starSystem_EmpireSet(StarSystem *starSystem, const int number);

/**
 * @brief Get the empire owning the star system
 * 
 * @param galaxy 
 * @return int 
 */
int starSystem_EmpireGet(const StarSystem *starSystem);

/**
 * @brief Set the name of the star system
 * 
 * @param galaxy 
 * @param string 
 */
void starSystem_NameSet(StarSystem *starSystem, const char* string);

/**
 * @brief Get the name of a star system
 * 
 * @param galaxy 
 * @return char* 
 */
char* starSystem_NameGet(StarSystem *starSystem);

/**
 * @brief Set the intel level of a star system with the enum IntelLevel
 * 
 * @param starSystem 
 * @param niveau 
 */
void starSystem_IntelLevelSet(StarSystem *starSystem, IntelLevel intelLevel);

/**
 * @brief 
 * 
 * @param starSystem 
 * @return IntelLevel 
 */
IntelLevel starSystem_IntelLevelGet(StarSystem *starSystem);

void starSystem_StarTypeSet(StarSystem *starSystem, StarType type);
StarType starSystem_StarTypeGet(StarSystem *starSystem);

void starSystem_StationCreate(StarSystem *starSystem);
Station *starSystem_StationGet(StarSystem *starSystem);

void hyperlane_DestinationSet(StarSystem *starSystem, int numeroHyperlane, int destination);
int hyperlane_DestinationGet(StarSystem *starSystem, int numeroHyperlane);

void hyperlane_XYSet(StarSystem *starSystem, int numeroHyperlane, int x, int y);
int hyperlane_XGet(StarSystem *starSystem, int numeroHyperlane);
int hyperlane_YGet(StarSystem *starSystem, int numeroHyperlane);

Planet *starSystem_PlanetAlloc(StarSystem *starSystem);
Planet *starSystem_PlanetGet(StarSystem *starSystem, int position);

void starSystem_PlanetHabitabilitySet(StarSystem *starSystem, int number, int habitability);
int starSystem_PlanetHabitabilityGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetRadiusOrbitSet(StarSystem *starSystem, int number, int) __attribute__((deprecated("use starSystem_PlanetGet")));
int starSystem_PlanetRadiusOrbitGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetXYSet(StarSystem *starSystem, int number, int x, int y) __attribute__((deprecated("use starSystem_PlanetGet")));
int starSystem_PlanetXGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));
int starSystem_PlanetYGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetTypeSet(StarSystem *starSystem, int number, PlanetType type) __attribute__((deprecated("use starSystem_PlanetGet")));
PlanetType starSystem_PlanetTypeGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetRadiusSet(StarSystem *starSystem, int number, int radius) __attribute__((deprecated("use starSystem_PlanetGet")));
int starSystem_PlanetRadiusGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

char* starSystem_PlanetNameGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetCityCreate(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetCityPopulationSet(StarSystem *starSystem, int number, int population) __attribute__((deprecated("use starSystem_PlanetGet")));
int starSystem_PlanetCityPopulationGet(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void starSystem_PlanetCityDistrictSet(StarSystem *starSystem, int number, int urban, int generator, int mining, int agriculture) __attribute__((deprecated("use starSystem_PlanetGet")));

void AddSystemPlanetCityUrbanDistrict(StarSystem *starSystem, int number, int urban) __attribute__((deprecated("use starSystem_PlanetGet")));
int GetSystemPlanetCityUrbanDistrict(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void AddSystemPlanetCityGeneratorDistrict(StarSystem *starSystem, int number, int generator) __attribute__((deprecated("use starSystem_PlanetGet")));
int GetSystemPlanetCityGeneratorDistrict(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void AddSystemPlanetCityMiningDistrict(StarSystem *starSystem, int number, int mining) __attribute__((deprecated("use starSystem_PlanetGet")));
int GetSystemPlanetCityMiningDistrict(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void AddSystemPlanetCityAgricultureDistrict(StarSystem *starSystem, int number, int agriculture) __attribute__((deprecated("use starSystem_PlanetGet")));
int GetSystemPlanetCityAgricultureDistrict(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void SetSystemPlanetCityBuilding(StarSystem *starSystem, int number, int buildingNumber, Building batiment, int level) __attribute__((deprecated("use starSystem_PlanetGet")));
Building GetSystemPlanetCityBuildingNumber(StarSystem *starSystem, int number, int buildingNumber) __attribute__((deprecated("use starSystem_PlanetGet")));
int GetSystemPlanetCityBuildingLevel(StarSystem *starSystem, int number, int buildingNumber) __attribute__((deprecated("use starSystem_PlanetGet")));

int CalculateSystemPlanetCityJob(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));
int GetSystemPlanetCityJob(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

void SetSystemPlanetNumber(StarSystem *starSystem, int number);
int starSystem_NumberOfPlanetGet(StarSystem *starSystem);

void SetSystemPlanetHabitableNumber(StarSystem *starSystem, int number);
int GetSystemPlanetHabitableNumber(StarSystem *starSystem);

void SetSystemPlanetInhabitedNumber(StarSystem *starSystem, int number);
int GetSystemPlanetInhabitedNumber(StarSystem *starSystem);

City *GetSystemPlanetCity(StarSystem *starSystem, int number) __attribute__((deprecated("use starSystem_PlanetGet")));

#endif