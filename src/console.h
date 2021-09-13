#ifndef _CONSOLE_INCLUDE
#define _CONSOLE_INCLUDE

#include "ai.h"
#include "camera.h"
#include "settings.h"
#include "time.h"

/* entry points ======================================================== */

void AfficherConsole(char *, Window *, EmpireList *, Camera *, Time *, Settings *);

void EcrireConsoleChar(char *);
void EcrireConsoleInt(int );

#endif