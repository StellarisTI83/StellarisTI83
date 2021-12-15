#ifndef H_AI
#define H_AI

typedef struct EmpireAiStruct EmpireAi;

/* entry points ======================================================== */

#include "empire.h"

void ai_EmpireGenerate( Empire *empire);

/**
 * @brief Function to update the actions that the AI make
 * 
 * @param empireListe 
 * @param systemeStellaires 
 * @param date 
 */
void        ai_Empire   (EmpireList *empireListe, 
                        StarSystem **systemeStellaires, 
                        Time *date);

#endif