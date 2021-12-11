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
#define MENU_WIDE_WIDTH         240
#define MENU_WIDE_HEIGHT        160

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
                MENU_CONTACTS_INFOS, 
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
 * @brief Open the asked menu, deprecated
 * 
 * @param window 
 * @param camera 
 * @param classMenu 
 * @param menuSysteme 
 */
void menu_OpenSystem( Window *window, 
                Camera *camera, 
                MenuClass classMenu, 
                MenuSystem menuSysteme);

/**
 * @brief Open the asked menu
 * 
 * @param window 
 * @param camera 
 * @param classMenu 
 */
void menu_Open( Window *window, 
                Camera *camera, 
                MenuClass classMenu);

/**
 * @brief Close all menus
 * 
 * @param window 
 * @param camera 
 */
void menu_Close(Window *window, 
                Camera *camera);

void menu_Initialize(   EmpireList *empireList, 
                        StarSystem **galaxy,
                        Settings *settings, 
                        Time *time,
                        Camera *camera,
                        Window *window,
                        Market *market);
// Windows functions

Window *window_Create();

MenuClass window_MenuGet(Window *window);
MenuSystem GetOpenedMenuDetails(Window *window);
void SetWindowMenuSystem(Window *window, MenuSystem menu);

void window_CommandPromptOpen(Window *window, Camera *camera, Time *date);
void window_CommandPromptClose(Window *window, Camera *camera, Time *date);
int window_CommandPromptStatusGet(Window *window);

void SetWindowPlanet(Window *window, int planete);
int GetWindowPlanet(Window *window);

void SetWindowSelection(Window *window, int selection);
int GetWindowSelection(Window *window);
void window_SelectionIncrement(Window *window);
void window_SelectionUnIncrement(Window *window);
void AddWindowSelection(Window *window, int number);

void SetWindowSelectedFleet(Window *window, int fleet);
int GetWindowSelectedFleet(Window *window);

void SetWindowPrevious(Window *window, int previous);
int GetWindowPrevious(Window *window);

Error GetWindowError(Window *window);
void SetWindowError(Window *window, Error error);
int GetWindowErrorCountDown(Window *window);
void UnincrementWindowErrorCountdown(Window *window);
void SetWindowErrorCountdown(Window *window, int countdown);

void SetWindowCity(Window *window, City *city);
City *GetWindowCity(Window *window);
void SetWindowEmpire(Window *window, int empire);
int GetWindowEmpire(Window *window);

void SetWindowScroll(Window *window, int scroll);
void AddWindowScroll(Window *window, int scroll);
int GetWindowScroll(Window *window);

void SetWindowFleetSize(Window *window, int size);
void AddWindowFleetSize(Window *window, int size);
int GetWindowFleetSize(Window *window);

WidgetWindow *window_WindowNew( Window *window, 
                                const char *title, 
                                const int width, 
                                const int height);
WidgetWindow *window_WindowGet(Window *window, const int index);
#endif