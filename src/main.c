#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include <compression.h>

#include "fonts/Standard.h"
#include "gfx/gfx.h"
#include "colors.h"

#include "main.h"
#include "ai.h"
#include "settings.h"
#include "dlc.h"
#include "new_game.h"

#include "locale/locale.h"

/* macros ============================================================== */

#define MAIN_MENU_LOAD 0
#define MAIN_MENU_NEW_GAME 1
#define MAIN_MENU_SETTINGS 2
#define MAIN_MENU_EXIT 3

#define MAIN_MENU_CHOICE_VERTICAL_GAP 15

/* structures ========================================================== */

typedef struct {
    int x;
    int y;
    double yVector;
    bool descend;
} StationVector;

/* private functions =================================================== */

/**
 * @brief Draw all the background
 * 
 */
static void mainMenu_DrawBackground(gfx_sprite_t **tile){
    int tileIndex = 0;
    int x = 0, y = 0;
    
    for (; tileIndex < 12; tileIndex++) {
        gfx_Sprite_NoClip(tile[tileIndex], x, y);
        x += background0_tile_width;
        if (x >= LCD_WIDTH) {
            y += background0_tile_height;
            x = 0;
        }
    }
}


/**
 * @brief Draw the station of the main menu
 * 
 * @param stationVector 
 * @param station 
 */
static void mainMenu_DrawStation(StationVector **stationVector, gfx_sprite_t *station){
    if((*stationVector)->yVector >= M_PI)
        (*stationVector)->descend = true;
    else if((*stationVector)->yVector <= -M_PI)
        (*stationVector)->descend = false;

    if((*stationVector)->descend)
        (*stationVector)->yVector -= 0.05;
    else
        (*stationVector)->yVector += 0.05;
    
    (*stationVector)->y = 120 + cos((*stationVector)->yVector) * 5;

    gfx_TransparentSprite_NoClip(station, (*stationVector)->x, (*stationVector)->y);
}


/**
 * @brief Alloc the station of the main menu
 * 
 * @param stationVector 
 * @param station 
 */
static void mainMenu_StationCreate(StationVector **stationVector, gfx_sprite_t **station) {
    *stationVector = malloc(sizeof(StationVector));
    (*stationVector)->x = 206;
    (*stationVector)->y = 120;
    (*stationVector)->yVector = 0;
    (*stationVector)->descend = false;


    // alloc the place for the station tile
    *station = gfx_MallocSprite(	backgroundStation_height,
                                backgroundStation_width);
                                
    zx7_Decompress(*station, backgroundStation_compressed);
}

/**
 * @brief Free the station of the main menu
 * 
 * @param stationVector 
 */
static void mainMenu_StationFree(StationVector **stationVector, gfx_sprite_t **station) {
    free(*station);
    free(*stationVector);
}

// FIXME Background don't work

/**
 * @brief Draw the Main Menu of the game 
 * 
 * @return int The choice
 */
static int mainMenu_Draw(){
    gfx_sprite_t *station = NULL;
    // gfx_sprite_t *tile[12] = {NULL};
    // gfx_sprite_t *tmp_ptr;
    int loop = true;
    char key;
    StationVector *stationVector = NULL;
    int selector = 0, yLevel = 100;
    int index = 0;
    char string[20];

    // Alloc the place and decompress the background tiles
    // index = 0;
    // while(index < 12) {
    //     tmp_ptr = gfx_MallocSprite( background0_tile_width, 
    //                                 background0_tile_height);
    //     zx7_Decompress( tmp_ptr,
    //                     var_gfx_appvar[var_gfx_background_gfx_sprites_backgroundStation_compressed_index + 1 + index]);
    //     tile[index] = tmp_ptr;
    //     index++;
    // }
    
    // Initialize the station
    mainMenu_StationCreate(&stationVector, &station);

    // set the palette for the main menu
    gfx_SetPalette(background_gfx_pal, sizeof_background_gfx_pal, 0);
    
    // Draw the background
    gfx_SetTransparentColor(COLOR_TRANSPARENT);
    while(loop) {
        key = os_GetCSC();

        // Draw the station and background
        // mainMenu_DrawBackground(tile);
        gfx_ZeroScreen();
        mainMenu_DrawStation(&stationVector, station);
        
        // Print the title of the game
        mainMenu_PrintSized("Stellaris", 30, 3, COLOR_WHITE, -30);
        gfx_SetTextXY(5, 220);
        gfx_PrintString(VERSION_GAME);
        #ifdef DEBUG_VERSION
        gfx_PrintString(" Debug");
        #endif

        
        // Print the choices of the menu
        yLevel = 100;
        for(index = 0; index < 4; index++) {
            if(index == selector) { // If it is selected
                gfx_SetTextFGColor(COLOR_YELLOW);
                gfx_SetColor(COLOR_YELLOW);
            } else {				// If it is not selected
                gfx_SetTextFGColor(COLOR_WHITE);
                gfx_SetColor(COLOR_WHITE);
            }
            switch(index) {
                case 0:
                    strcpy(string, _(lc_load));
                    break;
                case 1:
                    strcpy(string, _(lc_new_game));
                    break;
                case 2:
                    strcpy(string, _(lc_settings));
                    break;
                case 3:
                    strcpy(string, _(lc_exit));
                    break;
            }
            gfx_PrintStringXY(string, 5, yLevel);
            gfx_HorizLine_NoClip(5, yLevel - 3, strlen(string) * 8);
            yLevel += MAIN_MENU_CHOICE_VERTICAL_GAP;
        }

        // Test the keys
        switch(key) {
            case sk_Clear:
                loop = false;
                break;
            case sk_Down:
                selector++;
                break;
            case sk_Up:
                selector--;
                break;
            case sk_Enter:
                return selector;
                break;
            default:
                break;
        }
        if(selector > MAIN_MENU_EXIT)
            selector = MAIN_MENU_LOAD;
        if(selector < MAIN_MENU_LOAD)
            selector = MAIN_MENU_EXIT;
        gfx_SwapDraw();
    }
    // index = 0;
    // while(index < 12) {
    //     free(tile[index]);
    //     index++;
    // }
    mainMenu_StationFree(&stationVector, &station);
    return MAIN_MENU_EXIT;
}


