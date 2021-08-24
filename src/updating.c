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
 * @param starSystem 
 * @param time 
 * @param camera 
 * @param window 
 * @param settings 
 */
static void update_KeysTest(char *key, 
                            StarSystem **starSystem, 
                            Time *time, 
                            Camera *camera, 
                            Window *window, 
                            Settings *settings){
    // Camera keys binding
    if (GetCameraLockStatus(camera) != true){
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
                        if((starSystem_IntelLevelGet(starSystem[camera_SystemActualGet(camera)]) 
                        || settings_SeeAllGet(settings))
                        && camera_SystemActualGet(camera) != NO_SYSTEM
                        && camera_ZoomGet(camera) == ZOOM_MAX){
                            camera_MapTypeSet(camera, VUE_SYSTEM);
                            SetCameraSystem(camera, camera_SystemActualGet(camera));
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
                if((starSystem_IntelLevelGet(starSystem[camera_SystemActualGet(camera)]) 
                || settings_SeeAllGet(settings))
                && camera_SystemActualGet(camera) != NO_SYSTEM){
                    camera_MapTypeSet(camera, VUE_SYSTEM);
                    SetCameraSystem(camera, camera_SystemActualGet(camera));
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
 * Effectue les actions de station
 */
void EffectuerActionsStations(StarSystem **systemeStellaires, EmpireList* empireListe){
    int numero = 0, nombreDeVaisseaux = 0;
    int nombreDeCorvette = 0, nombreDeDestroyer = 0, nombreDeCroiseur = 0, nombreDeCuirasse = 0;
    OrdreStation ordre = AUCUN_ORDRE_STATION;
    int info1 = 0;
    int info2 = 0;
    int numeroEmpire = 0;
    // Empire *joueur = EmpireNumero(empireListe, 0);
    while(numero < (GALAXY_WIDTH * GALAXY_WIDTH) - 1){
        ordre = starSystem_StationOrderGet(systemeStellaires[numero]);
        if(ordre != AUCUN_ORDRE_STATION){
            if(starSystem_StationOrderProgressGet(systemeStellaires[numero]) > 1){
                starSystem_StationOrderProgressIncrement(systemeStellaires[numero]);
            }
            else{
                info1 = starSystem_StationInfo1Get(systemeStellaires[numero]);
                info2 = starSystem_StationInfo2Get(systemeStellaires[numero]);
                numeroEmpire = starSystem_EmpireGet(systemeStellaires[numero]);
                switch(ordre){
                case AMELIORER_STATION:
                    starSystem_StationLevelSet(systemeStellaires[numero], (Stationlevel)(starSystem_StationLevelGet(systemeStellaires[numero]) + 1));
                    break;
                case CONSTRUIRE_MODULE:
                    starSystem_StationModuleSet(systemeStellaires[numero], info1 - 1, (Module)info2);
                    break;
                case CONSTRUIRE_VAISSEAU:
                    nombreDeVaisseaux = info2;
                    switch(info1){
                        case 1:
                            info1 = FLOTTE_SCIENTIFIQUE;
                            nombreDeVaisseaux = 1;
                            break;
                        case 2:
                            info1 = FLOTTE_DE_CONSTRUCTION;
                            nombreDeVaisseaux = 1;
                            break;
                        case 3:
                            info1 = FLOTTE_MILITAIRE;
                            nombreDeCorvette = nombreDeVaisseaux;
                            break;
                        case 4:
                            info1 = FLOTTE_MILITAIRE;
                            nombreDeDestroyer = nombreDeVaisseaux;
                            break;
                        case 5:
                            info1 = FLOTTE_MILITAIRE;
                            nombreDeCroiseur = nombreDeVaisseaux;
                            break;
                        case 6:
                            info1 = FLOTTE_MILITAIRE;
                            nombreDeCuirasse = nombreDeVaisseaux;
                            break;
                    }
                    NouvelleFlotte(empire_FleetListGet(empire_Get(empireListe, numeroEmpire)), numero, (FlotteType)info1, nombreDeCorvette, nombreDeDestroyer, nombreDeCroiseur, nombreDeCuirasse);
                    break;
                default:
                    break;
                }
                starSystem_StationOrderEnd(systemeStellaires[numero]);
            }
        }
        numero++;
    }
}


/**
 * Effectue les actions des planetes
 */
void EffectuerActionsPlanetes(StarSystem **systemeStellaires){
    int i = 0, j = 0;
    City *villes = NULL;
    // Batiment *batiment = NULL;
    OrdreConstruction ordre;
    for(i = 0; i < GALAXY_WIDTH * GALAXY_WIDTH; i++){
        for(j = 0; j < starSystem_NumberOfPlanetGet(systemeStellaires[i]); j++){
            villes = GetSystemPlanetCity(systemeStellaires[i], j);
            if(villes != NULL){
                ordre = GetCityOrder(villes);
                if(ordre != 0){
                    if(GetCityOrderProgress(villes) > 1){
                        UnincrementCityOrderProgress(villes);
                    }
                    else if(GetCityOrderProgress(villes) == 1){
                        switch(GetCityOrder(villes)){
                            case CONSTRUIRE_DISTRICT_URBAIN:
                                AddSystemPlanetCityUrbanDistrict(systemeStellaires[i], j, 1);
                                break;
                            case CONSTRUIRE_DISTRICT_GENERATEUR:
                                AddSystemPlanetCityGeneratorDistrict(systemeStellaires[i], j, 1);
                                break;
                            case CONSTRUIRE_DISTRICT_MINIER:
                                AddSystemPlanetCityMiningDistrict(systemeStellaires[i], j, 1); 
                                break;
                            case CONSTRUIRE_DISTRICT_AGRICOLE:
                                AddSystemPlanetCityAgricultureDistrict(systemeStellaires[i], j, 1); 
                                break;
                            case CONSTRUIRE_BATIMENT:
                                SetSystemPlanetCityBuilding(systemeStellaires[i], j, GetCityOrderInfo1(villes), (Building)GetCityOrderInfo2(villes), 1 );
                                break;
                            default:
                                break;
                        }
                        EndCityOrder(villes);
                    }
                }
            }
        }
    }
}

static void UpdateWorld(EmpireList *empireListe, StarSystem **systemeStellaires){
    EffectuerActionsFlottes(empireListe, systemeStellaires);
    EffectuerActionsStations(systemeStellaires, empireListe);
    EffectuerActionsPlanetes(systemeStellaires);
    update_IntelLevel(systemeStellaires, empireListe);
}



static void UpdateEmpirePower(EmpireList *empireListe) {
    Empire *empire;
    int empireNumero = 0;
        empire = empire_Get(empireListe, empireNumero);
    while (empire != NULL) {
        CalculateEmpirePower(empire);
        empireNumero++;
        empire = empire_Get(empireListe, empireNumero);
    }
}

/* entry points ======================================================== */

void UpdatePlayersData(char appliquer, EmpireList *empireListe, StarSystem **systemeStellaires, NotificationList *notificationList){
    Empire *empire = NULL;
    Flotte *flotte = NULL;
    FlotteListe *flotteListe = NULL;
    Planet *planete = NULL;
    int empireIndex = 0;
    int empireArraySize = 0;
    int flotteIndex = 0;
    int flotteArraySize = 0;
    int systemIndex = 0;
    int planetaryIndex = 0;
    int planetaryArraySize = 0;
    int buildingIndex = 0;
    City *city;
    
    //retirer argent flottes
    empireArraySize = empire_ArraySize(empireListe);
    for(empireIndex = 0; empireIndex < empireArraySize; empireIndex++){
        empire = empire_Get(empireListe, empireIndex);

        //reinitialise tout les changements
        SetEmpireCreditChange(empire, 0);
        SetEmpireFoodChange(empire, 0);
        SetEmpireAlloysChange(empire, 0);
        SetEmpireMineralsChange(empire, 0);
        SetEmpireConsumerGoodsChange(empire, 0);

        flotteListe = empire_FleetListGet(empire);
        flotteArraySize = FleetArraySize(flotteListe);
        for(flotteIndex = 0; flotteIndex < flotteArraySize; flotteIndex++){
            flotte = FlotteNumero(flotteListe, flotteIndex);
            AddEmpireCreditChange(empire, -1);
        }
    }

    //retirer et ajouter argent systemes et planetes
    for(systemIndex = 0; systemIndex < GALAXY_WIDTH * GALAXY_WIDTH; systemIndex++){
        if(starSystem_EmpireGet(systemeStellaires[systemIndex]) != -1){
            empire = empire_Get(empireListe, starSystem_EmpireGet(systemeStellaires[systemIndex]));
            planetaryArraySize = starSystem_NumberOfPlanetGet(systemeStellaires[systemIndex]);

            for(planetaryIndex = 0; planetaryIndex < planetaryArraySize; planetaryIndex++){
                planete = starSystem_PlanetGet(systemeStellaires[systemIndex], planetaryIndex);
                
                //si la planete est habitée
                if((city = planet_CityGet(planete))){

                    AddEmpireCreditChange(empire, -city_AgricultureDistrictGet(city));
                    AddEmpireFoodChange(empire, city_AgricultureDistrictGet(city) * 3);

                    AddEmpireCreditChange(empire, -city_MiningDistrictGet(city));
                    AddEmpireMineralsChange(empire, 6 * city_MiningDistrictGet(city));
                    
                    AddEmpireCreditChange(empire, -2 * city_UrbanDistrictGet(city));
                    
                    AddEmpireCreditChange(empire, 8 * city_GeneratorDistrictGet(city));
                    
                    for(buildingIndex = 1; buildingIndex <= 6; buildingIndex++){
                        switch(city_BuildingGet(city, buildingIndex)){
                            case BUILDING_CAPITAL:
                                AddEmpireCreditChange(empire, -2);
                                break;
                            case BUILDING_FOUNDRIES:
                                AddEmpireCreditChange(empire, -2);
                                AddEmpireMineralsChange(empire, -6);
                                AddEmpireAlloysChange(empire, 6);
                                break;
                            case BUILDING_RESEARCH:
                                AddEmpireCreditChange(empire, -2);
                                break;
                            case BUILDING_CIVILIAN_INDUSTRIES:
                                AddEmpireCreditChange(empire, -2);
                                AddEmpireMineralsChange(empire, -6);
                                AddEmpireAlloysChange(empire, 6);
                                break;
                            case BUILDING_THEATRE:
                                AddEmpireCreditChange(empire, -2);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
    if(appliquer){
        for(empireIndex = 0; empireIndex < empireArraySize; empireIndex++){
            empire = empire_Get(empireListe, empireIndex);

            if(GetEmpireAlloys(empire) + GetEmpireAlloysChange(empire) <= 0){
                SetEmpireAlloys(empire, 0);
            }
            else
                AddEmpireAlloys(empire, GetEmpireAlloysChange(empire));
                
            if(GetEmpireCredit(empire) + GetEmpireCreditChange(empire) <= 0){
                SetEmpireCredit(empire, 0);
            }
            else 
                AddEmpireCredit(empire, GetEmpireCreditChange(empire));
            
            if(GetEmpireFood(empire) + GetEmpireFoodChange(empire) <= 0){
                SetEmpireFood(empire, 0);
            }
            else
                AddEmpireFood(empire, GetEmpireFoodChange(empire));
            
            if(GetEmpireMinerals(empire) + GetEmpireMineralsChange(empire) <= 0){
                SetEmpireMinerals(empire, 0);
            }
            else
                AddEmpireMinerals(empire, GetEmpireMineralsChange(empire));
        }

        //teste si il n'y a plus beaucoup de ressources
        empire = empire_Get(empireListe, 0);

        if((GetEmpireAlloys(empire) + (GetEmpireAlloysChange(empire) / 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
        if((GetEmpireCredit(empire) + (GetEmpireCreditChange(empire) / 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
        if((GetEmpireFood(empire) + (GetEmpireFoodChange(empire) / 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
        if((GetEmpireMinerals(empire) + (GetEmpireMineralsChange(empire) * 12)) <= 0) {
            NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
        }
    }
}

int game_Update( char *key, 
                EmpireList *empireList, 
                StarSystem **starSystem, 
                Time *time, 
                Camera *camera, 
                Window *window, 
                NotificationList *notificationList, 
                Settings *settings){
    // We test the keys
	update_KeysTest(key, 
                    starSystem, 
                    time, 
                    camera, 
                    window, 
                    settings);

    time_Update(time);

    if(GetTimeClock(time) == 0){
        if(((GetTimeDay(time) == 10) || (GetTimeDay(time) == 20)) || (GetTimeDay(time) == 30)){
            UpdateWorld(empireList, starSystem);
            UpdateEmpirePower(empireList);
        }
        if(GetTimeDay(time) == 1){
            UpdatePlayersData(true, empireList, starSystem, notificationList);
        }
        
        EmpireAI(empireList, starSystem, time);
        
    }
    return 1;
}

void update_IntelLevel(StarSystem **starSystem, EmpireList *empireList){
    int sizeFleet = 0;
    int indexFleet = 0;
    int systemIndex = 0;

    while(systemIndex < GALAXY_WIDTH * GALAXY_WIDTH) {
        if(starSystem_IntelLevelGet(starSystem[systemIndex])  >= INTEL_MEDIUM) {
            starSystem_IntelLevelSet(starSystem[systemIndex], INTEL_MEDIUM);
        }
        else {
            starSystem_IntelLevelSet(starSystem[systemIndex], INTEL_UNKNOWN);
        }
        if((starSystem_StationLevelGet(starSystem[systemIndex]) > INTEL_UNKNOWN) && (starSystem_EmpireGet(starSystem[systemIndex]) == NO_EMPIRE)) {
            starSystem_IntelLevelSet(starSystem[systemIndex], INTEL_FULL);
        }
        systemIndex++;
    }
    sizeFleet = FleetArraySize(empire_FleetListGet(empire_Get(empireList, 0)));
    dbg_sprintf(dbgout, "sizeFleet : %d\n", sizeFleet);
    while(indexFleet < sizeFleet){
        starSystem_IntelLevelSet(starSystem[GetFleetSystem(FlotteNumero(empire_FleetListGet(empire_Get(empireList, 0)), indexFleet))], INTEL_HIGH);
        indexFleet++;
    }
}