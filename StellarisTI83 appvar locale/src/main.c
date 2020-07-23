#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include "main.h"
#include "locale/lcFR.h"
#include "locale/lcEN.h"
#include <fileioc.h>
#include <graphx.h>

char* lc_charger = NULL;
char* lc_nouvelle_partie = NULL;
char* lc_options = NULL;
char* lc_quitter = NULL;
char* lc_sauvegarder = NULL;
char* lc_retour = NULL;
char* lc_create_galaxie = NULL;

char localeLanguage;
ti_var_t languageVar;

int main(void)
{
	char header = 0;
	int numeroString = 1;
	char chaineTemporaire[100];
	int fin = 1, finBoucle = 1;
	int footer = -1, taille = 0;
	ti_CloseAll();
	localeLanguage = LC_FR;
	gfx_Begin();
	gfx_SetTextXY(50, 50);
	gfx_PrintString("1");
	while(finBoucle){
		switch(localeLanguage){
			case LC_FR:
				ti_CloseAll();
				languageVar = ti_Open("lcFR","w+");
				ti_Rewind(languageVar);
				header = LC_FR;
				loadFR();
				break;
			case LC_EN:
				ti_SetArchiveStatus(true, languageVar);
				ti_CloseAll();
				languageVar = ti_Open("lcEN","w+");
				ti_Rewind(languageVar);
				header = LC_EN;
				loadEN();
				finBoucle = 0;
				break;
		}
		ti_Write(&header, sizeof(char), 1, languageVar);

		gfx_PrintString("2");
		numeroString = LC_CHARGER;
		while(fin == 1){
			switch(numeroString){
				case LC_CHARGER:
					taille = strlen(lc_charger) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_charger, taille * sizeof(char), 1, languageVar);
					break;
				case LC_NOUVELLE_PARTIE:
					taille = strlen(lc_nouvelle_partie) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_nouvelle_partie, taille * sizeof(char), 1, languageVar);
					break;
				case LC_OPTIONS:
					taille = strlen(lc_options) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_options, taille * sizeof(char), 1, languageVar);
					break;
				case LC_QUITTER:
					taille = strlen(lc_quitter) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_quitter, taille * sizeof(char), 1, languageVar);
					break;
				case LC_SAUVEGARDER:
					taille = strlen(lc_sauvegarder) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_sauvegarder, taille * sizeof(char), 1, languageVar);
					break;
				case LC_RETOUR:
					taille = strlen(lc_retour) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_retour, taille * sizeof(char), 1, languageVar);
					break;
				case LC_CREATE_GALAXIE:
					taille = strlen(lc_create_galaxie) + 1;
					ti_Write(&taille, sizeof(int), 1, languageVar);
					ti_Write(lc_create_galaxie, taille * sizeof(char), 1, languageVar);
					fin = 0;
					break;
			}
			numeroString++;
		}
		gfx_PrintString("3");
		ti_Write(&footer, sizeof(char), 1, languageVar);
		localeLanguage++;
		fin = 1;
	}
	while (!os_GetCSC());
	ti_SetArchiveStatus(true, languageVar);
	ti_CloseAll();
	gfx_End();
    return 0;
}
