#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include "menus.h"
#include "hud.h"

#include "screen.h"

/* entry points ======================================================== */
void DrawScreen(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche, NotificationList *notificationList) {
    gfx_ZeroScreen();

    DrawMap(empireListe, systemeStellaires, camera, key, date, fenetre, parametres);

    DrawHUD(empireListe, date, key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche, notificationList);

    DrawMenu(empireListe, date, key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche);
    
	gfx_SwapDraw();
}