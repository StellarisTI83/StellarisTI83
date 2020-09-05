#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include "gfx/gfx.h"

#include "main.h"

#include "boucle.h"

#include "locale/locale.h"

/* entry points ======================================================== */
int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Parametres *parametres, Date *date, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche, NotificationList *notificationList) {
	int boucle = true;
	char key = 0;
	
	while (boucle == true) {
		
		key = os_GetCSC();
		
		// mettre a jour les informations
		boucle = UpdateGame(&key, empireListe, systemeStellaires, date, camera, fenetre);

		//dessiner l'écran
		DrawScreen(&key, empireListe, systemeStellaires, date, camera, fenetre, parametres, sauvegarde, marche, notificationList);

		if((boot_CheckOnPressed()) || (boucle == 0)) {
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			// EmpireListeSupprimer(empireListe);
			return 0;
		}
	}
	return 0;
}
