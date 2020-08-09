#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

#include "systemes.h"
#include "camera.h"
#include "ai.h"

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

int GetFleetSystem(FlotteListe *flotteListe, int flotteNumber);

// void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Camera *camera, EmpireListe *empireListe, SystemeStellaire* systemeStellaires);
// void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires);

#endif