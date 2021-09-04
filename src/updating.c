/**
 * @file updating.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief The updating 
 * @version 0.1
 * @date 2021-08-22
 * 
 * @copyright GNU General Public License v3.0
 * 
 */
#include <debug.h>

#include "main.h"

#include "loop.h"
#include "fleet.h"
#include "map.h"
#include "galaxy.h"
#include "sauvegarde.h"
#include "settings.h"
#include "time.h"
#include "updating.h"

/* private functions =================================================== */
/**
 * @brief Test the keys
 * 
 * @param key 
 * @param empireList 
 * @param galaxy 
 * @param time 
 * @param camera 
 * @param window 
 * @param settings 
 */
static void update_KeysTest(char *key, 
                            StarSystem **galaxy, 
                            Time *time, 
                            Camera *camera, 
                            Window *window, 
                            Settings *settings){
    // Camera keys binding
    if (camera_LockGet(camera) != true){
        switch(*key){
            // Moving the camera up down left and right
            case sk_Up:
                camera_YVectorAdd(camera, -5);
                break;
            case sk_Down:
                camera_YVectorAdd(camera, +5);
                break;
            case sk_Left:
                camera_XVectorAdd(camera, -5);
                break;
            case sk_Right:
                camera_XVectorAdd(camera, +5);
                break;
            // Zoom out
            case sk_Mode:
                switch(camera_MapTypeGet(camera)){
                    case VUE_GALACTIC:
                        camera_ZoomSet(camera, camera_ZoomGet(camera) - 1);
                        break;
                    case VUE_SYSTEM:
                        camera_MapTypeSet(camera, VUE_GALACTIC);
                        break;
                    default:
                        break;
                }
                *key = 0;
                break;
            // Zoom in
            case sk_Del:
                switch(camera_MapTypeGet(camera)){
                    case VUE_GALACTIC:
                        if((starSystem_IntelLevelGet(galaxy[camera_SystemActualGet(camera)]) 
                        || settings_SeeAllGet(settings))
                        && camera_SystemActualGet(camera) != NO_SYSTEM
                        && camera_ZoomGet(camera) == ZOOM_MAX){
                            camera_MapTypeSet(camera, VUE_SYSTEM);
                            camera_SystemActualSet(camera, camera_SystemActualGet(camera));
                        }
                        camera_ZoomSet(camera, camera_ZoomGet(camera) + 1);
                        break;
                    case VUE_MAP:
                        camera_MapTypeSet(camera, VUE_GALACTIC);
                        break;
                    default:
                        break;
                }
                *key = 0;
                break;
            // Open the system
            case sk_Enter:
                if((starSystem_IntelLevelGet(galaxy[camera_SystemActualGet(camera)]) 
                || settings_SeeAllGet(settings))
                && camera_SystemActualGet(camera) != NO_SYSTEM){
                    camera_MapTypeSet(camera, VUE_SYSTEM);
                    camera_SystemActualSet(camera, camera_SystemActualGet(camera));
                }
                *key = 0;
                break;
            // Pause and unpause
            case sk_0:
                speed_TimeGet(time) ? time_Pause(time) : time_Unpause(time);
                break;
            // Change game speed
            case sk_Add:
                time_SpeedIncrement(time);
                break;
            case sk_Sub:
                time_SpeedUnincrement(time);
                break;

            default:
                break;
        }
    }
    // Other keys
    switch(*key){
        // Open the command prompt
        case sk_2nd:
            if(!GetCommandPromptStatus(window))
                OpenCommandPrompt(window, camera, time);
            else
                CloseCommandPrompt(window, camera, time);
            *key = 0;
            break;
        // Open the exit menu
        case sk_Clear:
            if(GetOpenedMenuClass(window) == MENU_EXIT)
                CloseMenu(window, camera);
            else
                OpenMenu(window, camera, MENU_EXIT, MENU_SYSTEME_AUCUN);
            *key = 0;
            break;

        // Open the market menus
        // The top buttons for the top hud
        case sk_Yequ:
            OpenMenu(window, camera, MENU_MARKET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, MENU_MARKET_CASH);
            break;
        case sk_Window:
            OpenMenu(window, camera, MENU_MARKET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, MENU_MARKET_MINERAL);
            break;
        case sk_Zoom:
            OpenMenu(window, camera, MENU_MARKET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, MENU_MARKET_FOOD);
            break;
        case sk_Trace:
            OpenMenu(window, camera, MENU_MARKET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, MENU_MARKET_ALLOY);
            break;
        case sk_Graph:
            OpenMenu(window, camera, MENU_MARKET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, MENU_MARKET_OTHER);
            break;

        // The left buttons for the left hud
        case sk_Recip:
            if(GetOpenedMenuClass(window) == MENU_MARKET)
                CloseMenu(window, camera);
            else
                OpenMenu(window, camera, MENU_MARKET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, 0);
            break;
        case sk_Math:
            if(GetOpenedMenuClass(window) == MENU_CONTACTS)
                CloseMenu(window, camera);
            else
                OpenMenu(window, camera, MENU_CONTACTS, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, 0);
            break;
        case sk_Square:
            if(GetOpenedMenuClass(window) == MENU_SCIENCE)
                CloseMenu(window, camera);
            else
                OpenMenu(window, camera, MENU_SCIENCE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, 0);
            break;
        case sk_Log:
            if(GetOpenedMenuClass(window) == MENU_FLEET)
                CloseMenu(window, camera);
            else
                OpenMenu(window, camera, MENU_FLEET, MENU_SYSTEME_AUCUN);
            SetWindowSelection(window, 0);
            break;

        default:
            break;
    }
    
}

