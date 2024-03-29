#include <stdbool.h>

#include <stdlib.h>
#include <string.h>

#include <debug.h>

#include "main.h"

#include "settings.h"

/* struct ============================================================== */
struct SettingsStruct{
	char starNumber;
	char empireNumber;
	bool seeAll;
    int fps;
    bool gameActive;
};

/* entry points ======================================================== */
Settings *setting_Malloc(){
    return calloc_count(1, sizeof(Settings));
}

void settings_StarNumberSet(Settings *settings, int starNumber){
    settings->starNumber = starNumber;
}

int settings_StarNumberGet(Settings *settings){
    return settings->starNumber;
}

void settings_EmpireNumberSet(Settings *settings, int starNumber){
    settings->starNumber = starNumber;
}

int settings_EmpireNumberGet(Settings *settings){
    return settings->starNumber;
}

void settings_SeeAllSet(Settings *settings, int seeAll){
    settings->seeAll = seeAll;
}

int settings_SeeAllGet(Settings *settings){
    return settings->seeAll;
}

void settings_GameActiveSet(Settings *settings, int gameActive){
    settings->gameActive = gameActive;
}

int settings_GameActiveGet(Settings *settings){
    return settings->gameActive;
}

void settings_GameStop(Settings *settings){		
    #ifdef DEBUG_VERSION
    dbg_sprintf(dbgout, "Close game\n");
    #endif
    settings->gameActive = false;
}

