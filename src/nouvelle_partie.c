/*************************************************
*												 *
*  Fichier pour la cration de nouvelles parties  *
*											     *
*************************************************/
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>

#include <graphx.h>
#include <fileioc.h>

#include "gfx/gfx.h"

#include "main.h"

#include "ai.h"
#include "boucle.h"
#include "camera.h"
#include "console.h"
#include "flottes.h"
#include "galaxy.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "notifications.h"
#include "parametres.h"
#include "systemes.h"
#include "time.h"

#include "locale/locale.h"



/* private functions =================================================== */
static int QuitterNouvellePartieAvertissement();
static int NouvellePartieEspece(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieGouvernement(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartiePrincipes(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieNom(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieParametres(EmpireListe *empireListe, Parametres *parametres);

/**
 * avertissement lorsqu'on veut quitter la creation d'une nouvelle partie
 */
int QuitterNouvellePartieAvertissement(){
	char key = 0, choix = 1;
	do {
		gfx_FillScreen(1);
		PrintCentered("ATTENTION", 50, 2, 3, 0);
		PrintCentered("Voulez-vous", 80, 2, 0, 0);
		PrintCentered("vraiment quitter?", 100, 2, 0, 0);
		switch (key) {
		case sk_Right:
			choix++;
			break;
		case sk_Left:
			choix--;
			break;
		}
		if (choix > 2) {choix = 0;}
		if (choix < 0) {choix = 2;}
		/*dessiner le choix*/
		switch (choix) {
		case 0:
			PrintCentered("oui    ", 160, 2, 4, 0);
			PrintCentered("    non",160, 2, 0, 0);
			break;
		case 1:
			PrintCentered("oui    ", 160, 2, 0, 0);
			PrintCentered("    non", 160, 2, 4, 0);
			break;
		}
		gfx_SwapDraw();
	} while((key = os_GetCSC()) != sk_Enter);
	switch (choix) {
	case 0: // si oui
		return 0;
		break;
	case 1: // si non
		return 1;
		break;
	}
	return 1;
}

/**
 * choix de l'espece
 */
int NouvellePartieEspece(EmpireListe *empireListe, Parametres *parametres){
	char key = 0, fin = 1;
	int choix = 0;
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);
	while(fin) {
		choix = GetEmpireSpecies(joueur);
		do {
			gfx_SwapDraw();		
			gfx_FillScreen(1);
			PrintCentered("Esp~ce", 20, 3, 0, 0);
			
			gfx_SetColor(6);
			
			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT - 50);
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 1);
			PrintText("Morales", 5, 140, 1, 1);
			PrintText("Nom", 5, 170, 1, 1);
			PrintText("Drapeau", 5, 200, 1, 1);
			PrintText("Parametres", 5, 230, 1, 1);

			switch(key) {
				case sk_Down:
					choix++;
					break;
				case sk_Up:
					choix--;
					break;
			}
			if (choix > 3) {choix = 0;}
			if (choix < 0) {choix = 3;}
			/*dessiner le choix*/
			switch (choix) {
			case 0:
				PrintCentered("Humaine", 70, 2, 4, 30);
				PrintCentered("Mamalienne", 110, 2, 0, 30);
				PrintCentered("Molluscoide", 150, 2, 0, 30);
				PrintCentered("Reptilienne", 190, 2, 0, 30);
				break;
			case 1:
				PrintCentered("Humaine", 70, 2, 0, 30);
				PrintCentered("Mamalienne", 110, 2, 4, 30);
				PrintCentered("Molluscoide", 150, 2, 0, 30);
				PrintCentered("Reptilienne", 190, 2, 0, 30);
				break;
			case 2:
				PrintCentered("Humaine", 70, 2, 0, 30);
				PrintCentered("Mamalienne", 110, 2, 0, 30);
				PrintCentered("Molluscoide", 150, 2, 4, 30);
				PrintCentered("Reptilienne", 190, 2, 0, 30);
				break;
			case 3:
				PrintCentered("Humaine", 70, 2, 0, 30);
				PrintCentered("Mamalienne", 110, 2, 0, 30);
				PrintCentered("Molluscoide", 150, 2, 0, 30);
				PrintCentered("Reptilienne", 190, 2, 4, 30);
				break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key) {
			case sk_Clear:
				return 1;
				break;
			default:
				SetEmpireSpecies(joueur, choix);
				fin = NouvellePartieGouvernement(empireListe, parametres);
				break;
		}
	}
	return 0;
}

/**
 * Choix du gouvernement
 */
int NouvellePartieGouvernement(EmpireListe *empireListe, Parametres *parametres){
	char key = 0, choix = 0, fin = 1;
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);
	while(fin) {
		choix = 0;
		do {
			gfx_SwapDraw();
			
			gfx_FillScreen(1);
			PrintCentered("Gouvernement", 20, 3, 0, 0);
			
			gfx_SetColor(6);
			
			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT-50);
			
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 5);
			PrintText("Morales", 5, 140, 1, 1);
			PrintText("Nom", 5, 170, 1, 1);
			PrintText("Drapeau", 5, 200, 1, 1);
			PrintText("Parametres", 5, 230, 1, 1);
			
			switch(key)
			{
				case sk_Down:
					choix++;
					break;
				case sk_Up:
					choix--;
					break;
			}
			if (choix > 3) {choix = 0;}
			if (choix < 0) {choix = 3;}
			/*dessiner le choix*/
			switch (choix)
			{
				case 0:
					PrintCentered("D/mocratie", 70, 2, 4, 30);
					PrintCentered("Dictature", 110, 2, 0, 30);
					PrintCentered("Corporation", 150, 2, 0, 30);
					PrintCentered("Ruche", 190, 2, 0, 30);
					break;
				case 1:
					PrintCentered("D/mocratie", 70, 2, 0, 30);
					PrintCentered("Dictature", 110, 2, 4, 30);
					PrintCentered("Corporation", 150, 2, 0, 30);
					PrintCentered("Ruche", 190, 2, 0, 30);
					break;
				case 2:
					PrintCentered("D/mocratie", 70, 2, 0, 30);
					PrintCentered("Dictature", 110, 2, 0, 30);
					PrintCentered("Corporation", 150, 2, 4, 30);
					PrintCentered("Ruche", 190, 2, 0, 30);
					break;
				case 3:
					PrintCentered("D/mocratie", 70, 2, 0, 30);
					PrintCentered("Dictature", 110, 2, 0, 30);
					PrintCentered("Corporation", 150, 2, 0, 30);
					PrintCentered("Ruche", 190, 2, 4, 30);
					break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key)
		{
			case sk_Clear:
				return 1;
				break;
			default:
				SetEmpireGouvernement(joueur, choix);
				fin = NouvellePartieNom(empireListe, parametres);
				break;
		}
	}
	return 0;
}

