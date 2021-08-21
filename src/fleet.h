#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

#include "generic_lists.h"

#include "ai.h"
#include "camera.h"
#include "systemes.h"

/* constants =========================================================== */

#define FLOTTE_AUCUNE_ACTION 0
#define FLOTTE_BOUGER 1
#define FLOTTE_ATTAQUER 2
#define FLOTTE_BOMBARDER 5

#define FLOTTE_INSPECTER 2
#define FLOTTE_RECHERCHER 5

#define FLOTTE_CONSTRUIRE_BASE 2
#define FLOTTE_COSNTRUIRE_MINE 5
#define FLOTTE_CONSTRUIRE_BASE_SCIENTIFIQUE 6

/* types =============================================================== */

typedef enum {FLOTTE_MILITAIRE, FLOTTE_SCIENTIFIQUE, FLOTTE_DE_CONSTRUCTION} FlotteType;

typedef enum {CORVETTE, DESTROYER, CROISEUR, CUIRASSE} TypeMilitaire;

/* structures ========================================================== */

typedef GenericList FlotteListe;

typedef GenericList FleetTemplateListe;

typedef struct FlotteStruct Flotte;

typedef struct FleetTemplateStruct FleetTemplate;

typedef struct VecteurStruct Vecteur;

/* entry points ======================================================== */

FlotteListe* CreerFlotteListe();
void SupprimerFlotteListe(FlotteListe* flotteliste);
int FleetArraySize(FlotteListe* flotteListe);
Flotte* FlotteNumero(FlotteListe* flotteliste, int numero);
int RecupererFlotteNumero(FlotteListe* flotteliste, Flotte* flotte);
Flotte* AjouterFlotte(FlotteListe* flotteliste);
void SupprimerFlotte(FlotteListe* flotteliste, int numero);

Flotte* NouvelleFlotte(FlotteListe *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);

int CalculateFleetPower(FlotteListe *flotteListe);
int GetFleetSystem(Flotte *flotte);
int GetFleetPath(Flotte *flotte, int index);
FlotteType GetFleetType(Flotte *flotte);
int GetFleetPower(Flotte *flotte);
int GetFleetPathProgress(Flotte *flotte);
void IncrementFleetPathProgress(Flotte *flotte);
void SetFleetPathProgress(Flotte *flotte, int progress);

int GetFleetProgress(Flotte *flotte);
void IncrementFleetProgress(Flotte *flotte);
void SetFleetProgress(Flotte *flotte, int progress);

int GetFleetX(Flotte *flotte);
int GetFleetY(Flotte *flotte);
int GetFleetXVector(Flotte *flotte);
int GetFleetYVector(Flotte *flotte);

int GetFleetHullPourcent(Flotte *flotte);
int GetFleetArmorPourcent(Flotte *flotte);
int GetFleetShieldPourcent(Flotte *flotte);

int GetFleetCorvetteNumber(Flotte *flotte);
int GetFleetDestroyerNumber(Flotte *flotte);
int GetFleetCruiserNumber(Flotte *flotte);
int GetFleetBattleshipNumber(Flotte *flotte);

char GetFleetAction(Flotte *flotte);
void SetFleetAction(Flotte *flotte, char action);
int GetFleetArriveSystem(Flotte *flotte);

void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Window *fenetre, Camera *camera, EmpireList *empireListe, StarSystem **systemeStellaires);
int MoveFleet(Flotte *flotte, int systeme, StarSystem **systemeStellaires);
void EffectuerActionsFlottes(EmpireList* empireListe, StarSystem **systemeStellaires);

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