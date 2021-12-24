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

typedef struct WindowManagerStruct WindowManager;

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
                WindowManager *window, 
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
void menu_OpenSystem( WindowManager *window, 
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
void menu_Open( WindowManager *window, 
                Camera *camera, 
                MenuClass classMenu);

/**
 * @brief Close all menus
 * 
 * @param window 
 * @param camera 
 */
void menu_Close(WindowManager *window, 
                Camera *camera);

void menu_Initialize(   EmpireList *empireList, 
                        StarSystem **galaxy,
                        Settings *settings, 
                        Time *time,
                        Camera *camera,
                        WindowManager *window,
                        Market *market);
// Windows functions

WindowManager *window_Create();

MenuClass window_MenuGet(WindowManager *window);
MenuSystem GetOpenedMenuDetails(WindowManager *window);
void SetWindowMenuSystem(WindowManager *window, MenuSystem menu);

void window_CommandPromptOpen(WindowManager *window, Camera *camera, Time *date);
void window_CommandPromptClose(WindowManager *window, Camera *camera, Time *date);
int window_CommandPromptStatusGet(WindowManager *window);

void SetWindowPlanet(WindowManager *window, int planete);
int GetWindowPlanet(WindowManager *window);

void SetWindowSelection(WindowManager *window, int selection);
int GetWindowSelection(WindowManager *window);
void window_SelectionIncrement(WindowManager *window);
void window_SelectionUnIncrement(WindowManager *window);
void AddWindowSelection(WindowManager *window, int number);

void SetWindowSelectedFleet(WindowManager *window, int fleet);
int GetWindowSelectedFleet(WindowManager *window);

void SetWindowPrevious(WindowManager *window, int previous);
int GetWindowPrevious(WindowManager *window);

Error GetWindowError(WindowManager *window);
void SetWindowError(WindowManager *window, Error error);
int GetWindowErrorCountDown(WindowManager *window);
void UnincrementWindowErrorCountdown(WindowManager *window);
void SetWindowErrorCountdown(WindowManager *window, int countdown);

void SetWindowCity(WindowManager *window, City *city);
City *GetWindowCity(WindowManager *window);
void SetWindowEmpire(WindowManager *window, int empire);
int GetWindowEmpire(WindowManager *window);

void SetWindowScroll(WindowManager *window, int scroll);
void AddWindowScroll(WindowManager *window, int scroll);
int GetWindowScroll(WindowManager *window);

void SetWindowFleetSize(WindowManager *window, int size);
void AddWindowFleetSize(WindowManager *window, int size);
int GetWindowFleetSize(WindowManager *window);

WidgetWindow *window_WindowNew( WindowManager *window, 
                                const char *title, 
                                const int width, 
                                const int height);
WidgetWindow *window_WindowGet(WindowManager *window, const int index);
#endif