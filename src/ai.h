#ifndef H_AI
#define H_AI

#include "generic_lists.h"

/* types =============================================================== */
/**
 * @enum Governement
 * @brief Governements types
 * 
 */
typedef enum {  GOVENEMENT_DEMOCRATIC, 
                GOVERNEMENT_OLIGARCHIC, 
                GOVERNEMENT_DICTATORIAL, 
                GOVERNEMENT_IMPERIAL, 
                GOVERNEMENT_HIVE_MIND, 
                GOVERNEMENT_MACHINE_INTELLIGENCE} Governement;

/**
 * @enum Ethic
 * @brief The ethics for the governements
 * 
 */
typedef enum {  ETHIC_AUTHORITARIAN, 
                ETHIC_FANATIC_AUTHORITARIAN, 
                ETHIC_EGALITARIAN, 
                ETHIC_FANATIC_EGALITARIAN,

                ETHIC_SPIRITUALIST, 
                ETHIC_FANATIC_SPIRITUALIST, 
                ETHIC_MATERIALIST, 
                ETHIC_FANATIC_MATERIALIST,

                ETHIC_MILITARIST, 
                ETHIC_FANATIC_MILITARIST, 
                ETHIC_PACIFIST, 
                ETHIC_FANATIC_PACIFIST,

                ETHIC_XENOPHILE, 
                ETHIC_FANATIC_XENOPHILE, 
                ETHIC_XENOPHOBE, 
                ETHIC_FANATIC_XENOPHOBE,
                
                ETHIC_GESTALT} Ethic;

/**
 * @enum Behavior
 * @brief IA behavior
 * 
 */
typedef enum {  BEHAVIOR_HOSTILE = -2, 
                BEHAVIOR_SUSPICIOUS, 
                BEHAVIOR_NEUTRAL, 
                BEHAVIOR_CORDIAL, 
                BEHAVIOR_FRIENDLY, 
                BEHAVIOR_PROTECTIVE} Behavior;

/**
 * @enum Agreements
 * @brief Possible agreements
 * 
 */
typedef enum {  AGREEMENT_NON_AGRESSION = 1, 
                AGREEMENT_RESEARCH      = 2, 
                AGREEMENT_COMMERCIAL    = 4} Agreements;
/**
 * @typedef EmpireList
 * @brief Base typedef for the list of all empires
 * 
 */
typedef GenericList EmpireList;

/**
 * Opinions
 */
typedef GenericList RelationsListe;

/* structures ========================================================== */

/**
 * @struct Empire
 * @brief Struct of an empire
 * 
 */
typedef struct EmpireStruct Empire;

/**
 * Relations
 */
typedef struct RelationsStruct Relations;


#define NO_EMPIRE -1

/* entry points ======================================================== */

#include "fleet.h"

/**
 * @brief Create the list of empires
 * 
 * @return EmpireList* 
 */
EmpireList* empire_ListCreate();
/**
 * @brief Free the list of empires
 * 
 * @param empireList 
 */
void empire_ListFree(EmpireList* empireList);

/**
 * @brief Get the number of empires
 * 
 * @param empireListe 
 * @return int 
 */
int empire_ArraySize(const EmpireList* empireList);

/**
 * @brief Get the pointer to an empire
 * 
 * @param empireList 
 * @param numero 
 * @return Empire* 
 */
Empire* empire_Get(const EmpireList* empireList, const int numero);

/**
 * @brief Add a new empty empire
 * 
 * @param empireList 
 * @return Empire* 
 */
Empire* empire_Add(EmpireList* empireList);

/**
 * @brief Free an empire
 * 
 * @param empireList 
 * @param numero 
 */
void empire_Free(EmpireList* empireList, int numero);


void empire_FleetListCreate(Empire *empire);
void empire_FleetAdd(Empire *empire, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses) __attribute__((deprecated("use empire_FleetListGet")));
FlotteListe *empire_FleetListGet(Empire *empire);

void EmpireCreerRelations(Empire *empire);
void EmpireRelationNouvelle(Empire *empire);
RelationsListe *EmpireRelationGetArray(Empire *empire);

void SetEmpireGouvernement(Empire *empire, Governement gouvernement);
Governement GetEmpireGouvernement(Empire *empire);

void empire_CapitalSystemSet(Empire *empire, int system);
int GetEmpireSystemCapital(Empire *empire);

void empire_ColorSet(Empire *empire, Governement couleur);
int GetEmpireColor(Empire *empire);

void SetEmpirePrincipes(Empire *empire, Ethic principe1, Ethic principe2, Ethic principe3);
Ethic GetEmpirePrincipes(Empire *empire, int numero);

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
Behavior RelationGetAttitude(Relations* relations);

void RelationAmeliorer(Relations* relations);
void RelationDegrader(Relations* relations);
void RelationGuerreDeclarer(Relations* relations);
void RelationInsulter(Relations* relations);
void RelationSetPacte(Relations* relations, Agreements pacte);
Agreements RelationGetPacteStatus(Relations* relations, Agreements pacte);

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