/**
 * @brief Update the starbases and do their actions
 * 
 * @param galaxy 
 * @param empireList 
 */
static void stations_ActionsUpdate(StarSystem **galaxy, EmpireList* empireList){
    int systemindex = 0;
    int corvetNumber, destroyerNumber, cruiserNumber, battleshipNumber;
    StationOrder stationOrder;
    int info1;
    int info2;
    int empireIndex;
    Station *station;

    // Check every station and check if a station has 
    for(systemindex = 0; systemindex < (GALAXY_WIDTH * GALAXY_WIDTH); systemindex++){
        if(starSystem_EmpireGet(galaxy[systemindex]) != NO_EMPIRE){
            station = starSystem_StationGet(galaxy[systemindex]);
            stationOrder = station_OrderGet(station);

            if(stationOrder){
                if(station_OrderProgressGet(station) > 0){ // If the order is not finished, we unincrement the progress
                    station_OrderProgressUnincrement(station);
                } else { // Else we do the order
                    info1 = station_OrderInfo1Get(station);
                    info2 = station_OrderInfo2Get(station);
                    empireIndex = starSystem_EmpireGet(galaxy[systemindex]);

                    switch(stationOrder){
                        case STATION_ORDER_UPGRADE:
                            station_LevelSet(station, (Stationlevel)(station_LevelGet(station) + 1));
                            break;
                        case STATION_ORDER_BUILD_MODULE:
                            station_ModuleSet(station, info1 - 1, (StationModule)info2);
                            break;
                        case STATION_ORDER_BUILD_SHIP:
                            corvetNumber = 0;
                            destroyerNumber = 0;
                            cruiserNumber = 0;
                            battleshipNumber = 0;
                            switch(info1){
                                case 1:
                                    info1 = FLOTTE_SCIENTIFIQUE;
                                    break;
                                case 2:
                                    info1 = FLOTTE_DE_CONSTRUCTION;
                                    break;
                                case 3:
                                    info1 = FLOTTE_MILITAIRE;
                                    corvetNumber = info2;
                                    break;
                                case 4:
                                    info1 = FLOTTE_MILITAIRE;
                                    destroyerNumber = info2;
                                    break;
                                case 5:
                                    info1 = FLOTTE_MILITAIRE;
                                    cruiserNumber = info2;
                                    break;
                                case 6:
                                    info1 = FLOTTE_MILITAIRE;
                                    battleshipNumber = info2;
                                    break;
                            }
                            fleet_New(  empire_FleetListGet(empire_Get(empireList, empireIndex)), 
                                        systemindex, 
                                        (FlotteType)info1, 
                                        corvetNumber, 
                                        destroyerNumber, 
                                        cruiserNumber, 
                                        battleshipNumber);
                            break;
                        default:
                            break;
                    }
                    station_OrderEnd(station);
                }
            }
        }
    }
}

/**
 * @brief Update the actions of planets
 * 
 * @param galaxy 
 */
