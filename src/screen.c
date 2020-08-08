#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include "hud.h"
#include "map.h"
#include "screen.h"



/* entry points ======================================================== */
void DrawScreen(){
    gfx_ZeroScreen();

    DrawMap();

    DrawHUD();

    DrawMenu();
    
	gfx_SwapDraw();
}