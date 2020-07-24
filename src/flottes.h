#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

typedef enum {FLOTTE_MILITAIRE, FLOTTE_SCIENTIFIQUE, FLOTTE_DE_CONSTRUCTION} FlotteType;

typedef struct NoeudStruct Noeud;

struct FlotteStruct {
	int x;
	int y;
	int vitesse;
	int angle;
	char type;
	int puissance;
	int coqueTotal;
	int coqueVie;
	int blindageTotal;
	int blindageVie;
	int bouclierTotal;
	int bouclierVie;
	char systeme;
	int nombreVaisseaux;
	char action;
	char systemeSuivant;
	char systemeArrive;
	char avancement;
	int* chemin;
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

Flotte* NouvelleFlotte(FlotteListe *flotteListe, Empire *empire, FlotteType type, int systeme);

// int* FlottePathFinding(int debut, int fin, SystemeStellaire *systemeStellaires, int* path);
// void SupprimerElementListe(Liste *liste, int numero);
// void AjouterElementListe(Liste *liste, Noeud *noeud);
// void CopierNoeud(Noeud *noeud1, Noeud *noeud2);

#endif