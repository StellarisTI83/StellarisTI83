#include <stdbool.h>


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
	
	while (loop) {
		
		key = os_GetCSC();
		
		// mettre a jour les informations
		loop = UpdateGame(&key, empireListe, starSystems, time, camera, window, notificationList, settings);

		//dessiner l'écran
		loop = DrawScreen(&key, empireListe, starSystems, time, camera, window, settings, marche, notificationList);

		if((!boot_CheckOnPressed()) || !loop) {
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			return 0;
		}
	}
	return 0;
}
