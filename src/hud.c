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
    switch (GetTimeSpeed(date)){
        case -2:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillRectangle_NoClip(100, 225, 3, 9);
            gfx_FillTriangle_NoClip(109, 225, 109, 233, 105, 229);
            gfx_FillTriangle_NoClip(114, 225, 114, 233, 110, 229);
            break;
        case -1:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillRectangle_NoClip(100, 225, 3, 9);
            gfx_FillTriangle_NoClip(109, 225, 109, 233, 105, 229);
            break;
        case 0:
            gfx_SetColor(COLOR_HUD_PAUSE_OUTLINES);
            gfx_FillRectangle_NoClip(105, 225, 3, 9);
            gfx_FillRectangle_NoClip(110, 225, 3, 9);
            break;
        case 1:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
            break;	
        case 2:
            gfx_SetColor(COLOR_WHITE);
            gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
            gfx_FillTriangle_NoClip(110, 225, 110, 233, 114, 229);
            break;
        case 3:
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
 * @param date 
 * @param camera 
 * @param settings 
 * @param systemeStellaires 
 */
static void hud_PrintInfos( Empire *player, 
                            Time *date, 
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

    PrintHUD(   GetEmpireCredit(player), 
                GetEmpireCreditChange(player), 
                HUD_UP_BAR_LEFT - 15, 
                HUD_UP_BAR_TEXT_X);

    // minerals
    gfx_TransparentSprite_NoClip(   minerai, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP, 
                                    1);

    PrintHUD(   GetEmpireMinerals(player), 
                GetEmpireMineralsChange(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP - 15, 
                HUD_UP_BAR_TEXT_X);

    // food
    gfx_TransparentSprite_NoClip(   food, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 2, 
                                    HUD_UP_BAR_UP);

    PrintHUD(   GetEmpireFood(player), 
                GetEmpireFoodChange(player), 
                HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 2 - 15, 
                HUD_UP_BAR_TEXT_X);

    // alloy
    gfx_TransparentSprite_NoClip(   fer, 
                                    HUD_UP_BAR_LEFT + HUD_UP_BAR_GAP * 3, 
                                    HUD_UP_BAR_UP);

    PrintHUD(   GetEmpireAlloys(player), 
                GetEmpireAlloysChange(player), 
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
    if (GetTimeDay(date) < 10) {
        sprintf(dayString, "0%d.", GetTimeDay(date));
    } else {
        sprintf(dayString, "%d.", GetTimeDay(date));
    }
    if (GetTimeMonth(date) < 10) {
        sprintf(monthString, "0%d.", GetTimeMonth(date));
    } else {
        sprintf(monthString, "%d.", GetTimeMonth(date));
    }
    sprintf(yearString, "%d", GetTimeYear(date));

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
    if ((GetCameraViewedSystem(camera) != -1) || (GetCameraMapType(camera) == SYSTEME)) {
        if (GetCameraMapType(camera) == NORMAL) {
            system = GetCameraViewedSystem(camera);
        } else {
            system = GetCameraSystem(camera);
        }
        if ((GetSystemIntelLevel(systemeStellaires[system]) == INCONNU) && (!settings_SeeAllGet(settings))) {
            gfx_PrintStringXY(  "Inconnu", 
                                132, 
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);
        } else {
            gfx_PrintStringXY(  GetSystemName(systemeStellaires[system]), 
                                LCD_WIDTH / 2 - strlen(GetSystemName(systemeStellaires[system])) * TEXT_HEIGHT / 2, 
                                LCD_HEIGHT - HUD_TIME_BAR_HEIGHT - HUD_NAME_BAR_HEIGHT / 2 - TEXT_HEIGHT / 2);
        }
    }

    hud_DrawSpeedIcon(date);
}




/**
 * @brief Draw the pointer at the center of the screen
 * 
 * @param camera 
 */
static void hud_DrawPointer(Camera *camera) {
    //pointeur
    if (GetCameraMapType(camera) == CARTE) {
        gfx_Line_NoClip(GetCameraX(camera) / 2.5 + 5, GetCameraY(camera) / 2.5 - 30, GetCameraX(camera) / 2.5 + 15, GetCameraY(camera) / 2.5 - 30);
        gfx_Line_NoClip(GetCameraX(camera) / 2.5 + 10, GetCameraY(camera) / 2.5 - 25, GetCameraX(camera) / 2.5 + 10, GetCameraY(camera) / 2.5 - 35);
        gfx_Circle_NoClip(GetCameraX(camera) / 2.5 + 10, GetCameraY(camera) / 2.5 - 30, 3);
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
    if (GetTimeSpeed(date) == 0) {
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
    
    //name above the bottom bar
    if((GetCameraViewedSystem(camera) != -1) || (GetCameraMapType(camera) == SYSTEME)) {
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
    if (GetTimeSpeed(date) == 0) {
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
    if ((GetCameraViewedSystem(camera) != -1) || (GetCameraMapType(camera) == SYSTEME)) {
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
 * @param empireListe EmpireListe*
 * @param date Date*
 * @param key char*
 * @param camera Camera*
 * @param systemeStellaires SystemeStellaire*
 * @param fenetre Window*
 * @param settings Settings*
 * @param notificationList NotificationList*
 * @return int 
 */
int hud_Draw(   EmpireListe *empireListe, 
                Time *date, 
                char *key, 
                Camera *camera, 
                StarSystem **systemeStellaires, 
                Window *fenetre, 
                Settings *settings, 
                NotificationList *notificationList)
{
    Empire *player = EmpireNumero(empireListe, 0);
    
    hud_DrawShapes(date, camera);

    hud_PrintInfos(player, date, camera, settings, systemeStellaires);
    
    hud_DrawPointer(camera);
    
    DrawNotifications(notificationList, date);

    if(GetCommandPromptStatus(fenetre) == true){ 
        AfficherConsole(key, fenetre, empireListe, camera, date, settings);
    }
 
    return 0;
}