/*nouvelle partie principes*/
int NouvellePartiePrincipes(EmpireListe *empireListe, Parametres *parametres) {
	char key = 0, fin = 1, selection = 2;
	char principe1 = -1, principe2 = -1, principe3 = -1, principe4 = -1;
	while(fin) {
		selection = 2;
		
		do {
			gfx_SwapDraw();
			gfx_FillScreen(1);
			PrintCentered("Morales", 20, 3, 0, 0);
			
			gfx_SetColor(6);
			
			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT-50);
			
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 5);
			PrintText("Morales", 5, 140, 1, 5);
			PrintText("Nom", 5, 170, 1, 1);
			PrintText("Drapeau", 5, 200, 1, 1);
			PrintText("Parametres", 5, 230, 1, 1);
			
			gfx_SetTransparentColor(2);
			
			/*dessiner les sprites des morales*/
			gfx_TransparentSprite_NoClip(ethic_fanatic_militarist, 190, 50);
			gfx_TransparentSprite_NoClip(ethic_militarist, 190, 90);
			gfx_TransparentSprite_NoClip(ethic_gestalt_consciousness, 190, 130);
			gfx_TransparentSprite_NoClip(ethic_pacifist, 190, 170);
			gfx_TransparentSprite_NoClip(ethic_fanatic_pacifist, 190, 210);
			
			gfx_TransparentSprite_NoClip(ethic_fanatic_authoritarian, 110, 130);
			gfx_TransparentSprite_NoClip(ethic_authoritarian, 150, 130);
			gfx_TransparentSprite_NoClip(ethic_egalitarian, 230, 130);
			gfx_TransparentSprite_NoClip(ethic_fanatic_egalitarian, 270, 130);		
			
			gfx_TransparentSprite_NoClip(ethic_fanatic_spiritualists, 133, 73);
			gfx_TransparentSprite_NoClip(ethic_spiritualists, 162, 102);
			gfx_TransparentSprite_NoClip(ethic_materialist, 218, 158);
			gfx_TransparentSprite_NoClip(ethic_fanatic_materialist, 247, 187);
			
			gfx_TransparentSprite_NoClip(ethic_fanatic_xenophile, 133, 187);
			gfx_TransparentSprite_NoClip(ethic_xenophile, 162, 158);
			gfx_TransparentSprite_NoClip(ethic_xenophobe, 218, 102);
			gfx_TransparentSprite_NoClip(ethic_fanatic_xenophobe, 247, 73);
			
			if (key)
			{
				gfx_TransparentSprite_NoClip(ethic_fanatic_militarist, 190, 50);
				gfx_TransparentSprite_NoClip(ethic_militarist, 190, 90);
				gfx_TransparentSprite_NoClip(ethic_gestalt_consciousness, 190, 130);
				gfx_TransparentSprite_NoClip(ethic_pacifist, 190, 170);
				gfx_TransparentSprite_NoClip(ethic_fanatic_pacifist, 190, 210);
				
				gfx_TransparentSprite_NoClip(ethic_fanatic_authoritarian, 110, 130);
				gfx_TransparentSprite_NoClip(ethic_authoritarian, 150, 130);
				gfx_TransparentSprite_NoClip(ethic_egalitarian, 230, 130);
				gfx_TransparentSprite_NoClip(ethic_fanatic_egalitarian, 270, 130);		
				
				gfx_TransparentSprite_NoClip(ethic_fanatic_spiritualists, 133, 73);
				gfx_TransparentSprite_NoClip(ethic_spiritualists, 162, 102);
				gfx_TransparentSprite_NoClip(ethic_materialist, 218, 158);
				gfx_TransparentSprite_NoClip(ethic_fanatic_materialist, 247, 187);

				gfx_TransparentSprite_NoClip(ethic_fanatic_xenophile, 133, 187);
				gfx_TransparentSprite_NoClip(ethic_xenophile, 162, 158);
				gfx_TransparentSprite_NoClip(ethic_xenophobe, 218, 102);
				gfx_TransparentSprite_NoClip(ethic_fanatic_xenophobe, 247, 73);
				
				switch(principe1)
				{
					case 1:
						gfx_TransparentSprite_NoClip(ethic_choice, 190, 50);
						break;
					case 2:
						gfx_TransparentSprite_NoClip(ethic_choice, 190, 90);
						break;	
					case 3:
						gfx_TransparentSprite_NoClip(ethic_choice, 190, 130);
						break;
					case 4:
						gfx_TransparentSprite_NoClip(ethic_choice, 190, 170);
						break;
					case 5:
						gfx_TransparentSprite_NoClip(ethic_choice, 110, 210);
						break;
				}
				switch(principe2)
				{					
					case 0:
						gfx_TransparentSprite_NoClip(ethic_choice, 110, 130);
						break;
					case 1:
						gfx_TransparentSprite_NoClip(ethic_choice, 150, 130);
						break;	
					case 4:
						gfx_TransparentSprite_NoClip(ethic_choice, 230, 130);
						break;
					case 5:
						gfx_TransparentSprite_NoClip(ethic_choice, 270, 130);
						break;
				}
				switch(principe3)
				{
					case 0:
						gfx_TransparentSprite_NoClip(ethic_choice, 133, 73);
						break;
					case 1:
						gfx_TransparentSprite_NoClip(ethic_choice, 162, 102);
						break;	
					case 4:
						gfx_TransparentSprite_NoClip(ethic_choice, 218, 158);
						break;
					case 5:
						gfx_TransparentSprite_NoClip(ethic_choice, 247, 187);
						break;
				}
				switch(principe4)
				{
					case 0:
						gfx_TransparentSprite_NoClip(ethic_choice, 133, 187);
						break;
					case 1:
						gfx_TransparentSprite_NoClip(ethic_choice, 162, 158);
						break;	
					case 4:
						gfx_TransparentSprite_NoClip(ethic_choice, 218, 102);
						break;
					case 5:
						gfx_TransparentSprite_NoClip(ethic_choice, 247, 73);
						break;
				}
			}
			switch(selection)
			{				
				case 0:
					switch(key)
					{
						case sk_2:
						selection = 1;
						break;
					}
					break;
				case 1:
					switch(key)
					{
						case sk_2:
						selection = 2;
						break;
						case sk_8:
						selection = 0;
						break;
					}
					break;
				case 2:
					switch(key)
					{
						case sk_2:
						selection = 3;
						break;
						case sk_8:
						selection = 1;
						break;
						case sk_4:
						selection = 6;
						break;
						case sk_6:
						selection = 7;
						break;
						case sk_7:
						selection = 10;
						break;
						case sk_3:
						selection = 11;
						break;
						case sk_1:
						selection = 14;
						break;
						case sk_9:
						selection = 15;
						break;
					}
					break;
				case 3:
					switch(key)
					{
						case sk_2:
						selection = 4;
						break;
						case sk_8:
						selection = 2;
						break;
					}
					break;
				case 4:
					switch(key)
					{
						case sk_8:
						selection = 3;
						break;
					}
					break;
				case 5:
					switch(key)
					{
						case sk_6:
						selection = 6;
						break;
					}
					break;
				case 6:
					switch(key)
					{
						case sk_4:
						selection = 5;
						break;
						case sk_6:
						selection = 2;
						break;
					}
					break;
				case 7:
					switch(key)
					{
						case sk_4:
						selection = 2;
						break;
						case sk_6:
						selection = 8;
						break;
					}
					break;
				case 8:
					switch(key)
					{
						case sk_4:
						selection = 7;
						break;
					}
					break;
				case 9:
					switch(key)
					{
						case sk_3:
						selection = 10;
						break;
					}
					break;
				case 10:
					switch(key)
					{
						case sk_7:
						selection = 9;
						break;
						case sk_3:
						selection = 2;
						break;
					}
					break;
				case 11:
					switch(key)
					{
						case sk_7:
						selection = 2;
						break;
						case sk_3:
						selection = 12;
						break;
					}
					break;
				case 12:
					switch(key)
					{
						case sk_7:
						selection = 11;
						break;
					}
					break;
				case 13:
					switch(key)
					{
						case sk_9:
						selection = 14;
						break;
					}
					break;
				case 14:
					switch(key)
					{
						case sk_1:
						selection = 13;
						break;
						case sk_9:
						selection = 2;
						break;
					}
					break;
				case 15:
					switch(key)
					{
						case sk_1:
						selection = 2;
						break;
						case sk_9:
						selection = 16;
						break;
					}
					break;
				case 16:
					switch(key)
					{
						case sk_1:
						selection = 15;
						break;
					}
					break;					
			}
			if (selection > 16) {selection = 0;}
			if (selection < 0) {selection = 16;}
			if (key == sk_5)
			{
				switch(selection)
				{
					case 0:
						principe1 = 1;
						break;
					case 1:
						principe1 = 2;
						break;
					case 2:
						principe1 = 3;
						break;
					case 3:
						principe1 = 4;
						break;
					case 4:
						principe1 = 5;
						break;
					case 5:
						principe2 = 1;
						break;
					case 6:
						principe2 = 2;
						break;
					case 7:
						principe2 = 4;
						break;
					case 8:
						principe2 = 5;
						break;
					case 9:
						principe3 = 1;
						break;
					case 10:
						principe3 = 2;
						break;
					case 11:
						principe3 = 4;
						break;
					case 12:
						principe3 = 5;
						break;
					case 13:
						principe4 = 1;
						break;
					case 14:
						principe4 = 2;
						break;
					case 15:
						principe4 = 4;
						break;
					case 16:
						principe4 = 5;
						break;
				}
			}
			/*dessiner la selection*/
			switch(selection)
			{
				case 0:
					gfx_TransparentSprite_NoClip(ethic_selected, 190, 50);
					break;
				case 1:
					gfx_TransparentSprite_NoClip(ethic_selected, 190, 90);
					break;	
				case 2:
					gfx_TransparentSprite_NoClip(ethic_selected, 190, 130);
					break;
				case 3:
					gfx_TransparentSprite_NoClip(ethic_selected, 190, 170);
					break;
				case 4:
					gfx_TransparentSprite_NoClip(ethic_selected, 190, 210);
					break;
				case 5:
					gfx_TransparentSprite_NoClip(ethic_selected, 110, 130);
					break;
				case 6:
					gfx_TransparentSprite_NoClip(ethic_selected, 150, 130);
					break;
				case 7:
					gfx_TransparentSprite_NoClip(ethic_selected, 230, 130);
					break;
				case 8:
					gfx_TransparentSprite_NoClip(ethic_selected, 270, 130);
					break;
				case 9:
					gfx_TransparentSprite_NoClip(ethic_selected, 133, 73);
					break;
				case 10:
					gfx_TransparentSprite_NoClip(ethic_selected, 162, 102);
					break;
				case 11:
					gfx_TransparentSprite_NoClip(ethic_selected, 218, 158);
					break;	
				case 12:
					gfx_TransparentSprite_NoClip(ethic_selected, 247, 187);
					break;
				case 13:
					gfx_TransparentSprite_NoClip(ethic_selected, 133, 187);
					break;
				case 14:
					gfx_TransparentSprite_NoClip(ethic_selected, 162, 158);
					break;
				case 15:
					gfx_TransparentSprite_NoClip(ethic_selected, 218, 102);
					break;
				case 16:
					gfx_TransparentSprite_NoClip(ethic_selected, 247, 73);
					break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key)
		{
			case sk_Clear:
				return 1;
				break;
			default:
				fin = NouvellePartieParametres(empireListe, parametres);
				break;
		}
	}
	return 0;
}

