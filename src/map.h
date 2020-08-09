#ifndef H_MAP
#define H_MAP

#include "ai.h"
#include "camera.h"
#include "flottes.h"
#include "parametres.h"
#include "systemes.h"
#include "time.h"

/* constants =========================================================== */

#define RAYON_DE_VUE_SYSTEME 160

/* entry points ======================================================== */

void DrawMap(EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Camera *camera, char *key, Date *date, Fenetre *fenetre, Parametres *parametres);

#endif