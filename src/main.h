#ifndef _MAIN_INCLUDE
#define _MAIN_INCLUDE
/**************macros**************/
#define LARGEUR_GALAXIE 11
#define VERSION_LOGICIEL "Alpha"
#define LIMITE_GAUCHE 180
#define LIMITE_HAUT 160

#define _(String) gettext(String)

#define TEXT_BG_COLOR 2

#define X_CENTRE_SYSTEME 480
#define Y_CENTRE_SYSTEME 360

#define FALSE 0
#define TRUE 1

#define FLOTTE_AUCUNE_ACTION 0
#define FLOTTE_BOUGER 1
#define FLOTTE_ATTAQUER 2
#define FLOTTE_BOMBARDER 5

#define FLOTTE_INSPECTER 2
#define FLOTTE_RECHERCHER 5

#define FLOTTE_CONSTRUIRE_BASE 2
#define FLOTTE_COSNTRUIRE_MINE 5
#define FLOTTE_CONSTRUIRE_BASE_SCIENTIFIQUE 6

typedef enum {INCONNU, FAIBLE, MOYEN, ELEVEE, TOTAL} NiveauDeConnaissance;

typedef enum {CARTE, NORMAL, SYSTEME} VueType;

typedef enum {AUCUNE, AVANT_POSTE, PORT_STELLAIRE, REDOUTE_STELLAIRE, FORTERESSE_STELLAIRE, CITADELLE} NiveauStation;

typedef enum {AUCUN, CHANTIER_SPATIAL, ANCRAGE, CANONS, MISSILES, HANGAR, CARREFOUR_COMMERCIAL, PANNEAUX_SOLAIRES} Module;

typedef enum {AUCUN, AMELIORER_STATION, CONSTRUIRE_MODULE, CONSTRUIRE_PLATEFORME, CONSTRUIRE_VAISSEAU} OrdreStation;

typedef enum {AUCUN = 1, CAPITALE, FONDERIE, LABORATOIRE, USINE_CIVILE, THEATRE} Batiment;

typedef enum {AUCUN, CONSTRUIRE_DISTRICT_URBAIN, CONSTRUIRE_DISTRICT_GENERATEUR, CONSTRUIRE_DISTRICT_MINIER, CONSTRUIRE_DISTRICT_AGRICOLE, CONSTRUIRE_BATIMENT} OrdreConstruction;

typedef enum {NO_ERROR, ACIER_MANQUE, MINERAI_MANQUE} Error;

#define ETOILE_TYPE_B 1
#define ETOILE_TYPE_A 2
#define ETOILE_TYPE_F 3
#define ETOILE_TYPE_G 4
#define ETOILE_TYPE_K 5
#define ETOILE_TYPE_M 6
#define ETOILE_TYPE_TROU_NOIR 7
#define ETOILE_TYPE_PULSAR 8
#define ETOILE_TYPE_ETOILE_A_NEUTRONS 9
/**************structures**************/

typedef struct FlotteListeStruct FlotteListe;
typedef struct FlotteStruct Flotte;
typedef struct EmpireStruct Empire;

typedef struct OrdreElement Ordre;
struct OrdreElement{
	int ordre;
	int info1; //numero du module / type de flotte / numero du batiment
	int info2; //type du module / nombre de vaisseaux dans la flotte
	int prix;
	int tempsTotal;
	int tempsActuel;
	Ordre* ordreSuivant;
};

typedef struct {
	Ordre* premierOrdre;
}OrdreFile;

typedef struct EmpireStruct {
	char nom[31];
	char espece;
	char gouvernement;
	char principe1;
	char principe2;
	char principe3;
	int credits;
	int minerais;
	int nourriture;
	int acier;
	int biensDeConsommation;
	FlotteListe* flotte;
	Empire* suivant;
};

typedef struct {
	Empire* premier;
} EmpireListe;

typedef struct {
	char nombreEtoiles;
	char nombreEmpires;
} Parametres;

typedef struct{
	char language;
	char premierLancement;
} ParametresGlobaux;

typedef struct {
	char vitesse;
	char vitesseSauvegardee;
	char jour;
	char mois;
	int annee;
	int horloge;
} Date;

