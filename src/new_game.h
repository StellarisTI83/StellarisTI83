#ifndef H_NEW_GAME
#define H_NEW_GAME

#include "ai.h"
#include "camera.h"
#include "galaxy.h"
#include "notifications.h"

#define NEW_GAME_START_YEAR     2200
#define NEW_GAME_START_MONTH    01
#define NEW_GAME_START_DAY      01

/* entry points ======================================================== */

/**
 * @brief Function to start a new game
 * 
 */
void newGame_Start();

/**
 * @brief Free every mallocs used by the game
 * 
 * @param empireListe 
 * @param galaxy 
 * @param settings 
 * @param time 
 * @param camera 
 * @param window 
 * @param market 
 * @param notificationList 
 */
void game_Close(EmpireList *empireListe, 
                StarSystem **galaxy,
                Settings *settings, 
                Time *time,
                Camera *camera,
                Window *window,
                Market *market,
                NotificationList *notificationList);
#endif