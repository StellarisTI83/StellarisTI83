#ifndef H_PLANETES
#define H_PLANETES


/* types =============================================================== */


typedef enum {  BUILDING_NONE,
                BUILDING_CAPITAL,
                BUILDING_FOUNDRIES, 
                BUILDING_RESEARCH, 
                BUILDING_CIVILIAN_INDUSTRIES, 
                BUILDING_THEATRE} Building;



typedef enum {  CITY_ORDER_NONE,
                CITY_DISTRICT_URBAN,
                CITY_DISTRICT_GENERATOR, 
                CITY_DISTRICT_MINING,
                CITY_DISTRICT_AGRICULTURE, 
                CITY_BUILDING} PlanetBuildType;

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

#define PLANET_MAX 5

/* flags =============================================================== */

#define F_HABITABLE (1)
#define F_RESEARCH_STATION (2)
#define F_MINE_STATION (4)

/* struct ============================================================== */

typedef struct CityStruct City;

typedef struct PlanetStruct Planet;

/* entry points ======================================================== */

#include "ordres.h"

Planet* planet_Alloc();

void    planet_HabitabilitySet(Planet *planete, int habitability);
int     planet_HabitabilityGet(const Planet *planete);

void    planet_OrbitRadiusSet(Planet *planete, int orbitRadius);
int     planet_OrbitRadiusGet(const Planet *planete);

void    planet_PositionSet(Planet *planete, int x, int y);
int     planet_XGet(const Planet *planete);
int     planet_YGet(const Planet *planete);

void    planet_TypeSet(Planet *planete, PlanetType type);
PlanetType  planet_TypeGet(const Planet *planete);

void    planet_SizeSet(Planet *planete, int radius);
int     planet_SizeGet(const Planet *planete);

void    planet_NameGenerate(Planet *planete);
char*   planet_NameGet(Planet *planete);

City*   planet_CityCreate(Planet *planete);
City*   planet_CityGet(const Planet *planete);

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
int     planet_CriminalityGet(const Planet *planete)                        __attribute__((deprecated("use planet_CityGet")));
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
City*   planet_CityGet(const Planet *planet);

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
int     city_PopulationGet(const City *city);

int city_Build(City *city, Empire *empire, const PlanetBuildType type, const Building building);

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
int     city_UrbanDistrictGet(const City *city);

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
int     city_GeneratorDistrictGet(const City *city);

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
int     city_MiningDistrictGet(const City *city);

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
int     city_AgricultureDistrictGet(const City *city);

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
Building city_BuildingGet(const City *city, const int index);
/**
 * @brief Get the level of a building in a city
 * 
 * @param city 
 * @param index 
 * @return int 
 */
int     city_BuildingLevelGet(const City *city, const int index);

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
int     city_JobGet(const City *city);
/**
 * @brief Get the criminality of a city
 * 
 * @param city 
 * @return int 
 */
int     city_CriminalityGet(const City *city);
/**
 * @brief Get the amienties of a city
 * 
 * @param city 
 * @return int 
 */
int     city_AmentiesGet(const City *city);

Ordre*  GetCityOrderElement(City *city);
OrdreFile* city_OrderQueueGet(City *city);
PlanetBuildType city_OrderGet(City *city);
int     city_OrderProgressGet(City *city);
void    city_OrderProgressUnincrement(City *city);
void    city_OrderEnd(City *city);

int     city_OrderInfo1Get(City *city);
int     city_OrderInfo2Get(City *city);

#endif