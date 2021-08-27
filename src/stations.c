#include <stdbool.h>
#include <stdlib.h>
#include <tice.h>

#include <string.h>

#include <debug.h>

#include "stations.h"
#include "main.h"

/* structures ========================================================== */
struct StationStruct{
    Stationlevel niveauStation;
    OrdreFile* ordreFile;

    int power;

    int hullBase; 	// base hull
    int hullActual;	// actuall hull
    int armorBase;
    int armorActual;
    int shieldBase;
    int shieldActual;

    StationModule modules[6];
};

/* entry points ======================================================== */
Station* station_Create(){
    Station *station = NULL;
    station = calloc(1, sizeof(Station));
    if(station == NULL)
        exit(EXIT_FAILURE);

    station->ordreFile = CreerFileOrdres();
    return station;
}

void station_Reinitialize(Station *station){
    if(station)
        memset(station, 0, sizeof(Station));
}

void station_LevelSet(Station *station, Stationlevel level){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying set station level : no station\n");
        #endif
        return;
    }
    station->niveauStation = level;
    switch(level){
        case STATION_NONE:
            station->hullActual = 0;
            station->hullBase = 0;
            station->armorActual = 0;
            station->armorBase = 0;
            station->shieldActual = 0;
            station->shieldBase = 0;

            station->modules[0] = STATION_MODULE_NONE;
            station->modules[1] = STATION_MODULE_NONE;

            station->power = 0;
            break;
        case STATION_OUTPOST:
            station->hullActual = 5000;
            station->hullBase = 5000;
            station->armorActual = 1000;
            station->armorBase = 1000;
            station->shieldActual = 600;
            station->shieldBase = 600;

            station->modules[0] = STATION_MODULE_NONE;
            station->modules[1] = STATION_MODULE_NONE;

            station->power = 200;
            break;
        case STATION_STARPORT:
            station->hullActual = 10000;
            station->hullBase = 10000;
            station->armorActual = 2000;
            station->armorBase = 2000;
            station->shieldActual = 1300;
            station->shieldBase = 1300;

            station->modules[2] = STATION_MODULE_NONE;
            station->modules[3] = STATION_MODULE_NONE;

            station->power = 500;
            break;
        case STATION_STARHOLD:
            station->hullActual = 20000;
            station->hullBase = 20000;
            station->armorActual = 5000;
            station->armorBase = 5000;
            station->shieldActual = 3000;
            station->shieldBase = 3000;

            station->modules[4] = STATION_MODULE_NONE;
            station->modules[5] = STATION_MODULE_NONE;

            station->power = 1000;
            break;
        case STATION_STARFORTRESS:
            station->hullActual = 40000;
            station->hullBase = 40000;
            station->armorActual = 10000;
            station->armorBase = 10000;
            station->shieldActual = 4200;
            station->shieldBase = 4200;

            station->power = 5000;
            break;
        case STATION_CITADEL:
            station->hullActual = 80000;
            station->hullBase = 80000;
            station->armorActual = 20000;
            station->armorBase = 20000;
            station->shieldActual = 7800;
            station->shieldBase = 7800;

            station->power = 10000;
            break;
    }
}
Stationlevel station_LevelGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station level : no station\n");
        #endif
        return STATION_NONE;
    }
    return station->niveauStation;
}

void station_ModuleSet(Station *station, int moduleNumber, StationModule module){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying set station module : no station\n");
        #endif
        return;
    }
    station->modules[moduleNumber] = module;
}
StationModule station_ModuleGet(Station *station, int moduleNumber){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station module : no station\n");
        #endif
        return STATION_MODULE_NONE;
    }
    return station->modules[moduleNumber];
}

OrdreFile *station_OrderQueueGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station orderqueue : no station\n");
        #endif
        return NULL;
    }
    return station->ordreFile;
}

void station_NewOrder(Station *station, StationOrder ordre, int empire){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying create station order : no station\n");
        #endif
        return;
    }
    int tempsTotal = 0;
    int info1 = 0;
    int info2 = 0;
    int prix = 0;
    NouvelOrdre(station->ordreFile, empire, ordre, tempsTotal, info1, info2, prix);
}
StationOrder station_OrderGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station order : no station\n");
        #endif
        return STATION_ORDER_NONE;
    }
    return GetOrder(station->ordreFile);
}
void station_OrderEnd(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying end station order : no station\n");
        #endif
        return;
    }
    FinirOrdre(station->ordreFile);
}

int station_OrderProgressGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station order progress : no station\n");
        #endif
        return -1;
    }
    return GetOrderProgress(station->ordreFile);
}
void station_OrderProgressUnincrement(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying unincrement station order progress : no station\n");
        #endif
        return;
    }
    return UnincrementOrderProgress(station->ordreFile);
}

int station_OrderInfo1Get(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station orderinfo1 : no station\n");
        #endif
        return -1;
    }
    return GetOrderInfo1(station->ordreFile);
}
int station_OrderInfo2Get(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station orderinfo2 : no station\n");
        #endif
        return -1;
    }
    return GetOrderInfo2(station->ordreFile);
}

int station_HullGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station hull : no station\n");
        #endif
        return -1;
    }
    return (station->hullActual * 100) / station->hullBase;
}
int station_ArmorGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station armor : no station\n");
        #endif
        return -1;
    }
    return (station->armorActual * 100) / station->armorBase;
}
int station_ShieldGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station shield : no station\n");
        #endif
        return -1;
    }
    return (station->shieldActual * 100) / station->shieldBase;
}
int station_PowerGet(Station *station){
    if(!station){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Error trying get station power : no station\n");
        #endif
        return -1;
    }
    return station->power;
}