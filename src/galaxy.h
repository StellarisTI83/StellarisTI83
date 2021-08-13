#ifndef _GALAXY_INCLUDE
#define _GALAXY_INCLUDE

#include "ai.h"
#include "camera.h"
#include "parametres.h"
#include "systemes.h"

/* constants =========================================================== */
#define RAYON_PLANETE1 30
#define RAYON_PLANETE2 60
#define RAYON_PLANETE3 90
#define RAYON_PLANETE4 110
#define RAYON_PLANETE5 130

void CreerEmpires(Parametres *parametres, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Camera *camera);
int ChargementNouvellePartieGalaxie(SystemeStellaire **systemeStellaires);

#endif