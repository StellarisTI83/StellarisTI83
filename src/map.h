#ifndef H_MAP
#define H_MAP

#include "ai.h"
#include "camera.h"
#include "fleet.h"
#include "settings.h"
#include "systemes.h"
#include "time.h"

/* constants =========================================================== */

#define RAYON_DE_VUE_SYSTEME 160

/* entry points ======================================================== */

void DrawMap(EmpireList *empireListe, StarSystem **systemeStellaires, Camera *camera, char *key, Time *date, Window *fenetre, Settings *parametres);

#endif