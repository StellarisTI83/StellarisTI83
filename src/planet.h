#ifndef H_PLANETES
#define H_PLANETES

#include "ordres.h"

/* types =============================================================== */


typedef enum {  BUILDING_NONE,
                BUILDING_CAPITAL,
                BUILDING_FOUNDRIES, 
                BUILDING_RESEARCH, 
                BUILDING_CIVILIAN_INDUSTRIES, 
                BUILDING_THEATRE} Building;



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

typedef struct CityStruct City;

typedef struct PlanetStruct Planet;

/* entry points ======================================================== */

Planet* planet_Alloc();

void    planet_HabitabilitySet(Planet *planete, int habitability);
int     planet_HabitabilityGet(Planet *planete);

void    planet_OrbitRadiusSet(Planet *planete, int orbitRadius);
int     planet_OrbitRadiusGet(Planet *planete);

void    planet_PositionSet(Planet *planete, int x, int y);
int     planet_GetX(Planet *planete);
int     planet_GetY(Planet *planete);

void    planet_TypeSet(Planet *planete, PlanetType type);
PlanetType  GetPlanetType(Planet *planete);

void    planet_SizeSet(Planet *planete, int radius);
int     planet_SizeGet(Planet *planete);

void    planet_NameGenerate(Planet *planete);
char*   planet_NameGet(Planet *planete);

City*   planet_CityCreate(Planet *planete);
City*   planet_CityGet(Planet *planete);

void    SetPlanetCityPopulation(Planet *planete, int population)    __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityPopulation(Planet *planete)                    __attribute__((deprecated("use planet_CityGet")));

void    SetPlanetCityDistrict(Planet *planete, int urban, int generator, int mining, int agriculture) __attribute__((deprecated("use planet_CityGet")));

void    AddPlanetCityUrbanDistrict(Planet *planete, int number)        __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityUrbanDistrictNumber(Planet *planete)               __attribute__((deprecated("use planet_CityGet")));

void    AddPlanetCityGeneratorDistrict(Planet *planete, int number)    __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityGeneratorDistrictNumber(Planet *planete)           __attribute__((deprecated("use planet_CityGet")));

void    AddPlanetCityMiningDistrict(Planet *planete, int number)       __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityMiningDistrictNumber(Planet *planete)              __attribute__((deprecated("use planet_CityGet")));

void    AddPlanetCityAgricultureDistrict(Planet *planete, int number)  __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityAgricultureDistrictNumber(Planet *planete)         __attribute__((deprecated("use planet_CityGet")));

void    SetPlanetCityBuilding(Planet *planete, int buildingNumber, Building building, int level)   __attribute__((deprecated("use planet_CityGet")));
Building GetPlanetCityBuildingNumber(Planet *planete, int buildingNumber) __attribute__((deprecated("use planet_CityGet")));  
int     GetPlanetCityBuildingLevel(Planet *planete, int buildingNumber) __attribute__((deprecated("use planet_CityGet")));

int     CalculatePlanetCityJob(Planet *planete)                         __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityJob(Planet *planete)                               __attribute__((deprecated("use planet_CityGet")));
int     planet_CriminalityGet(Planet *planete)                        __attribute__((deprecated("use planet_CityGet")));
int     GetPlanetCityAmienties(Planet *planete)                         __attribute__((deprecated("use planet_CityGet")));


/**
 * @brief Create the city for a planet
 * 
 * @param planet 
 * @return City* 
 */
City*   planet_CityCreate(Planet *planet);

/**
 * @brief To get the city structure of a planet
 * 
 * @param planet 
 * @return City* 
 */
City*   planet_CityGet(Planet *planet);

/**
 * @brief To set the population of a city
 * 
 * @param city 
 * @param population 
 */
void    city_PopulationSet(City *city, int population);
/**
 * @brief To get the population of a city
 * 
 * @param city 
 * @return int
 */
int     city_PopulationGet(City *city);

/**
 * @brief To initialize all of the districts of a planet
 * 
 * @param city 
 * @param urban 
 * @param generator 
 * @param mining 
 * @param agriculture 
 */
void    city_DistrictSet(City *city, int urban, int generator, int mining, int agriculture);

/**
 * @brief To add an urban district
 * 
 * @param city 
 * @param urban 
 */
void    city_UrbanDistrictAdd(City *city, int urban);
/**
 * @brief To get the number of urban district
 * 
 * @param city 
 * @return int 
 */
int     city_UrbanDistrictGet(City *city);

/**
 * @brief To add a generator district
 * 
 * @param city 
 * @param generator 
 */
void    city_GeneratorDistrictAdd(City *city, int generator);
/**
 * @brief To get the number of generator district
 * 
 * @param city 
 * @return int 
 */
int     city_GeneratorDistrictGet(City *city);

/**
 * @brief To add a mining district
 * 
 * @param city 
 * @param mining 
 */
void    city_MiningDistrictAdd(City *city, int mining);
/**
 * @brief To get the number of mining district
 * 
 * @param city 
 * @return int 
 */
int     city_MiningDistrictGet(City *city);

/**
 * @brief To add an agriculture district
 * 
 * @param city 
 * @param agriculture 
 */
void    city_AgricultureDistrictAdd(City *city, int agriculture);
/**
 * @brief To get the number of agrictulture district
 * 
 * @param city 
 * @return int 
 */
int     city_AgricultureDistrictGet(City *city);

/**
 * @brief Create a building in a city
 * 
 * @param city 
 * @param building 
 * @param index 
 * @param level 
 */
void    city_BuildingSet(City *city, Building building, int index, int level);
/**
 * @brief Get the type of the building in a city
 * 
 * @param city 
 * @param index 
 * @return Building 
 */
Building city_BuildingGet(City *city, int index);
/**
 * @brief Get the level of a building in a city
 * 
 * @param city 
 * @param index 
 * @return int 
 */
int     city_BuildingLevelGet(City *city, int index);

/**
 * @brief Update the number of jobs in a city and return it
 * 
 * @param city 
 * @return int 
 */
int     city_JobUpdate(City *city);
/**
 * @brief Get the number of job in a city without updating it
 * 
 * @param city 
 * @return int 
 */
int     city_JobGet(City *city);
/**
 * @brief Get the criminality of a city
 * 
 * @param city 
 * @return int 
 */
int     city_CriminalityGet(City *city);
/**
 * @brief Get the amienties of a city
 * 
 * @param city 
 * @return int 
 */
int     city_AmentiesGet(City *city);

Ordre*  GetCityOrderElement(City *city);
OrdreFile* GetCityOrderQueue(City *city);
OrdreConstruction GetCityOrder(City *city);
int     GetCityOrderProgress(City *city);
void    UnincrementCityOrderProgress(City *city);
void    EndCityOrder(City *city);

int     GetCityOrderInfo1(City *city);
int     GetCityOrderInfo2(City *city);

#endif