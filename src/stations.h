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

void SetStationLevel(Station *station, Stationlevel level);
Stationlevel GetStationLevel(Station *station);


#endif