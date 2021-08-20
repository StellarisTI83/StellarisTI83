#ifndef _NOUVELLE_PARTIE_INCLUDE
#define _NOUVELLE_PARTIE_INCLUDE

#include "ai.h"
#include "settings.h"


#define GALAXY_WIDTH 11

/* entry points ======================================================== */
int NouvellePartieAvertissement(EmpireList *empireListe, Settings *parametres);
void PrintText(const char *str, int x, int y, int taille, int color);

void ChargementNouvellePartie();

#endif