typedef struct {
	int x;
	int y;
	int xSysteme;
	int ySysteme;
	int vecteurx;
	int vecteury;
	char zoom;
	VueType mapType; // 1 = normal, 0 = carte
	char fenetre;
	char bloque;
	int systemeSelectione;
	int systeme;
	int selection;
	int ouverte;
	char bougerFlotte;
	int flotte;
	int empire;
} Camera;

typedef struct {
	NiveauStation niveauStation;
	OrdreFile *ordreFile;
	int puissance;
	int coqueTotal;//quantitée normale
	int coqueVie;//quantitée habituelle
	int blindageTotal;
	int blindageVie;
	int bouclierTotal;
	int bouclierVie;
	Module module1;
	Module module2;
	Module module3;
	Module module4;
	Module module5;
	Module module6;
} Station;

typedef struct{
	Batiment batiment1;
	int niveauBatiment1;

	Batiment batiment2;
	int niveauBatiment2;

	Batiment batiment3;
	int niveauBatiment3;

	Batiment batiment4;
	int niveauBatiment4;

	Batiment batiment5;
	int niveauBatiment5;

	Batiment batiment6;
	int niveauBatiment6;

	char districtsUrbains;
	char districtsGenerateurs;
	char districtsMiniers;
	char districtsAgricoles;
	char criminatlitee;
	int emplois;
	int amienties;

	OrdreFile *ordreFile;
} Villes;

typedef struct {
	char nom[10];
	int x;
	int y;
	char rayonOrbite;
	char taille;
	char habitable;
	char type; //1 = arid, 2 = Desert, 3 = Savanna	, 4 = Alpine, 5 = Arctic, 6 = Tundra, 7 = continental, 8 = ocean, 9 = tropical, 10 = monde aride, 11 = monde aride froid, 12 = brisé, 13 = monde gelé, 14 = géante gazeuse, 15 = monde en fusion, 16 = toxic world
	int population;
	Villes *villes;
} Planete;

typedef struct {
	char nom[10];
	int x;
	int y;
	char etoileType; //1 = B, 2 = A, 3 = F, 4 = G, 5 = K, 6 = M, 7 = trou noir, 8 = pulsar, 9 = etoile a neutron / B : violet, A : Bleu, F : Blanc, G K et M: orange
	char nombrePlanetes;
	char nombrePlanetesHabitees;
	char nombrePlanetesHabitables;
	char empire;
	int16_t hyperlane1;
	int16_t hyperlane2;
	int16_t hyperlane3;
	int16_t hyperlane4;
	NiveauDeConnaissance niveauDeConnaissance; //0 = inconnu, 1 = faible, 2 = moyen, 3 = haut, 4 = total (intel level)
	Planete* planete1;
	Planete* planete2;
	Planete* planete3;
	Planete* planete4;
	Planete* planete5;
	Station* station;
} SystemeStellaire;

typedef struct {
	int selection;
	int planete;
	int flotteSelectionee;
	char ouverte;
	char precedente;
	int scroll;
	Error error;
	int errorCountDown;
	bool commandPrompt;
	int nombreDeVaisseaux; //utilisé dans la création de flottes
	Villes *villes; //utilisé dans le chanement de batiment
} Fenetre;

typedef struct {
	int valeurMinerai;
	int valeurNourriture;
	int valeurFer;
} Marche;

/**************def**************/
int MainMenu(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);
void Options();

void PrintCentered(const char *str, int y, int taille, int color, int differenceX);
void PrintInt(int nombre);
int tailleInt(int nombre);


EmpireListe* EmpireListeCreer();
void EmpireListeSupprimer(EmpireListe*);
Empire* EmpireNumero(EmpireListe*, int numero);
Empire* EmpireAjouter(EmpireListe*);
void EmpireSupprimer(EmpireListe*, int numero);

void FlotteBouger(int numeroDeFlotte, int numeroDeEmpire, int systeme, Camera *camera, EmpireListe *empireListe, SystemeStellaire* systemeStellaires);
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires);

OrdreFile *CreerFileOrdres();
void SupprimerFileOrdres(OrdreFile *ordreFile);
void NouvelOrdre(OrdreFile* ordreFile, int ordre, int tempsTotal, int info1, int info2, int prix);
void FinirOrdre(OrdreFile *ordreFile);
Ordre* RecupererOrdre(OrdreFile *ordreFile);
int NombredOrdres(OrdreFile *ordreFile);


#endif