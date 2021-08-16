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
/**
 * 
 * */
char DrawScreen(char *key, EmpireListe *empireListe, StarSystem **systemeStellaires, Time *date, Camera *camera, Window *fenetre, Settings *parametres, Market *marche, NotificationList *notificationList) {
    int8_t fin = 1;
    gfx_ZeroScreen();

    DrawMap(empireListe, systemeStellaires, camera, key, date, fenetre, parametres);

    hud_Draw(empireListe, date, key, camera, systemeStellaires, fenetre, parametres, notificationList);

    fin = DrawMenu(empireListe, date, key, camera, systemeStellaires, fenetre, parametres, marche);
    
	gfx_SwapDraw();
    
    return fin;
}