#include <stdbool.h>
#include <time.h>

#include <debug.h>
#include <errno.h>

#include "main.h"

#include "loop.h"

/* entry points ======================================================== */
int game_MainLoop(	EmpireList *empireListe, 
					Settings *settings, 
					Time *time, 
					StarSystem **starSystems, 
					Camera *camera, 
					WindowManager *window, 
					Market *marche, 
					NotificationList *notificationList) {
	int loop = true;
	char key = 0;
	int fps;
	float updateTime, screenTime;
	
	while (settings_GameActiveGet(settings)) {
		// Get the pressed keys
		key = os_GetCSC();
		
    	timer_Set(2, 0);
		// mettre a jour les informations
		loop = loop && game_Update(&key, empireListe, starSystems, time, camera, window, notificationList, settings);
		updateTime = (float)timer_Get(2)*1000.0/(float)TIMER_FREQ;

    	timer_Set(2, 0);
		//dessiner l'écran
		loop = loop && draw_Screen(&key, empireListe, starSystems, time, camera, window, settings, marche, notificationList);
		screenTime = (float)timer_Get(2)*1000.0/(float)TIMER_FREQ;

		// Get the number of fps
		fps = TIMER_FREQ/timer_Get(1);
    	timer_Set(1, 0);
		if(fps > 0)
        	time_FPSSet(time, fps, updateTime, screenTime);
		
		if(/*!boot_CheckOnPressed() ||*/ !loop) {
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			return 0;
			
			#ifdef DEBUG_VERSION
			dbg_sprintf(dbgout, "Close game\n");
			#endif
		}
		
	}
	return 0;
			
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "Close game\n");
	#endif
}
