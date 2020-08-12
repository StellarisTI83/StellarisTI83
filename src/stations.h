#ifndef H_STATIONS
#define H_STATIONS

#include "ordres.h"

/* types =============================================================== */
typedef enum {AUCUNE, AVANT_POSTE, PORT_STELLAIRE, REDOUTE_STELLAIRE, FORTERESSE_STELLAIRE, CITADELLE} Stationlevel;

typedef enum {AUCUN, CHANTIER_SPATIAL, ANCRAGE, CANONS, MISSILES, HANGAR, CARREFOUR_COMMERCIAL, PANNEAUX_SOLAIRES} Module;

typedef enum {AUCUN, AMELIORER_STATION, CONSTRUIRE_MODULE, CONSTRUIRE_PLATEFORME, CONSTRUIRE_VAISSEAU} OrdreStation;

/* structures ========================================================== */
typedef struct StationStruct Station;

/* entry points ======================================================== */
Station* AllocStation();
void memsetStation(Station *station);

void SetStationLevel(Station *station, Stationlevel level);
Stationlevel GetStationLevel(Station *station);
void EndStationOrder(Station *station);

void SetStationModule(Station *station, int moduleNumber, Module module);
Module GetStationModule(Station *station, int moduleNumber);
Module *GetStationModulePointer(Station *station, int moduleNumber);

OrdreFile *GetStationOrderQueue(Station *station);

void NewStationOrdre(Station *station, OrdreStation ordre);
OrdreStation GetStationOrder(Station *station);

int GetStationOrderProgress(Station *station);
void UnincrementStationOrderProgress(Station *station);

int GetStationOrderInfo1(Station *station);
int GetStationOrderInfo2(Station *station);

int GetStationHullPourcent(Station *station);
int GetStationArmorPourcent(Station *station);
int GetStationShieldPourcent(Station *station);
int GetStationPower(Station *station);
#endif