/**
 * Choix du nom de l'empire
 */
int NouvellePartieNom(EmpireListe *empireListe, Parametres *parametres) {
	char key = 0, fin = 1, lettre = 0, majuscule = 1, curseur = 0, finBoucle = 0, erreur = 0;
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);
	while(fin) {
		finBoucle = 0;
		do {
			if(erreur > 0) {
				erreur--;
			}
			gfx_SwapDraw();
			gfx_FillScreen(1);
			PrintCentered("Nom", 20, 3, 0, 20);
			PrintCentered("Choississez le nom", 65, 1, 0, 30); 
			PrintCentered("de votre empire :", 80, 1, 0, 30); 
			PrintCentered("Entrer quand vous avez fini", 200, 1, 0, 30); 
			gfx_SetColor(6);
			
			if(erreur != 0) {
					gfx_SetTextFGColor(3);
					gfx_SetTextBGColor(1);
					gfx_PrintStringXY("Votre nom doit commencer par", 70, 160);
					gfx_PrintStringXY("une lettre", 70, 170);
			}

			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT-50);
			
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 5);
			PrintText("Morales", 5, 140, 1, 5);
			PrintText("Nom", 5, 170, 1, 5);
			PrintText("Drapeau", 5, 200, 1, 1);
			PrintText("Parametres", 5, 230, 1, 1);
		
			if (majuscule == 1) {
				switch(key) {
					case sk_Math:
						lettre = 'A';
						break;
					case sk_Apps:
						lettre = 'B';
						break;
					case sk_Prgm:
						lettre = 'C';
						break;
					case sk_Recip:
						lettre = 'D';
						break;
					case sk_Sin:
						lettre = 'E';
						break;
					case sk_Cos:
						lettre = 'F';
						break;
					case sk_Tan:
						lettre = 'G';
						break;
					case sk_Power:
						lettre = 'H';
						break;
					case sk_Square:
						lettre = 'I';
						break;
					case sk_Comma:
						lettre = 'J';
						break;
					case sk_LParen:
						lettre = 'K';
						break;
					case sk_RParen:
						lettre = 'L';
						break;
					case sk_Div:
						lettre = 'M';
						break;
					case sk_Log:
						lettre = 'N';
						break;
					case sk_7:
						lettre = 'O';
						break;
					case sk_8:
						lettre = 'P';
						break;
					case sk_9:
						lettre = 'Q';
						break;
					case sk_Mul:
						lettre = 'R';
						break;
					case sk_Ln:
						lettre = 'S';
						break;
					case sk_4:
						lettre = 'T';
						break;
					case sk_5:
						lettre = 'U';
						break;
					case sk_6:
						lettre = 'V';
						break;
					case sk_Sub:
						lettre = 'W';
						break;
					case sk_Store:
						lettre = 'X';
						break;
					case sk_1:
						lettre = 'Y';
						break;
					case sk_2:
						lettre = 'Z';
						break;
					case sk_0:
						lettre = ' ';
						break;
					case sk_Alpha:
							majuscule = 0;
						break;
				}
			}
			else {
				switch(key) {
					case sk_Math:
						lettre = 'a';
						break;
					case sk_Apps:
						lettre = 'b';
						break;
					case sk_Prgm:
						lettre = 'c';
						break;
					case sk_Recip:
						lettre = 'd';
						break;
					case sk_Sin:
						lettre = 'e';
						break;
					case sk_Cos:
						lettre = 'f';
						break;
					case sk_Tan:
						lettre = 'g';
						break;
					case sk_Power:
						lettre = 'h';
						break;
					case sk_Square:
						lettre = 'i';
						break;
					case sk_Comma:
						lettre = 'j';
						break;
					case sk_LParen:
						lettre = 'k';
						break;
					case sk_RParen:
						lettre = 'l';
						break;
					case sk_Div:
						lettre = 'm';
						break;
					case sk_Log:
						lettre = 'n';
						break;
					case sk_7:
						lettre = 'o';
						break;
					case sk_8:
						lettre = 'p';
						break;
					case sk_9:
						lettre = 'q';
						break;
					case sk_Mul:
						lettre = 'r';
						break;
					case sk_Ln:
						lettre = 's';
						break;
					case sk_4:
						lettre = 't';
						break;
					case sk_5:
						lettre = 'u';
						break;
					case sk_6:
						lettre = 'v';
						break;
					case sk_Sub:
						lettre = 'w';
						break;
					case sk_Store:
						lettre = 'x';
						break;
					case sk_1:
						lettre = 'y';
						break;
					case sk_2:
						lettre = 'z';
						break;
					case sk_0:
						lettre = ' ';
						break;
					case sk_Alpha:
						majuscule = 1;
						break;
				}
			}
			if (((lettre != '.') && (key &&  29 >= curseur)) && ((key != sk_Del && key != sk_Alpha) && key != sk_Enter)){
				SetEmpireName(joueur, curseur, lettre);
				lettre = '.';
				curseur++;
			}
			else if ((key == sk_Del) && (curseur > 0)){
				curseur--;
				SetEmpireName(joueur, curseur, ' ');
			}
			gfx_SetTextXY(70, 130);
			gfx_SetTextFGColor(0);
			gfx_SetTextBGColor(1);
			gfx_PrintString(GetEmpireNameString(joueur));
			if (majuscule == 0) {
				gfx_SetTextFGColor(0);
				gfx_SetTextBGColor(1);
				gfx_PrintStringXY("minuscule", 70, 150);
			} else {
				gfx_SetTextFGColor(0);
				gfx_SetTextBGColor(1);
				gfx_PrintStringXY("MAJUSCULE", 70, 150);
			}
			if((key = os_GetCSC()) == sk_Enter) {
				if(GetEmpireNameChar(joueur, 0) == ' ') {
					erreur = 200;
					finBoucle = 0;
				} else {
					finBoucle = 1;
				}
			}
		} while((finBoucle == 0) && (key != sk_Clear));
		switch (key)
		{
			case sk_Clear:
				return 1;
				break;
			default:
				fin = NouvellePartieParametres(empireListe, parametres);
				break;
		}
	}
	return 0;
}

