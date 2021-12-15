#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <string.h>

#include <debug.h>
#include <fileioc.h>


#include "main.h"

#include "locale.h"

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

localeLanguageEnum getLanguage(){
	return localeLanguage;
}

char initializeLanguage(){
	uint16_t indexString, taille = 0, index = 0;
	char chaineTemporaire[MAX_VALUE_LENGTH];
	char name[4] = {0};
	uint16_t numberOfString = 0;

	switch(localeLanguage){
		case LC_FR:
			strcpy(name, "lcFR");
			break;
		case LC_EN:
			strcpy(name, "lcEN");
			break;
		default:
			#ifdef DEBUG_VERSION
			dbg_sprintf(dbgerr, "No language with id %d\n", localeLanguage);
			#endif
			break;
	}
	if(name[0])
		languageVar = ti_Open(name, "r");

	ti_Read(&numberOfString, sizeof(uint16_t), 1, languageVar);

	indexString = 0;
	#ifdef DEBUG_VERSION
	dbg_printf("\nInitialize language '%s'\nof %d:", name, numberOfString);
	#endif

	while(index < numberOfString) {
		memset(&chaineTemporaire, 0, sizeof(char) * MAX_VALUE_LENGTH);
		ti_Read(&taille, sizeof(uint16_t), 1, languageVar);
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
				default:
					#ifdef DEBUG_VERSION
					dbg_sprintf(dbgerr, "No array with id %d\n", indexString);
					#endif
					break;
			}
		}
		index++;
		dbg_printf("%d (s:%d, k:%d)'%s'\n", index, taille, indexString, chaineTemporaire);
	}
	#ifdef DEBUG_VERSION
	dbg_sprintf(dbgout, "\n");
	#endif
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