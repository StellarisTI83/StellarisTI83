#ifndef H_PARAMETRES
#define H_PARAMETRES

/* structures ========================================================== */
typedef struct ParametresStruct Parametres;

/* entry points ======================================================== */
void SetStarNumber(Parametres *parametres, int starNumber);
int GetStarNumber(Parametres *parametres);

void SetEmpireNumber(Parametres *parametres, int starNumber);
int GetEmpireNumber(Parametres *parametres);

void SetSeeAll(Parametres *parametres, bool seeAll);
bool GetSeeAll(Parametres *parametres);

#endif