#ifndef H_UPDATING
#define H_UPDATING

#include "loop.h"
#include "fleet.h"
#include "map.h"
#include "galaxy.h"
#include "sauvegarde.h"
#include "settings.h"
#include "time.h"

/* entry points ======================================================== */
void UpdatePlayersData(char appliquer, EmpireList *empireListe, StarSystem **systemeStellaires, NotificationList *notificationList);

/**
 * @brief Entry point to update all the datas
 * 
 * @param key 
 * @param empireList 
 * @param starSystem 
 * @param time 
 * @param camera 
 * @param window 
 * @param notificationList 
 * @param settings 
 * @return int 
 */
int game_Update( char *key, 
                EmpireList *empireList, 
                StarSystem **starSystem, 
                Time *time, 
                Camera *camera, 
                Window *window, 
                NotificationList *notificationList, 
                Settings *settings);

/**
 * @brief Update all the intel levels
 * 
 * @param starSystem the galaxy
 * @param empireList 
 */
void update_IntelLevel(StarSystem **starSystem, EmpireList *empireList);
#endif