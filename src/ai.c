/**
 * @file ai.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief All ai related files
 * @version 0.1
 * @date 2021-08-26
 * 
 * @copyright GNU General Public License v3.0
 * 
 */


#include <debug.h>
#include <string.h>
#include <assert.h>

#include "main.h"

#include "ai.h"
#include "empire.h"
#include "galaxy.h"
#include "planet.h"
#include "generic_lists.h"

/* types =============================================================== */

typedef GenericList ConstructionList;
typedef GenericList ConstructionList;

typedef enum {
    credit, 
    mineral, 
    food, 
    alloys, 
    goods
} EconomicPriority;

typedef struct {
    union {
        struct {
            PlanetBuildType buildingType;
            Building building;
        }PlanetOrder;
        struct {
            int a;
        }FleetOrder;
    };
} AiConstuctionOrder;

struct EmpireAiStruct{
    EconomicPriority economicPriority;  // The ressource to prioritize
    int militaryPriority;
    ConstructionList* constructionList; // The contruction queue for the empire
    GenericList* priority;
};



/* entry points ======================================================== */

/**
 * @brief The ai for economy : Priority are calculated, priority are set, and
 * a queue is created. ai_Planet then pick in this queue what building to
 * construct on planets
 * 
 * @param empire 
 */
static void ai_EmpireEconomy(   Empire *empire){
    EmpireAi* ai = empire_AiGet(empire);
    assert(ai);
    AiConstuctionOrder* order = calloc(1, sizeof(AiConstuctionOrder));
    
    dbg_sprintf(dbgout, "Empire economy\n");
    switch(ai->economicPriority){
        case credit:
            order->PlanetOrder.buildingType = CITY_DISTRICT_GENERATOR;
            GenericCell_Add(ai->constructionList, order);
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "%d\n", order->PlanetOrder.buildingType);
            #endif
            break;

        case mineral:
            order->PlanetOrder.buildingType = CITY_DISTRICT_MINING;
            GenericCell_Add(ai->constructionList, order);
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "%d\n", order->PlanetOrder.buildingType);
            #endif
            break;

        case food:
            order->PlanetOrder.buildingType = CITY_DISTRICT_AGRICULTURE;
            GenericCell_Add(ai->constructionList, order);
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "%d\n", order->PlanetOrder.buildingType);
            #endif
            break;

        case alloys:
            order->PlanetOrder.buildingType = CITY_BUILDING;
            order->PlanetOrder.building = BUILDING_FOUNDRIES;
            GenericCell_Add(ai->constructionList, order);
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "%d\n", order->PlanetOrder.buildingType);
            #endif
            break;

        case goods:
            order->PlanetOrder.buildingType = CITY_BUILDING;
            order->PlanetOrder.building = BUILDING_CIVILIAN_INDUSTRIES;
            GenericCell_Add(ai->constructionList, order);
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "%d\n", order->PlanetOrder.buildingType);
            #endif
            break;

        default:
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgerr, "Unknown order id %d\n", ai->economicPriority);
            #endif
            break;
    }
}

static void ai_EmpireEconomyCalculate(Empire *empire){
    EmpireAi* ai = empire_AiGet(empire);
    int element[5];
    int priority = 0, index = 0;

    assert(ai);

    // We place the variation in a buffer easy to use
    element[0] = empire_CreditVariationGet(empire);
    element[1] = empire_MineralsVariationGet(empire);
    element[2] = empire_FoodVariationGet(empire);
    element[3] = empire_AlloysVariationGet(empire);
    element[4] = empire_ConsumerVariationGet(empire);

    //Here we get the worst variation of economy
    for (index = 1; index < 5; index++)
        if (element[index] < element[priority])
            priority = index;
    
    #ifdef DEBUG_VERSION
    dbg_sprintf(dbgout, "%d\n", ai->economicPriority);
    #endif
    ai->economicPriority = priority;
}