int NouvellePartieParametres(EmpireListe *empireListe, Parametres *parametres)
{
	char key = 0, choix = 0, fin = 1, nombresEmpires = 4;
	char nombreEmpiresChar[3] = "";
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);

	while(fin) {
		do {
			gfx_SwapDraw();
			
			gfx_FillScreen(1);
			PrintCentered("Parametres", 20, 3, 0, 0);
			
			gfx_SetColor(6);
			
			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT-50);
			
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 5);
			PrintText("Morales", 5, 140, 1, 5);
			PrintText("Nom", 5, 170, 1, 5);
			PrintText("Drapeau", 5, 200, 1, 5);
			PrintText("Parametres", 5, 230, 1, 5);
			switch(key) {
				case sk_Down:
					choix++;
					break;
				case sk_Up:
					choix--;
					break;
				case sk_Left:
					nombresEmpires--;
					break;
				case sk_Right:
					nombresEmpires++;
					break;
			}
			if (choix > 0) {choix = 0;}
			if (choix < 0) {choix = 0;}
			
			if (nombresEmpires > 6) {nombresEmpires = 6;}
			if (nombresEmpires < 2) {nombresEmpires = 2;}
			sprintf(nombreEmpiresChar, "%d", nombresEmpires);
			gfx_SetColor(1);
			gfx_FillRectangle_NoClip(100, 90, 200, 40);
			/*dessiner le choix*/
			switch (choix) {
			case 0:
				PrintCentered("Nombre d'empires", 70, 2, 4, 30);
				PrintCentered(nombreEmpiresChar, 100, 2, 4, 30);
				break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key) {
			case sk_Clear:
				return 1;
				break;
			default:
				return 0; // il retourne tout en haut dans MainMenu() et ouvre ChargementNouvellePartie()
				break;
		}
	}
	return 0;
}

