/**
 * @file map.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief All the map-drawing procedures
 * @version 0.1
 * @date 2021-12-28
 * 
 * @copyright GNU General Public License v3.0
 * 
 */
#include <string.h>

#include <debug.h>
#include <math.h>

#include <graphx.h>

#include "gfx/gfx.h"
#include "colors.h"

#include "main.h"

#include "map.h"
#include "galaxy.h"
#include "hud.h"

/* private functions =================================================== */

static void map_StarColorGet(int type);


static void map_DrawGalacticHabitable(  StarSystem *system, 
                                        Settings *settings, 
                                        int x, 
                                        int y){
    char habitable = 0, inhabit = 0;
    int planetIndex = 0;
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetColor(COLOR_STARSHIP_US_INTERIOR);

    // We count the number of haibtable planet and inhabited planet
    while(planetIndex < PLANET_MAX){
        if(planet_HabitabilityGet(starSystem_PlanetGet(system, planetIndex))) {
            habitable++;
            if(city_PopulationGet(planet_CityGet(starSystem_PlanetGet(system, planetIndex)))){
                inhabit++;
            }
        }
        planetIndex++;
    }

    // We draw it
    if(inhabit){
        gfx_FillRectangle(x - strlen(starSystem_NameGet(system)) * 4, y + 8, strlen(starSystem_NameGet(system)) * 8, 10);
        gfx_PrintStringXY(starSystem_NameGet(system), x - (strlen(starSystem_NameGet(system)) * 4), y + 9);
    } else {
        if(starSystem_IntelLevelGet(system) >= INTEL_LOW || settings_SeeAllGet(settings)) {
            if(starSystem_IntelLevelGet(system) >= INTEL_HIGH) {
                gfx_SetTextFGColor(COLOR_WHITE);
            } else {
                gfx_SetTextFGColor(COLOR_GRAY);
            }
            gfx_PrintStringXY(starSystem_NameGet(system), x - (strlen(starSystem_NameGet(system)) * 4), y + 9);
        }
    }
    if(habitable) {
        gfx_TransparentSprite(icon_planet_green, x - 10, y);
    }
}

/**
 *dessine les flottes sur la map
 */
