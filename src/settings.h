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

void settings_StarNumberSet(Settings *parametres, int starNumber);
int settings_StarNumberGet(Settings *parametres);

void settings_EmpireNumberSet(Settings *parametres, int starNumber);
int settings_EmpireNumberGet(Settings *parametres);

void settings_SeeAllSet(Settings *parametres, int seeAll);
int settings_SeeAllGet(Settings *parametres);

#endif