static void planets_ActionsUpdate(StarSystem **galaxy){
    int systemindex, planetIndex;
    City *city;
    Planet *planet;
    OrdreConstruction order;
    // We loop through every system and planet
    for(systemindex = 0; systemindex < GALAXY_WIDTH * GALAXY_WIDTH; systemindex++){
        if(starSystem_EmpireGet(galaxy[systemindex]) != NO_EMPIRE){
            for(planetIndex = 0; planetIndex < starSystem_NumberOfPlanetGet(galaxy[systemindex]); planetIndex++){
                planet = starSystem_PlanetGet(galaxy[systemindex], planetIndex);
                city = planet_CityGet(planet);
                if(city){
                    order = city_OrderGet(city);
                    if(order){
                        if(city_OrderProgressGet(city) > 0){ // If the oder is not finished, we unincrement the progress
                            city_OrderProgressUnincrement(city);
                        } else if(city_OrderProgressGet(city)){ // Else the order is finished, so we take actions
                            switch(city_OrderGet(city)){
                                case CONSTRUIRE_DISTRICT_URBAIN:
                                    city_UrbanDistrictAdd(city, 1);
                                    break;
                                case CONSTRUIRE_DISTRICT_GENERATEUR:
                                    city_GeneratorDistrictAdd(city, 1);
                                    break;
                                case CONSTRUIRE_DISTRICT_MINIER:
                                    city_MiningDistrictAdd(city, 1);
                                    break;
                                case CONSTRUIRE_DISTRICT_AGRICOLE:
                                    city_AgricultureDistrictAdd(city, 1);
                                    break;
                                case CONSTRUIRE_BATIMENT:
                                    city_BuildingSet(   city, 
                                                        (Building)city_OrderInfo2Get(city), 
                                                        city_OrderInfo1Get(city), 
                                                        1);
                                    break;
                                default:
                                    break;
                            }
                            city_OrderEnd(city);
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Update the actions of every units
 * 
 * @param empireList 
 * @param galaxy 
 */
static void update_Word(EmpireList *empireList, StarSystem **galaxy){
    fleet_ActionsUpdate(galaxy, empireList);
    stations_ActionsUpdate(galaxy, empireList);
    planets_ActionsUpdate(galaxy);
    update_IntelLevel(galaxy, empireList);
}

/**
 * @brief Update the power of each empire
 * 
 * @param empireList 
 */
static void update_EmpirePower(EmpireList *empireList) {
    int empireIndex = 0;
    Empire *empire = empire_Get(empireList, empireIndex);
    while(empire) {
        empire_PowerUpdate(empire);
        empireIndex++;
        empire = empire_Get(empireList, empireIndex);
    }
}

/* entry points ======================================================== */

void update_PlayersData(char update, 
                        EmpireList *empireList, 
                        StarSystem **galaxy, 
                        NotificationList *notificationList){
    Empire *empire;
    int     empireIndex;
    int     empireArraySize;

    Fleet *fleet;
    FleetList *fleetList;
    int     fleetIndex;
    int     fleetArraySize;
    
    int     systemIndex;
    Planet *planet;
    int     planetIndex;
    int     planetArraySize;
    City   *city;
    int     buildingIndex;
    
    
    empireArraySize = empire_ArraySize(empireList);
    for(empireIndex = 0; empireIndex < empireArraySize; empireIndex++){
        empire = empire_Get(empireList, empireIndex);

        // Reinitialize the change
        empire_CreditVariationSet(empire, 0);
        empire_FoodVariationSet(empire, 0);
        empire_AlloysVariationSet(empire, 0);
        empire_MineralsVariationSet(empire, 0);
        empire_ConsumerVariationSet(empire, 0);

        fleetList = empire_FleetListGet(empire);
        fleetArraySize = FleetArraySize(fleetList);
        for(fleetIndex = 0; fleetIndex < fleetArraySize; fleetIndex++){
            fleet = FlotteNumero(fleetList, fleetIndex);
            empire_CreditVariationAdd(empire, -1);
        }
    }

    // Run through every planet
    for(systemIndex = 0; systemIndex < GALAXY_WIDTH * GALAXY_WIDTH; systemIndex++){
        if(starSystem_EmpireGet(galaxy[systemIndex]) != NO_EMPIRE){
            empire = empire_Get(empireList, starSystem_EmpireGet(galaxy[systemIndex]));
            planetArraySize = starSystem_NumberOfPlanetGet(galaxy[systemIndex]);

            for(planetIndex = 0; planetIndex < planetArraySize; planetIndex++){
                planet = starSystem_PlanetGet(galaxy[systemIndex], planetIndex);
                
                // If the planet if habited
                if((city = planet_CityGet(planet))){

                    empire_CreditVariationAdd(empire, -city_AgricultureDistrictGet(city));
                    empire_FoodVariationAdd(empire, city_AgricultureDistrictGet(city) * 3);

                    empire_CreditVariationAdd(empire, -city_MiningDistrictGet(city));
                    empire_MineralsVariationAdd(empire, 6 * city_MiningDistrictGet(city));
                    
                    empire_CreditVariationAdd(empire, -2 * city_UrbanDistrictGet(city));
                    
                    empire_CreditVariationAdd(empire, 8 * city_GeneratorDistrictGet(city));
                    
                    for(buildingIndex = 0; buildingIndex < 6; buildingIndex++){
                        switch(city_BuildingGet(city, buildingIndex)){
                            case BUILDING_CAPITAL:
                                empire_CreditVariationAdd(empire, -2);
                                break;
                            case BUILDING_FOUNDRIES:
                                empire_CreditVariationAdd(empire, -2);
                                empire_MineralsVariationAdd(empire, -6);
                                empire_AlloysVariationAdd(empire, 6);
                                break;
                            case BUILDING_RESEARCH:
                                empire_CreditVariationAdd(empire, -2);
                                break;
                            case BUILDING_CIVILIAN_INDUSTRIES:
                                empire_CreditVariationAdd(empire, -2);
                                empire_MineralsVariationAdd(empire, -6);
                                empire_AlloysVariationAdd(empire, 6);
                                break;
                            case BUILDING_THEATRE:
                                empire_CreditVariationAdd(empire, -2);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
    if(update){
        for(empireIndex = 0; empireIndex < empireArraySize; empireIndex++){
            empire = empire_Get(empireList, empireIndex);

            if(empire_AlloysGet(empire) + empire_AlloysVariationGet(empire) <= 0){
                empire_AlloysSet(empire, 0);
            }
            else
                empire_AlloysAdd(empire, empire_AlloysVariationGet(empire));
                
            if(empire_CreditGet(empire) + empire_CreditVariationGet(empire) <= 0){
                empire_CreditSet(empire, 0);
            }
            else 
                empire_CreditAdd(empire, empire_CreditVariationGet(empire));
            
            if(empire_FoodGet(empire) + empire_FoodVariationGet(empire) <= 0){
                empire_FoodSet(empire, 0);
            }
            else
                empire_FoodAdd(empire, empire_FoodVariationGet(empire));
            
            if(empire_MineralsGet(empire) + empire_MineralsVariationGet(empire) <= 0){
                empire_MineralsSet(empire, 0);
            }
            else
                empire_MineralsAdd(empire, empire_MineralsVariationGet(empire));
        }

        //teste si il n'y a plus beaucoup de ressources
        empire = empire_Get(empireList, 0);

        if((empire_AlloysGet(empire) + (empire_AlloysVariationGet(empire) / 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
        if((empire_CreditGet(empire) + (empire_CreditVariationGet(empire) / 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
        if((empire_FoodGet(empire) + (empire_FoodVariationGet(empire) / 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
        if((empire_MineralsGet(empire) + (empire_MineralsVariationGet(empire) * 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
    }
}

void update_IntelLevel( StarSystem **galaxy, 
                        const EmpireList *empireList){
    int sizeFleet = 0;
    int indexFleet = 0;
    int systemIndex = 0;

    // Reinitialize every intel level
    while(systemIndex < GALAXY_WIDTH * GALAXY_WIDTH) {
        if(starSystem_IntelLevelGet(galaxy[systemIndex])  >= INTEL_MEDIUM) {
            starSystem_IntelLevelSet(galaxy[systemIndex], INTEL_MEDIUM);
        } else {
            starSystem_IntelLevelSet(galaxy[systemIndex], INTEL_UNKNOWN);
        }
        systemIndex++;
    }

    // If a fleet is in the star system, we set the intel level to full
    sizeFleet = FleetArraySize(empire_FleetListGet(empire_Get(empireList, 0)));
    while(indexFleet < sizeFleet){
        starSystem_IntelLevelSet(galaxy[GetFleetSystem(FlotteNumero(empire_FleetListGet(empire_Get(empireList, 0)), indexFleet))], INTEL_HIGH);
        indexFleet++;
    }
    systemIndex = 0;
}

int game_Update( char *key, 
                EmpireList *empireList, 
                StarSystem **galaxy, 
                Time *time, 
                Camera *camera, 
                Window *window, 
                NotificationList *notificationList, 
                Settings *settings){
    // We test the keys
	update_KeysTest(key, 
                    galaxy, 
                    time, 
                    camera, 
                    window, 
                    settings);

    // We update the time structure
    time_Update(time);

    if(!time_TickGet(time)){
        if(((time_DayGet(time) == 10) || (time_DayGet(time) == 20)) || (time_DayGet(time) == 30)){
            update_Word(empireList, galaxy);
            update_EmpirePower(empireList);
        }
        if(time_DayGet(time) == 1){
            update_PlayersData(true, empireList, galaxy, notificationList);
        }
        
        ai_Empire(empireList, galaxy, time);
        
    }
	camera_Update(camera);
    return 1;
}