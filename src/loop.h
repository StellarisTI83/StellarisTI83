#ifndef _BOUCLE_INCLUDE
#define _BOUCLE_INCLUDE

#include "ai.h"
#include "console.h"
#include "console.h"
#include "fleet.h"
#include "map.h"
#include "menus.h"
#include "notifications.h"
#include "sauvegarde.h"
#include "screen.h"
#include "time.h"
#include "updating.h"


/* entry points ======================================================== */

int game_MainLoop(	EmpireList *empireListe, 
					Settings *settings, 
					Time *time, 
					StarSystem **starSystems, 
					Camera *camera, 
					Window *window, 
					Market *marche, 
					NotificationList *notificationList);

#endif