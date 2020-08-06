#ifndef H_MAIN
#define H_MAIN

/* macros ============================================================== */
#define VERSION_LOGICIEL "v0.1.0-a"

#define _(String) gettext(String)

/* constants =========================================================== */
#define LARGEUR_GALAXIE 11

#define TEXT_BG_COLOR 2

#define LIMITE_GAUCHE 180
#define LIMITE_HAUT 160

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

#define RAYON_DE_VUE_SYSTEME 160

/* types =============================================================== */
typedef enum {AUCUN, CONSTRUIRE_DISTRICT_URBAIN, CONSTRUIRE_DISTRICT_GENERATEUR, CONSTRUIRE_DISTRICT_MINIER, CONSTRUIRE_DISTRICT_AGRICOLE, CONSTRUIRE_BATIMENT} OrdreConstruction;

typedef enum {NO_ERROR, ACIER_MANQUE, MINERAI_MANQUE} Error;

/* structures ========================================================== */

typedef struct FlotteListeStruct FlotteListe;
typedef struct FlotteStruct Flotte;

typedef struct {
	char vitesse;
	char vitesseSauvegardee;
	char jour;
	char mois;
	int annee;
	int horloge;
} Date;

typedef struct {
	int valeurMinerai;
	int valeurNourriture;
	int valeurFer;
} Marche;

/* entry points ======================================================== */
void PrintCentered(const char *str, int y, int taille, int color, int differenceX);
void PrintInt(int nombre);
int TailleInt(int nombre);

#endif