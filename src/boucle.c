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

#include "ai.h"
#include "boucle.h"
#include "console.h"
#include "flottes.h"
#include "map.h"
#include "menu.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"
#include "screen.h"
#include "time.h"
#include "updating.h"

#include "locale/locale.h"

/* entry points ======================================================== */
int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche){
	int finBoucle = 1;
	char key = 0;
	
	while (finBoucle == 1) {
		
		key = os_GetCSC();
		
		// mettre a jour les informations
		finBoucle = UpdateGame(&key, empireListe, systemeStellaires, date, camera, fenetre);

		//dessiner l'écran
		DrawScreen(&key, empireListe, systemeStellaires, date, camera, fenetre, parametres);

		
		gfx_SwapDraw();
		if((boot_CheckOnPressed()) || (finBoucle == 0))
		{
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			EmpireListeSupprimer(empireListe);
			return 0;
		}
	}
	return 0;
}
