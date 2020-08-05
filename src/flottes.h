#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

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

FlotteListe* FlotteListeCreer();
void FlotteListeSupprimer(FlotteListe* flotteliste);
Flotte* FlotteNumero(FlotteListe* flotteliste, int numero);
int FlotteNumeroRecuperer(FlotteListe* flotteliste, Flotte* flotte);
Flotte* FlotteAjouter(FlotteListe* flotteliste);
void FlotteSupprimer(FlotteListe* flotteliste, int numero);

Flotte* NouvelleFlotte(FlotteListe *flotteListe, int systeme, FlotteType type, int nombreDeCorvettes, int nombreDeDestroyers, int nombreDeCroiseurs, int nombreDeCuirasses);

// int* FlottePathFinding(int debut, int fin, SystemeStellaire *systemeStellaires, int* path);
// void SupprimerElementListe(Liste *liste, int numero);
// void AjouterElementListe(Liste *liste, Noeud *noeud);
// void CopierNoeud(Noeud *noeud1, Noeud *noeud2);

#endif