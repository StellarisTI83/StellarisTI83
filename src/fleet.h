#ifndef H_FLEET
#define H_FLEET

#include "generic_lists.h"


/* constants =========================================================== */

#define FLOTTE_AUCUNE_ACTION 0
#define FLOTTE_BOUGER 1
#define FLOTTE_ATTAQUER 2
#define FLOTTE_BOMBARDER 5

#define FLOTTE_INSPECTER 2
#define FLOTTE_RECHERCHER 5

#define FLOTTE_CONSTRUIRE_BASE 2
#define FLOTTE_CONSTRUIRE_MINE 5
#define FLOTTE_CONSTRUIRE_BASE_SCIENTIFIQUE 6

/* types =============================================================== */

typedef enum {  FLEET_MILITARY, 
                FLEET_SCIENTIFIC, 
                FLEET_CONSTRUCTION} FlotteType;

typedef enum {  CORVETTE, 
                DESTROYER, 
                CRUISER, 
                BATTLESHIP} TypeMilitaire;

/* struct ============================================================== */

typedef GenericList FleetList;

typedef GenericList FleetTemplateListe;

typedef struct FlotteStruct Fleet;

typedef struct FleetTemplateStruct FleetTemplate;

typedef struct VecteurStruct Vecteur;

#include "ai.h"
#include "camera.h"
#include "systemes.h"

/* entry points ======================================================== */

FleetList* fleet_ListCreate();
void fleet_ListFree(FleetList* flotteliste);
int FleetArraySize(const FleetList* flotteListe);
Fleet* FlotteNumero(const FleetList* flotteliste, const int numero);
int RecupererFlotteNumero(const FleetList* flotteliste, const Fleet* flotte);
Fleet* AjouterFlotte(FleetList* flotteliste);
void SupprimerFlotte(FleetList* flotteliste, int numero);

Fleet* fleet_New(FleetList *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);

int CalculateFleetPower(FleetList *flotteListe);
int GetFleetSystem(Fleet *flotte);
int GetFleetPath(Fleet *flotte, int index);
FlotteType GetFleetType(Fleet *flotte);
int GetFleetPower(Fleet *flotte);
int GetFleetPathProgress(Fleet *flotte);
void IncrementFleetPathProgress(Fleet *flotte);
void SetFleetPathProgress(Fleet *flotte, int progress);

int GetFleetProgress(Fleet *flotte);
void IncrementFleetProgress(Fleet *flotte);
void SetFleetProgress(Fleet *flotte, int progress);

int GetFleetX(Fleet *flotte);
int GetFleetY(Fleet *flotte);
int GetFleetXVector(Fleet *flotte);
int GetFleetYVector(Fleet *flotte);

int GetFleetHullPourcent(Fleet *flotte);
int GetFleetArmorPourcent(Fleet *flotte);
int GetFleetShieldPourcent(Fleet *flotte);

int GetFleetCorvetteNumber(Fleet *flotte);
int GetFleetDestroyerNumber(Fleet *flotte);
int GetFleetCruiserNumber(Fleet *flotte);
int GetFleetBattleshipNumber(Fleet *flotte);

char GetFleetAction(Fleet *flotte);
void SetFleetAction(Fleet *flotte, char action);
int GetFleetArriveSystem(Fleet *flotte);

void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Window *fenetre, Camera *camera, EmpireList *empireListe, StarSystem **systemeStellaires);
int MoveFleet(Fleet *flotte, int systeme, StarSystem **systemeStellaires);
void fleet_ActionsUpdate(StarSystem **systemeStellaires, EmpireList* empireListe);

FleetTemplateListe* fleet_TemplateListCreate();
void fleet_TemplateListFree(FleetTemplateListe* flotteliste);
int fleet_TemplateListSize(FleetTemplateListe* flotteListe);
FleetTemplate* fleet_TemplateGet(FleetTemplateListe* flotteliste, int numero);
FleetTemplate* fleet_TemplateAdd(FleetTemplateListe* flotteliste);
void fleet_TemplateDestroy(FleetTemplateListe* flotteliste, int numero);

void fleet_TemplateSetType(FleetTemplate* fleetTemplate, FlotteType type);
void fleet_TemplateSetPower(FleetTemplate* fleetTemplate, int puissance);
void fleet_TemplateSetHull(FleetTemplate* fleetTemplate, int hull);
void fleet_TemplateSetArmor(FleetTemplate* fleetTemplate, int armor);
void fleet_TemplateSetShield(FleetTemplate* fleetTemplate, int shield);

FlotteType fleet_TemplateGetType(FleetTemplate* fleetTemplate);
int fleet_TemplateGetPower(FleetTemplate* fleetTemplate);
int fleet_TemplateGetHull(FleetTemplate* fleetTemplate);
int fleet_TemplateGetArmor(FleetTemplate* fleetTemplate);
int fleet_TemplateGetShield(FleetTemplate* fleetTemplate);
#endif