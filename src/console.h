#ifndef _CONSOLE_INCLUDE
#define _CONSOLE_INCLUDE

#include "ai.h"
#include "camera.h"
#include "settings.h"
#include "time.h"

/* entry points ======================================================== */

void console_show(char *, Window *, EmpireList *, Camera *, Time *, Settings *);

void console_writeChar(char *);
void console_writeInt(int );

#endif