#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <tice.h>

#include <debug.h>
#include <math.h>

#include "main.h"

#include "planet.h"

char * planetNames[] = {
    "Egnepra",
    "Aceonerth",
    "Tochade",
    "Pemapus",
    "Ruliv",
    "Chuturn",
    "Gidetera",
    "Gneputani",
    "Nora",
    "Gao",
    "Vunubos",
    "Zolveiril",
    "Ostriea",
    "Hibbillon",
    "Cheruta",
    "Puthea",
    "Cuinia",
    "Gachuruta",
    "Gone",
    "Thade",
    "Bevotov",
    "Chavivis",
    "Yuvore",
    "Biseron",
    "Zauter",
    "Sony",
    "Michethea",
    "Gravutis",
    "Lille",
    "Strade",
    "Kidririli",
    "Bonvanus",
    "Icharvis",
    "Zilminda",
    "Yutune",
    "Thaelia",
    "Luchazuno",
    "Truutis",
    "Lleshan",
    "Cyke",
    "Themeanov",
    "Miccoiria",
    "Gichurn",
    "Nibronoe",
    "Nieliv",
    "Roirus",
    "Bamipra",
    "Cregocaro",
    "Brypso",
    "Deron",
    "Kindounus",
    "Danniula",
    "Ondosie",
    "Yangomia",
    "Hephus",
    "Haiturn",
    "Crumuturn",
    "Loxistea",
    "Brade",
    "Tryria",
    "Yovoanide",
    "Ellaclite",
    "Naccone",
    "Relnoth",
    "Beuliv",
    "Thauyama",
    "Chayolara",
    "Phugemia",
    "Larth",
    "Griri"
};

/* structures ========================================================== */
struct CityStruct{
    Building building0;
    int levelBuilding0;

    Building building1;
    int levelBuilding1;

    Building building2;
    int levelBuilding2;

    Building building3;
    int levelBuilding3;

    Building building4;
    int levelBuilding4;

    Building building5;
    int levelBuilding5;

    char districtUrban;
    char districtGenerator;
    char districtMining;
    char districtAgriculture;
    char criminatlitee; 
    int population;
    int job;
    int amienties;

    OrdreFile *ordreFile;
};

struct PlanetStruct{
    char name[10];
    int x;
    int y;
    char orbitRadius;
    char size;
    unsigned char flags;
    PlanetType planetType;
    City *city;
};

/* entry points ======================================================== */

// Planet functions
Planet *planet_Alloc(){
    Planet *planet = NULL;
    planet = calloc(1, sizeof(Planet));
    if(planet == NULL)
        exit(EXIT_FAILURE);
    return planet;
}

void planet_HabitabilitySet(Planet *planet, int habitability){
    if(habitability)
        planet->flags = (planet->flags | F_HABITABLE);
    else
        planet->flags = (planet->flags | !F_HABITABLE);
}
int planet_HabitabilityGet(Planet *planet){
    if((planet->flags & F_HABITABLE) != 0)
        return true;
    else
        return false;
}

void planet_OrbitRadiusSet(Planet *planet, int orbitRadius){
    planet->orbitRadius = orbitRadius;
}
int planet_OrbitRadiusGet(Planet *planet){
    return planet->orbitRadius;
}

void planet_PositionSet(Planet *planet, int x, int y){
    planet->x = x;
    planet->y = y;
}
int planet_GetX(Planet *planet){
    return planet->x;
}
int planet_GetY(Planet *planet){
    return planet->y;
}

void planet_TypeSet(Planet *planet, PlanetType type){
    planet->planetType = type;
}
PlanetType GetPlanetType(Planet *planet){
    return planet->planetType;
}

void planet_SizeSet(Planet *planet, int radius){
    planet->size = radius;
}
int planet_SizeGet(Planet *planet){
    return planet->size;
}

void planet_NameGenerate(Planet *planet){
    int index = randInt(0, (sizeof(planetNames)/sizeof(planetNames[0])) - 1);
    char *string = planetNames[index];
    if(strlen(string) <= 9)
        strcpy(planet->name, string);
    #ifdef DEBUG_VERSION
    else
        dbg_sprintf(dbgerr, "Name %s index %d too long\n", string, index);
    #endif
}
char* planet_NameGet(Planet *planet){
    return planet->name;
}


// Compatibility (deprecated)

void SetPlanetCityPopulation(Planet *planet, int population){
    if(planet && planet->city)
	    planet->city->population = population;
}
int GetPlanetCityPopulation(Planet *planet){
    if(planet && planet->city)
	    return planet->city->population;
    else
        return 0;
}

