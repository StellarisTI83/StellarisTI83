#ifndef H_AI
#define H_AI

/* types =============================================================== */
/**
 * Differents gouvernements possibles
 */
typedef enum {DEMOCRATIE, OLIGARCHIE, DICTATORIALE, IMPERIALE, RUCHE, MACHINE_INTELLIGENCE} Gouvernement;

/**
 * Differents principes (ethics)
 */
typedef enum {AUTHORITARIAN, FANATIC_AUTHORITARIAN, EGALITARIAN, FANATIC_EGALITARIAN,
SPIRITUALIST, FANATIC_SPIRITUALIST, MATERIALIST, FANATIC_MATERIALIST,
MILITARIST, FANATIC_MILITARIST, PACIFIST, FANATIC_PACIFIST,
XENOPHILE, FANATIC_XENOPHILE, XONOPHOBE, FANATIC_XENOPHOBE,
GESTALT} Principe;

/* structures ========================================================== */
/**
 * Empire
 */
typedef struct EmpireStruct Empire;

/**
 * Liste des empires
 */
typedef struct EmpireListeStruct EmpireListe;

/* entry points ======================================================== */
EmpireListe* EmpireListeCreer();
void EmpireListeSupprimer(EmpireListe*);
Empire* EmpireNumero(EmpireListe*, int numero);
Empire* EmpireAjouter(EmpireListe*);
void EmpireSupprimer(EmpireListe*, int numero);


void CreerEmpireFlotte(Empire *empire);

void SetGouvernement(Empire *empire, Gouvernement gouvernement);
Gouvernement GetGouvernement(Empire *empire);

void SetColor(Empire *empire, Gouvernement couleur);
int GetColor(Empire *empire);

void SetPrincipes(Empire *empire, Principe principe1, Principe principe2, Principe principe3);
Principe GetPrincipes(Empire *empire, int numero);

void SetSpecies(Empire *empire, int espece);
int GetSpecies(Empire *empire);

void SetName(Empire *empire, int curseur, char lettre);
char GetnameChar(Empire *empire, int cursor);
char* GetnameString(Empire *empire);

void AddCredit(Empire *empire, int argent);
int GetCredit(Empire *empire);

void AddMinerals(Empire *empire, int minerais);
int GetMinerals(Empire *empire);

void AddFood(Empire *empire, int nourriture);
int GetFood(Empire *empire);

void AddAlloys(Empire *empire, int alloy);
int GetAlloys(Empire *empire);

void AddConsumerGoods(Empire *empire, int consumerGoods);
int GetConsumerGoods(Empire *empire);

#endif