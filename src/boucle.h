#ifndef _BOUCLE_INCLUDE
#define _BOUCLE_INCLUDE

#include "ai.h"
#include "console.h"
#include "console.h"
#include "flottes.h"
#include "map.h"
#include "menus.h"
#include "nouvelle_partie.h"
#include "notifications.h"
#include "sauvegarde.h"
#include "screen.h"
#include "time.h"
#include "updating.h"


/* entry points ======================================================== */

int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Parametres *parametres, Date *date, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche, NotificationList *notificationList);	

#endif