#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <fileioc.h>
#include "main.h"

#define MENU_MARKET_CASH    0
#define MENU_MARKET_MINERAL 1
#define MENU_MARKET_FOOD    2
#define MENU_MARKET_ALLOY   3
#define MENU_MARKET_OTHER   4

/**
 * @brief Class of menus
 * 
 */
typedef enum {  MENU_EXIT = -1, 
                MENU_NONE, 
                MENU_SYSTEME, 
                MENU_MARKET, 
                MENU_FLEET, 
                MENU_SCIENCE, 
                MENU_CONTACTS, 
                MENU_CONTACTS_DETAILS, 
                MENU_CONTACTS_EFFECTUER_ACTION} ClassMenu;

/**
 * @brief Subclass for the system menus
 * 
 */
typedef enum {  MENU_SYSTEME_AUCUN, 
                MENU_SYSTEME_FLOTTES, 
                MENU_SYSTEME_ETOILE, 
                MENU_SYSTEME_PLANETE_RESUME, 
                MENU_SYSTEME_PLANETE_DISTRICT, 
                MENU_SYSTEME_PLANETE_BATIMENT, 
                MENU_SYSTEME_PLANETE_BATIMENT_CHOIX, 
                MENU_SYSTEME_FLOTTE_DETAILS, 
                MENU_SYSTEME_STATION_RESUME, 
                MENU_SYSTEME_STATION_MODULES, 
                MENU_SYSTEME_STATION_MODULES_CHOIX, 
                MENU_SYSTEME_STATION_CHANTIER,
                MENU_SYSTEME_STATION_CHANTIER_CHOIX} MenuSystem;

#include "ai.h"
#include "camera.h"
#include "fleet.h"
#include "time.h"
#include "settings.h"
#include "systemes.h"

int DrawMenu(EmpireList *empireListe, Time *date, char *key, Camera *camera, StarSystem **systemeStellaires, Window *fenetre, Settings *parametres, Market *marche);

#endif