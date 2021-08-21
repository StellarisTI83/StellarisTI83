#ifndef H_UPDATING
#define H_UPDATING

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
#endif