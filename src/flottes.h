#ifndef _FLOTTES_INCLUDE
#define _FLOTTES_INCLUDE

typedef enum {FLOTTE_MILITAIRE, FLOTTE_SCIENTIFIQUE, FLOTTE_DE_CONSTRUCTION} FlotteType;

struct FlotteStruct {
	int x;
	int y;
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
	Flotte* suivant;
};

struct FlotteListeStruct {
	Flotte* premier;
};

FlotteListe* FlotteListeCreer();
void FlotteListeSupprimer(FlotteListe* flotteliste);
Flotte* FlotteNumero(FlotteListe* flotteliste, int numero);
int FlotteNumeroRecuperer(FlotteListe* flotteliste, Flotte* flotte);
Flotte* FlotteAjouter(FlotteListe* flotteliste);
void FlotteSupprimer(FlotteListe* flotteliste, int numero);

Flotte* NouvelleFlotte(FlotteListe *flotteListe, Empire *empire, FlotteType type, int systeme);

#endif