#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

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

typedef struct FlotteListeStruct FlotteListe;

typedef struct FlotteStruct Flotte;

typedef struct VecteurStruct Vecteur;

/* entry points ======================================================== */

FlotteListe* CreerFlotteListe();
void SupprimerFlotteListe(FlotteListe* flotteliste);
int FleetArraySize(FlotteListe* flotteListe);
Flotte* NumeroFlotte(FlotteListe* flotteliste, int numero);
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

void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Fenetre *fenetre, Camera *camera, EmpireListe *empireListe, SystemeStellaire **systemeStellaires);
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire **systemeStellaires);

#endif