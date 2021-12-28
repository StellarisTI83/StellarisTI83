#ifndef H_MAP
#define H_MAP

#include "ai.h"
#include "camera.h"
#include "fleet.h"
#include "settings.h"
#include "systemes.h"
#include "time.h"

/* constants =========================================================== */

#define RAYON_DE_VUE_SYSTEME 160

/* entry points ======================================================== */
/**
 * @brief Draw the map, galaxy and system view
 * 
 * @param key 
 * @param empireListe 
 * @param galaxy 
 * @param camera 
 * @param window 
 * @param settings 
 */
void map_Draw(	char *key,
				EmpireList *empireListe, 
				StarSystem **galaxy, 
				Camera *camera, 
				WindowManager *window, 
				Settings *settings);

#endif