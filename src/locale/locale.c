#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include "locale.h"
#include <fileioc.h>

char localeLanguage;
ti_var_t languageVar;

char* lc_charger_array;
char* lc_nouvelle_partie_array;
char* lc_options_array;
char* lc_quitter_array;
char* lc_sauvegarder_array;
char* lc_retour_array;
char* lc_create_galaxie_array;

void setLanguage(char languageNumber){
	localeLanguage = languageNumber;
	initializeLanguage();
}

char getLanguage(){
	return localeLanguage;
}

char initializeLanguage(){
	char header = 0;
	uint16_t indexString, taille = 0, fin = 0;
	char chaineTemporaire[MAX_VALUE_LENGTH];
	uint16_t numberOfString = 0;

	ti_CloseAll();
	switch(localeLanguage){
		case LC_FR:
			languageVar = ti_Open("lcFR", "r");
			break;
		case LC_EN:
			languageVar = ti_Open("lcEN", "r");
			break;
	}
	ti_Read(&numberOfString, sizeof(uint16_t), 1, languageVar);

	indexString = 0;
	dbg_printf("of %d:", numberOfString);

	while(fin < numberOfString) {
		memset(&chaineTemporaire, 0, sizeof(char) * MAX_VALUE_LENGTH);
		ti_Read(&taille, sizeof(uint16_t), 1, languageVar);
		strcpy(&chaineTemporaire, "Error");
		if(taille < MAX_VALUE_LENGTH){
			ti_Read(&indexString, sizeof(uint16_t), 1, languageVar);
			ti_Read(&chaineTemporaire, sizeof(char) * taille, 1, languageVar);
			switch(indexString){
				case lc_load:
					lc_charger_array = calloc(1, (taille + 1) * sizeof(char));
					memcpy(lc_charger_array, chaineTemporaire, taille);
					break;
				case lc_new_game:
					lc_nouvelle_partie_array = calloc(1, (taille + 1) * sizeof(char));
					memcpy(lc_nouvelle_partie_array, chaineTemporaire, taille);
					break;
				case lc_settings:
					lc_options_array = calloc(1, (taille + 1) * sizeof(char));;
					memcpy(lc_options_array, chaineTemporaire, taille);
					break;
				case lc_exit:
					lc_quitter_array = calloc(1, (taille + 1) * sizeof(char));;
					memcpy(lc_quitter_array, chaineTemporaire, taille);
					break;
				case lc_save:
					lc_sauvegarder_array = calloc(1, (taille + 1) * sizeof(char));;
					memcpy(lc_sauvegarder_array, chaineTemporaire, taille);
					break;
				case lc_back:
					lc_retour_array = calloc(1, (taille + 1) * sizeof(char));;
					memcpy(lc_retour_array, chaineTemporaire, taille);
					break;
				case lc_create_galaxie:
					lc_create_galaxie_array = calloc(1, (taille + 1) * sizeof(char));;
					memcpy(lc_create_galaxie_array, chaineTemporaire, taille);
					break;
			}
		}
		fin++;
		dbg_printf("%d (s:%d, k:%d)'%s'\n", fin, taille, indexString, chaineTemporaire);
	}
	ti_Close(languageVar);
	return 1;
}


char* gettext(local_keys numero){
	switch(numero){
		case lc_load:
			return lc_charger_array;
			break;
		case lc_new_game:
			return lc_nouvelle_partie_array;
			break;
		case lc_settings:
			return lc_options_array;
			break;
		case lc_exit:
			return lc_quitter_array;
			break;
		case lc_save:
			return lc_sauvegarder_array;
			break;
		case lc_back:
			return lc_retour_array;
			break;
		case lc_create_galaxie:
			return lc_create_galaxie_array;
			break;
		default:
			return "Error";
			break;
	}
}