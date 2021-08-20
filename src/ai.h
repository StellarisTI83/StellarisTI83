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

/**
 * Differentes attitudes
 */
typedef enum {ATTITUDE_HOSTILE = -2, ATTITUDE_SUSPICIOUS, ATTITUDE_NEUTRAL, ATTITUDE_CORDIAL, ATTITUDE_FRIENDLY, ATTITUDE_PROTECTIVE} Attitude;

/**
 * Differents pactes
 */
typedef enum {PACTE_NON_AGRESSION = 1, PACTE_RECHERCHE = 2, PACTE_COMMERCIAL = 4} Pacte;

typedef GenericList EmpireList;

/**
 * Opinions
 */
typedef GenericList RelationsListe;

/* structures ========================================================== */

/**
 * Empire
 */
typedef struct EmpireStruct Empire;
/**
 * Relations
 */
typedef struct RelationsStruct Relations;


#define NO_EMPIRE -1

/* entry points ======================================================== */

#include "fleet.h"

EmpireList* empire_ListCreate();
void empire_ListFree(EmpireList*);
int empire_ArraySize(EmpireList* empireListe);
Empire* empire_Get(EmpireList*, int numero);
Empire* empire_Add(EmpireList*);
void empire_Free(EmpireList*, int numero);


void empire_FleetListCreate(Empire *empire);
void empire_FleetAdd(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);
FlotteListe *empire_FleetListGet(Empire *empire);

void EmpireCreerRelations(Empire *empire);
void EmpireRelationNouvelle(Empire *empire);
RelationsListe *EmpireRelationGetArray(Empire *empire);

void SetEmpireGouvernement(Empire *empire, Gouvernement gouvernement);
Gouvernement GetEmpireGouvernement(Empire *empire);

void empire_CapitalSystemSet(Empire *empire, int system);
int GetEmpireSystemCapital(Empire *empire);

void empire_ColorSet(Empire *empire, Gouvernement couleur);
int GetEmpireColor(Empire *empire);

void SetEmpirePrincipes(Empire *empire, Principe principe1, Principe principe2, Principe principe3);
Principe GetEmpirePrincipes(Empire *empire, int numero);

void empire_SpeciesSet(Empire *empire, int espece);
int GetEmpireSpecies(Empire *empire);

void empire_SpeciesClothesSet(Empire *empire, int clothes);
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

void empire_NameGenerate(Empire *empire);

void CalculateEmpirePower(Empire *empire);
int GetEmpirePowerMilitary(Empire *empire);
int GetEmpirePowerScientific(Empire *empire);
int GetEmpirePowerEconomic(Empire *empire);

RelationsListe* RelationListeCreer();
void ai_RelationsUpdate(EmpireList* empireListe);
void RelationListeUpdate(RelationsListe* relationsListe, EmpireList* empireListe);
void RelationListeSupprimer(RelationsListe* relationsListe);
int RelationArraySize(RelationsListe* relationsListe);
Relations* RelationNumero(RelationsListe* relationsListe, int numero);
Relations* RelationAjouter(RelationsListe* relationsListe);
void RelationSupprimer(RelationsListe* relationsListe, int numero);

int RelationGetOpinion(Relations* relations);
Attitude RelationGetAttitude(Relations* relations);

void RelationAmeliorer(Relations* relations);
void RelationDegrader(Relations* relations);
void RelationGuerreDeclarer(Relations* relations);
void RelationInsulter(Relations* relations);
void RelationSetPacte(Relations* relations, Pacte pacte);
Pacte RelationGetPacteStatus(Relations* relations, Pacte pacte);

void EmpireAI(EmpireList *empireListe, StarSystem **systemeStellaires, Time *date);

/**
 * @brief Generate the structure and the system of an empire
 * 
 * @param empire 
 * @param empireIndex 
 * @param empireStarSystem 
 * @param systemIndex 
 * @param color 
 */
void empire_Generate(   Empire *empire, 
                        int empireIndex,
                        StarSystem *empireStarSystem, 
                        int systemIndex,
                        int color);

#endif