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

static void update_KeysTest(char *key, 
                            EmpireList *empireList, 
                            StarSystem **starSystem, 
                            Time *time, 
                            Camera *camera, 
                            Window *window, 
                            Settings *settings){
    // Camera keys binding
    if (GetCameraLockStatus(camera) != true){
        switch(*key){
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
            case sk_Clear:
                OpenMenu(window, camera, MENU_EXIT, MENU_SYSTEME_AUCUN);
                *key = 0;
                break;
            case sk_Mode:
                switch(GetCameraMapType(camera)){
                    case VUE_GALACTIC:
                        camera_ZoomSet(camera, camera_ZoomGet(camera) - 1);
                        break;
                    case VUE_SYSTEM:
                        SetCameraMapType(camera, VUE_GALACTIC);
                        break;
                    default:
                        break;
                }
                *key = 0;
                break;
            case sk_Del:
                switch(GetCameraMapType(camera)){
                    case VUE_GALACTIC:
                        if((starSystem_IntelLevelGet(starSystem[camera_SystemActualGet(camera)]) 
                            || settings_SeeAllGet(settings))
                            && camera_SystemActualGet(camera) != NO_SYSTEM
                            && camera_ZoomGet(camera) == 2){
                            SetCameraMapType(camera, VUE_SYSTEM);
                        }

                        camera_ZoomSet(camera, camera_ZoomGet(camera) + 1);
                        break;
                    case VUE_MAP:
                        SetCameraMapType(camera, VUE_GALACTIC);
                        break;
                    default:
                        break;
                }
                *key = 0;
                break;

            default:
                break;
        }
    }
    switch(*key){
        case sk_2nd:
            if(!GetCommandPromptStatus(window))
                OpenCommandPrompt(window, camera, time);
            else
                CloseCommandPrompt(window, camera, time);
            *key = 0;
            break;
        default:
            break;
    }
    
}

/**
 * calcule le niveau de connaissance (intel level) du systeme
 */