void SetPlanetCityDistrict(Planet *planet, int urban, int generator, int mining, int agriculture){
    planet->city->districtUrban = urban;
    planet->city->districtGenerator = generator;
    planet->city->districtMining = mining;
    planet->city->districtAgriculture = agriculture;
}

void AddPlanetCityUrbanDistrict(Planet *planet, int number){
    planet->city->districtUrban += number;
}
int GetPlanetCityUrbanDistrictNumber(Planet *planet){
    return planet->city->districtUrban;
}

void AddPlanetCityGeneratorDistrict(Planet *planet, int number){
    planet->city->districtGenerator += number;
}
int GetPlanetCityGeneratorDistrictNumber(Planet *planet){
    return planet->city->districtGenerator;
}

void AddPlanetCityMiningDistrict(Planet *planet, int number){
    planet->city->districtMining += number;
}
int GetPlanetCityMiningDistrictNumber(Planet *planet){
    return planet->city->districtMining;
}

void AddPlanetCityAgricultureDistrict(Planet *planet, int number){
    planet->city->districtAgriculture += number;
}
int GetPlanetCityAgricultureDistrictNumber(Planet *planet){
    return planet->city->districtAgriculture;
}

void SetPlanetCityBuilding(Planet *planet, int buildingNumber, Building building, int level){
    switch(buildingNumber){
        case 1:
            planet->city->building0 = building;
            planet->city->levelBuilding0 = level;
            break;
        case 2:
            planet->city->building1 = building;
            planet->city->levelBuilding1 = level;
            break;
        case 3:
            planet->city->building2 = building;
            planet->city->levelBuilding2 = level;
            break;
        case 4:
            planet->city->building3 = building;
            planet->city->levelBuilding3 = level;
            break;
        case 5:
            planet->city->building4 = building;
            planet->city->levelBuilding4 = level;
            break;
        case 6:
            planet->city->building5 = building;
            planet->city->levelBuilding5 = level;
            break;
    }
}
Building GetPlanetCityBuildingNumber(Planet *planet, int buildingNumber){
    switch(buildingNumber){
        case 1:
            return planet->city->building0;
            break;
        case 2:
            return planet->city->building1;
            break;
        case 3:
            return planet->city->building2;
            break;
        case 4:
            return planet->city->building3;
            break;
        case 5:
            return planet->city->building4;
            break;
        case 6:
            return planet->city->building5;
            break;
    }
    return 0;
}
int GetPlanetCityBuildingLevel(Planet *planet, int buildingNumber){
    switch(buildingNumber){
        case 1:
            return planet->city->levelBuilding0;
            break;
        case 2:
            return planet->city->levelBuilding1;
            break;
        case 3:
            return planet->city->levelBuilding2;
            break;
        case 4:
            return planet->city->levelBuilding3;
            break;
        case 5:
            return planet->city->levelBuilding4;
            break;
        case 6:
            return planet->city->levelBuilding5;
            break;
    }
    return 0;
}

int CalculatePlanetCityJob(Planet *planet){
    if(planet->city != NULL){
        int job = 0;
        job += planet->city->districtUrban;
        job += planet->city->districtGenerator * 2;
        job += planet->city->districtMining * 2;
        job += planet->city->districtAgriculture * 2;

        if(planet->city->building0 != BUILDING_NONE)
            job += 2;
        if(planet->city->building1 != BUILDING_NONE)
            job += 2;
        if(planet->city->building2 != BUILDING_NONE)
            job += 2;
        if(planet->city->building3 != BUILDING_NONE)
            job += 2;
        if(planet->city->building4 != BUILDING_NONE)
            job += 2;
        if(planet->city->building5 != BUILDING_NONE)
            job += 2;
            
        planet->city->job = job;
        return job;
    }
    else 
        return 0;
}
int GetPlanetCityJob(Planet *planet){
    if(planet->city)
        return planet->city->job;
    else 
        return 0;
}

int planet_CriminalityGet(Planet *planet){
    if(planet->city)
        return planet->city->criminatlitee;
    else 
        return 0;
}

int GetPlanetCityAmienties(Planet *planet){
    if(planet->city)
        return planet->city->amienties;
    else 
        return 0;
}



// City functions
City *planet_CityCreate(Planet *planet){
    if(planet){
        planet->city = calloc(1, sizeof(City));
        planet->city->ordreFile = CreerFileOrdres();
        return planet->city;
    } else
        return NULL;
}

City *planet_CityGet(Planet *planet){
    if(planet)
        return planet->city;
    else
        return NULL;
}

void city_PopulationSet(City *city, int population){
    if(city)
        city->population = population;
}
int city_PopulationGet(City *city){
    if(city)
        return city->population;
    else
        return 0;
}

void city_DistrictSet(City *city, int urban, int generator, int mining, int agriculture){
    if(city){
        city->districtUrban = urban;
        city->districtGenerator = generator;
        city->districtMining = mining;
        city->districtAgriculture = agriculture;
    }
}