// FIXME Background don't work

/**
 * @brief 
 * 
 * @return int 
 */
static int mainMenu_Settings(){
    gfx_sprite_t *station = NULL;
    // gfx_sprite_t *tile[12];
    int loop = true;
    char key;
    StationVector *stationVector = NULL;
    int selector = 0, yLevel = 100;
    int index = 0;
    char language = getLanguage();
    char* languageStr = "Francais";

    // alloc the place and decompress the background tiles
    // for (index = 0; index < 12; index++) {
    //     tile[index] = gfx_MallocSprite(background0_tile_width, 
    //                         background0_tile_height);
    //     zx7_Decompress(	tile[index], 
    //                     var_gfx_appvar[var_gfx_background_gfx_sprites_backgroundStation_compressed_index + 1 + index]);
    // }

    // Initialize the station
    mainMenu_StationCreate(&stationVector, &station);

    while(loop) {
        key = os_GetCSC();

        // Draw the station and background
        // mainMenu_DrawBackground(tile);
        gfx_ZeroScreen();
        mainMenu_DrawStation(&stationVector, station);

        // Print the version of the game
        mainMenu_PrintSized(_(lc_settings), 30, 2, COLOR_WHITE, 0);
        gfx_SetTextXY(5, 220);
        gfx_PrintString(VERSION_GAME);
        #ifdef DEBUG_VERSION
        gfx_PrintString(" Debug");
        #endif

        switch(language){
            case LC_FR:
                languageStr = "Francais";
                break;
            case LC_EN:
                languageStr = "English";
                break;
        }
        
        // Print the left part of the options
        yLevel = 80;
        for(index = 0; index < 1; index++) {
            gfx_SetTextFGColor(COLOR_WHITE);
            gfx_SetColor(COLOR_WHITE);
            gfx_SetTextXY(10, yLevel);
            switch(index) {
                case 0:
                    gfx_PrintString("Language");
                    break;
            }
            yLevel += MAIN_MENU_CHOICE_VERTICAL_GAP;
        }
        
        // Print the changing part of the options
        yLevel = 80;
        for(index = 0; index < 1; index++) {
            if(index == selector) { // If it is selected
                gfx_SetTextFGColor(COLOR_YELLOW);
                gfx_SetColor(COLOR_YELLOW);
            } else {				// If it is not selected
                gfx_SetTextFGColor(COLOR_WHITE);
                gfx_SetColor(COLOR_WHITE);
            }
            switch(index) {
                case 0:
                    gfx_PrintStringXY("<", LCD_WIDTH / 2 - TEXT_HEIGHT, yLevel);
                    gfx_PrintStringXY(">", (LCD_WIDTH / 2) + TEXT_HEIGHT * 10, yLevel);
                    gfx_PrintStringXY(languageStr, (LCD_WIDTH / 2) + TEXT_HEIGHT * 5 - TEXT_HEIGHT * strlen(languageStr)/2, yLevel);
                    break;
            }
            yLevel += MAIN_MENU_CHOICE_VERTICAL_GAP;
        }

        // Test the keys
        switch(key) {
            case sk_Clear:
                loop = false;
                break;
            case sk_Down:
                selector++;
                break;
            case sk_Up:
                selector--;
                break;
            case sk_Left:
                language--;
                break;
            case sk_Right:
                language++;
                break;
            case sk_Enter:
                switch(selector){
                    case 0:
                        setLanguage(language);
                        break;
                    default:
                        language = LC_EN;
                        setLanguage(language);
                        break;
                }
                break;
            default:
                break;
        }
        if(selector > 0)
            selector = 0;
        if(selector < 0)
            selector = 0;
        if(language > LC_EN)
            language = LC_FR;
        if(language < LC_FR)
            language = LC_EN;
        gfx_SetTextFGColor(COLOR_WHITE);
        gfx_SetColor(COLOR_WHITE);
        gfx_SwapDraw();
    }
    // for (index = 0; index < 12; index++) {
    //     free(tile[index]);
    // }
    mainMenu_StationFree(&stationVector, &station);
    return MAIN_MENU_EXIT;
}