static void ai_Planet(  Empire *empire){
    Planet* planet = empire_PlanetGet(empire, 0);
    int planetIndex = 1;
    EmpireAi* ai = empire_AiGet(empire);
    int minerals = empire_MineralsGet(empire);
    AiConstuctionOrder* planetaryConstructionOrder = GenericCell_Get(ai->constructionList, 0);

    while(planetaryConstructionOrder && planet){
        //Here we get the planet
        dbg_printf("Planet %d %d %d\n", planetIndex, planetaryConstructionOrder->PlanetOrder.buildingType, minerals);
        if(minerals > 400) {
            if(planetaryConstructionOrder->PlanetOrder.buildingType == CITY_BUILDING){
                order_NewDeprecated(city_OrderQueueGet(planet_CityGet(planet)),
                    CITY_BUILDING,
                    1, 
                    10,
                    planetaryConstructionOrder->PlanetOrder.building,
                    1,
                    400
                );
                empire_MineralsAdd(empire, -400);
                free(planetaryConstructionOrder);
                GenericCell_Free(ai->constructionList, 0);
                planetaryConstructionOrder = GenericCell_Get(ai->constructionList, 0);
            }
        } else if(minerals > 50){
            order_NewDeprecated(city_OrderQueueGet(planet_CityGet(planet)), 
                planetaryConstructionOrder->PlanetOrder.buildingType, 
                1, 
                12, 
                0, 
                0, 
                50
            );
            empire_MineralsAdd(empire, -50);
            free(planetaryConstructionOrder);
            GenericCell_Free(ai->constructionList, 0);
            planetaryConstructionOrder = GenericCell_Get(ai->constructionList, 0);
        }
        planet = empire_PlanetGet(empire, planetIndex++);
    }
}





static void ai_EmpireFleetCivilian(Empire *empire, Fleet *flotte, StarSystem **systemeStellaires){
    if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
        // int systeme = GetFleetSystem(flotte);
        if(GetFleetType(flotte) == FLEET_SCIENTIFIC){
            if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
                int systemIndex = 0;
                int systemeDestination = 0;
                for(systemIndex = 0; systemIndex < 4; systemIndex++){
                    systemeDestination = hyperlane_DestinationGet(systemeStellaires[GetFleetSystem(flotte)], systemIndex);
                    if(!starSystem_EmpireGet(systemeStellaires[systemeDestination]) && systemeDestination != 255){
                        MoveFleet(flotte, hyperlane_DestinationGet(systemeStellaires[GetFleetSystem(flotte)], systemIndex), systemeStellaires);
                        #ifdef DEBUG_VERSION
                            dbg_sprintf(dbgout, "Empire %p move science fleet from %d to %d\n", empire, GetFleetSystem(flotte), hyperlane_DestinationGet(systemeStellaires[GetFleetSystem(flotte)], systemIndex));
                        #endif
                        systemIndex = 4;
                    }
                }
            }
        }
    }
}

static void ai_EmpireFleetMilitary(){

}

static void ai_EmpireFleetManager(Empire *empire, StarSystem **systemeStellaires){
    FleetList *flotteListe = empire_FleetListGet(empire);
    int tailleFlotte = FleetArraySize(flotteListe);
    if(tailleFlotte > 0){
        int compteurFlotte = 0;
        Fleet *flotte = NULL;

        while(compteurFlotte < tailleFlotte){
            flotte = FlotteNumero(flotteListe, compteurFlotte);
            if(GetFleetType(flotte) == FLEET_MILITARY)
                ai_EmpireFleetMilitary();
            
            else
                ai_EmpireFleetCivilian(empire, flotte, systemeStellaires);

            compteurFlotte++;
        }
    }
}

void ai_EmpireGenerate( Empire *empire){
    EmpireAi* empireAi = calloc(1, sizeof(EmpireAi));
    assert(empireAi);
    empire_AiSet(empire, empireAi);
    empireAi->constructionList = GenericList_Create();
    ai_EmpireEconomyCalculate(empire);
    dbg_printf("Generate ai\n");
}

void ai_Empire( EmpireList *empireList, 
                StarSystem **starSystem, 
                Time *time){
    Empire *empire = NULL;
    int empireIndex = 1;
    int empireTotalNumber = 0;

    empireTotalNumber = empire_ArraySize(empireList);
    while(empireIndex < empireTotalNumber){
        empire = empire_Get(empireList, empireIndex);
        if(time_DayGet(time) == 10 || time_DayGet(time) == 20 || time_DayGet(time) == 30){
            ai_EmpireEconomy(empire);
            ai_Planet(empire);
        }

        ai_EmpireFleetManager(empire, starSystem);
        

        empireIndex++;
    }
}