void city_UrbanDistrictAdd(City *city, int urban){
    if(city)
        city->districtUrban += urban;
}
int city_UrbanDistrictGet(City *city){
    if(city)
        return city->districtUrban;
    else
        return 0;
}

void city_GeneratorDistrictAdd(City *city, int generator){
    if(city)
        city->districtGenerator += generator;
}
int city_GeneratorDistrictGet(City *city){
    if(city)
        return city->districtGenerator;
    else
        return 0;
}

void city_MiningDistrictAdd(City *city, int mining){
    if(city)
        city->districtMining += mining;
}
int city_MiningDistrictGet(City *city){
    if(city)
        return city->districtMining;
    else
        return 0;
}

void city_AgricultureDistrictAdd(City *city, int agriculture){
    if(city)
        city->districtAgriculture += agriculture;
}
int city_AgricultureDistrictGet(City *city){
    if(city)
        return city->districtAgriculture;
    else
        return 0;
}

void city_BuildingSet(City *city, Building building, int index, int level){
    if(city)
        switch(index){
            case 0:
                city->building0 = building;
                city->levelBuilding0 = level;
                break;
            case 1:
                city->building1 = building;
                city->levelBuilding1 = level;
                break;
            case 2:
                city->building2 = building;
                city->levelBuilding2 = level;
                break;
            case 3:
                city->building3 = building;
                city->levelBuilding3 = level;
                break;
            case 4:
                city->building4 = building;
                city->levelBuilding4 = level;
                break;
            case 5:
                city->building5 = building;
                city->levelBuilding5 = level;
                break;
        }
}
Building city_BuildingGet(City *city, int index){
    if(city)
        switch(index){
            case 0:
                return city->building0;
                break;
            case 1:
                return city->building1;
                break;
            case 2:
                return city->building2;
                break;
            case 3:
                return city->building3;
                break;
            case 4:
                return city->building4;
                break;
            case 5:
                return city->building5;
                break;
            default:
                return 0;
                break;
        }
    else
        return 0;
}
int city_BuildingLevelGet(City *city, int index){
    if(city)
        switch(index){
            case 0:
                return city->levelBuilding0;
                break;
            case 1:
                return city->levelBuilding0;
                break;
            case 2:
                return city->levelBuilding0;
                break;
            case 3:
                return city->levelBuilding0;
                break;
            case 4:
                return city->levelBuilding0;
                break;
            case 5:
                return city->levelBuilding0;
                break;
            default:
                return 0;
                break;
        }
    else
        return 0;
}

int city_JobUpdate(City *city){
    if(city != NULL){
        int job = 0;
        job += city->districtUrban;
        job += city->districtGenerator * 2;
        job += city->districtMining * 2;
        job += city->districtAgriculture * 2;

        if(city->building0 != BUILDING_NONE)
            job += 2;
        if(city->building1 != BUILDING_NONE)
            job += 2;
        if(city->building2 != BUILDING_NONE)
            job += 2;
        if(city->building3 != BUILDING_NONE)
            job += 2;
        if(city->building4 != BUILDING_NONE)
            job += 2;
        if(city->building5 != BUILDING_NONE)
            job += 2;
            
        city->job = job;
        return job;
    }
    else 
        return 0;
}
int city_JobGet(City *city){
    if(city)
        return city->job;
    else 
        return 0;
}
int city_CriminalityGet(City *city){
    if(city)
        return city->criminatlitee;
    else
        return 0;
}
int city_AmentiesGet(City *city){
    if(city)
        return city->amienties;
    else
        return 0;
}

Ordre* GetCityOrderElement(City *city){
    if(city)
        return RecupererOrdre(city->ordreFile);
    else
        return NULL;
}
OrdreFile* GetCityOrderQueue(City *city){
    if(city)
        return city->ordreFile;
    else
        return NULL;
}
OrdreConstruction GetCityOrder(City *city){
    if(city)
        return GetOrder(city->ordreFile);
    else
        return 0;
}
int GetCityOrderProgress(City *city){
    if(city)
        return GetOrderProgress(city->ordreFile);
    else
        return 0;
}
void UnincrementCityOrderProgress(City *city){
    if(city)
        UnincrementOrderProgress(city->ordreFile);
}
void EndCityOrder(City *city){
    if(city)
        FinirOrdre(city->ordreFile);
}

int GetCityOrderInfo1(City *city){
    if(city)
        return GetOrderInfo1(city->ordreFile);
    else
        return 0;
}
int GetCityOrderInfo2(City *city){
    if(city)
        return GetOrderInfo2(city->ordreFile);
    else
        return 0;
}