/**
 * @brief Initialize the 
 * 
 * @return int 0 if no error,  
 */
static int main_InitializeAll(){
    if(!var_gfx_init()){
        os_SetCursorPos(1, 1);
        os_PutStrFull("Missing var_gfx.8xv");
        os_SetCursorPos(2, 1);
        os_PutStrFull("Plase download it");
        while(!os_GetCSC());

        return true;
    }

    ti_CloseAll();
    
    srandom(rtc_Time());
    
    gfx_Begin();
    gfx_SetDrawBuffer();

    // Set the palette for the main menu
    gfx_SetPalette(background_gfx_pal, sizeof_background_gfx_pal, 0);

    gfx_SetTextBGColor(COLOR_TRANSPARENT);
    gfx_SetTextFGColor(COLOR_WHITE);
    gfx_SetColor(COLOR_WHITE);

    gfx_SetFontData(font_logo);
    gfx_SetMonospaceFont(TEXT_HEIGHT);

    gfx_SetTextTransparentColor(COLOR_TRANSPARENT);

    setLanguage(LC_FR);
    return false;
}

/* entry points ======================================================== */

void mainMenu_PrintSized(const char *str, int y, int taille, int color, int xOffset){
    int x, a, i;
    gfx_TempSprite(ch, 8, 8);

    gfx_SetTextFGColor(color);
    
    x = (LCD_WIDTH - strlen(str) * 8 * taille)/2 + xOffset;
    a = 1;
    i = 0;
    
    // Each character is put in a sprite and then sized
    while (a != 0)
    {
        ch = gfx_GetSpriteChar(str[i]);
        gfx_ScaledTransparentSprite_NoClip(ch, x, y, taille, taille);
        a = str[i];
        x += 8 * taille;
        i++;
    }
}

void mainMenu_PrintInt(int number){
    if(number < 10){
        gfx_PrintInt(number, 1);
    }
    else if(number < 100){
        gfx_PrintInt(number, 2);
    }
    else if(number < 1000){
        gfx_PrintInt(number, 3);
    }
    else if(number < 10000){
        gfx_PrintInt(number, 4);
    }
    else  if(number < 100000){
        gfx_PrintInt(number, 5);
    }
    else{
        gfx_PrintInt(number, 6);		
    }
}

int mainMenu_IntLen(int number){
    int i = 1;
    if(number < 0){
        number = -number;
    }
    while(number >= 1){
        number /= 10;
        i++;
    }
    return i;
}

void mainMenu_PrintMultipleLines(char *str) {
    int init_size = strlen(str);
    char *string1;
    char delim[] = " ";
    int lineSize = 0;
    int x = gfx_GetTextX();

    char *ptr = NULL;

    string1 = calloc(1, init_size + 1);
    strcpy(string1, str);
    ptr = strtok(string1, delim);
    while(ptr != NULL) {
        lineSize += strlen(string1) + 1;
        if (lineSize > 20) {
            gfx_SetTextXY(x, gfx_GetTextY() + 13);
            lineSize = 0;
        }
        gfx_PrintString(ptr);
        ptr = strtok(string1, delim);
    }
    free(string1);
}

/**
 * @brief Entry point of the software
 * 
 * @return int 
 */

int main(void){
    char loop = false;
    char choice = 0;
    FleetTemplateListe *fleetTemplateList = NULL;
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "\n*********************");
        dbg_sprintf(dbgout, "\n* Started Stellaris");
        dbg_sprintf(dbgout, "\n* %s", VERSION_GAME);
        dbg_sprintf(dbgout, "\n* Debug activated");
        dbg_sprintf(dbgout, "\n*********************\n\n");
    #endif

    fleetTemplateList = dlc_Load("data");

    loop = !main_InitializeAll();
    while(loop){
        choice = mainMenu_Draw();
        switch(choice){
            case MAIN_MENU_LOAD:
                break;
            case MAIN_MENU_NEW_GAME:
                newGame_Start();
                break;
            case MAIN_MENU_SETTINGS:
                loop = mainMenu_Settings();
                break;
            case MAIN_MENU_EXIT:
                loop = false;
                break;
            default:
                break;
        }
    }
    
    dlc_Unload(fleetTemplateList);
    gfx_End();
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "\nClose Stellaris\nEXIT SUCCESS\n\n");
    #endif
    
    return 0;
}