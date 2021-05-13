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

#include "planetes.h"

/* structures ========================================================== */
struct VillesStruct{
	Batiment batiment1;
	int niveauBatiment1;

	Batiment batiment2;
	int niveauBatiment2;

	Batiment batiment3;
	int niveauBatiment3;

	Batiment batiment4;
	int niveauBatiment4;

	Batiment batiment5;
	int niveauBatiment5;

	Batiment batiment6;
	int niveauBatiment6;

	char districtsUrbains;
	char districtsGenerateurs;
	char districtsMiniers;
	char districtsAgricoles;
	char criminatlitee;
	int emplois;
	int amienties;

	OrdreFile *ordreFile;
};

struct PlaneteStruct{
	char nom[10];
	int x;
	int y;
	char rayonOrbite;
	char taille;
	unsigned char flags;
	PlanetType planetType; 
	int population;
	Villes *villes;
};

/* entry points ======================================================== */
Planete *AllocPlanet(){
	Planete *planete = NULL;
	planete = calloc(1, sizeof(Planete));
	if(planete == NULL)
		exit(EXIT_FAILURE);
	return planete;
}

void SetPlanetHabitability(Planete *planete, bool habitability){
	planete->flags = (planete->flags | F_HABITABLE);
}
bool GetPlanetHabitability(Planete *planete){
	if((planete->flags & F_HABITABLE) != 0)
		return true;
	else
		return false;
}

void SetPlanetOrbitRadius(Planete *planete, int orbitRadius){
	planete->rayonOrbite = orbitRadius;
}
int GetPlanetOrbitRadius(Planete *planete){
	return planete->rayonOrbite;
}

void SetPlanetXY(Planete *planete, int x, int y){
	planete->x = x;
	planete->y = y;
}
int GetPlanetX(Planete *planete){
	return planete->x;
}
int GetPlanetY(Planete *planete){
	return planete->y;
}

void SetPlanetType(Planete *planete, PlanetType type){
	planete->planetType = type;
}
PlanetType GetPlanetType(Planete *planete){
	return planete->planetType;
}

void SetPlanetRadius(Planete *planete, int radius){
	planete->taille = radius;
}
int GetPlanetRadius(Planete *planete){
	return planete->taille;
}

void SetPlanetName(Planete *planete, char* string){
	if(strlen(string) < 9)
		strcpy(planete->nom, string);
}
char* GetPlanetName(Planete *planete){
	return planete->nom;
}

void CreatePlanetCity(Planete *planete){
	planete->villes = calloc(1, sizeof(Villes));
	planete->villes->ordreFile = CreerFileOrdres();
}
bool GetPlanetCityStatus(Planete *planete){
	if(planete->villes != NULL){
		return true;
	} else {
		return false;
	}
}

void SetPlanetCityPopulation(Planete *planete, int population){
	planete->population = population;
}
int GetPlanetCityPopulation(Planete *planete){
	return planete->population;
}

void SetPlanetCityDistrict(Planete *planete, int urban, int generator, int mining, int agriculture){
	planete->villes->districtsUrbains = urban;
	planete->villes->districtsGenerateurs = generator;
	planete->villes->districtsMiniers = mining;
	planete->villes->districtsAgricoles = agriculture;
}

void AddPlanetCityUrbanDistrict(Planete *planete, int number){
	planete->villes->districtsUrbains += number;
}
int GetPlanetCityUrbanDistrictNumber(Planete *planete){
	return planete->villes->districtsUrbains;
}

void AddPlanetCityGeneratorDistrict(Planete *planete, int number){
	planete->villes->districtsGenerateurs += number;
}
int GetPlanetCityGeneratorDistrictNumber(Planete *planete){
	return planete->villes->districtsGenerateurs;
}

void AddPlanetCityMiningDistrict(Planete *planete, int number){
	planete->villes->districtsMiniers += number;
}
int GetPlanetCityMiningDistrictNumber(Planete *planete){
	return planete->villes->districtsMiniers;
}

void AddPlanetCityAgricultureDistrict(Planete *planete, int number){
	planete->villes->districtsAgricoles += number;
}
int GetPlanetCityAgricultureDistrictNumber(Planete *planete){
	return planete->villes->districtsAgricoles;
}

