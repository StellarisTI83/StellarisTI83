#ifndef H_HUD
#define H_HUD

#include "ai.h"
#include "camera.h"
#include "main.h"
#include "notifications.h"
#include "settings.h"
#include "time.h"

#define HUD_WIDTH 22
#define HUD_HEIGHT 140 
#define HUD_TIME_BAR_WIDTH 120  
#define HUD_TIME_BAR_HEIGHT 22
#define HUD_TIME_BAR_OFFSET (LCD_WIDTH - HUD_TIME_BAR_WIDTH)/2
#define HUD_NAME_BAR_HEIGHT 10

#define HUD_UP_BAR_LEFT 55
#define HUD_UP_BAR_UP 1
#define HUD_UP_BAR_GAP 45
#define HUD_UP_BAR_TEXT_X 13

#define HUD_SIDE_BAR_UP 50
#define HUD_SIDE_BAR_GAP 20

/**
 * @brief Function to draw the HUD on the screen
 * 
 * @param empireList EmpireList*
 * @param time Time*
 * @param key char*
 * @param camera Camera*
 * @param galaxy StarSystem*
 * @param window Window*
 * @param settings Settings*
 * @param notificationList NotificationList*
 * @return int 
 */
int hud_Draw(   char *key, 
                EmpireList *empireList, 
                Time *time,
                Camera *camera, 
                StarSystem **galaxy, 
                WindowManager *window, 
                Settings *settings, 
                NotificationList *notificationList);
#endif