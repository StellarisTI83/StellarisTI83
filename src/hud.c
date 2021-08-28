/**
 * @file hud.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief The hud drawing routines
 * @version 0.1
 * @date 2021-08-21
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
static void PrintHUD(   const unsigned int currentState, 
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
 * @param date 
 */
static void hud_DrawSpeedIcon(Time *date){
    //pause / avance
    switch (speed_TimeGet(date)){
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
 * @param systemeStellaires 
 */
static void hud_PrintInfos( Empire *player, 
                            Time *time, 
                            Camera *camera,
                            Settings *settings,
                            StarSystem **systemeStellaires){
    char dayString[11];
    char monthString[8];
    char yearString[5];
    int16_t system = 0;

    // top bar
    
    // credit
    gfx_TransparentSprite_NoClip(   credit, 
                                    HUD_UP_BAR_LEFT,
                                    HUD_UP_BAR_UP);

    PrintHUD(   empire_CreditGet(player), 
                empire_CreditVariationGet(player), 
                HUD_UP_BAR_LEFT - 15, 
                HUD_UP_BAR_TEXT_X);

    // minerals
    gfx_TransparentSprite_NoClip(   minerai, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP, 
                                    1);

    PrintHUD(   empire_MineralsGet(player), 
                empire_MineralsVariationGet(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP - 15, 
                HUD_UP_BAR_TEXT_X);

    // food
    gfx_TransparentSprite_NoClip(   food, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 2, 
                                    HUD_UP_BAR_UP);

    PrintHUD(   empire_FoodGet(player), 
                empire_FoodVariationGet(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 2 - 15, 
                HUD_UP_BAR_TEXT_X);

    // alloy
    gfx_TransparentSprite_NoClip(   fer, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 3, 
                                    HUD_UP_BAR_UP);

    PrintHUD(   empire_AlloysGet(player), 
                empire_AlloysVariationGet(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 3 - 15, 
                HUD_UP_BAR_TEXT_X);
    

    
    // side bar
    gfx_TransparentSprite_NoClip(   contact, 
                                    (HUD_WIDTH - contact_width) / 2, 
                                    HUD_SIDE_BAR_UP);

    gfx_TransparentSprite_NoClip(   market_icon, 
                                    (HUD_WIDTH - market_icon_width) / 2, 
                                    HUD_SIDE_BAR_UP + HUD_SIDE_BAR_GAP);

    gfx_TransparentSprite_NoClip(   science, 
                                    (HUD_WIDTH - science_width) / 2, 
                                    HUD_SIDE_BAR_UP + HUD_SIDE_BAR_GAP * 2);

    gfx_TransparentSprite_NoClip(   alliedFleet, 
                                    (HUD_WIDTH - alliedFleet_width) / 2, 
                                    HUD_SIDE_BAR_UP + HUD_SIDE_BAR_GAP * 3);
    


    // bottom bar

    // date
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

    if(IsCameraMoveFleet(camera)){
        gfx_SetColor(COLOR_HUD_BACKGROUND);
        gfx_FillRectangle_NoClip(80, 28, 160, 11);
        gfx_SetColor(COLOR_HUD_OUTLINES);
        gfx_Rectangle_NoClip(80, 28, 160, 11);
        gfx_PrintStringXY("Choisir destination", 84, 30);
    }

    // name of the system
    if ((camera_SystemActualGet(camera) != NO_SYSTEM) || (camera_MapTypeGet(camera) == VUE_SYSTEM)) {
        if (camera_MapTypeGet(camera) == VUE_GALACTIC) {
            system = camera_SystemActualGet(camera);
        } else {
            system = camera_SystemAimedGet(camera);
        }
        if ((starSystem_IntelLevelGet(systemeStellaires[system]) == INTEL_UNKNOWN) && (!settings_SeeAllGet(settings))) {
            gfx_PrintStringXY(  "Inconnu", 
                                132,
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);
        } else {
            gfx_PrintStringXY(  starSystem_NameGet(systemeStellaires[system]), 
                                LCD_WIDTH / 2 - strlen(starSystem_NameGet(systemeStellaires[system])) * TEXT_HEIGHT / 2, 
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);
        }
        gfx_SetTextXY(100, 100);
        gfx_PrintUInt(system, 3);
        gfx_PrintString(" ");
        gfx_PrintUInt(starSystem_IntelLevelGet(systemeStellaires[system]), 3);
    }

    // print fps
    gfx_SetTextXY(LCD_WIDTH - 2 * TEXT_HEIGHT, 1);
    gfx_PrintUInt(time_FPSGet(time), 2);
    gfx_SetTextXY(LCD_WIDTH - 8 * TEXT_HEIGHT, TEXT_HEIGHT + 2);
    gfx_PrintUInt(camera_XSystemGet(camera), 3);
    gfx_PrintString(" ");
    gfx_PrintUInt(camera_YSystemGet(camera), 3);

    hud_DrawSpeedIcon(time);
}




/**
 * @brief Draw the pointer at the center of the screen
 * 
 * @param camera 
 */
static void hud_DrawPointer(Camera *camera) {
    //pointeur
    if (camera_MapTypeGet(camera) == VUE_MAP) {
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
 * @param date
 * @param camera
 */
static void hud_DrawShapes( Time *date, 
                            Camera *camera){
    //draw the background
    gfx_SetColor(COLOR_HUD_BACKGROUND);
    //top bar
    gfx_FillRectangle(0, 0 , LCD_WIDTH, HUD_WIDTH);
    //side bar
    gfx_FillRectangle(0, HUD_WIDTH, HUD_WIDTH, HUD_HEIGHT - HUD_WIDTH);
    gfx_FillTriangle(0, HUD_HEIGHT, HUD_WIDTH, HUD_HEIGHT, 0, HUD_HEIGHT + HUD_WIDTH);
    
    //bottom bar (date)
    if (speed_TimeGet(date) == 0) {
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
    if (speed_TimeGet(date) == 0) {
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

/**
 * @brief Function to draw the HUD on the screen
 * 
 * @param empireListe EmpireList*
 * @param date Date*
 * @param key char*
 * @param camera Camera*
 * @param systemeStellaires SystemeStellaire*
 * @param fenetre Window*
 * @param settings Settings*
 * @param notificationList NotificationList*
 * @return int 
 */
int hud_Draw(   EmpireList *empireListe, 
                Time *date, 
                char *key, 
                Camera *camera, 
                StarSystem **systemeStellaires, 
                Window *fenetre, 
                Settings *settings, 
                NotificationList *notificationList)
{
    Empire *player = empire_Get(empireListe, 0);
    
    hud_DrawShapes(date, camera);

    hud_PrintInfos(player, date, camera, settings, systemeStellaires);
    
    hud_DrawPointer(camera);
    
    DrawNotifications(notificationList, date);

    if(GetCommandPromptStatus(fenetre) == true){ 
        AfficherConsole(key, fenetre, empireListe, camera, date, settings);
    }
 
    return 0;
}