static void DessinerFlottesMap(EmpireList* empireListe, Empire* joueur, StarSystem **systemeStellaires, Camera* camera, Settings *parametres){
    Fleet* flotte;
    Empire *empire;
    int system, xFlotte, yFlotte;
    int index = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int empireIndex = 1;
    int fleetIndex = 0;
    int size = 0;
    int empireSize = 0;
    //dessiner flottes joueur
    flotte = FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), 0);
    size = FleetArraySize(empire_FleetListGet(joueur));
    while(fleetIndex <= size) {
        system = GetFleetSystem(flotte);
        xFlotte = starSystem_XGet(systemeStellaires[system]) * camera_ZoomGet(camera) - camera_XGet(camera) + 165;
        yFlotte = starSystem_YGet(systemeStellaires[system]) * camera_ZoomGet(camera) - camera_YGet(camera) + 110;
        if(((xFlotte > 0) && (xFlotte < 320)) && ((0 < yFlotte) && (yFlotte < 240))){
            while(gfx_GetPixel(xFlotte + 4, yFlotte + 1) != 0){
                xFlotte += 11;
            }
            gfx_TransparentSprite(icon_fleet_our, xFlotte, yFlotte);
            switch(GetFleetType(flotte)){
                case FLEET_MILITARY:
                    gfx_TransparentSprite(icon_ship_military_our, xFlotte + 6, yFlotte - 1);
                    if (GetFleetPower(flotte) > 500)
                    {
                        gfx_TransparentSprite(icon_ship_military_our, xFlotte + 10, yFlotte - 1);
                    }
                    if (GetFleetPower(flotte) > 1500)
                    {
                        gfx_TransparentSprite(icon_ship_military_our, xFlotte + 8, yFlotte - 4);
                    }
                    break;
                case FLEET_CONSTRUCTION:
                    gfx_TransparentSprite(icon_ship_construction_our, xFlotte + 6, yFlotte - 4);
                    break;
                case FLEET_SCIENTIFIC:
                    gfx_TransparentSprite(icon_ship_science_our, xFlotte + 6, yFlotte - 4);
                    break;
            }
        }
        //dessiner chemin
        index = GetFleetPathProgress(flotte) - 1;
        gfx_SetColor(13);
        while((GetFleetPath(flotte, index) != 0) && (GetFleetPath(flotte, index + 1) != 0)){
            x1 = starSystem_XGet(systemeStellaires[GetFleetPath(flotte, index)]) * camera_ZoomGet(camera) - camera_XGet(camera) + 160;
            y1 = starSystem_YGet(systemeStellaires[GetFleetPath(flotte, index)]) * camera_ZoomGet(camera) - camera_YGet(camera) + 120;
            x2 = starSystem_XGet(systemeStellaires[GetFleetPath(flotte, index + 1)]) * camera_ZoomGet(camera) - camera_XGet(camera) + 160;
            y2 = starSystem_YGet(systemeStellaires[GetFleetPath(flotte, index + 1)]) * camera_ZoomGet(camera) - camera_YGet(camera) + 120;
            if((((x1 > 0) && (x1 < 320)) && ((0 < y1) && (y1 < 240))) && (((x2 > 0) && (x2 < 320)) && ((0 < y2) && (y2 < 240)))){
                gfx_Line_NoClip(x1, y1, x2, y2);
            }
            index++;
        }
        fleetIndex++;
        flotte = FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), fleetIndex);
    }
    empire = empire_Get(empireListe, 1);
    empireIndex = 1;
    empireSize = empire_ArraySize(empireListe);
    while(empireIndex <= empireSize){
        fleetIndex = 0;
        flotte = FlotteNumero(empire_FleetListGet(empire), 0);
        size = FleetArraySize(empire_FleetListGet(empire));
        while(fleetIndex <= size) {
            system = GetFleetSystem(flotte);
            if((starSystem_IntelLevelGet(systemeStellaires[system]) >= INTEL_MEDIUM) || settings_SeeAllGet(parametres)){
                xFlotte = starSystem_XGet(systemeStellaires[system]) * camera_ZoomGet(camera) - camera_XGet(camera) + 165;
                yFlotte = starSystem_YGet(systemeStellaires[system]) * camera_ZoomGet(camera) - camera_YGet(camera) + 110;
                if(((xFlotte > 0) && (xFlotte < 320)) && ((0 < yFlotte) && (yFlotte < 240))){
                    while(gfx_GetPixel(xFlotte + 4, yFlotte + 1) != 0){
                        xFlotte += 11;
                    }
                    gfx_TransparentSprite(icon_fleet_neutral, xFlotte, yFlotte);
                    switch(GetFleetType(flotte)){
                    case FLEET_MILITARY:
                        gfx_TransparentSprite(icon_ship_military_neutral, xFlotte + 6, yFlotte - 1);
                        if (GetFleetPower(flotte) > 500){
                            gfx_TransparentSprite(icon_ship_military_neutral, xFlotte + 10, yFlotte - 1);
                        }
                        if (GetFleetPower(flotte) > 1500){
                            gfx_TransparentSprite(icon_ship_military_neutral, xFlotte + 8, yFlotte - 4);
                        }
                        break;
                    case FLEET_CONSTRUCTION:
                        gfx_TransparentSprite(icon_ship_construction_neutral, xFlotte + 6, yFlotte - 4);
                        break;
                    case FLEET_SCIENTIFIC:
                        gfx_TransparentSprite(icon_ship_science_neutral, xFlotte + 6, yFlotte - 4);
                        break;
                    }
                }
            }
            fleetIndex++;
            flotte = FlotteNumero(empire_FleetListGet(empire), fleetIndex);
        }
        empireIndex++;
        empire = empire_Get(empireListe, empireIndex);
    }
}

/**
 *Dessine une hyperlane
 */
static void map_DrawOneHyperlane(   StarSystem *origin , 
                                    StarSystem *destination, 
                                    int x, 
                                    int y, 
                                    Camera* camera){
    int xLn, yLn;
    xLn = starSystem_XGet(destination);
    yLn = starSystem_YGet(destination);
    if(!starSystem_IntelLevelGet(origin) || !starSystem_IntelLevelGet(destination)) {
        gfx_SetColor(COLOR_HYPERLANES_UNKNOWN);
    } else {
        gfx_SetColor(COLOR_HYPERLANES_KNOWN);
    }
    
    xLn = xLn * camera_ZoomGet(camera) - camera_XGet(camera) + 160;
    yLn = yLn * camera_ZoomGet(camera) - camera_YGet(camera) + 120;

    if ((0 <= x) && (x <= SCREEN_WIDTH) && (HUD_WIDTH <= y) && (y <= SCREEN_HEIGHT) && (0 <= xLn) && (xLn <= SCREEN_WIDTH) && (HUD_WIDTH <= yLn) && (yLn <= SCREEN_HEIGHT)) {
        gfx_Line_NoClip(x, y, xLn, yLn);
    }
}

