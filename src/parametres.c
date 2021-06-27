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

#include "main.h"

#include "parametres.h"

/* structures ========================================================== */
struct ParametresStruct{
	char starNumber;
	char empireNumber;
	int seeAll;
    int fps;
};

/* entry points ======================================================== */
Parametres *AllocParametres(){
    return calloc(1, sizeof(Parametres));
}

void SetStarNumber(Parametres *parametres, int starNumber){
    parametres->starNumber = starNumber;
}

int GetStarNumber(Parametres *parametres){
    return parametres->starNumber;
}

void SetEmpireNumber(Parametres *parametres, int starNumber){
    parametres->starNumber = starNumber;
}

int GetEmpireNumber(Parametres *parametres){
    return parametres->starNumber;
}

void SetSeeAll(Parametres *parametres, int seeAll){
    parametres->seeAll = seeAll;
}

int GetSeeAll(Parametres *parametres){
    return parametres->seeAll;
}