/**
 * Initialize all for a new game
 */
static int InitializeNewGame(EmpireListe **empireListe, Date **date, Camera **camera, Fenetre **fenetre, Marche **marche, Parametres **parametres, ti_var_t *sauvegarde){
	Empire *joueur = NULL;
	joueur = EmpireNumero(*empireListe, 1);
	ti_CloseAll();
	*sauvegarde = ti_Open("sauv", "w");

	*parametres = AllocParametres();

	SetSeeAll(*parametres, false);

	*date = AllocDate();
	SetTime(*date, 1, 1, 2200);
	SetTimeSpeed(*date, 0, 1);
	AddTimeClock(*date);

	*camera = AllocCamera();
	SetCameraX(*camera, 380);
	SetCameraY(*camera, 360);
	SetCameraXSystem(*camera, 320);
	SetCameraYSystem(*camera, 240);
	SetCameraXVector(*camera, 0);
	SetCameraYVector(*camera, 0);

	SetCameraZoom(*camera, 2);
	SetCameraMapType(*camera, SYSTEME);
	
	*fenetre = AllocFenetre();
	CloseMenu(*fenetre, *camera);

	CloseCommandPrompt(*fenetre, *camera, *date);
	return 0;
}


/* entry points ======================================================== */

/**
 * Message d'avertissement avant une nouvelle partie. 
 * Cela supprimera toute sauvegarde
 */
