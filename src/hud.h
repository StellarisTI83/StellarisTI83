#ifndef H_HUD
#define H_HUD

#include "ai.h"
#include "camera.h"
#include "main.h"
#include "notifications.h"
#include "parametres.h"
#include "time.h"

int DrawHUD(EmpireListe *empireListe, Date *date, char *key, Camera *camera, SystemeStellaire **systemeStellaires, Fenetre *fenetre, Parametres *parametres, NotificationList *notificationList);

#endif