#ifndef H_AI
#define H_AI

#include "generic_lists.h"

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

typedef GenericList EmpireListe;

/* structures ========================================================== */

/**
 * Empire
 */
typedef struct EmpireStruct Empire;

/* entry points ======================================================== */

#include "flottes.h"

EmpireListe* EmpireListeCreer();
void EmpireListeSupprimer(EmpireListe*);
int EmpireArraySize(EmpireListe* empireListe);
Empire* EmpireNumero(EmpireListe*, int numero);
Empire* EmpireAjouter(EmpireListe*);
void EmpireSupprimer(EmpireListe*, int numero);


void CreerEmpireFlotte(Empire *empire);
void EmpireNouvelleFlotte(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);
FlotteListe *GetFleetArray(Empire *empire);

void SetEmpireGouvernement(Empire *empire, Gouvernement gouvernement);
Gouvernement GetEmpireGouvernement(Empire *empire);

void SetEmpireSystemCapital(Empire *empire, int system);
int GetEmpireSystemCapital(Empire *empire);

void SetEmpireColor(Empire *empire, Gouvernement couleur);
int GetEmpireColor(Empire *empire);

void SetEmpirePrincipes(Empire *empire, Principe principe1, Principe principe2, Principe principe3);
Principe GetEmpirePrincipes(Empire *empire, int numero);

void SetEmpireSpecies(Empire *empire, int espece);
int GetEmpireSpecies(Empire *empire);

void SetEmpireClothes(Empire *empire, int clothes);
int GetEmpireClothes(Empire *empire);

void SetEmpireName(Empire *empire, int curseur, char lettre);
char GetEmpireNameChar(Empire *empire, int cursor);
char* GetEmpireNameString(Empire *empire);

void SetEmpireCredit(Empire *empire, int argent);
void AddEmpireCredit(Empire *empire, int argent);
int GetEmpireCredit(Empire *empire);
void SetEmpireCreditChange(Empire *empire, int change);
void AddEmpireCreditChange(Empire *empire, int change);
int GetEmpireCreditChange(Empire *empire);

void SetEmpireMinerals(Empire *empire, int minerais);
void AddEmpireMinerals(Empire *empire, int minerais);
int GetEmpireMinerals(Empire *empire);
void SetEmpireMineralsChange(Empire *empire, int change);
void AddEmpireMineralsChange(Empire *empire, int change);
int GetEmpireMineralsChange(Empire *empire);

void SetEmpireFood(Empire *empire, int nourriture);
void AddEmpireFood(Empire *empire, int nourriture);
int GetEmpireFood(Empire *empire);
void SetEmpireFoodChange(Empire *empire, int change);
void AddEmpireFoodChange(Empire *empire, int change);
int GetEmpireFoodChange(Empire *empire);

void SetEmpireAlloys(Empire *empire, int alloy);
void AddEmpireAlloys(Empire *empire, int alloy);
int GetEmpireAlloys(Empire *empire);
void SetEmpireAlloysChange(Empire *empire, int change);
void AddEmpireAlloysChange(Empire *empire, int change);
int GetEmpireAlloysChange(Empire *empire);

void SetEmpireConsumerGoods(Empire *empire, int consumerGoods);
void AddEmpireConsumerGoods(Empire *empire, int consumerGoods);
int GetEmpireConsumerGoods(Empire *empire);
void SetEmpireConsumerGoodsChange(Empire *empire, int change);
void AddEmpireConsumerGoodsChange(Empire *empire, int change);
int GetEmpireConsumerGoodsChange(Empire *empire);

void EmpireGenerateRandomName(Empire *empire);

void CalculateEmpireFleetPower(Empire *empire);
int GetEmpireFleetPower(Empire *empire);

void EmpireAI(EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date);

#endif