int NouvellePartieAvertissement(EmpireListe *empireListe, Parametres *parametres){
	char key = 0, choix = 1, fin = 1, nouvellePartie = 1;
	gfx_FillScreen(0);
	gfx_SwapDraw();
	while(fin){
		do{
			gfx_FillScreen(7);
			PrintCentered("ATTENTION", 50, 2, 3, 0);
			PrintCentered("Cela supprimera", 80, 2, 1, 0);
			PrintCentered("toute sauvegarde", 100, 2, 1, 0);
			PrintCentered("Continuer?", 140, 2, 1, 0);
			switch (key){
				case sk_Right:
					choix++;
					break;
				case sk_Left:
					choix--;
					break;
			}
			if (choix > 2) {
				choix = 0;
			}
			if (choix < 0) {
				choix = 2;
			}

			/*dessiner le choix*/
			switch (choix){
			case 0:
				PrintCentered("oui    ", 180, 2, 4, 0);
				PrintCentered("    non",180, 2, 1, 0);
				break;
			case 1:
				PrintCentered("oui    ", 180, 2, 1, 0);
				PrintCentered("    non", 180, 2, 4, 0);
				break;
			}
			gfx_SwapDraw();
		} while((key = os_GetCSC()) != sk_Enter);
		switch (choix){
		case 0: // si "oui"
			while (nouvellePartie){
				fin = NouvellePartieEspece(empireListe, parametres);
				if (fin){
					nouvellePartie = QuitterNouvellePartieAvertissement();
				} else {
					return 0;
				}
			}
			return 1;
			break;
		case 1: // si "non"
			return 1;
			break;
		}
	}
	return 1;
}

