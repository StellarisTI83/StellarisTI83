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

#include "stations.h"

/* structures ========================================================== */
struct StationStruct{
	Stationlevel niveauStation;
	OrdreFile* ordreFile;

	int puissance;

	int coqueTotal; // quantitée normale
	int coqueRestante; // quantitée actuelle
	int blindageTotal;
	int blindageRestante;
	int bouclierTotal;
	int bouclierRestante;

	Module modules[6];
};

/* entry points ======================================================== */
Station* AllocStation(){
	Station *station = NULL;
	station = calloc(1, sizeof(Station));
	if(station == NULL)
		exit(EXIT_FAILURE);

	station->ordreFile = CreerFileOrdres();
	return station;
}

void memsetStation(Station *station){
	if(station != NULL)
		memset(station, 0, sizeof(Station));
}

void SetStationLevel(Station *station, Stationlevel level){
	station->niveauStation = level;
	switch(level){
		case AUCUN:
			station->coqueRestante = 0;
			station->coqueTotal = 0;
			station->blindageRestante = 0;
			station->blindageTotal = 0;
			station->bouclierRestante = 0;
			station->bouclierTotal = 0;

			station->modules[0] = AUCUN;
			station->modules[1] = AUCUN;

			station->puissance = 0;
			break;
		case AVANT_POSTE:
			station->coqueRestante = 5000;
			station->coqueTotal = 5000;
			station->blindageRestante = 1000;
			station->blindageTotal = 1000;
			station->bouclierRestante = 600;
			station->bouclierTotal = 600;

			station->modules[0] = AUCUN;
			station->modules[1] = AUCUN;

			station->puissance = 200;
			break;
		case PORT_STELLAIRE:
			station->coqueRestante = 10000;
			station->coqueTotal = 10000;
			station->blindageRestante = 2000;
			station->blindageTotal = 2000;
			station->bouclierRestante = 1300;
			station->bouclierTotal = 1300;

			station->modules[2] = AUCUN;
			station->modules[3] = AUCUN;

			station->puissance = 500;
			break;
		case REDOUTE_STELLAIRE:
			station->coqueRestante = 20000;
			station->coqueTotal = 20000;
			station->blindageRestante = 5000;
			station->blindageTotal = 5000;
			station->bouclierRestante = 3000;
			station->bouclierTotal = 3000;

			station->modules[4] = AUCUN;
			station->modules[5] = AUCUN;

			station->puissance = 1000;
			break;
		case FORTERESSE_STELLAIRE:
			station->coqueRestante = 40000;
			station->coqueTotal = 40000;
			station->blindageRestante = 10000;
			station->blindageTotal = 10000;
			station->bouclierRestante = 4200;
			station->bouclierTotal = 4200;

			station->puissance = 5000;
			break;
		case CITADELLE:
			station->coqueRestante = 80000;
			station->coqueTotal = 80000;
			station->blindageRestante = 20000;
			station->blindageTotal = 20000;
			station->bouclierRestante = 7800;
			station->bouclierTotal = 7800;

			station->puissance = 10000;
			break;
	}
}
Stationlevel GetStationLevel(Station *station){
	return station->niveauStation;
}

void SetStationModule(Station *station, int moduleNumber, Module module){
	station->modules[moduleNumber] = module;
}
Module GetStationModule(Station *station, int moduleNumber){
	return station->modules[moduleNumber];
}
Module *GetStationModulePointer(Station *station, int moduleNumber){
	return &station->modules[moduleNumber];
}

OrdreFile *GetStationOrderQueue(Station *station){
	return station->ordreFile;
}

void NewStationOrdre(Station *station, OrdreStation ordre){
	int tempsTotal = 0;
	int info1 = 0;
	int info2 = 0;
	int prix = 0;
	NouvelOrdre(station->ordreFile, ordre, tempsTotal, info1, info2, prix);
}
OrdreStation GetStationOrder(Station *station){
	return GetOrder(station->ordreFile);
}
void EndStationOrder(Station *station){
	FinirOrdre(station->ordreFile);
}

int GetStationOrderProgress(Station *station){
	return GetOrderProgress(station->ordreFile);
}
void UnincrementStationOrderProgress(Station *station){
	return UnincrementOrderProgress(station->ordreFile);
}

int GetStationOrderInfo1(Station *station){
	return GetOrderInfo1(station->ordreFile);
}
int GetStationOrderInfo2(Station *station){
	return GetOrderInfo2(station->ordreFile);
}

int GetStationHullPourcent(Station *station){
	return (station->coqueRestante * 100) / station->coqueTotal;
}
int GetStationArmorPourcent(Station *station){
	return (station->blindageRestante * 100) / station->blindageTotal;
}
int GetStationShieldPourcent(Station *station){
	return (station->bouclierRestante * 100) / station->bouclierTotal;
}
int GetStationPower(Station *station){
	return station->puissance;
}