#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include "systemes.h"
#include "stations.h"

/* structures ========================================================== */
struct HyperlaneStruct{
	int destination;
	int x;
	int y;
};

struct SystemeStellaireStruct{
	char nom[10];
	int x;
	int y;

	EtoileType etoileType; //1 = B, 2 = A, 3 = F, 4 = G, 5 = K, 6 = M, 7 = trou noir, 8 = pulsar, 9 = etoile a neutron / B : violet, A : Bleu, F : Blanc, G K et M: orange
	char nombrePlanetes;
	char nombrePlanetesHabitees;
	char nombrePlanetesHabitables;
	char empire;

	Hyperlane hyperlane[4];

	NiveauDeConnaissance niveauDeConnaissance; //0 = inconnu, 1 = faible, 2 = moyen, 3 = haut, 4 = total (intel level)

	Planete *planetes[5];
	Station* station;
};

/* entry points ======================================================== */

SystemeStellaire* AllocSystem(){
	return calloc(1, sizeof(SystemeStellaire));
}

void SetSystemXY(SystemeStellaire *systemeStellaire, int x, int y){
	systemeStellaire->x = x;
	systemeStellaire->y = y;
}
int GetSystemX(SystemeStellaire *systemeStellaire){
	return systemeStellaire->x;
}
int GetSystemY(SystemeStellaire *systemeStellaire){
	return systemeStellaire->y;
}

//station
void CreateSystemStation(SystemeStellaire *systemeStellaire){
	systemeStellaire->station = AllocStation();
}

void SetSystemStationModule(SystemeStellaire *systemeStellaire, int moduleNumber, Module module){
	SetStationModule(systemeStellaire->station, moduleNumber, module);
}
Module GetSystemStationModule(SystemeStellaire *systemeStellaire, int moduleNumber){
	return GetStationModule(systemeStellaire->station, moduleNumber);
}

void SetSystemIntelLevel(SystemeStellaire *systemeStellaire, NiveauDeConnaissance niveau){
	systemeStellaire->niveauDeConnaissance = niveau;
}
NiveauDeConnaissance GetSystemIntelLevel(SystemeStellaire *systemeStellaire){
	return systemeStellaire->niveauDeConnaissance;
}

void SetSystemStarType(SystemeStellaire *systemeStellaire, EtoileType type){
	systemeStellaire->etoileType = type;
}
EtoileType GetSystemStarType(SystemeStellaire *systemeStellaire){
	return systemeStellaire->etoileType;
}

//stations
void SetSystemStationLevel(SystemeStellaire *systemeStellaire, Stationlevel level){
	SetStationLevel(systemeStellaire->station, level);
}
Stationlevel GetSystemStationLevel(SystemeStellaire *systemeStellaire){
	return GetStationLevel(systemeStellaire->station);
}

OrdreStation GetSystemStationOrder(SystemeStellaire *systemeStellaire){
	return GetStationOrder(systemeStellaire->station);
}
void EndSystemStationOrder(SystemeStellaire *systemeStellaire){
	EndStationOrder(systemeStellaire->station);
}

int GetSystemStationOrderProgress(SystemeStellaire *systemeStellaire){
	return GetStationOrderProgress(systemeStellaire->station);
}
void UnincrementSystemStationOrderProgress(SystemeStellaire *systemeStellaire){
	UnincrementStationOrderProgress(systemeStellaire->station);
}

int GetSystemStationInfo1(SystemeStellaire *systemeStellaire){
	return GetStationOrderInfo1(systemeStellaire->station);
}
int GetSystemStationInfo2(SystemeStellaire *systemeStellaire){
	return GetStationOrderInfo2(systemeStellaire->station);
}

void SetSystemEmpire(SystemeStellaire *systemeStellaire, int empire){
	systemeStellaire->empire = empire;
}
int GetSystemEmpire(SystemeStellaire *systemeStellaire){
	return systemeStellaire->empire;
}

void SetSystemName(SystemeStellaire *systemeStellaire, char* string){
	if(strlen(string) < 9)
		strcpy(systemeStellaire->nom, string);
}
char* GetSystemName(SystemeStellaire *systemeStellaire){
	return systemeStellaire->nom;
}



//hyperlanes
void SetHyperlaneDestination(SystemeStellaire *systemeStellaire, int numeroHyperlane, int destination){
	systemeStellaire->hyperlane[numeroHyperlane].destination = destination;
}
int GetHyperlaneDestination(SystemeStellaire *systemeStellaire, int numeroHyperlane){
	return systemeStellaire->hyperlane[numeroHyperlane].destination;
}

void SetHyperlaneXY(SystemeStellaire *systemeStellaire, int numeroHyperlane, int x, int y){
	systemeStellaire->hyperlane[numeroHyperlane].x = x;
	systemeStellaire->hyperlane[numeroHyperlane].y = y;
}
int GetHyperlaneX(SystemeStellaire *systemeStellaire, int numeroHyperlane){
	return systemeStellaire->hyperlane[numeroHyperlane].x;
}
int GetHyperlaneY(SystemeStellaire *systemeStellaire, int numeroHyperlane){
	return systemeStellaire->hyperlane[numeroHyperlane].y;
}

// planetes
void CreateSystemPlanet(SystemeStellaire *systemeStellaire, int position){
	systemeStellaire->planetes[position] = AllocPlanet();
}

void SetSystemPlanetHabitability(SystemeStellaire *systemeStellaire, int number, bool habitability){
	SetPlanetHabitability(systemeStellaire->planetes[number], habitability);
}
bool GetSystemPlanetHabitability(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetHabitability(systemeStellaire->planetes[number]);
}

