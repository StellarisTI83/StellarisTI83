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

#include "fonts/Standard.h"
#include "gfx/gfx.h"

#include "main.h"
#include "nouvelle_partie.h"
#include "ai.h"
#include "parametres.h"

#include "locale/locale.h"

/* private functions =================================================== */

/**
 * Initialise toutes les variables
 */
static void InitializeAll(EmpireListe **empireListe, Parametres *parametres){
	Empire *joueur = NULL;
	ti_CloseAll();
	
	srandom(rtc_Time());
	
	gfx_Begin();
    gfx_SetDrawBuffer();
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
	gfx_SetFontData(font_logo);
	gfx_SetMonospaceFont(8);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);

	setLanguage(LC_FR);	

	*empireListe = EmpireListeCreer();
	joueur = EmpireAjouter(*empireListe);
	CreerEmpireFlotte(EmpireNumero(*empireListe, 1));
	
}

/**
 * Menu des options
 */
static void Options(){
	char key = 0, choix = 0, fin = 1;
	char language = getLanguage();
	char* languageStr = "Francais";
	while(fin) {
		do {
			gfx_SwapDraw();
			
			gfx_FillScreen(255);
			PrintCentered("Parametres", 20, 3, 0, 0);
			switch(key){
				case sk_Down:
					choix++;
					break;
				case sk_Up:
					choix--;
					break;
				case sk_Left:
					language--;
					break;
				case sk_Right:
					language++;
					break;
			}
			
			if (choix > 0) {
				choix = 0;
			}
			if (choix < 0) {
				choix = 0;
			}
			
			if(language > 2){language = 1;}
			if(language < 1){language = 2;}
			/*dessiner le choix*/
			switch(language){
				case LC_FR:
					languageStr = "Francais";
					break;
				case LC_EN:
					languageStr = "English";
					break;
			}
			switch (choix)
			{
				case 0:
					PrintCentered("Langue", 70, 2, 4, 0);
					PrintCentered(languageStr, 100, 2, 4, 0);
					break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		
		switch (key)
		{
			case sk_Clear:
				return;
				break;
			case sk_Enter:
				setLanguage(language);
				return;
				break;
		}
	}
	return;
}

/**
 * Menu principal
 */
static int MainMenu(EmpireListe *empireListe, Parametres *parametres){
	char choix = 0, fin = 0, key = 0, erreur = 0;
	int espacement = 0, niveau = 0;

	gfx_SetDrawBuffer();
	/*imprimer tout*/
	
	gfx_SetMonospaceFont(8);
	gfx_SetTextBGColor(4);
	gfx_SetTextFGColor(1);
	gfx_SetColor(1);

	/*faire le choix*/
	while((key = os_GetCSC()) != sk_Enter) {
		gfx_ZeroScreen();
		PrintCentered("Stellaris", 10, 3, 1, -30);
		gfx_PrintStringXY(VERSION_LOGICIEL, 5, 220);
		gfx_SetTextXY(LCD_WIDTH - strlen(__DATE__) * 8 - strlen(__TIME__) * 8 - 36, 220);
		gfx_PrintString(" ");
		gfx_PrintString(__DATE__);
		gfx_PrintString(" ");
		gfx_PrintString(__TIME__);
		switch(key) {
			case sk_Down:
				choix++;
				break;
			case sk_Up:
				choix--;
				break;
			case sk_Clear:
				return fin;
				break;
		}
		if(boot_CheckOnPressed()) {
			return fin;
		}
		if (choix > 3) {choix = 0;}
		if (choix < 0) {choix = 3;}
		gfx_SetTextXY(5, 140);
		niveau = 100;
		espacement = 15;
		
		/*Dessine le menu suivant le choix*/
		if(choix == 0) {
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
		}
		gfx_PrintStringXY(_(LC_CHARGER), 5, niveau);
		gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_CHARGER)) * 8);
		gfx_SetTextFGColor(1);
		gfx_SetColor(1);

		if(choix == 1) {
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
		}
		niveau += espacement;
		gfx_PrintStringXY(_(LC_NOUVELLE_PARTIE), 5, niveau);
		gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_NOUVELLE_PARTIE)) * 8);
		gfx_SetTextFGColor(1);
		gfx_SetColor(1);

		if(choix == 2) {
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
		}
		niveau += espacement;
		gfx_PrintStringXY(_(LC_OPTIONS), 5, niveau);
		gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_OPTIONS)) * 8);
		gfx_SetTextFGColor(1);
		gfx_SetColor(1);

		if(choix == 3){
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
		}
		niveau += espacement;
		gfx_PrintStringXY(_(LC_QUITTER), 5, niveau);
		gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_QUITTER)) * 8);
		gfx_SetTextFGColor(1);
		gfx_SetColor(1);

		/*Passe le buffer à l'écran*/
		gfx_SwapDraw();
	}
	/*On arrive ici si le joueur clique sur entrée et on vérifie son choix*/
	switch (choix){
		case 0:
			/*erreur = ChargementAnciennePartie(empireListe);
			if(erreur == 1){
				gfx_FillScreen(255);
				PrintCentered("Aucune sauvegarde", 96, 2, 0, 0);
				while(!os_GetCSC());
				fin = 1;
			}*/
			fin = 1;
			break;
		case 1:
			fin = NouvellePartieAvertissement(empireListe, parametres);
			if (fin == 0){
				/*lancer la nouvelle partie*/
				ChargementNouvellePartie(empireListe, parametres);
			}
			break;
		case 2:
			Options();
			fin = 1;
			break;
		case 3:
			fin = 0;
			break;
	}
	return fin;
}

