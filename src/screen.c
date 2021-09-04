#include <graphx.h>

#include "menus.h"
#include "hud.h"

#include "screen.h"

/* entry points ======================================================== */

char draw_Screen(   char *key, 
                    EmpireList *empireListe, 
                    StarSystem **galaxy, 
                    Time *time, 
                    Camera *camera, 
                    Window *window, 
                    Settings *settings, 
                    Market *market, 
                    NotificationList *notificationList) {
    int8_t endGame;
    gfx_ZeroScreen();

    map_Draw(   empireListe, 
                galaxy, 
                camera, 
                key, 
                window, 
                settings);

    hud_Draw(empireListe, time, key, camera, galaxy, window, settings, notificationList);

    endGame = DrawMenu(empireListe, time, key, camera, galaxy, window, settings, market);
    
	gfx_SwapDraw();
    
    return endGame;
}