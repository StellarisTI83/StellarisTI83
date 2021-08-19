#include <stdbool.h>
#include <time.h>

#include <debug.h>
#include <errno.h>

#include "main.h"

#include "loop.h"

/* entry points ======================================================== */
int game_MainLoop(	EmpireListe *empireListe, 
					Settings *settings, 
					Time *time, 
					StarSystem **starSystems, 
					Camera *camera, 
					Window *window, 
					Market *marche, 
					NotificationList *notificationList) {
	int loop = true;
	char key = 0;
	long fps;
	
	while (loop) {
		key = os_GetCSC();
		
		// mettre a jour les informations
		loop = UpdateGame(&key, empireListe, starSystems, time, camera, window, notificationList, settings);

		//dessiner l'écran
		loop = DrawScreen(&key, empireListe, starSystems, time, camera, window, settings, marche, notificationList);
		
		fps = TIMER_FREQ/timer_Get(1);
    	timer_Set(1, 0);
		if(fps > 0)
        	time_FPSSet(time, fps);
		
		if(/*!boot_CheckOnPressed() ||*/ !loop) {
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			return 0;
		}
	}
	return 0;
}