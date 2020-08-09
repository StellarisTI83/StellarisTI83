#ifndef H_SCREEN
#define H_SCREEN

#include "ai.h"
#include "hud.h"
#include "map.h"

/* entry points ======================================================== */
void DrawScreen(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre, Parametres *parametres);

#endif