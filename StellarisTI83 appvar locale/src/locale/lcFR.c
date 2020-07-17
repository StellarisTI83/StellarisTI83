#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include "../main.h"
#include "lcFR.h"
#include <fileioc.h>

extern char* lc_charger;
extern char* lc_nouvelle_partie;
extern char* lc_options;
extern char* lc_quitter;
extern char* lc_sauvegarder;
extern char* lc_retour;
extern char* lc_create_galaxie;

extern char localeLanguage;
extern ti_var_t languageVar;

void loadFR(){
	lc_charger = "Charger";
	lc_nouvelle_partie = "Nouvelle partie";
	lc_options = "Options";
	lc_quitter = "Quitter";
	lc_sauvegarder = "Sauvegarder";
	lc_retour = "Retour";
	lc_create_galaxie = "Cr/ation de la galaxie";


}