/**
 *dessiner la map en vision normale et galactique
 */
static void DessinerVueMap(StarSystem **systemeStellaires, Camera *camera, EmpireList *empireListe, Settings *parametres){
    int i = 0, x = 0, y = 0;
    //dessiner carte
    gfx_SetTextConfig(gfx_text_clip);
    camera_SystemAimedSet(camera, -1);
    while (i < (GALAXY_WIDTH * GALAXY_WIDTH)) {
        if(camera_MapTypeGet(camera) == VUE_GALAXY_LARGE){
            x = starSystem_XGet(systemeStellaires[i]) * camera_ZoomGet(camera) - camera_XGet(camera) + 160;
            y = starSystem_YGet(systemeStellaires[i]) * camera_ZoomGet(camera) - camera_YGet(camera) + 120;
        } else if(camera_MapTypeGet(camera) == VUE_GALAXY_NORMAL) {
            x = starSystem_XGet(systemeStellaires[i]) / 2.5 + 10;
            y = starSystem_YGet(systemeStellaires[i]) / 2.5 - 30;
        }
        if ( (((0 <= x) && (x <= 320)) && ((0 <= y)&& (y <= 240))) && ((starSystem_XGet(systemeStellaires[i]) != 0) && (starSystem_YGet(systemeStellaires[i]) != 0)) ) {
            if((starSystem_IntelLevelGet(systemeStellaires[i]) != INTEL_UNKNOWN) || settings_SeeAllGet(parametres)) {
                if(starSystem_EmpireGet(systemeStellaires[i]) != -1){
                    gfx_SetColor(empire_ColorGet(empire_Get(empireListe, starSystem_EmpireGet(systemeStellaires[i]))));
                    if(camera_MapTypeGet(camera) == VUE_GALAXY_LARGE){
                        gfx_Circle(x, y, 20 * camera_ZoomGet(camera));
                        gfx_Circle(x, y, (19 * camera_ZoomGet(camera)) + 1);
                    } else {
                        gfx_Circle(x, y, 10);
                        gfx_Circle(x, y, 9);
                    }
                }
                
                if(camera_MapTypeGet(camera) == VUE_GALAXY_LARGE){
                }
            }
            
            if(camera_MapTypeGet(camera) == VUE_GALAXY_LARGE){
            } else {
                map_StarColorGet(starSystem_StarTypeGet(systemeStellaires[i]));
                gfx_SetPixel(x, y);
            }
        }

        //Le if suivant teste si la systeme est celui selectionné
        if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y))) {
            gfx_SetColor(9);
            gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
            camera_SystemAimedSet(camera, i);
        }
        i++;
    }
    gfx_SetTextConfig(gfx_text_noclip);
}


/**
 * Dessine les fleches des hyperlane en vue systeme
 * */
static void DessinerHyperlanesSysteme(StarSystem **systemeStellaires, Camera *camera){
    StarSystem *systeme;
    int hyperlane;
    int x, y;
    int index;

    systeme = systemeStellaires[camera_SystemAimedGet(camera)];
    
    gfx_SetColor(COLOR_HYPERLANES_UNKNOWN);
    index = 0;
    // verifie que les hyperlane existent
    while(index < 4){
        hyperlane = hyperlane_DestinationGet(systeme, index);
        if(hyperlane != 255){
            x = hyperlane_XGet(systeme, index) - camera_XSystemGet(camera);
            y = hyperlane_YGet(systeme, index) - camera_YSystemGet(camera);

            if(((0 < x) && (x < 310)) && ((0 < y) && (y < 230))){
                gfx_Rectangle_NoClip(x, y, 10, 10);
            }
        }
        index++;
    }
}

static void map_StarColorGet(int type){	
    switch(type) {
        case STAR_TYPE_B: // B
            gfx_SetColor(COLOR_STAR_B);
            break;
        case STAR_TYPE_A: // A
            gfx_SetColor(COLOR_STAR_A);
            break;
        case STAR_TYPE_F: // F
            gfx_SetColor(COLOR_STAR_F);
            break;
        case STAR_TYPE_G: // G
            gfx_SetColor(COLOR_STAR_G);
            break;
        case STAR_TYPE_K: // K
            gfx_SetColor(COLOR_STAR_K);
            break;
        case STAR_TYPE_M: // M
            gfx_SetColor(COLOR_STAR_M);
            break;
        case STAR_TYPE_BLACKHOLE: // trou noir
            gfx_SetColor(COLOR_STAR_BLACK_HOLE);
            break;
        case STAR_TYPE_PULSAR: // pulsar
            gfx_SetColor(COLOR_STAR_PULSAR);
            break;
        case STAR_TYPE_NEUTRON: // étoile a neutrons
            gfx_SetColor(COLOR_STAR_NEUTRINOS);
            break;
        default:
            gfx_SetColor(COLOR_STAR_DEFAULT);
            break;
    }
}

