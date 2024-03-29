/**
 * @file hud.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief The hud drawing routines
 * @version 0.1
 * @time 2021-08-21
 * 
 * @copyright GNU General Public License v3.0
 * 
 */
#include <stdbool.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <graphx.h>

#include "gfx/gfx.h"
#include "colors.h"

#include "main.h"

#include "console.h"
#include "hud.h"

/* private functions =================================================== */

/**
 * @brief 
 * 
 * @param currentState 
 * @param change 
 * @param x 
 * @param y 
 */
static void hud_RessourcePrint(   const unsigned int currentState, 
                        const int change, 
                        const int x, 
                        const int y) {
    char nombreStr[10];
    char changeStr[5];

    // print current state
    if(currentState < 1000){
        sprintf(nombreStr, "%d", currentState);
    } else {
        sprintf(nombreStr, "%d", currentState / 1000);
        strcat(nombreStr, "K");
    }

    // print change
    if (change >= 0){
        gfx_SetTextFGColor(COLOR_WHITE);
        sprintf(changeStr, "+%d", change);
    } else {
        gfx_SetTextFGColor(COLOR_RED);
        sprintf(changeStr, "%d", change);
    }

    strcat(nombreStr, changeStr);

    gfx_PrintStringXY(nombreStr, x, y);
}

/**
 * @brief Draw the hexagon of the flag
 * 
 */
static void hud_DrawFlag(){
    //coordinates of the hexagon
    int flag[12] = {
        17, 0,
        0,  10,
        0,  30,
        17, 40,
        35, 30,
        35, 10,
    };
    // flag background
    // gfx_SetColor(6);
    // gfx_FillTriangle_NoClip(17, 30, 35, 30, 17, 40);
    // gfx_FillRectangle_NoClip(17, 20, 18,10);
    
    // flag outines
    gfx_SetColor(COLOR_HUD_OUTLINES);
    gfx_Polygon_NoClip(flag, 6);
    // gfx_FloodFill(17, 1, 0);
    
}





/**
 * @brief Draw the icon of the speed / pause
 * 
 * @param time 
 */
static void hud_DrawSpeedIcon(Time *time){
    //pause / avance
    switch (speed_TimeGet(time)){
        case TIME_SPEED_VERY_SLOW:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillRectangle_NoClip(100, 225, 3, 9);
            gfx_FillTriangle_NoClip(109, 225, 109, 233, 105, 229);
            gfx_FillTriangle_NoClip(114, 225, 114, 233, 110, 229);
            break;
        case TIME_SPEED_SLOW:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillRectangle_NoClip(100, 225, 3, 9);
            gfx_FillTriangle_NoClip(109, 225, 109, 233, 105, 229);
            break;
        case TIME_SPEED_PAUSE:
            gfx_SetColor(COLOR_HUD_PAUSE_OUTLINES);
            gfx_FillRectangle_NoClip(105, 225, 3, 9);
            gfx_FillRectangle_NoClip(110, 225, 3, 9);
            break;
        case TIME_SPEED_NORMAL:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
            break;	
        case TIME_SPEED_FAST:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
            gfx_FillTriangle_NoClip(110, 225, 110, 233, 114, 229);
            break;
        case TIME_SPEED_VERY_FAST:
            gfx_SetColor(1);
            gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
            gfx_FillTriangle_NoClip(110, 225, 110, 233, 114, 229);
            gfx_FillTriangle_NoClip(115, 225, 115, 233, 119, 229);
            break;
    }
    gfx_SetColor(11);
}

/**
 * @brief Print the text of the HUD
 * 
 * @param player 
 * @param time 
 * @param camera 
 * @param settings 
 * @param galaxy 
 */