/**
 * Entrée de la création d'une nouvelle partie
 */
void ChargementNouvellePartie(EmpireListe *empireListe, Parametres *parametres){
	ti_var_t sauvegarde;
	char fin = 0;
	Date *date = NULL;
	Camera *camera = NULL;
	Fenetre *fenetre = NULL;
	Marche *marche = NULL;
	SystemeStellaire *systemeStellaires[LARGEUR_GALAXIE * LARGEUR_GALAXIE];
	NotificationList *notificationList = CreateNotificationList();
	
	InitializeNewGame(&empireListe, &date, &camera, &fenetre, &marche, &parametres, &sauvegarde);

	fin = ChargementNouvellePartieGalaxie(systemeStellaires);
	
	SetEmpireNumber(parametres, 4);
	CreerEmpires(parametres, empireListe, systemeStellaires, camera);
	UpdatePlayersData(false, empireListe, systemeStellaires, notificationList);

	PauseGame(date);

	gfx_SetDrawBuffer();
	StellarisBoucle(&sauvegarde, empireListe, parametres, date, systemeStellaires, camera, fenetre, marche, notificationList);
}

/**
 *utilisé dans les menus de création de nouvelle partie pour afficher le texte
 */
void PrintText(const char *str, int x, int y, int taille, int color) {
	int a, i;
    gfx_TempSprite(ch, 8, 8);

	/*fait un "fond vert" au sprite et le rend transparent*/
	
    gfx_SetTextFGColor(color);
    gfx_SetTextBGColor(TEXT_BG_COLOR);
	gfx_SetTransparentColor(TEXT_BG_COLOR);
	
	a = 1;
	i = 0;
	
    /*sort chaque char dans un sprite et le grossit à la taille désirée
	 * on peut pas grossir un string sinon
	 * police du logo de stellaris
	 */
    while (a != 0) {
		ch = gfx_GetSpriteChar(str[i]);
        gfx_ScaledTransparentSprite_NoClip(ch, x, y, taille, taille);
		a = str[i];
		x += 8 * taille;
		i++;
    }
}
