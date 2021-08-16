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

int hud_Draw(   EmpireListe *empireListe, 
                Time *date, 
                char *key, 
                Camera *camera, 
                StarSystem **systemeStellaires, 
                Window *fenetre, 
                Settings *parametres, 
                NotificationList *notificationList);
#endif