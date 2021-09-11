#ifndef H_PARAMETRES
#define H_PARAMETRES

/* struct ============================================================== */
/**
 * @brief 
 * 
 */
typedef struct SettingsStruct Settings;

/* entry points ======================================================== */
Settings *setting_Malloc();

void settings_StarNumberSet(Settings *settings, int starNumber);
int settings_StarNumberGet(Settings *settings);

void settings_EmpireNumberSet(Settings *settings, int starNumber);
int settings_EmpireNumberGet(Settings *settings);

void settings_SeeAllSet(Settings *settings, int seeAll);
int settings_SeeAllGet(Settings *settings);

void settings_GameActiveSet(Settings *settings, int gameActive);
int settings_GameActiveGet(Settings *settings);
#endif