#ifndef H_EMPIRE
#define H_EMPIRE

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
 * Diplomacy
 */
typedef GenericList DiplomacyList;

/**
 * List of the planets the empire has
 */
typedef GenericList EmpirePlanetList;

/* struct ============================================================== */

/**
 * @struct Empire
 * @brief Struct of an empire
 * 
 */
typedef struct EmpireStruct Empire;

/**
 * Diplomacy
 */
typedef struct RelationsStruct Diplomacy;


#define NO_EMPIRE -1

#include "fleet.h"
#include "time.h"

// Empire list
EmpireList* empire_ListCreate();
void        empire_ListFree(EmpireList* empireList);
int         empire_ArraySize(const EmpireList* empireList);

// Empire creation and destruction
Empire*     empire_Get(const EmpireList* empireList, const int numero);
Empire*     empire_Add(EmpireList* empireList);
void        empire_Free(EmpireList* empireList, int numero);

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
                        const int empireIndex,
                        StarSystem *empireStarSystem, 
                        const int systemIndex,
                        const int color);

// Fleet
void        empire_FleetListCreate  (Empire *empire);
FleetList*  empire_FleetListGet     (Empire *empire);

// Governement
void        empire_GovernementSet   (Empire *empire, Governement governement);
Governement empire_GovernementGet   (Empire *empire);

// Color and system
void        empire_SystemCapitalSet (Empire *empire, int system);
int         empire_SystemCapitalGet (Empire *empire);

void        empire_ColorSet         (Empire *empire, Governement couleur);
int         empire_ColorGet         (Empire *empire);

// Ethic
void        empire_EthicSet     (Empire *empire, Ethic ethic1, Ethic ethic2, Ethic ethic3);
Ethic       empire_EthicGet     (Empire *empire, int index);

// Species
void        empire_SpeciesSet       (Empire *empire, int espece);
int         empire_SpeciesGet       (Empire *empire);

void        empire_SpeciesClothesSet(Empire *empire, int clothes);
int         empire_SpeciesClothesGet(Empire *empire);

// Name
void        empire_NameGenerate     (Empire *empire);
char*       empire_NameStringGet    (Empire *empire);

EmpirePlanetList*   empire_PlanetListCreate ();
void    empire_PlanetListFree   (EmpirePlanetList* empirePlanetList);
Planet* empire_PlanetGet        (const Empire* empire, const int index);
Planet* empire_PlanetAdd        (const Empire* empire, Planet *planet);

// Ressources
void        empire_CreditSet        (Empire *empire, int credits);
void        empire_CreditAdd        (Empire *empire, int credits);
int         empire_CreditGet        (Empire *empire);
void        empire_CreditVariationSet(Empire *empire, int variation);
void        empire_CreditVariationAdd(Empire *empire, int variation);
int         empire_CreditVariationGet(Empire *empire);

void        empire_MineralsSet      (Empire *empire, int minerals);
void        empire_MineralsAdd      (Empire *empire, int minerals);
int         empire_MineralsGet      (Empire *empire);
void        empire_MineralsVariationSet(Empire *empire, int variation);
void        empire_MineralsVariationAdd(Empire *empire, int variation);
int         empire_MineralsVariationGet(Empire *empire);

void        empire_FoodSet          (Empire *empire, int foods);
void        empire_FoodAdd          (Empire *empire, int foods);
int         empire_FoodGet          (Empire *empire);
void        empire_FoodVariationSet (Empire *empire, int variation);
void        empire_FoodVariationAdd (Empire *empire, int variation);
int         empire_FoodVariationGet (Empire *empire);

void        empire_AlloysSet        (Empire *empire, int alloy);
void        empire_AlloysAdd        (Empire *empire, int alloy);
int         empire_AlloysGet        (Empire *empire);
void        empire_AlloysVariationSet(Empire *empire, int variation);
void        empire_AlloysVariationAdd(Empire *empire, int variation);
int         empire_AlloysVariationGet(Empire *empire);

void        empire_ConsumerSet      (Empire *empire, int consumerGoods);
void        empire_ConsumerAdd      (Empire *empire, int consumerGoods);
int         empire_ConsumerGet      (Empire *empire);
void        empire_ConsumerVariationSet(Empire *empire, int variation);
void        empire_ConsumerVariationAdd(Empire *empire, int variation);
int         empire_ConsumerVariationGet(Empire *empire);

// Power
void        empire_PowerUpdate      (Empire *empire);
int         empire_PowerMilitaryGet (Empire *empire);
int         empire_PowerScienceGet  (Empire *empire);
int         empire_PowerEconomicGet (Empire *empire);

// Diplomacy
void        empire_DiplomacyCreate  (Empire *empire);
void        empire_DiplomacyNew     (Empire *empire);
DiplomacyList *empire_DiplomacyListGet(Empire *empire);

DiplomacyList* diplomacy_ListCreate ();
void        diplomacy_AllUpdate     (EmpireList* empireListe);
void        diplomacy_Update        (DiplomacyList* relationsListe, EmpireList* empireListe);
void        diplomacy_ListFree      (DiplomacyList* relationsListe);
int         diplomacy_ArraySize     (DiplomacyList* relationsListe);

Diplomacy*  diplomacy_RelationsGet  (DiplomacyList* relationsListe, int numero);
Diplomacy*  diplomacy_RelationsAdd  (DiplomacyList* relationsListe);
void        diplomacy_RelationsFree (DiplomacyList* relationsListe, int numero);

int         diplomacy_OpinionGet    (Diplomacy* relations);
Behavior    diplomacy_AttitudeGet   (Diplomacy* relations);

void        diplomacy_RelationsUpgrade  (Diplomacy* relations);
void        diplomacy_RelationDowngrade (Diplomacy* relations);
void        diplomacy_WarDeclare    (Diplomacy* relations);
void        diplomacy_Insult        (Diplomacy* relations);
void        diplomacy_AgreementSign     (Diplomacy* relations, Agreements pacte);
Agreements  diplomacy_AgreementStatus   (Diplomacy* relations, Agreements pacte);

EmpireAi* empire_AiGet(const Empire* empire);
void empire_AiSet(Empire* empire, EmpireAi* empireAi);

#endif