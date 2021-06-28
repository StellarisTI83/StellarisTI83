#ifndef H_MAIN
#define H_MAIN

/* macros ============================================================== */

/**
 * Version du jeu
 */
#define VERSION_LOGICIEL "v0.3.1-a"

/* Debug Version*/

#define DEBUG_VERSION

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

#define RAYON_DE_VUE_SYSTEME 160

/* types =============================================================== */

typedef enum {NO_ERROR, ACIER_MANQUE, MINERAI_MANQUE} Error;

/* structures ========================================================== */

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