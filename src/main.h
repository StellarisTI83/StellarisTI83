/**************macros**************/
#define LARGEUR_GALAXIE 11
#define VERSION_LOGICIEL "pre-alpha"
#define LIMITE_GAUCHE 180
#define LIMITE_HAUT 160

#define _(String) gettext(String)

#define TEXT_BG_COLOR 2

#define FLOTTE_MILITAIRE 1
#define FLOTTE_SCIENTIFIQUE 2
#define FLOTTE_DE_CONSTRUCTION 3

#define FALSE 0
#define TRUE 1

#define MENU_QUITTER -1
#define MENU_AUCUN 0
#define MENU_SYSTEME 1
#define MENU_MARCHE 2
#define MENU_FLOTTE 3
#define MENU_RECHERCHE 4
#define MENU_CONTACTS 5

#define FLOTTE_AUCUNE_ACTION 0
#define FLOTTE_BOUGER 1
#define FLOTTE_ATTAQUER 2
#define FLOTTE_BOMBARDER 5

#define FLOTTE_INSPECTER 2
#define FLOTTE_RECHERCHER 5

#define FLOTTE_CONSTRUIRE_BASE 2
#define FLOTTE_COSNTRUIRE_MINE 5
#define FLOTTE_CONSTRUIRE_BASE_SCIENTIFIQUE 6

#define NIVEAU_DE_CONNAISSANCE_INCONNU 0
#define NIVEAU_DE_CONNAISSANCE_FAIBLE 1
#define NIVEAU_DE_CONNAISSANCE_MOYEN 2
#define NIVEAU_DE_CONNAISSANCE_ELEVEE 3
#define NIVEAU_DE_CONNAISSANCE_TOTAL 4
/**************structures**************/

typedef struct FlotteListeStruct FlotteListe;
typedef struct EmpireStruct Empire;

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
	char jour;
	char mois;
	int annee;
	int horloge;
} Date;

typedef struct {
	int x;
	int y;
	int vecteurx;
	int vecteury;
	char zoom;
	char mapType; // 1 = normal, 0 = carte
	char fenetre;
	char bloque;
	int selection;
	int ouverte;
	char bougerFlotte;
	int flotte;
	int empire;
} Camera;

typedef struct {
	char nom[10];
	int x;
	int y;
	char rayonOrbite;
	char taille;
	char habitable;
	char type; //1 = arid, 2 = Desert, 3 = Savanna	, 4 = Alpine, 5 = Arctic, 6 = Tundra, 7 = continental, 8 = ocean, 9 = tropical, 10 = monde aride, 11 = monde aride froid, 12 = brisé, 13 = monde gelé, 14 = géante gazeuse, 15 = monde en fusion, 16 = toxic world
	short population;
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
	char niveauStation;
	int16_t hyperlane1;
	int16_t hyperlane2;
	int16_t hyperlane3;
	int16_t hyperlane4;
	char niveauConnaissance; //0 = inconnu, 1 = faible, 2 = moyen, 3 = haut, 4 = total (intel level)
	Planete* planete1;
	Planete* planete2;
	Planete* planete3;
	Planete* planete4;
	Planete* planete5;
} SystemeStellaire;

typedef struct FlotteStruct Flotte;
struct FlotteStruct {
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

typedef struct {
	int selection;
	int flotteSelectionee;
	char ouverte;
	char precedente;
} Fenetre;

typedef struct {
	int valeurMinerai;
	int valeurNourriture;
	int valeurFer;
} Marche;
/**************def**************/
int MainMenu(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);

void Options();

int NouvellePartieAvertissement(Empire *joueur, Parametres *parametres);

int QuitterNouvellePartieAvertissement();
int NouvellePartieEspece(Empire *joueur, Parametres *parametres);
int NouvellePartieGouvernement(Empire *joueur, Parametres *parametres);
int NouvellePartiePrincipes(Empire *joueur, Parametres *parametres);
int NouvellePartieNom(Empire *joueur, Parametres *parametres);
int NouvellePartieDrapeau(Empire *joueur, Parametres *parametres);
int NouvellePartieParametres(Empire *joueur, Parametres *parametres);

void ChargementNouvellePartie(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);
int ChargementNouvellePartieGalaxie(Parametres *parametres, ti_var_t *sauvegarde, SystemeStellaire *systemeStellaires, FlotteListe *flotteJoueur, Camera *camera);
int ChargementAnciennePartie(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);

int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);
int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, FlotteListe *flotteJoueur, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche);
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires);
void StellarisMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteListe, Date *date, Fenetre *fenetre, Empire *joueur);

int StellarisSauvegarde(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteListe, Marche *marche);
void PrintCentered(const char *str, int y, int taille, int color, int differenceX);
void PrintHUD(const unsigned int nombre, const int change, int x, int y);
void PrintText(const char *str, int x, int y, int taille, int color);
void PrintInt(int nombre);

FlotteListe* FlotteListeCreer();
void FlotteListeSupprimer(FlotteListe* flotteliste);
Flotte* FlotteNumero(FlotteListe* flotteliste, int numero);
int FlotteNumeroRecuperer(FlotteListe* flotteliste, Flotte* flotte);
Flotte* FlotteAjouter(FlotteListe* flotteliste);
void FlotteSupprimer(FlotteListe* flotteliste, int numero);

EmpireListe* EmpireListeCreer();
void EmpireListeSupprimer(EmpireListe*);
Empire* EmpireNumero(EmpireListe*, int numero);
Empire* EmpireAjouter(EmpireListe*);
void EmpireSupprimer(EmpireListe*, int numero);

void KeyAction(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur);
void CouleurPlanete(char);
void DessinerHyperlane(int8_t niveauConnaissance1, int8_t niveauConnaissance2, int16_t x, int16_t y, int16_t xLn, int16_t yLn, Camera* camera);
int DessinerPlanete(Planete* planete, int niveau, int numero, char* nom, Fenetre* fenetre);
void FlotteBouger(int numeroDeFlotte, int numeroDeEmpire, int systeme, Camera *camera, EmpireListe *empireListe, SystemeStellaire* systemeStellaires);
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires);