void SetSystemPlanetOrbitRadius(SystemeStellaire *systemeStellaire, int number, int orbitRadius){
	SetPlanetOrbitRadius(systemeStellaire->planetes[number], orbitRadius);
}
int GetSystemPlanetOrbitRadius(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetOrbitRadius(systemeStellaire->planetes[number]);
}

void SetSystemPlanetXY(SystemeStellaire *systemeStellaire, int number, int x, int y){
	SetPlanetXY(systemeStellaire->planetes[number], x, y);
}
int GetSystemPlanetX(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetX(systemeStellaire->planetes[number]);
}
int GetSystemPlanetY(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetY(systemeStellaire->planetes[number]);
}

void SetSystemPlanetType(SystemeStellaire *systemeStellaire, int number, PlanetType type){
	SetPlanetType(systemeStellaire->planetes[number], type);
}
PlanetType GetSystemPlanetType(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetType(systemeStellaire->planetes[number]);
}

void SetSystemPlanetRadius(SystemeStellaire *systemeStellaire, int number, int radius){
	SetPlanetRadius(systemeStellaire->planetes[number], radius);
}
int GetSystemPlanetRadius(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetRadius(systemeStellaire->planetes[number]);
}

void SetSystemPlanetName(SystemeStellaire *systemeStellaire, int number, char* string){
	SetPlanetName(systemeStellaire->planetes[number], string);
}
char* GetSystemPlanetName(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetName(systemeStellaire->planetes[number]);
}

void CreateSystemPlanetCity(SystemeStellaire *systemeStellaire, int number){
	CreatePlanetCity(systemeStellaire->planetes[number]);
}


void SetSystemPlanetCityPopulation(SystemeStellaire *systemeStellaire, int number, int population){
	SetPlanetCityPopulation(systemeStellaire->planetes[number], population);
}
int GetSystemPlanetCityPopulation(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCityPopulation(systemeStellaire->planetes[number]);
}

void SetSystemPlanetCityDistrict(SystemeStellaire *systemeStellaire, int number, int urban, int generator, int mining, int agriculture){
	SetPlanetCityDistrict(systemeStellaire->planetes[number], urban, generator, mining, agriculture);
}

void AddSystemPlanetCityUrbanDistrict(SystemeStellaire *systemeStellaire, int number, int urban){
	AddPlanetCityUrbanDistrict(systemeStellaire->planetes[number], urban);
}
int GetSystemPlanetCityUrbanDistrict(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCityUrbanDistrictNumber(systemeStellaire->planetes[number]);
}

void AddSystemPlanetCityGeneratorDistrict(SystemeStellaire *systemeStellaire, int number, int generator){
	AddPlanetCityGeneratorDistrict(systemeStellaire->planetes[number], generator);
}
int GetSystemPlanetCityGeneratorDistrict(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCityGeneratorDistrictNumber(systemeStellaire->planetes[number]);
}

void AddSystemPlanetCityMiningDistrict(SystemeStellaire *systemeStellaire, int number, int mining){
	AddPlanetCityMiningDistrict(systemeStellaire->planetes[number], mining);
}
int GetSystemPlanetCityMiningDistrict(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCityMiningDistrictNumber(systemeStellaire->planetes[number]);
}

void AddSystemPlanetCityAgricultureDistrict(SystemeStellaire *systemeStellaire, int number, int agriculture){
	AddPlanetCityAgricultureDistrict(systemeStellaire->planetes[number], agriculture);
}
int GetSystemPlanetCityAgricultureDistrict(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCityAgricultureDistrictNumber(systemeStellaire->planetes[number]);
}

void SetSystemPlanetCityBuilding(SystemeStellaire *systemeStellaire, int number, int buildingNumber, Batiment batiment, int level){
	SetPlanetCityBuilding(systemeStellaire->planetes[number], buildingNumber, batiment, level);
}
Batiment GetSystemPlanetCityBuildingNumber(SystemeStellaire *systemeStellaire, int number, int buildingNumber){
	return GetPlanetCityBuildingNumber(systemeStellaire->planetes[number], buildingNumber);
}
int GetSystemPlanetCityBuildingLevel(SystemeStellaire *systemeStellaire, int number, int buildingNumber){
	return GetPlanetCityBuildingNumber(systemeStellaire->planetes[number], buildingNumber);
}

int CalculateSystemPlanetCityJob(SystemeStellaire *systemeStellaire, int number){
	return CalculatePlanetCityJob(systemeStellaire->planetes[number]);
}
int GetSystemPlanetCityJob(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCityJob(systemeStellaire->planetes[number]);
}

Planete *GetPlanet(SystemeStellaire *systemeStellaire, int number){
	return systemeStellaire->planetes[number];
}

void SetSystemPlanetNumber(SystemeStellaire *systemeStellaire, int number){
	systemeStellaire->nombrePlanetes = number;
}
int GetSystemPlanetNumber(SystemeStellaire *systemeStellaire){
	return systemeStellaire->nombrePlanetes;
}

void SetSystemPlanetHabitableNumber(SystemeStellaire *systemeStellaire, int number){
	systemeStellaire->nombrePlanetesHabitables = number;
}
int GetSystemPlanetHabitableNumber(SystemeStellaire *systemeStellaire){
	return systemeStellaire->nombrePlanetesHabitables;
}

void SetSystemPlanetInhabitedNumber(SystemeStellaire *systemeStellaire, int number){
	systemeStellaire->nombrePlanetesHabitees = number;
}
int GetSystemPlanetInhabitedNumber(SystemeStellaire *systemeStellaire){
	return systemeStellaire->nombrePlanetesHabitees;
}

Villes *GetSystemPlanetCity(SystemeStellaire *systemeStellaire, int number){
	return GetPlanetCity(systemeStellaire->planetes[number]);
}