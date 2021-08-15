#ifndef _NOUVELLE_PARTIE_INCLUDE
#define _NOUVELLE_PARTIE_INCLUDE

#include "ai.h"
#include "parametres.h"



/* entry points ======================================================== */
int NouvellePartieAvertissement(EmpireListe *empireListe, Parametres *parametres);
void PrintText(const char *str, int x, int y, int taille, int color);

void ChargementNouvellePartie();

#endif