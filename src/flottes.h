#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

#include "systemes.h"
#include "camera.h"
#include "ai.h"

typedef enum {FLOTTE_MILITAIRE, FLOTTE_SCIENTIFIQUE, FLOTTE_DE_CONSTRUCTION} FlotteType;

typedef enum {CORVETTE, DESTROYER, CROISEUR, CUIRASSE} TypeMilitaire;

typedef struct NoeudStruct Noeud;

typedef struct {
	double xVecteur;
	double yVecteur;
} Vecteur;

struct FlotteStruct {
	int x;
	int y;
	Vecteur vecteur;

	FlotteType type;

	int puissance;
	int coqueTotal;
	int coqueVie;
	int blindageTotal;
	int blindageVie;
	int bouclierTotal;
	int bouclierVie;

	char systeme;
	char action;
	char destination; // planete ou etoile plus prècisement
	char avancementTrajet;
	char systemeArrive;
	char avancement;

	int chemin[50];

	int nombreVaisseaux;
	int nombreDeCorvette;
	int nombreDeDestroyer;
	int nombreDeCroiseur;
	int nombreDeCuirasse;
	Flotte* suivant;
};

struct FlotteListeStruct {
	Flotte* premier;
};

struct NoeudStruct {
	int numero;
	Noeud* parent;
	int x;
	int y;
	int g; //distance depuis le début
	int h; //heuristique
	int f; //g+h
} ;

typedef struct {
	Noeud* noeud;
} Liste;

FlotteListe* CreerFlotteListe();
void SupprimerFlotteListe(FlotteListe* flotteliste);
Flotte* NumeroFlotte(FlotteListe* flotteliste, int numero);
int RecupererFlotteNumero(FlotteListe* flotteliste, Flotte* flotte);
Flotte* AjouterFlotte(FlotteListe* flotteliste);
void SupprimerFlotte(FlotteListe* flotteliste, int numero);

Flotte* NouvelleFlotte(FlotteListe *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);

void BougerFlotte(int numeroDeFlotte, int numeroDeEmpire, int systeme, Camera *camera, EmpireListe *empireListe, SystemeStellaire* systemeStellaires);
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires);

#endif