/**
 *Dessine l'étoile en vue systeme
 */
static void DessinerEtoile(StarSystem *systeme, Camera* camera, WindowManager *windowManager, char* key){
    int xEtoile = SYSTEM_SPECIAL_X - camera_XSystemGet(camera), yEtoile = SYSTEM_SPECIAL_Y - camera_YSystemGet(camera), rayon = 0;
    switch(starSystem_StarTypeGet(systeme)){
        case STAR_TYPE_B:
            rayon = 6;
            break;
        case STAR_TYPE_A:
            rayon = 5;
            break;
        case STAR_TYPE_F:
            rayon = 4;
            break;
        case STAR_TYPE_G:
            rayon = 3;
            break;
        case STAR_TYPE_K:
            rayon = 2;
            break;
        case STAR_TYPE_M:
            rayon = 2;
            break;
        case STAR_TYPE_BLACKHOLE:
            rayon = 2;
            break;
        case STAR_TYPE_PULSAR:
            rayon = 3;
            break;
        case STAR_TYPE_NEUTRON:
            rayon = 4;
            break;
        default:
            rayon = 3;
            break;
    }
    if(starSystem_StarTypeGet(systeme) < 6){
        map_StarColorGet(starSystem_StarTypeGet(systeme));
        gfx_FillCircle(xEtoile, yEtoile, rayon);
    }
    else{
        map_StarColorGet(starSystem_StarTypeGet(systeme));
        switch(starSystem_StarTypeGet(systeme)){
            case STAR_TYPE_BLACKHOLE: //trou noir
                gfx_Circle(xEtoile, yEtoile, 4);
                break;
            case STAR_TYPE_PULSAR: //pulsar
                gfx_FillCircle(xEtoile, yEtoile, 1);
                break;
            case STAR_TYPE_NEUTRON: ///toile a neutrons
                gfx_FillCircle(xEtoile, yEtoile, 1);
                gfx_Circle(xEtoile, yEtoile, 8);
                break;
            default:
                gfx_Rectangle(xEtoile, yEtoile, 4, 4);
        }
    }
    if(((150 <= xEtoile) && (170 >= xEtoile)) && ((110 <= yEtoile) && (130 >= yEtoile)))
    {
        gfx_SetColor(9);
        gfx_Rectangle_NoClip(xEtoile - 8, yEtoile - 8, 16, 16);
        if((*key == sk_Enter) && (camera_LockGet(camera) == false)){
            menu_OpenSystem(windowManager, camera, MENU_SYSTEM, MENU_SYSTEME_ETOILE);
            *key = 0;
        }
    }
}

/**
 *Set la bonne couleur suivant le type de la planète
 */
void CouleurPlanete(char type){
    switch(type){
        case 1:
            gfx_SetColor(20);
            break;
        case 2:
            gfx_SetColor(21);
            break;
        case 3:
            gfx_SetColor(22);
            break;
        case 4:
            gfx_SetColor(23);
            break;
        case 5:
            gfx_SetColor(23);
            break;
        case 6:
            gfx_SetColor(24);
            break;
        case 7:
            gfx_SetColor(25);
            break;
        case 8:
            gfx_SetColor(26);
            break;
        case 9:
            gfx_SetColor(25);
            break;
        case 10:
            gfx_SetColor(20);
            break;
        case 11:
            gfx_SetColor(23);
            break;
        case 12:
            gfx_SetColor(20);
            break;
        case 13:
            gfx_SetColor(23);
            break;
        case 14:
            gfx_SetColor(29);
            break;
        case 15:
            gfx_SetColor(27);
            break;
        case 16:
            gfx_SetColor(28);
            break;
    }
}

/**
 *Dessine une planète pour le menu systeme de la fonction StellarisHUD
 */
