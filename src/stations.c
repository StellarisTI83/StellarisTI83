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

void SetStationLevel(Station *station, Stationlevel level){
	station->niveauStation = level;
}
Stationlevel GetStationLevel(Station *station){
	return station->niveauStation;
}