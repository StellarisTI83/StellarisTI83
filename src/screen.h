#ifndef H_SCREEN
#define H_SCREEN

#include "ai.h"
#include "hud.h"
#include "map.h"
#include "notifications.h"

/* entry points ======================================================== */
/**
 * @brief Draw everything on the screen
 * 
 * @param key 
 * @param empireListe 
 * @param galaxy 
 * @param time 
 * @param camera 
 * @param window 
 * @param settings 
 * @param market 
 * @param notificationList 
 * @return char 
 */
char draw_Screen(   char *key, 
                    EmpireList *empireListe, 
                    StarSystem **galaxy, 
                    Time *time, 
                    Camera *camera, 
                    WindowManager *window, 
                    Settings *settings, 
                    Market *market, 
                    NotificationList *notificationList);
#endif