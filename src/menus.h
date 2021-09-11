#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <fileioc.h>
#include "main.h"

#define MENU_MARKET_CASH    0
#define MENU_MARKET_MINERAL 1
#define MENU_MARKET_FOOD    2
#define MENU_MARKET_ALLOY   3
#define MENU_MARKET_OTHER   4

#define MENU_EXIT_X             110
#define MENU_EXIT_Y             40 + MENU_BUTTON_GAP
#define MENU_EXIT_WIDTH         120
#define MENU_EXIT_HEIGHT        160

#define MENU_BUTTON_HEIGHT      (TEXT_HEIGHT * 2 + 2)
#define MENU_BUTTON_GAP         2
#define MENU_BUTTON_GAP_Y_GAP   (MENU_BUTTON_GAP + MENU_BUTTON_HEIGHT)

/**
 * @brief Class of menus
 * 
 */
typedef enum {  MENU_EXIT = -1, 
                MENU_NONE, 
                MENU_SYSTEM, 
                MENU_MARKET, 
                MENU_FLEET, 
                MENU_SCIENCE, 
                MENU_CONTACTS, 
                MENU_CONTACTS_DETAILS, 
                MENU_CONTACTS_EFFECTUER_ACTION} MenuClass;

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

/* struct ============================================================== */

typedef struct WindowStruct Window;

#include "ai.h"
#include "camera.h"
#include "fleet.h"
#include "time.h"
#include "settings.h"
#include "systemes.h"
#include "widget.h"

/* entry points ======================================================== */
/**
 * @brief Function that draw all the menus
 * 
 * @param empireList 
 * @param time 
 * @param key 
 * @param camera 
 * @param galaxy 
 * @param window 
 * @param settings 
 * @param market 
 * @return int 
 */
int menus_Draw( char *key,
                EmpireList *empireList, 
                Time *time,
                Camera *camera, 
                StarSystem **galaxy, 
                Window *window, 
                Settings *settings, 
                Market *market);

/**
 * @brief Open the asked menu
 * 
 * @param fenetre 
 * @param camera 
 * @param classMenu 
 * @param menuSysteme 
 */
void menu_Open( Window *fenetre, 
                Camera *camera, 
                MenuClass classMenu, 
                MenuSystem menuSysteme);

/**
 * @brief Close all menus
 * 
 * @param fenetre 
 * @param camera 
 */
void menu_Close(Window *fenetre, 
                Camera *camera);

// Windows functions

Window *window_Create();

MenuClass window_MenuGet(Window *fenetre);
MenuSystem GetOpenedMenuDetails(Window *fenetre);
void SetWindowMenuSystem(Window *fenetre, MenuSystem menu);

void window_CommandPromptOpen(Window *fenetre, Camera *camera, Time *date);
void window_CommandPromptClose(Window *fenetre, Camera *camera, Time *date);
int window_CommandPromptStatusGet(Window *fenetre);

void SetWindowPlanet(Window *fenetre, int planete);
int GetWindowPlanet(Window *fenetre);

void SetWindowSelection(Window *fenetre, int selection);
int GetWindowSelection(Window *fenetre);
void window_SelectionIncrement(Window *fenetre);
void window_SelectionUnIncrement(Window *fenetre);
void AddWindowSelection(Window *fenetre, int number);

void SetWindowSelectedFleet(Window *fenetre, int fleet);
int GetWindowSelectedFleet(Window *fenetre);

void SetWindowPrevious(Window *fenetre, int previous);
int GetWindowPrevious(Window *fenetre);

Error GetWindowError(Window *fenetre);
void SetWindowError(Window *fenetre, Error error);
int GetWindowErrorCountDown(Window *fenetre);
void UnincrementWindowErrorCountdown(Window *fenetre);
void SetWindowErrorCountdown(Window *fenetre, int countdown);

void SetWindowCity(Window *fenetre, City *city);
City *GetWindowCity(Window *fenetre);
void SetWindowEmpire(Window *fenetre, int empire);
int GetWindowEmpire(Window *fenetre);

void SetWindowScroll(Window *fenetre, int scroll);
void AddWindowScroll(Window *fenetre, int scroll);
int GetWindowScroll(Window *fenetre);

void SetWindowFleetSize(Window *fenetre, int size);
void AddWindowFleetSize(Window *fenetre, int size);
int GetWindowFleetSize(Window *fenetre);

WidgetWindow *window_WindowNew( Window *window, 
                                const char *title, 
                                const int width, 
                                const int height);
WidgetWindow *window_WindowGet(Window *window, const int index);
#endif