static void DessinerPlanete(StarSystem* systeme, Planet* planete, Camera* camera, WindowManager *windowManager, int numero, char* key){
    int x, y;
    char nomNumero[10], decalage = 0;
    City *city = planet_CityGet(planete);

    x = planet_XGet(planete) - camera_XSystemGet(camera);
    y = planet_YGet(planete) - camera_YSystemGet(camera);
    gfx_SetColor(11);
    gfx_Circle(SYSTEM_SPECIAL_X - camera_XSystemGet(camera), SYSTEM_SPECIAL_Y - camera_YSystemGet(camera), planet_OrbitRadiusGet(planete));
    CouleurPlanete(planet_TypeGet(planete));
    gfx_FillCircle(x, y, planet_SizeGet(planete));
    gfx_SetTextFGColor(1);
    if(((50 < x) && (x < 310)) && ((10 < y) && (y < 220))){
        if(planet_HabitabilityGet(planete)){
            if(city_PopulationGet(city) > 0){
                gfx_SetColor(6);
                gfx_FillRectangle(x - strlen(planet_NameGet(planete)) * 4 - 2, y + 8, strlen(planet_NameGet(planete)) * 8 + 4, 10);
                gfx_SetColor(7);
                gfx_Rectangle(x - strlen(planet_NameGet(planete)) * 4 - 2, y + 8, strlen(planet_NameGet(planete)) * 8 + 4, 10);
            } else {
                gfx_TransparentSprite(icon_planet_green, x - (strlen(planet_NameGet(planete)) * 4) - 9, y + 9);
            }
            gfx_PrintStringXY(planet_NameGet(planete), x - (strlen(planet_NameGet(planete)) * 4), y + 9);
        } else {
            switch(numero){
                case 0:
                    decalage = 8;
                    strcpy(nomNumero, " I");
                    break;
                case 1:
                    decalage = 12;
                    strcpy(nomNumero, " II");
                    break;
                case 2:
                    decalage = 16;
                    strcpy(nomNumero, " III");
                    break;
                case 3:
                    decalage = 12;
                    strcpy(nomNumero, " IV");
                    break;
                case 4:
                    decalage = 8;
                    strcpy(nomNumero, " V");
                    break;
            }
            // gfx_SetColor(0);
            // gfx_FillRectangle(x - strlen(systeme->nom) * 4 - 2 - decalage, y + 8, strlen(systeme->nom) * 8 + 4 + (decalage * 2), 10);
            
            gfx_SetTextXY(x - (strlen(starSystem_NameGet(systeme)) * 4) - decalage, y + 9);
            gfx_PrintString(starSystem_NameGet(systeme));
            gfx_PrintString(nomNumero);
        }
    }

    if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y)))
    {
        gfx_SetColor(9);
        gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
        if((*key == sk_Enter) && (!camera_LockGet(camera))){
            menu_OpenSystem(windowManager, camera, MENU_SYSTEM, MENU_SYSTEME_PLANETE_RESUME);
            SetWindowPlanet(windowManager, numero);
            *key = 0;
        }
    }
}

/**
 *Dessine la base du systeme
 */
void DessinerBase(StarSystem *systeme, Camera* camera, WindowManager* windowManager, char* key){
    int x, y;
    Station *station = starSystem_StationGet(systeme);
    if(station_LevelGet(station) != STATION_NONE){
        gfx_SetColor(1);
        x = 465 - camera_XSystemGet(camera);
        y = 345 - camera_YSystemGet(camera);
        
        //verifie que la base soit dans l'écran
        if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
            switch(station_LevelGet(station)){ //dessine la station
                case STATION_OUTPOST:
                    gfx_SetPixel(x, y);
                    break;
                case STATION_STARPORT:
                    gfx_Line_NoClip(x - 1, y - 1, x + 1, y + 1);
                    break;
                case STATION_STARHOLD:
                    gfx_Line_NoClip(x - 1, y - 1, x + 1, y + 1);
                    gfx_Line_NoClip(x - 1, y + 1, x + 1, y - 1);
                    break;
                case STATION_STARFORTRESS:
                    gfx_Line_NoClip(x - 2, y, x, y - 2);
                    gfx_Line_NoClip(x - 2, y, x, y + 2);
                    gfx_Line_NoClip(x + 2, y, x, y - 2);
                    gfx_Line_NoClip(x + 2, y, x, y + 2);
                    gfx_SetPixel(x, y);
                    break;
                case STATION_CITADEL:
                    gfx_Line_NoClip(x - 2, y, x, y - 2);
                    gfx_Line_NoClip(x - 2, y, x, y + 2);
                    gfx_Line_NoClip(x + 2, y, x, y - 2);
                    gfx_Line_NoClip(x + 2, y, x, y + 2);
                    gfx_SetPixel(x, y);
                    gfx_SetPixel(x - 2, y - 2);//coins
                    gfx_SetPixel(x - 2, y + 2);
                    gfx_SetPixel(x + 2, y - 2);
                    gfx_SetPixel(x + 2, y + 2);
                    break;
                default:
                    break;
            }

            if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (camera_LockGet(camera) == false)))) {
                gfx_SetColor(9);
                gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
                if((*key == sk_Enter) && (camera_LockGet(camera) == false)){
                    menu_OpenSystem(windowManager, camera, MENU_SYSTEM, MENU_SYSTEME_STATION_RESUME);
                    SetWindowSelection(windowManager, 1);
                    *key = 0;
                }
            }
        }
    }
}

