#ifndef _NOUVELLE_PARTIE_INCLUDE
#define _NOUVELLE_PARTIE_INCLUDE

#include "ai.h"
#include "parametres.h"

/* constants =========================================================== */
#define RAYON_PLANETE1 30
#define RAYON_PLANETE2 60
#define RAYON_PLANETE3 90
#define RAYON_PLANETE4 110
#define RAYON_PLANETE5 130

/* entry points ======================================================== */
int NouvellePartieAvertissement(EmpireListe *empireListe, Parametres *parametres);
void PrintText(const char *str, int x, int y, int taille, int color);

void ChargementNouvellePartie(EmpireListe *empireListe, Parametres *parametres);

#endif