void SetPlanetCityBuilding(Planete *planete, int buildingNumber, Batiment batiment, int level){
	switch(buildingNumber){
		case 1:
			planete->villes->batiment1 = batiment;
			planete->villes->niveauBatiment1 = level;
			break;
		case 2:
			planete->villes->batiment2 = batiment;
			planete->villes->niveauBatiment2 = level;
			break;
		case 3:
			planete->villes->batiment3 = batiment;
			planete->villes->niveauBatiment3 = level;
			break;
		case 4:
			planete->villes->batiment4 = batiment;
			planete->villes->niveauBatiment4 = level;
			break;
		case 5:
			planete->villes->batiment5 = batiment;
			planete->villes->niveauBatiment5 = level;
			break;
		case 6:
			planete->villes->batiment6 = batiment;
			planete->villes->niveauBatiment6 = level;
			break;
	}
}
Batiment GetPlanetCityBuildingNumber(Planete *planete, int buildingNumber){
	switch(buildingNumber){
		case 1:
			return planete->villes->batiment1;
			break;
		case 2:
			return planete->villes->batiment2;
			break;
		case 3:
			return planete->villes->batiment3;
			break;
		case 4:
			return planete->villes->batiment4;
			break;
		case 5:
			return planete->villes->batiment5;
			break;
		case 6:
			return planete->villes->batiment6;
			break;
	}
	return 0;
}
int GetPlanetCityBuildingLevel(Planete *planete, int buildingNumber){
	switch(buildingNumber){
		case 1:
			return planete->villes->niveauBatiment1;
			break;
		case 2:
			return planete->villes->niveauBatiment2;
			break;
		case 3:
			return planete->villes->niveauBatiment3;
			break;
		case 4:
			return planete->villes->niveauBatiment4;
			break;
		case 5:
			return planete->villes->niveauBatiment5;
			break;
		case 6:
			return planete->villes->niveauBatiment6;
			break;
	}
	return 0;
}

int CalculatePlanetCityJob(Planete *planete){
	if(planete->villes != NULL){
		int job = 0;
		job += planete->villes->districtsUrbains;
		job += planete->villes->districtsGenerateurs * 2;
		job += planete->villes->districtsMiniers * 2;
		job += planete->villes->districtsAgricoles * 2;

		if(planete->villes->batiment1 != AUCUN)
			job += 2;
		if(planete->villes->batiment2 != AUCUN)
			job += 2;
		if(planete->villes->batiment3 != AUCUN)
			job += 2;
		if(planete->villes->batiment4 != AUCUN)
			job += 2;
		if(planete->villes->batiment5 != AUCUN)
			job += 2;
		if(planete->villes->batiment6 != AUCUN)
			job += 2;
			
		planete->villes->emplois = job;
		return job;
	}
	else 
		return 0;
}
int GetPlanetCityJob(Planete *planete){
	if(planete->villes != NULL)
		return planete->villes->emplois;
	else 
		return 0;
}

int GetPlanetCityCrimianity(Planete *planete){
	if(planete->villes != NULL)
		return planete->villes->criminatlitee;
	else 
		return 0;
}

int GetPlanetCityAmienties(Planete *planete){
	if(planete->villes != NULL)
		return planete->villes->amienties;
	else 
		return 0;
}

Villes *GetPlanetCity(Planete *planete){
	return planete->villes;
}

Ordre* GetCityOrderElement(Villes *villes){
	return RecupererOrdre(villes->ordreFile);
}
OrdreFile* GetCityOrderQueue(Villes *villes){
	return villes->ordreFile;
}
OrdreConstruction GetCityOrder(Villes *villes){
	return GetOrder(villes->ordreFile);
}
int GetCityOrderProgress(Villes *villes){
	return GetOrderProgress(villes->ordreFile);
}
void UnincrementCityOrderProgress(Villes *villes){
	UnincrementOrderProgress(villes->ordreFile);
}
void EndCityOrder(Villes *villes){
	FinirOrdre(villes->ordreFile);
}

int GetCityOrderInfo1(Villes *villes){
	return GetOrderInfo1(villes->ordreFile);
}
int GetCityOrderInfo2(Villes *villes){
	return GetOrderInfo2(villes->ordreFile);
}
