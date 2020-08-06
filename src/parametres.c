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

#include "parametres.h"

/* structures ========================================================== */
struct ParametresStruct{
	char starNumber;
	char empireNumber;
	bool seeAll;
};

/* entry points ======================================================== */
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

void SetSeeAll(Parametres *parametres, bool seeAll){
    parametres->seeAll = seeAll;
}

bool GetSeeAll(Parametres *parametres){
    return parametres->seeAll;
}

