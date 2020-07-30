#ifndef _BOUCLE_INCLUDE
#define _BOUCLE_INCLUDE

#include "console.h"

#define MENU_QUITTER -1
#define MENU_AUCUN 0
#define MENU_SYSTEME 1
#define MENU_MARCHE 2
#define MENU_FLOTTE 3
#define MENU_RECHERCHE 4
#define MENU_CONTACTS 5

#define MENU_SYSTEME_FLOTTES 1
#define MENU_SYSTEME_ETOILE 2
#define MENU_SYSTEME_PLANETE_RESUME 3
#define MENU_SYSTEME_PLANETE_DISTRICT 4
#define MENU_SYSTEME_PLANETE_BATIMENT 5
#define MENU_SYSTEME_PLANETE_BATIMENT_CHOIX 6
#define MENU_SYSTEME_FLOTTE_DETAILS 7
#define MENU_SYSTEME_STATION_RESUME 8
#define MENU_SYSTEME_STATION_MODULES 9
#define MENU_SYSTEME_STATION_MODULES_CHOIX 10
#define MENU_SYSTEME_STATION_CHANTIER 11
#define MENU_SYSTEME_STATION_CHANTIER_CHOIX 12

int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche, Console *console);	
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires, Fenetre *fenetre);
void EffectuerActionsStations(SystemeStellaire *systemeStellaires, Empire *joueur);
void EffectuerActionsPlanetes(SystemeStellaire *systemeStellaires, Empire *joueur);

void StellarisMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteListe, Date *date, Fenetre *fenetre, Empire *joueur, Console *console);

int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t* sauvegarde, Marche *marche, Console *console);
void PrintHUD(const unsigned int nombre, const int change, int x, int y);

int Menus(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche);
int8_t MenuQuitter(char* key, ti_var_t* sauvegarde, EmpireListe* empireListe, Empire* joueur, Parametres* parametres, Date* date, SystemeStellaire* systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre);

void MenuSysteme(char* key, EmpireListe* empireListe, Empire* joueur, Parametres* parametres, Date* date, SystemeStellaire* systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre);
void MenuSystemeFlotte(char* key, EmpireListe* empireListe, Empire* joueur, SystemeStellaire* systemeStellaires, Camera* camera, Fenetre* fenetre);

void MenuSystemeEtoile(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

void MenuSystemePlaneteResume(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

void MenuSystemePlaneteDistrict(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, EmpireListe *empireListe);
void OrdreDistrictNom(Villes *villes);

void MenuSystemePlaneteBatiments(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, EmpireListe *empireListe);
void MenuSystemePlaneteBatimentChoix(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
char *PlaneteBatimentNom(Batiment batiment, int niveau, char* nom);

void MenuSystemeFlotteDetails(char *key, SystemeStellaire *systemeStellaires, EmpireListe *empireListe, Empire* joueur, Camera *camera, Fenetre *fenetre);

void MenuSystemeStationResume(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
char* OrdreStationNom(Station *station, int numeroDuModule, char* nomDuModule, int niveau);

void MenuSystemeStationModules(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
char* ModuleNom(Module module, char* nomDuModule);

void MenuSystemeStationModulesChoix(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

void MenuSystemeStationChantier(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
void MenuSystemeStationChantierChoix(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

void MenuMarche(char *key, Empire* joueur, Marche *marche, Camera *camera, Fenetre *fenetre);

void MenuListeFLottes(char *key, Empire *joueur, Camera *camera, Fenetre *fenetre);

void MenuRecherche(char *key, Camera *camera, Fenetre *fenetre);

void MenuContacts(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre);

#endif _BOUCLE_INCLUDE