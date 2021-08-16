#ifndef H_MAIN
#define H_MAIN

/* macros ============================================================== */

/**
 * @brief Game version
 * 
 */
#define VERSION_LOGICIEL "v0.4.0-a"

/**
 * @brief If defined, debug version
 * 
 */

#define DEBUG_VERSION


/* constants =========================================================== */

#define TEXT_BG_COLOR	2
#define TEXT_HEIGHT		8

#define X_CENTRE_SYSTEME 480
#define Y_CENTRE_SYSTEME 360

#define RAYON_DE_VUE_SYSTEME 160

/* types =============================================================== */

typedef enum {NO_ERROR, ACIER_MANQUE, MINERAI_MANQUE} Error;

/* structures ========================================================== */

typedef struct {
	int valeurMinerai;
	int valeurNourriture;
	int valeurFer;
} Market;

/* entry points ======================================================== */

/**
 * @brief Print sized string
 * 
 * @param str           The string printed
 * @param y             Y position of the string
 * @param taille        Size to print
 * @param color
 * @param xOffset       xOffset of the
 */
void mainMenu_PrintMultipleLines(char *str);

/**
 * @brief Print a string on multiple lines after \c gfx_SetTextXY
 * 
 * @param str 
 */
void mainMenu_PrintSized(const char *str, int y, int taille, int color, int differenceX);

/**
 * @brief Print an int with the right size
 * 
 * @param number 
 */
void mainMenu_PrintInt(int nombre);


/**
 * @brief Return the number of character in an int
 * 
 * @param number 
 * @return int 
 */
int mainMenu_IntLen(int nombre);

#endif