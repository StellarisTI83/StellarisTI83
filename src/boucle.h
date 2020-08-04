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

int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche);	
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires, Fenetre *fenetre);
void EffectuerActionsStations(SystemeStellaire *systemeStellaires, Empire *joueur);
void EffectuerActionsPlanetes(SystemeStellaire *systemeStellaires, Empire *joueur);

void StellarisMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, Date *date, Fenetre *fenetre, Empire *joueur, Parametres *parametres);

int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t* sauvegarde, Marche *marche);


#endif _BOUCLE_INCLUDE