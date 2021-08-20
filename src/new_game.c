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
    AddTimeClock(*time);

    *camera = AllocCamera();
    SetCameraXVector(*camera, 0);
    SetCameraYVector(*camera, 0);
    
    SetCameraZoom(*camera, ZOOM_MAX);
    SetCameraMapType(*camera, VUE_SYSTEM);

    *window = AllocFenetre();
    CloseMenu(*window, *camera);
    
    timer_Disable(1);
    timer_Set(1, ONE_SECOND);
    timer_SetReload(1, ONE_SECOND);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
}

/* entry points ======================================================== */

void newGame_Start(){
    EmpireList *empireListe = NULL;
    StarSystem *starSystem[GALAXY_WIDTH * GALAXY_WIDTH];
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
    
    galaxy_CreateNew(starSystem);
	settings_EmpireNumberSet(settings, 4);

	galaxy_StartEmpiresInitialize(settings, empireListe, starSystem, camera);
	UpdatePlayersData(false, empireListe, starSystem, notificationList);

	gfx_SetDrawBuffer();
    game_MainLoop(  empireListe, 
                    settings, 
                    time, 
                    starSystem, 
                    camera, 
                    window, 
                    market, 
                    notificationList);

    game_Close( empireListe, 
                starSystem, 
                settings, 
                time, 
                camera, 
                window, 
                market,
                notificationList);
}

void game_Close(EmpireList *empireListe, 
                StarSystem **starSystem,
                Settings *settings, 
                Time *time,
                Camera *camera,
                Window *window,
                Market *market,
                NotificationList *notificationList){
    int index = 0;
    if(empireListe)
        empire_ListFree(empireListe);

    if(starSystem){
        index = 0;
        while(index < GALAXY_WIDTH * GALAXY_WIDTH){
            if(starSystem[index])
                free(starSystem[index]);
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