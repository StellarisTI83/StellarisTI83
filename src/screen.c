/**
 * @file screen.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief The routine drawing the screen
 * @version 0.1
 * @date 2021-09-05
 * 
 * @copyright GNU General Public License v3.0
 * 
 */
#include <graphx.h>

#include "menus.h"
#include "hud.h"

#include "screen.h"

/* entry points ======================================================== */

char draw_Screen(   char *key, 
                    EmpireList *empireList, 
                    StarSystem **galaxy, 
                    Time *time, 
                    Camera *camera, 
                    WindowManager *window, 
                    Settings *settings, 
                    Market *market, 
                    NotificationList *notificationList) {
    int8_t endGame;
    gfx_ZeroScreen();

    map_Draw(   key,
                empireList, 
                galaxy, 
                camera, 
                window, 
                settings);

    hud_Draw(   key,
                empireList, 
                time, 
                camera, 
                galaxy, 
                window, 
                settings, 
                notificationList);

    endGame = menus_Draw(   key, 
                            empireList, 
                            time, 
                            camera, 
                            galaxy, 
                            window, 
                            settings, 
                            market);
    
	gfx_SwapDraw();
    
    return endGame;
}