/**
 *Dessine les flottes du systeme
 */
static void DessinerFlottesSysteme(EmpireList *empireListe, Camera *camera, WindowManager *windowManager, char* key){
    Empire* empire = NULL;
    Fleet* flotte = NULL;
    int empireIndex = 0, fleetIndex = 0;
    int empireSize = 0;
    int sizeFleet = 0;
    int x, y;
    gfx_SetColor(11);
    empire = empire_Get(empireListe, 0);
    fleetIndex = 0;
    flotte = FlotteNumero(empire_FleetListGet(empire), 0);
    sizeFleet = FleetArraySize(empire_FleetListGet(empire));
    while(fleetIndex <= sizeFleet){
        if(GetFleetSystem(flotte) == camera_SystemAimedGet(camera)){
            x = GetFleetX(flotte) - camera_XSystemGet(camera) - 3;
            y = GetFleetY(flotte) - camera_YSystemGet(camera) - 3;
            while(gfx_GetPixel(x + 4, y + 1) != 0){
                y += 15;
            }
            if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
                if(GetFleetProgress(flotte) == 0){
                    gfx_TransparentSprite(icon_fleet_our, x, y);
                    switch(GetFleetType(flotte)){
                    case FLEET_MILITARY:
                        gfx_TransparentSprite(icon_ship_military_our, x + 6, y - 1);
                        if (GetFleetPower(flotte) > 500) {
                            gfx_TransparentSprite(icon_ship_military_our, x + 10, y - 1);
                        }
                        if (GetFleetPower(flotte) > 1500) {
                            gfx_TransparentSprite(icon_ship_military_our, x + 8, y - 4);
                        }
                        break;
                    case FLEET_CONSTRUCTION:
                        gfx_TransparentSprite(icon_ship_construction_our, x + 6, y - 4);
                        break;
                    case FLEET_SCIENTIFIC:
                        gfx_TransparentSprite(icon_ship_science_our, x + 6, y - 4);
                        break;
                    }
                } else if(GetFleetProgress(flotte) == 1) {//dessinne l'hypervitesse
                    gfx_SetColor(1);
                    gfx_FillCircle_NoClip(x + 3, y, 5);
                    SetFleetProgress(flotte, 2);
                }
                x += 3;
                y += 3;
                if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (camera_LockGet(camera) == false)))){
                    gfx_SetColor(9);
                    gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);			
                    if((*key == sk_Enter) && (camera_LockGet(camera) == false)){
                        menu_OpenSystem(windowManager, camera, MENU_SYSTEM, MENU_SYSTEME_FLOTTE_DETAILS);
                        SetWindowSelection(windowManager, 1);
                        SetWindowSelectedFleet(windowManager, fleetIndex);
                        SetWindowPrevious(windowManager, 1);
                        *key = 0;
                    }
                }
            }
        }
        fleetIndex++;
        flotte = FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), fleetIndex);
    }
    empire = empire_Get(empireListe, 1);
    empireIndex = 1;
    empireSize = empire_ArraySize(empireListe);
    while(empireIndex <= empireSize){
        fleetIndex = 0;
        flotte = FlotteNumero(empire_FleetListGet(empire), 0);
        sizeFleet = FleetArraySize(empire_FleetListGet(empire));
        while(fleetIndex <= sizeFleet){
            if(GetFleetSystem(flotte) == camera_SystemAimedGet(camera)){
                x = GetFleetX(flotte) - camera_XSystemGet(camera) - 3;
                y = GetFleetY(flotte) - camera_YSystemGet(camera) - 3;
                while(gfx_GetPixel(x + 4, y + 1) != 0){
                    y += 15;
                }
                if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
                    if(GetFleetProgress(flotte) == 0){
                        gfx_TransparentSprite(icon_fleet_neutral, x, y);
                        switch(GetFleetType(flotte)){
                        case FLEET_MILITARY:
                            gfx_TransparentSprite(icon_ship_military_neutral, x + 6, y - 1);
                            if (GetFleetPower(flotte) > 500) {
                                gfx_TransparentSprite(icon_ship_military_neutral, x + 10, y - 1);
                            }
                            if (GetFleetPower(flotte) > 1500) {
                                gfx_TransparentSprite(icon_ship_military_neutral, x + 8, y - 4);
                            }
                            break;
                        case FLEET_CONSTRUCTION:
                            gfx_TransparentSprite(icon_ship_construction_neutral, x + 6, y - 4);
                            break;
                        case FLEET_SCIENTIFIC:
                            gfx_TransparentSprite(icon_ship_science_neutral, x + 6, y - 4);
                            break;
                        }
                    } else if(GetFleetProgress(flotte) == 1) {//dessinne l'hypervitesse
                        gfx_SetColor(1);
                        gfx_FillCircle_NoClip(x + 3, y, 5);
                        SetFleetProgress(flotte, 2);
                    }
                    x += 3;
                    y += 3;
                    if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (camera_LockGet(camera) == false)))){
                        gfx_SetColor(9);
                        gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);			
                        if((*key == sk_Enter) && (camera_LockGet(camera) == false)){
                            menu_OpenSystem(windowManager, camera, MENU_SYSTEM, MENU_SYSTEME_FLOTTE_DETAILS);
                            SetWindowSelection(windowManager, 1);
                            SetWindowSelectedFleet(windowManager, fleetIndex);
                            SetWindowPrevious(windowManager, 1);
                            *key = 0;
                        }
                    }
                }
            }
            fleetIndex++;
            flotte = FlotteNumero(empire_FleetListGet(empire), fleetIndex);
        }
        empireIndex++;
        empire = empire_Get(empireListe, empireIndex);
    }
}

