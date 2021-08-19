#ifndef H_PLANETES
#define H_PLANETES

#include "ordres.h"

/* types =============================================================== */


typedef enum {  AUCUN_DISTRICT_VILLE,
                CAPITALE,
                FONDERIE, 
                LABORATOIRE, 
                USINE_CIVILE, 
                THEATRE} Batiment;



typedef enum {  AUCUN_ORDRE_VILLE,
                CONSTRUIRE_DISTRICT_URBAIN,
                CONSTRUIRE_DISTRICT_GENERATEUR, 
                CONSTRUIRE_DISTRICT_MINIER,
                CONSTRUIRE_DISTRICT_AGRICOLE, 
                CONSTRUIRE_BATIMENT} OrdreConstruction;

/**
 * @brief The different planet types
 * 
 */
typedef enum {  HABITABLE_ARID = 1, 
                HABITABLE_DESERT, 
                HABITABLE_SAVANNA, 
                HABITABLE_ALPINE, 
                HABITABLE_ARCTIC, 
                HABITABLE_TUNDRA,
                HABITABLE_CONTINENTAL, 
                HABITABLE_OCEAN, 
                HABITABLE_TROPICAL,
                ARID, 
                COLD_ARID, 
                BROKE, 
                FROZEN, 
                GAS_GIANT, 
                MOLTEN, 
                TOXIC} PlanetType;

/* flags =============================================================== */

#define F_HABITABLE (1)
#define F_RESEARCH_STATION (2)
#define F_MINE_STATION (4)

/* structures ========================================================== */

typedef struct VillesStruct City;

typedef struct PlaneteStruct Planete;

/* entry points ======================================================== */

Planete *AllocPlanet();

void SetPlanetHabitability(Planete *planete, int habitability);
int GetPlanetHabitability(Planete *planete);

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
int GetPlanetCityStatus(Planete *planete);
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

City *GetPlanetCity(Planete *planete);

Ordre* GetCityOrderElement(City *villes);
OrdreFile* GetCityOrderQueue(City *villes);
OrdreConstruction GetCityOrder(City *villes);
int GetCityOrderProgress(City *villes);
void UnincrementCityOrderProgress(City *villes);
void EndCityOrder(City *villes);

int GetCityOrderInfo1(City *villes);
int GetCityOrderInfo2(City *villes);

#endif