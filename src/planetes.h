#ifndef H_PLANETES
#define H_PLANETES

#include "ordres.h"

/* types =============================================================== */

typedef enum {AUCUN = 1, CAPITALE, FONDERIE, LABORATOIRE, USINE_CIVILE, THEATRE} Batiment;

typedef enum {AUCUN, CONSTRUIRE_DISTRICT_URBAIN, CONSTRUIRE_DISTRICT_GENERATEUR, CONSTRUIRE_DISTRICT_MINIER,
    CONSTRUIRE_DISTRICT_AGRICOLE, CONSTRUIRE_BATIMENT} OrdreConstruction;

/* 1 = arid, 2 = Desert, 3 = Savanna, 4 = Alpine, 5 = Arctic, 6 = Tundra, 7 = continental, 8 = ocean, 9 = tropical, 10 = monde aride,
 * 11 = monde aride froid, 12 = brisé, 13 = monde gelé, 14 = géante gazeuse, 15 = monde en fusion, 16 = toxic world
 */
typedef enum {HABITABLE_ARID = 1, HABITABLE_DESERT, HABITABLE_SAVANNA, HABITABLE_ALPINE, HABITABLE_ARCTIC, HABITABLE_TUNDRA,
    HABITABLE_CONTINENTAL, HABITABLE_OCEAN, HABITABLE_TROPICAL, ARID, COLD_ARID, BROKE, FROZEN, GAS_GIANT, MOLTEN, TOXIC} PlanetType;

/* structures ========================================================== */

typedef struct VillesStruct Villes;

typedef struct PlaneteStruct Planete;

/* entry points ======================================================== */

Planete *AllocPlanet();

void SetPlanetHabitability(Planete *planete, bool habitability);
bool GetPlanetHabitability(Planete *planete);

void SetPlanetOrbitRadius(Planete *planete, int orbitRadius);
int GetPlanetOrbitRadius(Planete *planete);

void SetPlanetXY(Planete *planete, int x, int y);
int GetPlanetX(Planete *planete);
int GetPlanetY(Planete *planete);

void SetPlanetType(Planete *planete, PlanetType type);
PlanetType GetPlanetType(Planete *planete);

void SetPlanetRadius(Planete *planete, int radius);
int GetPlanetRadius(Planete *planete);

void SetPlanetName(Planete *planete, char* string);
char* GetPlanetName(Planete *planete);

void CreatePlanetCity(Planete *planete);
bool GetPlanetCityStatus(Planete *planete);
void SetPlanetCityPopulation(Planete *planete, int population);
int GetPlanetCityPopulation(Planete *planete);

void SetPlanetCityDistrict(Planete *planete, int urban, int generator, int mining, int agriculture);

void AddPlanetCityUrbanDistrict(Planete *planete, int number);
int GetPlanetCityUrbanDistrictNumber(Planete *planete);

void AddPlanetCityGeneratorDistrict(Planete *planete, int number);
int GetPlanetCityGeneratorDistrictNumber(Planete *planete);

void AddPlanetCityMiningDistrict(Planete *planete, int number);
int GetPlanetCityMiningDistrictNumber(Planete *planete);

void AddPlanetCityAgricultureDistrict(Planete *planete, int number);
int GetPlanetCityAgricultureDistrictNumber(Planete *planete);

void SetPlanetCityBuilding(Planete *planete, int buildingNumber, Batiment batiment, int level);
Batiment GetPlanetCityBuildingNumber(Planete *planete, int buildingNumber);
int GetPlanetCityBuildingLevel(Planete *planete, int buildingNumber);

int CalculatePlanetCityJob(Planete *planete);
int GetPlanetCityJob(Planete *planete);
int GetPlanetCityCrimianity(Planete *planete);
int GetPlanetCityAmienties(Planete *planete);

Villes *GetPlanetCity(Planete *planete);

Ordre* GetCityOrderElement(Villes *villes);
OrdreFile* GetCityOrderQueue(Villes *villes);
OrdreConstruction GetCityOrder(Villes *villes);
int GetCityOrderProgress(Villes *villes);
void UnincrementCityOrderProgress(Villes *villes);
void EndCityOrder(Villes *villes);

int GetCityOrderInfo1(Villes *villes);
int GetCityOrderInfo2(Villes *villes);

#endif