/**
 *dessine le systeme en vue systeme
 */
static void DessinerVueSysteme(StarSystem **systemeStellaires, Camera* camera, WindowManager *windowManager, EmpireList* empireListe, char *key){
    StarSystem* systeme;
    int j = 0;

    systeme = systemeStellaires[camera_SystemAimedGet(camera)];
    DessinerEtoile(systeme, camera, windowManager, key);
    DessinerBase(systeme, camera, windowManager, key);
    DessinerFlottesSysteme(empireListe, camera, windowManager, key);
    DessinerHyperlanesSysteme(systemeStellaires, camera);
    while(j < starSystem_NumberOfPlanetGet(systeme)) {
        DessinerPlanete(systeme, starSystem_PlanetGet(systeme, j), camera, windowManager, j, key);
        j++;
    }
}

static void map_DrawGalacticStar(StarType type, int x, int y, int zoom){
    map_StarColorGet(type);
    switch(type){
        default:
        case STAR_TYPE_A:
        case STAR_TYPE_F:
        case STAR_TYPE_G:
        case STAR_TYPE_M:
            gfx_FillCircle(x, y, 2 * zoom);
            break;
        case STAR_TYPE_K:
        case STAR_TYPE_B:
        case STAR_TYPE_PULSAR: 
            gfx_FillCircle(x, y, 1 * zoom);
            break;
        case STAR_TYPE_BLACKHOLE: 
            gfx_Circle(x, y, 2 * zoom);
            break;
        case STAR_TYPE_NEUTRON: 
            gfx_FillCircle(x, y, 1 * zoom);
            gfx_Circle(x, y, 3 * zoom);
            break;
    }
}

static void map_DrawHyperlanes( StarSystem *system,
                                StarSystem **galaxy,
                                Camera *camera,
                                int x,
                                int y){
    char hyperLane;
    int i = 2;
    for(;i < 4;hyperLane = hyperlane_DestinationGet(system, i++))
        map_DrawOneHyperlane(system, galaxy[(int)hyperLane], x, y, camera);
}

