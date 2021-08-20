#ifndef H_SCREEN
#define H_SCREEN

#include "ai.h"
#include "hud.h"
#include "map.h"
#include "notifications.h"

/* entry points ======================================================== */
char DrawScreen(char *key, EmpireList *empireListe, StarSystem **systemeStellaires, Time *date, Camera *camera, Window *fenetre, Settings *parametres, Market *marche, NotificationList *notificationList);

#endif