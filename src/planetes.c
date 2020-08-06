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
	char habitable;
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
	planete->habitable = habitability;
}
bool GetPlanetHabitability(Planete *planete){
	return planete->habitable;
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