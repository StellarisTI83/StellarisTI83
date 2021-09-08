#include <graphx.h>
#include <tice.h>

#include <debug.h>

#include "gfx/gfx.h"
#include "colors.h"

#include "ai.h"
#include "camera.h"
#include "galaxy.h"
#include "loop.h"
#include "notifications.h"

#include "new_game.h"

/* private functions =================================================== */

/**
 * @brief Initialize the main structures of the game
 * 
 * @param empireListe 
 * @param settings 
 * @param time 
 * @param camera 
 * @param window 
 */
static void newGame_Initialize( EmpireList **empireListe, 
                                Settings **settings, 
                                Time **time,
                                Camera **camera,
                                Window **window){
    *empireListe = empire_ListCreate();
    empire_Add(*empireListe);
    empire_FleetListCreate(empire_Get(*empireListe, 0));

    
    *settings = setting_Malloc();
    settings_SeeAllSet(*settings, false);

    *time = time_Alloc();
    time_DateSet(*time, NEW_GAME_START_DAY, NEW_GAME_START_MONTH, NEW_GAME_START_YEAR);
    time_SpeedSet(*time, 0, 1);
    time_TickIncrement(*time);

    *camera = camera_Create();
    camera_XVectorSet(*camera, 0);
    camera_YVectorSet(*camera, 0);
    camera_XSystemSet(*camera, SYSTEM_MIDDLE_X);
    camera_YsystemSet(*camera, SYSTEM_MIDDLE_Y);
    
    camera_ZoomSet(*camera, ZOOM_MAX);
    camera_MapTypeSet(*camera, VUE_SYSTEM);

    *window = window_Create();
    menu_Close(*window, *camera);
    
    timer_Disable(1);
    timer_Set(1, ONE_SECOND);
    timer_SetReload(1, ONE_SECOND);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
}

/* entry points ======================================================== */

void newGame_Start(){
    EmpireList *empireListe = NULL;
    StarSystem *galaxy[GALAXY_WIDTH * GALAXY_WIDTH];
    Window *window = NULL;
    Settings *settings = NULL;
    Camera *camera = NULL;
    Time *time = NULL;
	Market *market = NULL;
	NotificationList *notificationList = CreateNotificationList();

    gfx_FillScreen(COLOR_DARK_GREEN);
    gfx_BlitBuffer();
    gfx_SetPalette(gfx_pal, sizeof_background_gfx_pal, 0);

    newGame_Initialize(&empireListe, &settings, &time, &camera, &window);
    
    galaxy_CreateNew(galaxy);
	settings_EmpireNumberSet(settings, 4);

	galaxy_StartEmpiresInitialize(settings, empireListe, galaxy, camera);
	update_PlayersData(false, empireListe, galaxy, notificationList);

	gfx_SetDrawBuffer();
    game_MainLoop(  empireListe, 
                    settings, 
                    time, 
                    galaxy, 
                    camera, 
                    window, 
                    market, 
                    notificationList);

    game_Close( empireListe, 
                galaxy, 
                settings, 
                time, 
                camera, 
                window, 
                market,
                notificationList);
}

void game_Close(EmpireList *empireListe, 
                StarSystem **galaxy,
                Settings *settings, 
                Time *time,
                Camera *camera,
                Window *window,
                Market *market,
                NotificationList *notificationList){
    int index = 0;
    if(empireListe)
        empire_ListFree(empireListe);

    if(galaxy){
        index = 0;
        while(index < GALAXY_WIDTH * GALAXY_WIDTH){
            if(galaxy[index])
                free(galaxy[index]);
            index++;
        }
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free galaxy\n");
        #endif
    }

    if(settings){
        free(settings);
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free settings\n");
        #endif
    }

    if(time){
        free(time);
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free time\n");
        #endif
    }

    if(camera){
        free(camera);
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free camera\n");
        #endif
    }

    if(window){
        free(window);
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free window\n");
        #endif
    }

    if(market){
        free(market);
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Free market\n");
        #endif
    }

    if(notificationList)
        FreeNotificationList(notificationList);
}