static void CalculerNiveauDeConnaissance(StarSystem **systemeStellaires, EmpireList *empireListe){
    int sizeFleet = 0;
    int indexFleet = 0;
    int numeroSysteme = 0;

    while(numeroSysteme < GALAXY_WIDTH * GALAXY_WIDTH) {
        if(starSystem_IntelLevelGet(systemeStellaires[numeroSysteme])  >= INTEL_MEDIUM) {
            starSystem_IntelLevelSet(systemeStellaires[numeroSysteme], INTEL_MEDIUM);
        }
        else {
            starSystem_IntelLevelSet(systemeStellaires[numeroSysteme], INTEL_UNKNOWN);
        }
        if((starSystem_StationLevelGet(systemeStellaires[numeroSysteme]) > INTEL_UNKNOWN) && (starSystem_EmpireGet(systemeStellaires[numeroSysteme]) == -1)) {
            starSystem_IntelLevelSet(systemeStellaires[numeroSysteme], INTEL_FULL);
        }
        numeroSysteme++;
    }
    sizeFleet = FleetArraySize(empire_FleetListGet(empire_Get(empireListe, 0)));
    while(indexFleet <= sizeFleet){
        starSystem_IntelLevelSet(systemeStellaires[GetFleetSystem(FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), indexFleet))], INTEL_HIGH);
        indexFleet++;
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
        for(j = 0; j < GetSystemPlanetNumber(systemeStellaires[i]); j++){
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

/**
 * Update time
 */
static void UpdateTime(Time *date, char *key, Window *fenetre){
    //différentes actions des touches
    if(!GetCommandPromptStatus(fenetre)){
        switch(*key){
            case sk_0://pause / dépause
                if(GetTimeSpeed(date) == 0){
                    UnpauseGame(date); 
                } else {
                    PauseGame(date);
                }
                break;
            case sk_Add:
                IncrementTimeSpeed(date);
                break;
            case sk_Sub:
                UnincrementTimeSpeed(date);
                break;
        }
    }
    UpdateClock(date);
}

static void UpdateWorld(EmpireList *empireListe, StarSystem **systemeStellaires){
    EffectuerActionsFlottes(empireListe, systemeStellaires);
    EffectuerActionsStations(systemeStellaires, empireListe);
    EffectuerActionsPlanetes(systemeStellaires);
    CalculerNiveauDeConnaissance(systemeStellaires, empireListe);
}

static void TestKey(char *key, EmpireList *empireListe, StarSystem **systemeStellaires, Time *date, Camera *camera, Window *fenetre, Settings *parametres){
    if(GetCameraMapType(camera) == VUE_GALACTIC){
        switch(*key){
        case sk_Up:
            if (GetCameraLockStatus(camera) != true)
                camera_YVectorAdd(camera, -5);
            break;
        case sk_Down:
            if (GetCameraLockStatus(camera) != true)
                camera_YVectorAdd(camera, +5);
            break;
        case sk_Left:
            if (GetCameraLockStatus(camera) != true)
                camera_XVectorAdd(camera, -5);
            break;
        case sk_Right:
            if (GetCameraLockStatus(camera) != true)
                camera_XVectorAdd(camera, +5);
            break;
        case sk_Mode:
            if (GetCameraLockStatus(camera) != true) {
                camera_ZoomSet(camera, camera_ZoomGet(camera) - 1);
                if(camera_ZoomGet(camera) >= 1) {
                    camera_XSet(camera, camera_XGet(camera) * 0.5);
                    camera_YSet(camera, camera_YGet(camera) * 0.5);
                }
            }
            break;
        case sk_Del:
            if ((camera_ZoomGet(camera) == 2) && (((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (camera_SystemActualGet(camera) != -1)) && ((IsCameraMoveFleet(camera) == false) && ((starSystem_IntelLevelGet(systemeStellaires[camera_SystemActualGet(camera)]) != INTEL_UNKNOWN) || settings_SeeAllGet(parametres))))) {
                SetCameraLock(camera, false);
                SetCameraMapType(camera, VUE_SYSTEM);
                if(camera_SystemAimedGet(camera) != camera_SystemActualGet(camera)) {
                    SetCameraXSystem(camera, 320);
                    SetCameraYSystem(camera, 240);
                }
                SetCameraSystem(camera, camera_SystemActualGet(camera));
                #ifdef DEBUG_VERSION
                    dbg_sprintf((char*)dbgout, "open system %d\n", camera_SystemActualGet(camera));
                #endif
                *key = 0;
            }
            if (GetCameraLockStatus(camera) != true) {
                camera_ZoomSet(camera, camera_ZoomGet(camera) + 1);
                if (camera_ZoomGet(camera) < 3 && camera_ZoomGet(camera) >= 0) {
                    camera_XSet(camera, camera_XGet(camera) * 2);
                    camera_YSet(camera, camera_YGet(camera) * 2);
                }
            }
            break;
        case sk_Enter:
            if (((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (camera_SystemActualGet(camera) != -1)) && ((IsCameraMoveFleet(camera) == false) && (starSystem_IntelLevelGet(systemeStellaires[camera_SystemActualGet(camera)]) != INTEL_UNKNOWN || settings_SeeAllGet(parametres)))){
                SetCameraLock(camera, false);
                SetCameraMapType(camera, VUE_SYSTEM);
                if(camera_SystemAimedGet(camera) != camera_SystemActualGet(camera)) {
                    SetCameraXSystem(camera, 320);
                    SetCameraYSystem(camera, 240);
                }
                SetCameraSystem(camera, camera_SystemActualGet(camera));
                #ifdef DEBUG_VERSION
                    dbg_sprintf((char*)dbgout, "open system %d\n", camera_SystemActualGet(camera));
                #endif
                *key = 0;
            } else if(((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (camera_SystemActualGet(camera) != -1)) && (IsCameraMoveFleet(camera) == true)) {
                BougerFlotte(GetCameraFleet(camera), GetCameraEmpire(camera), camera_SystemActualGet(camera), fenetre, camera, empireListe, systemeStellaires);
                *key = 0;
            }
            break;
        }
    }

    //touches generales

    switch(*key){
    case sk_Clear:
        if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (IsCameraMoveFleet(camera) == false)) {
            OpenMenu(fenetre, camera, MENU_EXIT, MENU_SYSTEME_AUCUN);
            PauseGame(date);
            *key = 0;
        } else if(IsCameraMoveFleet(camera) == true) {
            SetCameraMoveFleet(camera, false);
            *key = 0;
        }
        break;
    case sk_Yequ :
        if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 1)) {
            OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 1);
        } else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 1)) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Window :
        if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 2)) {
            OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 2);
        } else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 2)) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Zoom :
        if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 3)) {
            OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 3);
        } else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 3)) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Trace :
        if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 4)) {
            OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 4);
        } else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 4)) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Graph :
        if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 5)) {
            OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 5);
        } else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 5)) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Recip:
        if ((GetOpenedMenuClass(fenetre) != MENU_MARCHE) && (GetOpenedMenuClass(fenetre) != MENU_EXIT)) {
            OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 2);
        } else if (GetOpenedMenuClass(fenetre) == MENU_MARCHE) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Math:
        if ((GetOpenedMenuClass(fenetre) != MENU_CONTACTS) && (GetOpenedMenuClass(fenetre) != MENU_EXIT)) {
            OpenMenu(fenetre, camera, MENU_CONTACTS, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 1);
        } else if (GetOpenedMenuClass(fenetre) == MENU_CONTACTS) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Square:
        if ((GetOpenedMenuClass(fenetre) != MENU_RECHERCHE) && (GetOpenedMenuClass(fenetre) != MENU_EXIT)) {
            OpenMenu(fenetre, camera, MENU_RECHERCHE, MENU_SYSTEME_AUCUN);
        } else if (GetOpenedMenuClass(fenetre) == MENU_RECHERCHE) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_Log:
        if ((GetOpenedMenuClass(fenetre) != MENU_FLOTTE) && (GetOpenedMenuClass(fenetre) != MENU_EXIT)) {
            OpenMenu(fenetre, camera, MENU_FLOTTE, MENU_SYSTEME_AUCUN);
            SetWindowSelection(fenetre, 0);
        } else if (GetOpenedMenuClass(fenetre) == MENU_FLOTTE) {
            CloseMenu(fenetre, camera);
        }
        break;
    case sk_2nd:
        if(!GetCommandPromptStatus(fenetre))
            OpenCommandPrompt(fenetre, camera, date);
        else
            CloseCommandPrompt(fenetre, camera, date);
        *key = 0;
        break;
    }
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
    int planetaryBuildingIndex = 0;
    
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
            planetaryArraySize = GetSystemPlanetNumber(systemeStellaires[systemIndex]);

            for(planetaryIndex = 0; planetaryIndex < planetaryArraySize; planetaryIndex++){
                planete = GetPlanet(systemeStellaires[systemIndex], planetaryIndex);
                
                //si la planete est habitée
                if(planet_CityGet(planete)){

                    AddEmpireCreditChange(empire, -GetPlanetCityAgricultureDistrictNumber(planete));
                    AddEmpireFoodChange(empire, GetPlanetCityAgricultureDistrictNumber(planete) * 3);

                    AddEmpireCreditChange(empire, -GetPlanetCityMiningDistrictNumber(planete));
                    AddEmpireMineralsChange(empire, 6 * GetPlanetCityMiningDistrictNumber(planete));
                    
                    AddEmpireCreditChange(empire, -2 * GetPlanetCityUrbanDistrictNumber(planete));
                    
                    AddEmpireCreditChange(empire, 8 * GetPlanetCityGeneratorDistrictNumber(planete));
                    
                    for(planetaryBuildingIndex = 1; planetaryBuildingIndex <= 6; planetaryBuildingIndex++){
                        switch(GetPlanetCityBuildingNumber(planete, planetaryBuildingIndex)){
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
                    empireList, 
                    starSystem, 
                    time, 
                    camera, 
                    window, 
                    settings);

    UpdateTime(time, key, window);
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
