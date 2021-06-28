#ifndef H_SCREEN
#define H_SCREEN

#include "ai.h"
#include "hud.h"
#include "map.h"
#include "notifications.h"

/* entry points ======================================================== */
char DrawScreen(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche, NotificationList *notificationList);

#endif