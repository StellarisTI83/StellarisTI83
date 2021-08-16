#ifndef H_SYSTEMES
#define H_SYSTEMES

#include "planetes.h"
#include "stations.h"

/* types =============================================================== */
typedef enum {INCONNU, FAIBLE, MOYEN, ELEVEE, TOTAL} IntelLevel;

typedef enum {ETOILE_TYPE_B = 1, ETOILE_TYPE_A, ETOILE_TYPE_F, ETOILE_TYPE_G, ETOILE_TYPE_K, ETOILE_TYPE_M, ETOILE_TYPE_TROU_NOIR, ETOILE_TYPE_PULSAR, ETOILE_TYPE_ETOILE_A_NEUTRONS} StarType;

/* structures ========================================================== */
typedef struct HyperlaneStruct Hyperlane;

typedef struct StarSystemStruct StarSystem;

/* internal public functions =========================================== */
StarSystem* starSystem_Create();

void starSystem_SetXY(StarSystem *systemeStellaire, int x, int y);
int starSystem_GetX(StarSystem *systemeStellaire);
int starSystem_GetY(StarSystem *systemeStellaire);

void SetSystemEmpire(StarSystem *systemeStellaire, int number);
int GetSystemEmpire(StarSystem *systemeStellaire);

void SetSystemName(StarSystem *systemeStellaire, char* string);
char* GetSystemName(StarSystem *systemeStellaire);

void SetSystemIntelLevel(StarSystem *systemeStellaire, IntelLevel niveau);
IntelLevel GetSystemIntelLevel(StarSystem *systemeStellaire);

void SetSystemStarType(StarSystem *systemeStellaire, StarType type);
StarType GetSystemStarType(StarSystem *systemeStellaire);

Station *GetSystemStation(StarSystem *systemeStellaire);

void CreateSystemStation(StarSystem *systemeStellaire);
void SetSystemStationModule(StarSystem *systemeStellaire, int moduleNumber, Module module);
Module GetSystemStationModule(StarSystem *systemeStellaire, int moduleNumber);

void SetSystemStationLevel(StarSystem *systemeStellaire, Stationlevel level);
Stationlevel GetSystemStationLevel(StarSystem *systemeStellaire);

OrdreStation GetSystemStationOrder(StarSystem *systemeStellaire);
void EndSystemStationOrder(StarSystem *systemeStellaire);

int GetSystemStationOrderProgress(StarSystem *systemeStellaire);
void UnincrementSystemStationOrderProgress(StarSystem *systemeStellaire);

int GetSystemStationInfo1(StarSystem *systemeStellaire);
int GetSystemStationInfo2(StarSystem *systemeStellaire);


void SetHyperlaneDestination(StarSystem *systemeStellaire, int numeroHyperlane, int destination);
int GetHyperlaneDestination(StarSystem *systemeStellaire, int numeroHyperlane);

void SetHyperlaneXY(StarSystem *systemeStellaire, int numeroHyperlane, int x, int y);
int GetHyperlaneX(StarSystem *systemeStellaire, int numeroHyperlane);
int GetHyperlaneY(StarSystem *systemeStellaire, int numeroHyperlane);

void CreateSystemPlanet(StarSystem *systemeStellaire, int position);
Planete *GetSystemPlanet(StarSystem *systemeStellaire, int position);

void SetSystemPlanetHabitability(StarSystem *systemeStellaire, int number, int habitability);
int GetSystemPlanetHabitability(StarSystem *systemeStellaire, int number);

void SetSystemPlanetOrbitRadius(StarSystem *systemeStellaire, int number, int);
int GetSystemPlanetOrbitRadius(StarSystem *systemeStellaire, int number);

void SetSystemPlanetXY(StarSystem *systemeStellaire, int number, int x, int y);
int GetSystemPlanetX(StarSystem *systemeStellaire, int number);
int GetSystemPlanetY(StarSystem *systemeStellaire, int number);

void SetSystemPlanetType(StarSystem *systemeStellaire, int number, PlanetType type);
PlanetType GetSystemPlanetType(StarSystem *systemeStellaire, int number);

void SetSystemPlanetRadius(StarSystem *systemeStellaire, int number, int radius);
int GetSystemPlanetRadius(StarSystem *systemeStellaire, int number);

void SetSystemPlanetName(StarSystem *systemeStellaire, int number, char* string);
char* GetSystemPlanetName(StarSystem *systemeStellaire, int number);

void CreateSystemPlanetCity(StarSystem *systemeStellaire, int number);

void SetSystemPlanetCityPopulation(StarSystem *systemeStellaire, int number, int population);
int GetSystemPlanetCityPopulation(StarSystem *systemeStellaire, int number);

void SetSystemPlanetCityDistrict(StarSystem *systemeStellaire, int number, int urban, int generator, int mining, int agriculture);

void AddSystemPlanetCityUrbanDistrict(StarSystem *systemeStellaire, int number, int urban);
int GetSystemPlanetCityUrbanDistrict(StarSystem *systemeStellaire, int number);

void AddSystemPlanetCityGeneratorDistrict(StarSystem *systemeStellaire, int number, int generator);
int GetSystemPlanetCityGeneratorDistrict(StarSystem *systemeStellaire, int number);

void AddSystemPlanetCityMiningDistrict(StarSystem *systemeStellaire, int number, int mining);
int GetSystemPlanetCityMiningDistrict(StarSystem *systemeStellaire, int number);

void AddSystemPlanetCityAgricultureDistrict(StarSystem *systemeStellaire, int number, int agriculture);
int GetSystemPlanetCityAgricultureDistrict(StarSystem *systemeStellaire, int number);

void SetSystemPlanetCityBuilding(StarSystem *systemeStellaire, int number, int buildingNumber, Batiment batiment, int level);
Batiment GetSystemPlanetCityBuildingNumber(StarSystem *systemeStellaire, int number, int buildingNumber);
int GetSystemPlanetCityBuildingLevel(StarSystem *systemeStellaire, int number, int buildingNumber);

int CalculateSystemPlanetCityJob(StarSystem *systemeStellaire, int number);
int GetSystemPlanetCityJob(StarSystem *systemeStellaire, int number);

Planete *GetPlanet(StarSystem *systemeStellaire, int number);

void SetSystemPlanetNumber(StarSystem *systemeStellaire, int number);
int GetSystemPlanetNumber(StarSystem *systemeStellaire);

void SetSystemPlanetHabitableNumber(StarSystem *systemeStellaire, int number);
int GetSystemPlanetHabitableNumber(StarSystem *systemeStellaire);

void SetSystemPlanetInhabitedNumber(StarSystem *systemeStellaire, int number);
int GetSystemPlanetInhabitedNumber(StarSystem *systemeStellaire);

Villes *GetSystemPlanetCity(StarSystem *systemeStellaire, int number);

#endif