#ifndef H_AI
#define H_AI

#include "flottes.h"

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
void EmpireNouvelleFlotte(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);
FlotteListe *GetFleetArray(Empire *empire);

void SetEmpireGouvernement(Empire *empire, Gouvernement gouvernement);
Gouvernement GetEmpireGouvernement(Empire *empire);

void SetEmpireColor(Empire *empire, Gouvernement couleur);
int GetEmpireColor(Empire *empire);

void SetEmpirePrincipes(Empire *empire, Principe principe1, Principe principe2, Principe principe3);
Principe GetEmpirePrincipes(Empire *empire, int numero);

void SetEmpireSpecies(Empire *empire, int espece);
int GetEmpireSpecies(Empire *empire);

void SetEmpireName(Empire *empire, int curseur, char lettre);
char GetEmpireNameChar(Empire *empire, int cursor);
char* GetEmpireNameString(Empire *empire);

void AddEmpireCredit(Empire *empire, int argent);
int GetEmpireCredit(Empire *empire);

void AddEmpireMinerals(Empire *empire, int minerais);
int GetEmpireMinerals(Empire *empire);

void AddEmpireFood(Empire *empire, int nourriture);
int GetEmpireFood(Empire *empire);

void AddEmpireAlloys(Empire *empire, int alloy);
int GetEmpireAlloys(Empire *empire);

void AddEmpireConsumerGoods(Empire *empire, int consumerGoods);
int GetEmpireConsumerGoods(Empire *empire);

#endif