static void hud_PrintInfos( Empire *player, 
                            Time *time, 
                            Camera *camera,
                            Settings *settings,
                            StarSystem **galaxy){
    char dayString[11];
    char monthString[8];
    char yearString[5];
    int16_t system = 0;

    // top bar
    
    // credit
    gfx_TransparentSprite_NoClip(   icon_credit, 
                                    HUD_UP_BAR_LEFT,
                                    HUD_UP_BAR_UP);

    hud_RessourcePrint(   empire_CreditGet(player), 
                empire_CreditVariationGet(player), 
                HUD_UP_BAR_LEFT - 15, 
                HUD_UP_BAR_TEXT_X);

    // minerals
    gfx_TransparentSprite_NoClip(   icon_mineral, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP, 
                                    1);

    hud_RessourcePrint(   empire_MineralsGet(player), 
                empire_MineralsVariationGet(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP - 15, 
                HUD_UP_BAR_TEXT_X);

    // food
    gfx_TransparentSprite_NoClip(   icon_food, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 2, 
                                    HUD_UP_BAR_UP);

    hud_RessourcePrint(   empire_FoodGet(player), 
                empire_FoodVariationGet(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 2 - 15, 
                HUD_UP_BAR_TEXT_X);

    // alloy
    gfx_TransparentSprite_NoClip(   icon_alloy, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 3, 
                                    HUD_UP_BAR_UP);

    hud_RessourcePrint(   empire_AlloysGet(player), 
                empire_AlloysVariationGet(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 3 - 15, 
                HUD_UP_BAR_TEXT_X);
    

    
    // side bar
    gfx_TransparentSprite_NoClip(   icon_contact, 
                                    (HUD_WIDTH - icon_contact_width) / 2, 
                                    HUD_SIDE_BAR_UP);

    gfx_TransparentSprite_NoClip(   icon_market, 
                                    (HUD_WIDTH - icon_market_width) / 2, 
                                    HUD_SIDE_BAR_UP + HUD_SIDE_BAR_GAP);

    gfx_TransparentSprite_NoClip(   icon_science, 
                                    (HUD_WIDTH - icon_science_width) / 2, 
                                    HUD_SIDE_BAR_UP + HUD_SIDE_BAR_GAP * 2);

    gfx_TransparentSprite_NoClip(   icon_fleet_allied, 
                                    (HUD_WIDTH - icon_fleet_allied_width) / 2, 
                                    HUD_SIDE_BAR_UP + HUD_SIDE_BAR_GAP * 3);
    


    // bottom bar

    // time
    if (time_DayGet(time) < 10) {
        sprintf(dayString, "0%d.", time_DayGet(time));
    } else {
        sprintf(dayString, "%d.", time_DayGet(time));
    }
    if (time_MonthGet(time) < 10) {
        sprintf(monthString, "0%d.", time_MonthGet(time));
    } else {
        sprintf(monthString, "%d.", time_MonthGet(time));
    }
    sprintf(yearString, "%d", time_YearGet(time));

    strcat(monthString, yearString);
    strcat(dayString, monthString);

    gfx_PrintStringXY(  dayString, 
                        LCD_WIDTH / 2 - 4 * TEXT_HEIGHT, 
                        LCD_HEIGHT - HUD_TIME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);

    if(camera_FleetMoveGet(camera)){
        gfx_SetColor(COLOR_HUD_BACKGROUND);
        gfx_FillRectangle_NoClip(80, 28, 160, 11);
        gfx_SetColor(COLOR_HUD_OUTLINES);
        gfx_Rectangle_NoClip(80, 28, 160, 11);
        gfx_PrintStringXY("Choisir destination", 84, 30);
    }

    // name of the system
    if ((camera_SystemActualGet(camera) != NO_SYSTEM) || (camera_MapTypeGet(camera) == VUE_SYSTEM)) {
        if (camera_MapTypeGet(camera) == VUE_GALAXY_LARGE) {
            system = camera_SystemActualGet(camera);
        } else {
            system = camera_SystemAimedGet(camera);
        }
        if ((starSystem_IntelLevelGet(galaxy[system]) == INTEL_UNKNOWN) && (!settings_SeeAllGet(settings))) {
            gfx_PrintStringXY(  "Inconnu", 
                                132,
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);
        } else {
            gfx_PrintStringXY(  starSystem_NameGet(galaxy[system]), 
                                LCD_WIDTH / 2 - strlen(starSystem_NameGet(galaxy[system])) * TEXT_HEIGHT / 2, 
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);
        }
        #ifdef DEBUG_VERSION
        gfx_SetTextXY(100, 100);
        gfx_PrintUInt(system, 3);
        gfx_PrintString(" ");
        gfx_PrintUInt(starSystem_IntelLevelGet(galaxy[system]), 3);
        #endif
    }

    // print fps
    #ifdef DEBUG_FPS
    gfx_SetTextXY(LCD_WIDTH - 2 * TEXT_HEIGHT, 1);
    gfx_PrintUInt(time_FPSGet(time), 2);
    char buffer[20];
    sprintf(buffer, "%f", time_UpdateTimeGet(time));
    gfx_PrintStringXY(buffer, LCD_WIDTH - 8 * TEXT_HEIGHT, TEXT_HEIGHT + 2);
    sprintf(buffer, "%f", time_ScreenTimeGet(time));
    gfx_PrintStringXY(buffer, LCD_WIDTH - 9 * TEXT_HEIGHT, TEXT_HEIGHT * 2 + 3);
    sprintf(buffer, "%lu", ramGet());
    gfx_PrintStringXY(buffer, LCD_WIDTH - 5 * TEXT_HEIGHT, TEXT_HEIGHT * 3 + 4);
    #endif

    hud_DrawSpeedIcon(time);
}




/**
 * @brief Draw the pointer at the center of the screen
 * 
 * @param camera 
 */
static void hud_DrawPointer(Camera *camera) {
    //pointeur
    if (camera_MapTypeGet(camera) == VUE_GALAXY_LARGE) {
        gfx_Line_NoClip(camera_XGet(camera) / 2.5 + 5, camera_YGet(camera) / 2.5 - 30, camera_XGet(camera) / 2.5 + 15, camera_YGet(camera) / 2.5 - 30);
        gfx_Line_NoClip(camera_XGet(camera) / 2.5 + 10, camera_YGet(camera) / 2.5 - 25, camera_XGet(camera) / 2.5 + 10, camera_YGet(camera) / 2.5 - 35);
        gfx_Circle_NoClip(camera_XGet(camera) / 2.5 + 10, camera_YGet(camera) / 2.5 - 30, 3);
    } else {
        gfx_Line_NoClip(150, 120, 170, 120);
        gfx_Line_NoClip(160, 110, 160, 130);
        gfx_FillCircle_NoClip(160, 120, 2);
        gfx_Circle_NoClip(160, 120, 4);
        gfx_Circle_NoClip(160, 120, 8);
    }
}


/**
 * @brief Function to draw the shapes of the screen
 * 
 * @param time
 * @param camera
 */
static void hud_DrawShapes( Time *time, 
                            Camera *camera){
    //draw the background
    gfx_SetColor(COLOR_HUD_BACKGROUND);
    //top bar
    gfx_FillRectangle(0, 0 , LCD_WIDTH, HUD_WIDTH);
    //side bar
    gfx_FillRectangle(0, HUD_WIDTH, HUD_WIDTH, HUD_HEIGHT - HUD_WIDTH);
    gfx_FillTriangle(0, HUD_HEIGHT, HUD_WIDTH, HUD_HEIGHT, 0, HUD_HEIGHT + HUD_WIDTH);
    
    //bottom bar (time)
    if (speed_TimeGet(time) == 0) {
        gfx_SetColor(COLOR_HUD_PAUSE_BACKGROUND);
    }
    
    gfx_FillRectangle(	HUD_TIME_BAR_OFFSET,                        // x
                        LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,           // y
                        HUD_TIME_BAR_WIDTH,                         // width
                        HUD_TIME_BAR_HEIGHT);                       // height
    
    gfx_FillTriangle(	HUD_TIME_BAR_OFFSET,                        // x up   right
                        LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,           // y up   right
                        HUD_TIME_BAR_OFFSET - HUD_TIME_BAR_HEIGHT,  // x down left
                        LCD_HEIGHT,                                 // y down left
                        HUD_TIME_BAR_OFFSET,                        // x down right
                        LCD_HEIGHT);                                // y down right
                        
    gfx_FillTriangle(	LCD_WIDTH - HUD_TIME_BAR_OFFSET,            // x up   left
                        LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,           // y up   left
                        LCD_WIDTH - HUD_TIME_BAR_OFFSET + HUD_TIME_BAR_HEIGHT,  // x down right
                        LCD_HEIGHT,                                 // y down right
                        LCD_WIDTH - HUD_TIME_BAR_OFFSET,            // x down left
                        LCD_HEIGHT);                                // y down left
    
    // name above the bottom bar
    if((camera_SystemActualGet(camera) != NO_SYSTEM) || (camera_MapTypeGet(camera) == VUE_SYSTEM)) {
        gfx_FillRectangle(  HUD_TIME_BAR_OFFSET + HUD_NAME_BAR_HEIGHT,              // x
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT, // y
                            HUD_TIME_BAR_WIDTH - 2 * HUD_NAME_BAR_HEIGHT,           // width
                            HUD_NAME_BAR_HEIGHT);                                   // height
                            
        gfx_FillTriangle(   HUD_TIME_BAR_OFFSET + HUD_NAME_BAR_HEIGHT,              // x up   right
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT, // y up   right
                            HUD_TIME_BAR_OFFSET + HUD_NAME_BAR_HEIGHT,              // x down right
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,                       // y down right
                            HUD_TIME_BAR_OFFSET,                                    // x down left
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT);                      // y down left

        gfx_FillTriangle(   LCD_WIDTH - HUD_TIME_BAR_OFFSET - HUD_NAME_BAR_HEIGHT,  // x up   left
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT, // y up   left
                            LCD_WIDTH - HUD_TIME_BAR_OFFSET - HUD_NAME_BAR_HEIGHT,  // x down left
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,                       // y down left
                            LCD_WIDTH - HUD_TIME_BAR_OFFSET,                        // x down right
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT);                      // y down right
    }

    //draw the outlines of the hud
    gfx_SetColor(COLOR_HUD_OUTLINES);
    //top bar
    gfx_HorizLine_NoClip(HUD_WIDTH, HUD_WIDTH, LCD_WIDTH - HUD_WIDTH);
    //side bar
    gfx_VertLine_NoClip(HUD_WIDTH, HUD_WIDTH, HUD_HEIGHT - HUD_WIDTH);
    gfx_Line_NoClip(HUD_WIDTH, HUD_HEIGHT, 0, HUD_HEIGHT + HUD_WIDTH);



    //down bar
    if (speed_TimeGet(time) == 0) {
        gfx_SetColor(COLOR_HUD_PAUSE_OUTLINES);
    }
    gfx_HorizLine_NoClip((  LCD_WIDTH - HUD_TIME_BAR_WIDTH)/2,      // x
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,       // y
                            HUD_TIME_BAR_WIDTH);                    // width
    
    gfx_Line_NoClip(    HUD_TIME_BAR_OFFSET,                        // x up   right
                        LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,           // y up   right
                        HUD_TIME_BAR_OFFSET - HUD_TIME_BAR_HEIGHT,  // x down left
                        LCD_HEIGHT);                                // y down left

    gfx_Line_NoClip(    LCD_WIDTH - HUD_TIME_BAR_OFFSET,            // x up   right
                        LCD_HEIGHT - HUD_TIME_BAR_HEIGHT,           // y up   right
                        LCD_WIDTH - HUD_TIME_BAR_OFFSET + HUD_TIME_BAR_HEIGHT,  // x up   right
                        LCD_HEIGHT);                                // y up   right
    
    //name above the bottom bar
    if ((camera_SystemActualGet(camera) != NO_SYSTEM) || (camera_MapTypeGet(camera) == VUE_SYSTEM)) {
        gfx_HorizLine_NoClip(   HUD_TIME_BAR_OFFSET + HUD_NAME_BAR_HEIGHT,              // x
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT, // y
                                HUD_TIME_BAR_WIDTH - 2 * HUD_NAME_BAR_HEIGHT);          // width

        gfx_Line_NoClip(    HUD_TIME_BAR_OFFSET + HUD_NAME_BAR_HEIGHT,                  // x up   right
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT,     // y up   right
                            HUD_TIME_BAR_OFFSET,                                        // x down left
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT);                          // y down left

        gfx_Line_NoClip(    LCD_WIDTH - HUD_TIME_BAR_OFFSET - HUD_NAME_BAR_HEIGHT,      // x up   left
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT,     // y up   left
                            LCD_WIDTH - HUD_TIME_BAR_OFFSET,                            // x down right
                            LCD_HEIGHT - HUD_TIME_BAR_HEIGHT);                          // y down right
    }

    //draw flag
    hud_DrawFlag();
}

/* entry points ======================================================== */

int hud_Draw(   char *key, 
                EmpireList *empireList, 
                Time *time,
                Camera *camera, 
                StarSystem **galaxy, 
                WindowManager *window, 
                Settings *settings, 
                NotificationList *notificationList) {
    Empire *player = empire_Get(empireList, 0);
    
    hud_DrawShapes(time, camera);

    hud_PrintInfos(player, time, camera, settings, galaxy);
    
    hud_DrawPointer(camera);
    
    DrawNotifications(notificationList, time);

    if(window_CommandPromptStatusGet(window)){ 
        console_show(key, window, empireList, camera, time, settings);
    }
 
    return 0;
}
