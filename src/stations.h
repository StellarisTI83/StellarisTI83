#ifndef H_STATIONS
#define H_STATIONS


/* types =============================================================== */
/**
 * @brief Differents levels for a station, affecting the health and power of the
 *        station
 * 
 */
typedef enum {  STATION_NONE = 0, 
                STATION_OUTPOST, 
                STATION_STARPORT, 
                STATION_STARHOLD, 
                STATION_STARFORTRESS, 
                STATION_CITADEL} Stationlevel;

/**
 * @brief Differents possibles modules in a starbase
 * 
 */
typedef enum {  STATION_MODULE_NONE = 0, 
                STATION_MODULE_SHIPYARD, 
                STATION_MODULE_ANCHORAGE, 
                STATION_MODULE_GUN, 
                STATION_MODULE_MISSILES, 
                STATION_MODULE_HANGAR, 
                STATION_MODULE_TRADE_HUB, 
                STATION_MODULE_SOLAR_PANEL} StationModule;

/**
 * @brief Orders that a starbase can do
 * 
 */
typedef enum {  STATION_ORDER_NONE = 0, 
                STATION_ORDER_UPGRADE, 
                STATION_ORDER_BUILD_MODULE, 
                STATION_ORDER_BUILD_PLATFORM, 
                STATION_ORDER_BUILD_SHIP} StationOrder;

/* struct ============================================================== */

/**
 * @brief The struct of a starbase
 * 
 */
typedef struct StationStruct Station;

/* entry points ======================================================== */

#include "ordres.h"

Station* station_Create();
void station_Reinitialize(Station *station);

void station_LevelSet(Station *station, Stationlevel level);
Stationlevel station_LevelGet(Station *station);

void station_ModuleSet(Station *station, int moduleNumber, StationModule module);
StationModule station_ModuleGet(Station *station, int moduleNumber);

OrdreFile *station_OrderQueueGet(Station *station);

void station_NewOrder(Station *station, StationOrder ordre, int empire);
void station_OrderEnd(Station *station);
StationOrder station_OrderGet(Station *station);

int station_OrderProgressGet(Station *station);
void station_OrderProgressUnincrement(Station *station);

int station_OrderInfo1Get(Station *station);
int station_OrderInfo2Get(Station *station);

int station_HullGet(Station *station);
int station_ArmorGet(Station *station);
int station_ShieldGet(Station *station);
int station_PowerGet(Station *station);

#endif