/* entry points ======================================================== */

/**
 * Imprime un texte centré
 * 
 * \param *str String à imprimer
 * \param Y position y où imprimer
 * \param taille Taille du texte (int)
 * \param color Couleur du texte
 * \param differenceX Décalage en X du texte
 */ 
void PrintCentered(const char *str, int y, int taille, int color, int differenceX){
    int x, a, longueur, i;
    gfx_TempSprite(ch, 8, 8);
	gfx_SetFontData(font_logo);

	/*fait un "fond vert" au sprite et le rend transparent*/
    gfx_SetTextFGColor(color);
    gfx_SetTextBGColor(TEXT_BG_COLOR);
	gfx_SetTransparentColor(TEXT_BG_COLOR);
	
    x = abs((LCD_WIDTH - strlen(str) * 8 * taille)/2 + differenceX);
	a = 1;
	i = 0;
	
	/*sort chaque char dans un sprite et le grossit à la taille désirée
	on peut pas grossir un string sinon
	police du logo de stellaris*/
    while (a != 0)
    {
		ch = gfx_GetSpriteChar(str[i]);
        gfx_ScaledTransparentSprite_NoClip(ch, x, y, taille, taille);
		a = str[i];
		x += 8 * taille;
		i++;
    }
}

/**
 * Imprime un entier de la bonne longueur suivant sa taille. 
 * Utilise gfx_PrintInt.
 */
void PrintInt(int nombre){
	if(nombre < 10){
		gfx_PrintInt(nombre, 1);
	}
	else if(nombre < 100){
		gfx_PrintInt(nombre, 2);
	}
	else if(nombre < 1000){
		gfx_PrintInt(nombre, 3);
	}
	else if(nombre < 10000){
		gfx_PrintInt(nombre, 4);
	}
	else  if(nombre < 100000){
		gfx_PrintInt(nombre, 5);
	}
	else{
		gfx_PrintInt(nombre, 6);		
	}
}

/**
 * Renvoie la taille du int envoyé (nombre de chiffres)
 */
int TailleInt(int nombre){
	int i = 1;
	if(nombre < 0){
		nombre = -nombre;
	}
	while(nombre >= 1){
		nombre /= 10;
		i++;
	}
	return i;
}

/**
 * MAIN
 */

int main(void){
	char fin = 1, partie = 0;
	
	EmpireListe *empireListe = NULL;
	Parametres *parametres = NULL;

	InitializeAll(&empireListe, parametres);
	while (fin){
		fin = MainMenu(empireListe, parametres);
	}
	
	gfx_End();
	
    return 0;
}