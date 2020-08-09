#ifndef _CONSOLE_INCLUDE
#define _CONSOLE_INCLUDE

#include "ai.h"
#include "camera.h"
#include "parametres.h"
#include "time.h"

/* entry points ======================================================== */

void AfficherConsole(char *, Fenetre *, EmpireListe *, Camera *, Date *, Parametres *);

void EcrireConsoleChar(char *);
void EcrireConsoleInt(int );

#endif