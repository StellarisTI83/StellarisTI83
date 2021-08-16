#ifndef _GALAXY_INCLUDE
#define _GALAXY_INCLUDE

#include "ai.h"
#include "camera.h"
#include "settings.h"
#include "systemes.h"

#define GALAXY_WIDTH    11

#define SPACE_BETWEEN_STARS     50
#define RANDOMIZE_COEFFICIENT   100

#define LIMIT_LEFT  180
#define LIMIT_TOP   160

/* constants =========================================================== */
#define RAYON_PLANETE1 30
#define RAYON_PLANETE2 60
#define RAYON_PLANETE3 90
#define RAYON_PLANETE4 110
#define RAYON_PLANETE5 130

void CreerEmpires(Settings *parametres, EmpireListe *empireListe, StarSystem **systemeStellaires, Camera *camera);
/**
 * @brief Create a new galaxy in the sent aray
 * 
 * @param systemeStellaires 
 */
void galaxy_CreateNew(StarSystem **starSystem);

#endif