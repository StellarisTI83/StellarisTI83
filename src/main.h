#ifndef H_MAIN
#define H_MAIN

/* macros ============================================================== */

/**
 * @brief Game version
 * 
 */
#define VERSION_GAME "v0.4.0-a"

/**
 * @brief If defined, debug version
 * 
 */
#define DEBUG_VERSION

#ifndef DEBUG_VERSION
#define NDEBUG
#endif



/* constants =========================================================== */

#define TEXT_BG_COLOR	2
#define TEXT_HEIGHT		8

/* types =============================================================== */

typedef enum {NO_ERROR, ACIER_MANQUE, MINERAI_MANQUE} Error;

/* struct ============================================================== */

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
int main_IntLength(int nombre);



/**
 * @brief Same as malloc, but help to know how much ram is left
 * 
 * @param _Size 
 * @return void* 
 */
void *malloc_count(size_t _Size);

/**
 * @brief Same as calloc, but help to know how much ram is left
 * 
 * @param _Count 
 * @param _Size 
 * @return void* 
 */
void *calloc_count(size_t _Count, size_t _Size);

/**
 * @brief Same as free, but help to know how much ram is left
 * 
 * @param _Block 
 */
void free_count(void *_Block);

/**
 * @brief To get the used ram
 * 
 * @return unsigned long 
 */
unsigned long ramGet();

#endif