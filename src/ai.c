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

typedef enum {
    credit, 
    mineral, 
    food, 
    alloys, 
    goods
} EconomicPriority;

typedef struct {
    AiOrdreConstruction ordreConstruction;
    Building building;
} PlanetaryConstructionOrder;

struct EmpireAiStruct{
    EconomicPriority economicPriority;
    int militaryPriority;
    GenericList* constructionList;
};


/* entry points ======================================================== */

static void ai_Planet(  Empire *empire){
    Planet* planet = empire_PlanetGet(empire, 0);
    int planetIndex = 1;
    EmpireAi* ai = empire_AiGet(empire);
    int minerals = empire_MineralsGet(empire);
    PlanetaryConstructionOrder* planetaryConstructionOrder = GenericCell_Get(ai->constructionList, 0);

    while(planetaryConstructionOrder && planet){
        //Here we get the planet
        dbg_printf("Planet %d %d %d\n", planetIndex, planetaryConstructionOrder->ordreConstruction, minerals);
        if(minerals > 400) {
            if(planetaryConstructionOrder->ordreConstruction == CONSTRUIRE_BATIMENT){
                order_New(city_OrderQueueGet(planet_CityGet(planet)),
                    CONSTRUIRE_BATIMENT,
                    1, 
                    10,
                    planetaryConstructionOrder->building,
                    1,
                    400
                );
                empire_MineralsAdd(empire, -400);
                free(planetaryConstructionOrder);
                GenericCell_Free(ai->constructionList, 0);
                planetaryConstructionOrder = GenericCell_Get(ai->constructionList, 0);
            }
        } else if(minerals > 50){
            order_New(city_OrderQueueGet(planet_CityGet(planet)), 
                planetaryConstructionOrder->ordreConstruction, 
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

static void ai_EmpireEconomyCalculate(Empire *empire){
    EmpireAi* ai = empire_AiGet(empire);
    int element[5];
    int priority = 0, index = 0;

    assert(ai);

    element[0] = empire_CreditVariationGet(empire);
    element[1] = empire_MineralsVariationGet(empire);
    element[2] = empire_FoodVariationGet(empire);
    element[3] = empire_AlloysVariationGet(empire);
    element[4] = empire_ConsumerVariationGet(empire);

    //Here we get the worst variation of economy
    for (index = 1; index < 5; index++)
        if (element[index] < element[priority])
            priority = index;
    
    dbg_sprintf(dbgout, "%d\n", ai->economicPriority);
    ai->economicPriority = priority;
}

static void ai_EmpireEconomy(   Empire *empire){
    EmpireAi* ai = empire_AiGet(empire);
    assert(ai);
    PlanetaryConstructionOrder* order = calloc(1, sizeof(PlanetaryConstructionOrder));
    
    dbg_sprintf(dbgout, "empireconomy\n");
    switch(ai->economicPriority){
        case credit:
            order->ordreConstruction = CONSTRUIRE_DISTRICT_GENERATEUR;
            GenericCell_Add(ai->constructionList, order);
    dbg_sprintf(dbgout, "%d\n", order->ordreConstruction);
            break;
        case mineral:
            order->ordreConstruction = CONSTRUIRE_DISTRICT_MINIER;
            GenericCell_Add(ai->constructionList, order);
    dbg_sprintf(dbgout, "%d\n", order->ordreConstruction);
            break;
        case food:
            order->ordreConstruction = CONSTRUIRE_DISTRICT_AGRICOLE;
            GenericCell_Add(ai->constructionList, order);
    dbg_sprintf(dbgout, "%d\n", order->ordreConstruction);
            break;
        case alloys:
            order->ordreConstruction = CONSTRUIRE_BATIMENT;
            order->building = BUILDING_FOUNDRIES;
            GenericCell_Add(ai->constructionList, order);
    dbg_sprintf(dbgout, "%d\n", order->ordreConstruction);
            break;
        case goods:
            order->ordreConstruction = CONSTRUIRE_BATIMENT;
            order->building = BUILDING_CIVILIAN_INDUSTRIES;
            GenericCell_Add(ai->constructionList, order);
    dbg_sprintf(dbgout, "%d\n", order->ordreConstruction);
            break;
        default:
            #ifdef DEBUG_VERSION
            dbg_sprintf(dbgerr, "Unknown order id %d\n", ai->economicPriority);
            #endif
            break;
    }
}

static void ai_EmpireFleetCivilian(Empire *empire, Fleet *flotte, StarSystem **systemeStellaires){
    if(GetFleetAction(flotte) == FLOTTE_AUCUNE_ACTION){
        // int systeme = GetFleetSystem(flotte);
        if(GetFleetType(flotte) == FLOTTE_SCIENTIFIQUE){
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
            if(GetFleetType(flotte) == FLOTTE_MILITAIRE)
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
