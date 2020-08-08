#ifndef _BOUCLE_INCLUDE
#define _BOUCLE_INCLUDE

#include "console.h"

int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche);	
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires, Fenetre *fenetre);
void EffectuerActionsStations(SystemeStellaire *systemeStellaires, Empire *joueur);
void EffectuerActionsPlanetes(SystemeStellaire *systemeStellaires, Empire *joueur);

void DrawMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, Date *date, Fenetre *fenetre, Empire *joueur, Parametres *parametres);

int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t* sauvegarde, Marche *marche);


#endif _BOUCLE_INCLUDE