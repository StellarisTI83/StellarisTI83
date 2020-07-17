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

char* lc_charger;
char* lc_nouvelle_partie;
char* lc_options;
char* lc_quitter;
char* lc_sauvegarder;
char* lc_retour;
char* lc_create_galaxie;

void setLanguage(char languageNumber){
	localeLanguage = languageNumber;
	initializeLanguage();
}

char getLanguage(){
	return localeLanguage;
}

char initializeLanguage(){
	char header = 0;
	int numeroString, taille = 0, fin = 0;
	char chaineTemporaire[100];
	ti_CloseAll();
	switch(localeLanguage){
		case LC_FR:
			languageVar = ti_Open("lcFR", "r");
			break;
		case LC_EN:
			languageVar = ti_Open("lcEN", "r");
			break;
	}
	ti_Read(&header, sizeof(char), 1, languageVar);
	if(header != localeLanguage){
		return LOCALE_ERROR;
	}
	numeroString = 1;
	fin = 1;
	while(fin){
		ti_Read(&taille, sizeof(int), 1, languageVar);
		strcpy(&chaineTemporaire, "Error");
		ti_Read(&chaineTemporaire, sizeof(char) * taille, 1, languageVar);
		switch(numeroString){
			case LC_CHARGER:
				lc_charger = malloc(taille * sizeof(char));
				strcpy(lc_charger, chaineTemporaire);
				break;
			case LC_NOUVELLE_PARTIE:
				lc_nouvelle_partie = malloc(taille * sizeof(char));
				strcpy(lc_nouvelle_partie, chaineTemporaire);
				break;
			case LC_OPTIONS:
				lc_options = malloc(taille * sizeof(char));
				strcpy(lc_options, chaineTemporaire);
				break;
			case LC_QUITTER:
				lc_quitter = malloc(taille * sizeof(char));
				strcpy(lc_quitter, chaineTemporaire);
				break;
			case LC_SAUVEGARDER:
				lc_sauvegarder = malloc(taille * sizeof(char));
				strcpy(lc_sauvegarder, chaineTemporaire);
				break;
			case LC_RETOUR:
				lc_retour = malloc(taille * sizeof(char));
				strcpy(lc_retour, chaineTemporaire);
				break;
			case LC_CREATE_GALAXIE:
				lc_create_galaxie = malloc(taille * sizeof(char));
				strcpy(lc_create_galaxie, chaineTemporaire);
				fin = 0;
				break;
		}
		numeroString++;
	}
	ti_Close(languageVar);
	return 1;
}


char* gettext(int numero){
	switch(numero){
		case LC_CHARGER:
			return lc_charger;
			break;
		case LC_NOUVELLE_PARTIE:
			return lc_nouvelle_partie;
			break;
		case LC_OPTIONS:
			return lc_options;
			break;
		case LC_QUITTER:
			return lc_quitter;
			break;
		case LC_SAUVEGARDER:
			return lc_sauvegarder;
			break;
		case LC_RETOUR:
			return lc_retour;
			break;
		case LC_CREATE_GALAXIE:
			return lc_create_galaxie;
			break;
		default:
			return "Error";
			break;
	}
}