static void map_DrawGalacticNormal(	Empire *player,
                                    EmpireList *empireList, 
                                    StarSystem **galaxy, 
                                    Camera *camera, 
                                    WindowManager *window, 
                                    Settings *settings){
    int systemIndex = 0;
    
    int x, y;

    int zoom = camera_ZoomGet(camera);

    // We check that the camera is not out of the limits
    if (camera_XGet(camera) < LIMIT_LEFT) { // The left limit
        camera_XSet(camera, LIMIT_LEFT);
        camera_XVectorSet(camera, 0);
    } else if ((GALAXY_WIDTH + 1) * SPACE_BETWEEN_STARS  * camera_ZoomGet(camera) < camera_XGet(camera)) { // The right limit
        camera_XSet(camera, (GALAXY_WIDTH + 1) * SPACE_BETWEEN_STARS * camera_ZoomGet(camera));
        camera_XVectorSet(camera, 0);
    }
    if (camera_YGet(camera) < LIMIT_TOP) { // The up limit
        camera_YSet(camera, LIMIT_TOP);
        camera_YVectorSet(camera, 0);
    } else if((GALAXY_WIDTH + 1) * SPACE_BETWEEN_STARS  * camera_ZoomGet(camera) < camera_YGet(camera)) {
        camera_YSet(camera, (GALAXY_WIDTH + 1) * SPACE_BETWEEN_STARS  * camera_ZoomGet(camera));
        camera_YVectorSet(camera, 0);
    }

    gfx_SetTextConfig(gfx_text_clip); // To clip the text
    camera_SystemAimedSet(camera, NO_SYSTEM); // Reinitialize the aimed system

    while(systemIndex < (GALAXY_WIDTH * GALAXY_WIDTH)) {        
        x = starSystem_XGet(galaxy[systemIndex]) * camera_ZoomGet(camera) - camera_XGet(camera) + 160;
        y = starSystem_YGet(galaxy[systemIndex]) * camera_ZoomGet(camera) - camera_YGet(camera) + 120;
        if ((((0 <= x) && (x <= SCREEN_WIDTH)) && ((HUD_WIDTH <= y) && (y <= SCREEN_HEIGHT))) && (starSystem_XGet(galaxy[systemIndex]) && starSystem_YGet(galaxy[systemIndex]))) {
            // We draw the icon for when a system is habitable
            map_DrawGalacticHabitable(galaxy[systemIndex], settings, x, y);

            map_DrawHyperlanes(galaxy[systemIndex], galaxy, camera, x, y);

            // We draw the star itself
            map_DrawGalacticStar(starSystem_StarTypeGet(galaxy[systemIndex]), x, y, zoom);

            // Test if the system is the one in the 
            if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y))) {
                gfx_SetColor(COLOR_ORANGE);
                gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
                camera_SystemAimedSet(camera, systemIndex);
            }
        }

        systemIndex++;
    }
    gfx_SetTextConfig(gfx_text_noclip); // To stop the clipping of the text (faster)
}

/* entry points ======================================================== */

void map_Draw(	char *key,
                EmpireList *empireList, 
                StarSystem **galaxy, 
                Camera *camera, 
                WindowManager *windowManager, 
                Settings *settings){
    // We get the pointer of the player
    Empire *player = empire_Get(empireList, 0);


    switch(camera_MapTypeGet(camera)){
        case VUE_GALAXY_LARGE:
            // map_DrawGalacticLarge(player, empireList, galaxy, camera, windowManager, settings);
            dbg_printf("VUE_GALAXY_LARGE\n");
            break;

        case VUE_GALAXY_NORMAL:
            map_DrawGalacticNormal(player, empireList, galaxy, camera, windowManager, settings);
            dbg_printf("VUE_GALAXY_NORMAL\n");
            break;

        case VUE_SYSTEM:
            if(camera_XVectorGet(camera)) {
                camera_XSystemAdd(camera, camera_XVectorGet(camera));
                if(camera_XVectorGet(camera) > 0) {
                    camera_XVectorAdd(camera, -1);
                } else if(camera_XVectorGet(camera) < 0) {
                    camera_XVectorAdd(camera, +1);
                }
            }
            if(camera_YVectorGet(camera)) {
                camera_YSystemAdd(camera, camera_YVectorGet(camera));
                if(camera_YVectorGet(camera) > 0) {
                    camera_YVectorAdd(camera, -1);
                } else if(camera_YVectorGet(camera) < 0) {
                    camera_YVectorAdd(camera, +1);
                }
            }


            //fait en sorte que la camera sorte pas des limites
            while(pow((double)(camera_XSystemGet(camera) - 320), 2.0) + pow((double)(camera_YSystemGet(camera) - 240), 2.0) > pow((double)SYSTEM_VIEW_RADIUS, 2.0)){
                camera_XVectorSet(camera, 0);
                camera_YVectorSet(camera, 0);
                if(camera_XSystemGet(camera) - 320 > 0){
                    camera_XSystemAdd(camera, -1);
                }
                if(camera_XSystemGet(camera) - 320 < 0){
                    camera_XSystemAdd(camera, +1);
                }
                if(camera_YSystemGet(camera) - 240 > 0){
                    camera_YSystemAdd(camera, -1);
                }
                if(camera_YSystemGet(camera) - 240 < 0){
                    camera_YSystemAdd(camera, +1);
                }
            }
            DessinerVueSysteme(galaxy, camera, windowManager, empireList, key);
            dbg_printf("VUE_SYSTEM\n");
            break;

        default:
            camera_MapTypeSet(camera, VUE_GALAXY_NORMAL);
            camera_XSet(camera, 0);
            camera_YSet(camera, 0);
            break;
    }

}
