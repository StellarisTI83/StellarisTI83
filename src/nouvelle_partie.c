/*************************************************
*												 *
*  Fichier pour la cration de nouvelles parties  *
*											     *
*************************************************/
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
#include "map.h"
#include "nouvelle_partie.h"
#include "noms.c"
#include "sauvegarde.h"
#include "flottes.h"
#include "console.h"

#include "locale/locale.h"

/*message d'avertissement*/
int NouvellePartieAvertissement(Empire *joueur, Parametres *parametres)
{
	char key = 0, choix = 1, fin = 1, nouvellePartie = 1;
	gfx_SetDrawScreen();

	while(fin)
	{
		gfx_FillScreen(255);
		PrintCentered("ATTENTION", 50, 2, 3, 0);
		PrintCentered("Cela supprimera", 80, 2, 0, 0);
		PrintCentered("toute sauvegarde", 100, 2, 0, 0);
		PrintCentered("Continuer?", 140, 2, 0, 0);

		do
		{
			switch (key)
			{
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
			switch (choix)
			{
				case 0:
				PrintCentered("oui    ", 180, 2, 4, 0);
				PrintCentered("    non",180, 2, 0, 0);
				break;
				case 1:
				PrintCentered("oui    ", 180, 2, 0, 0);
				PrintCentered("    non", 180, 2, 4, 0);
				break;
			}
		} while((key = os_GetCSC()) != sk_Enter);
		switch (choix)
		{
			case 0:
			while (nouvellePartie)
			{
				fin = NouvellePartieEspece(joueur, parametres);
				if (fin)
				{
					nouvellePartie = QuitterNouvellePartieAvertissement();
				}
				else
				{
					return 0;
				}
			}
				return 1;
				break;
			case 1:
				return 1;
				break;
		}
	}
	return 1;
}

/*avertissement lorsqu'on veut quitter la creation d'une nouvelle partie*/
int QuitterNouvellePartieAvertissement()
{
	char key = 0, choix = 1, fin = 1;
	gfx_SetDrawScreen();
	gfx_FillScreen(255);
	PrintCentered("ATTENTION", 50, 2, 3, 0);
	PrintCentered("Voulez-vous", 80, 2, 0, 0);
	PrintCentered("vraiment quitter?", 100, 2, 0, 0);
	do
	{
		switch (key)
		{
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
		switch (choix)
		{
			case 0:
				PrintCentered("oui    ", 160, 2, 4, 0);
				PrintCentered("    non",160, 2, 0, 0);
			break;
			case 1:
				PrintCentered("oui    ", 160, 2, 0, 0);
				PrintCentered("    non", 160, 2, 4, 0);
			break;
		}
	} while((key = os_GetCSC()) != sk_Enter);
	switch (choix)
	{
		case 0:
			return 0;
			break;
		case 1:
			return 1;
			break;
	}
	return 1;
}

/**************************************Menu de nouvelle partie**************************************/
/*choix de l'espece*/
int NouvellePartieEspece(Empire *joueur, Parametres *parametres)
{
	char key = 0, choix = 0, fin = 1, i = 0;
	gfx_SetDrawBuffer();
	joueur->espece = 0;
	joueur->gouvernement = 0;
	joueur->principe1 = 20;
	joueur->principe2 = 20;
	joueur->principe3 = 20;
	while (30 >= i)
	{
	joueur->nom[i] = ' ';
	i++;
	}
	joueur->nom[30] = '\0';
	while(fin)
	{
		choix = joueur->espece;

		do
		{
			gfx_SwapDraw();		
			gfx_FillScreen(255);
			PrintCentered("Esp~ce", 20, 3, 0, 0);
			
			gfx_SetColor(6);
			
			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT-50);
			
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 1);
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
					PrintCentered("Humaine", 70, 2, 4, 30);
					PrintCentered("Robotis/e", 110, 2, 0, 30);
					PrintCentered("Reptilienne", 150, 2, 0, 30);
					PrintCentered("Aviaire", 190, 2, 0, 30);
					break;
				case 1:
					PrintCentered("Humaine", 70, 2, 0, 30);
					PrintCentered("Robotis/e", 110, 2, 4, 30);
					PrintCentered("Reptilienne", 150, 2, 0, 30);
					PrintCentered("Aviaire", 190, 2, 0, 30);
					break;
				case 2:
					PrintCentered("Humaine", 70, 2, 0, 30);
					PrintCentered("Robotis/e", 110, 2, 0, 30);
					PrintCentered("Reptilienne", 150, 2, 4, 30);
					PrintCentered("Aviaire", 190, 2, 0, 30);
					break;
				case 3:
					PrintCentered("Humaine", 70, 2, 0, 30);
					PrintCentered("Robotis/e", 110, 2, 0, 30);
					PrintCentered("Reptilienne", 150, 2, 0, 30);
					PrintCentered("Aviaire", 190, 2, 4, 30);
					break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key)
		{
			case sk_Clear:
				return 1;
				break;
			default:
				joueur->espece = choix;
				fin = NouvellePartieGouvernement(joueur, parametres);
				break;
		}
	}
	return 0;
}

/*choix du gouvernement*/
int NouvellePartieGouvernement(Empire *joueur, Parametres *parametres)
{
	char key = 0, choix = 0, fin = 1;
	while(fin)
	{
		choix = joueur->gouvernement;
		do
		{
			gfx_SwapDraw();
			
			gfx_FillScreen(255);
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
				joueur->gouvernement = choix;
				fin = NouvellePartiePrincipes(joueur, parametres);
				break;
		}
	}
	return 0;
}

/*nouvelle partie principes*/
int NouvellePartiePrincipes(Empire *joueur, Parametres *parametres)
{
	char key = 0, fin = 1, selection = 2, point = 3;
	char principe1 = -1, principe2 = -1, principe3 = -1, principe4 = -1, principe5 =-1;
	while(fin)
	{
		selection = 2;
		
		do
		{
			gfx_SwapDraw();
			gfx_FillScreen(255);
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
				fin = NouvellePartieNom(joueur, parametres);
				break;
		}
	}
	return 0;
}

/*nouvelle partie nom*/
int NouvellePartieNom(Empire *joueur, Parametres *parametres)
{
	char key = 0, fin = 1, lettre = 0, majuscule = 1, curseur = 0, i = 0, finBoucle = 0, erreur = 0;
	while(fin)
	{
		finBoucle = 0;
		do
		{
			if(erreur > 0){
				erreur--;
			}
			gfx_SwapDraw();
			gfx_FillScreen(255);
			PrintCentered("Nom", 20, 3, 0, 20);
			PrintCentered("Choississez le nom", 65, 1, 0, 30); 
			PrintCentered("de votre empire :", 80, 1, 0, 30); 
			PrintCentered("Entrer quand vous avez fini", 200, 1, 0, 30); 
			gfx_SetColor(6);
			
			if(erreur != 0){
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
		
			if (majuscule == 1)
			{
				switch(key)
				{
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
			else
			{
				switch(key)
				{
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
				joueur->nom[curseur] = lettre;
				lettre = '.';
				curseur++;
			}
			else if ((key == sk_Del) && (curseur > 0)){
				curseur--;
				joueur->nom[curseur] = ' ';
			}
			gfx_SetTextXY(70, 130);
			gfx_SetTextFGColor(0);
			gfx_SetTextBGColor(1);
			gfx_PrintString(joueur->nom);
			if (majuscule == 0)
			{
				gfx_SetTextFGColor(0);
				gfx_SetTextBGColor(1);
				gfx_PrintStringXY("minuscule", 70, 150);
			}
			else
			{
				gfx_SetTextFGColor(0);
				gfx_SetTextBGColor(1);
				gfx_PrintStringXY("MAJUSCULE", 70, 150);
			}
			if((key = os_GetCSC()) == sk_Enter)
			{
				if(joueur->nom[0] == ' ')
				{
					erreur = 200;
					finBoucle = 0;
				}
				else
				{
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
				fin = NouvellePartieParametres(joueur, parametres);
				break;
		}
	}
	return 0;
}

int NouvellePartieDrapeau(Empire *joueur, Parametres *parametres)
{
	char key = 0, choix = 0, fin = 1;
	while(fin)
	{
		do
		{
			gfx_SwapDraw();
			gfx_FillScreen(255);
			PrintCentered("Drapeau", 20, 3, 0, 0);
			
			gfx_SetColor(6);
			
			gfx_FillRectangle(0, 50, 65, LCD_HEIGHT-50);
			
			/*barre latérale*/
			PrintText("Esp~ce", 5, 80, 1, 5);
			PrintText("Gouv.", 5, 110, 1, 5);
			PrintText("Morales", 5, 140, 1, 5);
			PrintText("Nom", 5, 170, 1, 5);
			PrintText("Drapeau", 5, 200, 1, 5);
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
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key)
		{
			case sk_Clear:
				return 1;
				break;
			default:
				fin = NouvellePartieParametres(joueur, parametres);
				break;
		}
	}
	return 0;
}

int NouvellePartieParametres(Empire *joueur, Parametres *parametres)
{
	char key = 0, choix = 0, fin = 1;
	char nombreEmpiresChar[3] = "";
	parametres->nombreEmpires = 4;
	while(fin)
	{
		do
		{
			gfx_SwapDraw();
			
			gfx_FillScreen(255);
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
			switch(key)
			{
				case sk_Down:
					choix++;
					break;
				case sk_Up:
					choix--;
					break;
				case sk_Left:
					parametres->nombreEmpires--;
					break;
				case sk_Right:
					parametres->nombreEmpires++;
					break;
			}
			if (choix > 0) {choix = 0;}
			if (choix < 0) {choix = 0;}
			
			if (parametres->nombreEmpires > 6) {parametres->nombreEmpires = 6;}
			if (parametres->nombreEmpires < 2) {parametres->nombreEmpires = 2;}
			sprintf(nombreEmpiresChar, "%d", parametres->nombreEmpires);
			gfx_SetColor(1);
			gfx_FillRectangle_NoClip(100, 90, 200, 40);
			/*dessiner le choix*/
			switch (choix)
			{
				case 0:
					PrintCentered("Nombre d'empires", 70, 2, 4, 30);
					PrintCentered(nombreEmpiresChar, 100, 2, 4, 30);
					break;
			}
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key)
		{
			case sk_Clear:
				return 1;
				break;
			default:
				return 0;
				break;
		}
	}
	return 0;
}

/**********************************************Chargement de la partie**********************************************/

/******************nouvelle partie******************/
/*chargement de la nouvelle partie*/
void ChargementNouvellePartie(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche)
{
	ti_var_t sauvegarde;
	char fin = 0;
	Flotte* flotte = NULL;
	Empire* empire = NULL;
	int compteur = 0, compteurEmpires = 0;

	/*creer sauvegarde*/
	ti_CloseAll();
	sauvegarde = ti_Open("sauv", "w");
	joueur->credits = 100;
	joueur->minerais = 100;
	joueur->nourriture = 200;
	joueur->acier = 100;
	joueur->biensDeConsommation = 100;
	joueur->flotte = flotteJoueur;
	
	date->jour = 1;
	date->mois = 1;
	date->annee = 2200;
	date->vitesse = 0;
	date->vitesseSauvegardee = 1;
	date->horloge = 0;
	
	camera->x = 380;
	camera->y = 360;
	camera->xSysteme = 320;
	camera->ySysteme = 240;
	camera->vecteurx = 0;
	camera->vecteury = 0;
	camera->zoom = 1;
	camera->mapType = SYSTEME;
	camera->fenetre = MENU_AUCUN;
	camera->bloque = FALSE;
	camera->bougerFlotte = FALSE;

	fenetre->villes = NULL;
	fenetre->error = NO_ERROR;
	fenetre->commandPrompt = false;
	marche->valeurMinerai = 50;

	fin = ChargementNouvellePartieGalaxie(parametres, &sauvegarde, systemeStellaires, flotteJoueur, camera);
	
	//StellarisSauvegarde(&sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
	StellarisBoucle(&sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, flotteJoueur, fenetre, marche);
}

int ChargementNouvellePartieGalaxie(Parametres *parametres, ti_var_t *sauvegarde, SystemeStellaire *systemeStellaires, FlotteListe *flotteJoueur, Camera *camera)
{
	int *galaxie = NULL;
	int i = 0, j = 0, espaceEntreEtoiles = 50, barreDeChargement = 1, k = 0, etoile = 0, nombrePlanetes = 0, nombrePlanetesHabitables = 0, trouNoir = 0, fin = 0, nomInt = 0;
	int coefficientDeplacementStellaire = 100, coefficientX = 0, coefficientY = 0, rayon = ((espaceEntreEtoiles * LARGEUR_GALAXIE) - espaceEntreEtoiles) / 2 - 25, lane = 255, rayonInterieur = 50;
	int x = LIMITE_GAUCHE;
	int y = LIMITE_HAUT;
	int nombreHyperlanes = 0, hyperLane1 = 0, hyperLane2 = 0, hyperLane3 = 0, hyperlaneSup1, hyperlaneSup2;
	int planeteHabitable1 = 0, planeteHabitable2 = 0, planeteHabitable3 = 0, planeteHabitable4 = 0, planeteHabitable5 = 0, nombreAleatoire = 0;
	Flotte* flotte = NULL;
	Planete *planete = NULL;
	galaxie = malloc(LARGEUR_GALAXIE * LARGEUR_GALAXIE * 2 * sizeof(int));
	if(galaxie == 0)
	{
		return 0;
	}
	gfx_SetDrawScreen();
	gfx_FillScreen(255);
	PrintCentered(_(LC_CREATE_GALAXIE), 120, 1, 0, 0);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(49, 159, 222, 7);
	gfx_SetColor(4);
	//creer matrice
	while(j < LARGEUR_GALAXIE)
	{
		while(i < LARGEUR_GALAXIE*2)
		{
			galaxie[j*LARGEUR_GALAXIE*2	+i] = x;
			galaxie[j*LARGEUR_GALAXIE*2+i+1] = y;
			x += espaceEntreEtoiles;
			i += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, barreDeChargement += 3, 5);
		i = 0;
		x = LIMITE_GAUCHE;
		y += espaceEntreEtoiles;
		j++;
	}
	
	//aleatoire matrice
	i = 0;
	j = 0;
	while(j < LARGEUR_GALAXIE)
	{
		while(i < LARGEUR_GALAXIE*2)
		{
			x = galaxie[j*LARGEUR_GALAXIE*2+i];
			y = galaxie[j*LARGEUR_GALAXIE*2+i+1];
			coefficientX = randInt (-coefficientDeplacementStellaire, coefficientDeplacementStellaire);
			coefficientY = randInt (-coefficientDeplacementStellaire, coefficientDeplacementStellaire);
			x = x + (coefficientX / 5);
			y = y + (coefficientY / 5);
			galaxie[j*LARGEUR_GALAXIE*2+i] = x;
			galaxie[j*LARGEUR_GALAXIE*2+i+1] = y;
			i += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, barreDeChargement += 3, 5);
		i = 0;
		j++;
	}
	
	//arrondir matrice
	i = 0;
	j = 0;
	while(j < LARGEUR_GALAXIE)
	{
		while(i < LARGEUR_GALAXIE * 2)
		{
			x = galaxie[j*LARGEUR_GALAXIE*2+i];
			y = galaxie[j*LARGEUR_GALAXIE*2+i+1];
			if(((pow(x - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2) + pow(y - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2)) > (pow(rayon,2))) || ((pow(x - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2) + pow(y - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2)) < (pow(rayonInterieur = 50,2))))
			{
				x = 0;
				y = 0;
			}
			galaxie[j*LARGEUR_GALAXIE*2+i] = x;
			galaxie[j*LARGEUR_GALAXIE*2+i+1] = y;
			i += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, barreDeChargement += 3, 5);
		i = 0;
		j++;
	}
	
	k = 0;
	//enregistrer matrice et generer hyperlane
	i = 0;
	j = 0;
	while(j < LARGEUR_GALAXIE)
	{
		while(i < LARGEUR_GALAXIE*2)
		{
			memset_fast(&systemeStellaires[k], 0, sizeof(SystemeStellaire));
			x = galaxie[j*LARGEUR_GALAXIE*2+i];
			y = galaxie[j*LARGEUR_GALAXIE*2+i+1];
			systemeStellaires[k].x = x;
			systemeStellaires[k].y = y;
			//malloc de la station
			systemeStellaires[k].station = (Station*)malloc(sizeof(Station));
			memset(systemeStellaires[k].station, 0, sizeof(Station));
			systemeStellaires[k].station->ordreFile = CreerFileOrdres();
			etoile = randInt(1, 100);
			trouNoir = 0;
			systemeStellaires[k].niveauDeConnaissance = INCONNU;
			if(etoile <= 10)
			{
				etoile = ETOILE_TYPE_B;
			}
			else if(etoile <= 30)
			{
				etoile = ETOILE_TYPE_A;
			}
			else if(etoile <= 44)
			{
				etoile = ETOILE_TYPE_F;
			}
			else if(etoile <= 57)
			{
				etoile = ETOILE_TYPE_G;
			}
			else if(etoile <= 70)
			{
				etoile = ETOILE_TYPE_K;
			}
			else if(etoile <= 85)
			{
				etoile = ETOILE_TYPE_M;
			}
			else if(etoile <= 90)
			{
				etoile = ETOILE_TYPE_TROU_NOIR;
				trouNoir = TRUE;
			}
			else if(etoile <= 95)
			{
				etoile = ETOILE_TYPE_PULSAR;
			}
			else
			{
				etoile = ETOILE_TYPE_ETOILE_A_NEUTRONS;
			}
			systemeStellaires[k].station->niveauStation = AUCUNE;
			systemeStellaires[k].etoileType = etoile;
			systemeStellaires[k].nombrePlanetes = 0;
			nombrePlanetes = 0;
			nombrePlanetes = randInt(1, 100);
			if(nombrePlanetes <= 8)
			{
				nombrePlanetes = 1;
			}
			else if(nombrePlanetes <= 20)
			{
				nombrePlanetes = 2;
			}
			else if(nombrePlanetes <= 50)
			{
				nombrePlanetes = 3;
			}
			else if(nombrePlanetes <= 80)
			{
				nombrePlanetes = 4;
			}
			else
			{
				nombrePlanetes = 5;
			}
			if(trouNoir == 1)
			{
				nombrePlanetes = 0;
			}
			systemeStellaires[k].nombrePlanetes = nombrePlanetes;
			
			if (((systemeStellaires[k].etoileType == ETOILE_TYPE_M) || (systemeStellaires[k].etoileType == ETOILE_TYPE_G)) || (systemeStellaires[k].etoileType == ETOILE_TYPE_K))
			{
				nombrePlanetesHabitables = randInt(0, nombrePlanetes);
				systemeStellaires[k].nombrePlanetesHabitables = nombrePlanetesHabitables;
			}
			nombreHyperlanes = randInt(1, 15);
			
			hyperLane1 = 1;
			hyperLane2 = 1;
			hyperLane3 = 1;
			
			if(nombreHyperlanes == 1)
			{
				do
				{
					hyperlaneSup1 = randInt(1, 3);
					hyperlaneSup2 = randInt(1, 3);
				} while(hyperlaneSup1 == hyperlaneSup2);
				switch(hyperlaneSup1)
					{
					case 1:
						hyperLane1 = 0;
						break;
					case 2:
						hyperLane2 = 0;
						break;
					/*case 3:
						hyperLane3 = 0;
						break;
				}
				switch(hyperlaneSup2)
				{
					case 1:
						hyperLane1 = 0;
						break;
					case 2:
						hyperLane2 = 0;
						break;
					case 3:
						hyperLane3 = 0;
						break;*/
				}
			}
			else if(nombreHyperlanes <= 5)
			{
				hyperlaneSup1 = randInt(1, 3);
				switch(hyperlaneSup1)
				{
					case 1:
						hyperLane1 = 0;
						break;
					case 2:
						hyperLane2 = 0;
						break;
					/*case 3:
						hyperLane3 = 0;
						break;*/
				}
			}	
			else
			{
				hyperLane1 = 1;
				hyperLane2 = 1;
				//hyperLane3 = 1;
			}
			
			/*if((systemeStellaires[k - LARGEUR_GALAXIE].x == 0)||((systemeStellaires[k - 1].x == 0)||(systemeStellaires[k + 1].x == 0)))
			{
				hyperLane1 = 1;
				hyperLane2 = 1;
				hyperLane3 = 1;
			}*/
			
			systemeStellaires[k].hyperlane1 = 255;
			if((systemeStellaires[k - LARGEUR_GALAXIE].x != 0) && (hyperLane1))
			{
				systemeStellaires[k].hyperlane1 = k - LARGEUR_GALAXIE;
			}
			
			systemeStellaires[k].hyperlane2 = 255;
			if((systemeStellaires[k - 1].x != 0) && (hyperLane2))
			{
				systemeStellaires[k].hyperlane2 = k - 1;
			}
			
			systemeStellaires[k].hyperlane3 = 255;
			if((systemeStellaires[k + 1].x != 0) && (hyperLane3))
			{
				systemeStellaires[k].hyperlane3 = k + 1;
			}
			
			nomInt = randInt(0, (sizeof(nomGalaxies)/sizeof(nomGalaxies[0])) - 1);
			strcpy(systemeStellaires[k].nom, nomGalaxies[nomInt]);
			
			
			//gestion des planetes
			
			if(nombrePlanetes >= 1)
			{
				planeteHabitable1 = randInt(1, 10);
			}			
			if(nombrePlanetes >= 2)
			{
				planeteHabitable2 = randInt(1, 10);
			}			
			if(nombrePlanetes >= 3)
			{
				planeteHabitable3 = randInt(1, 10);
			}			
			if(nombrePlanetes >= 4)
			{
				planeteHabitable4 = randInt(1, 10);
			}			
			if(nombrePlanetes == 5)
			{
				planeteHabitable5 = randInt(1, 10);
			} 
			systemeStellaires[k].planete1 = 0;
			systemeStellaires[k].planete2 = 0;
			systemeStellaires[k].planete3 = 0;
			systemeStellaires[k].planete4 = 0;
			systemeStellaires[k].planete5 = 0;
			
			if(nombrePlanetes >= 1) //allouer l'espace memoire des planetes
			{
				systemeStellaires[k].planete1 = (Planete*)malloc(sizeof(Planete));
				memset(systemeStellaires[k].planete1, 0, sizeof(Planete));
			}			
			if(nombrePlanetes >= 2)
			{
				systemeStellaires[k].planete2 = (Planete*)malloc(sizeof(Planete));
				memset(systemeStellaires[k].planete2, 0, sizeof(Planete));
			}			
			if(nombrePlanetes >= 3)
			{
				systemeStellaires[k].planete3 = (Planete*)malloc(sizeof(Planete));
				memset(systemeStellaires[k].planete3, 0, sizeof(Planete));
			}			
			if(nombrePlanetes >= 4)
			{
				systemeStellaires[k].planete4 = (Planete*)malloc(sizeof(Planete));
				memset(systemeStellaires[k].planete4, 0, sizeof(Planete));
			}			
			if(nombrePlanetes == 5)
			{
				systemeStellaires[k].planete5 = (Planete*)malloc(sizeof(Planete));
				memset(systemeStellaires[k].planete5, 0, sizeof(Planete));
			}
			
			switch(nombrePlanetes) //initialiser les planetes
			{
				case 5:
					systemeStellaires[k].planete5->habitable = 0;
					systemeStellaires[k].planete5->rayonOrbite = randInt(RAYON_PLANETE5, RAYON_PLANETE5 + 10);
					systemeStellaires[k].planete5->x = randInt(X_CENTRE_SYSTEME - systemeStellaires[k].planete5->rayonOrbite, X_CENTRE_SYSTEME + systemeStellaires[k].planete5->rayonOrbite); //aleatoire de x
					systemeStellaires[k].planete5->y = sqrt(pow((double)systemeStellaires[k].planete5->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete5->x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME; //calcule de y pour ce x
					
					systemeStellaires[k].planete5->type = randInt(1, 100);
					if(systemeStellaires[k].planete5->type <= 60) {
						systemeStellaires[k].planete5->type = 14; //geante gazeuse
						systemeStellaires[k].planete5->habitable = 0;
					}
					else if (systemeStellaires[k].planete5->type <= 90){
						systemeStellaires[k].planete5->type = 11; //monde aride froid
						systemeStellaires[k].planete5->habitable = 0;
					}
					else {
						systemeStellaires[k].planete5->type = 12; //monde brisé
						systemeStellaires[k].planete5->habitable = 0;
					}
					
					systemeStellaires[k].planete5->taille = randInt(1, 100);
					if(systemeStellaires[k].planete5->type == 14) {
						systemeStellaires[k].planete5->taille = 5;
					}
					if(systemeStellaires[k].planete5->taille <= 10) {
						systemeStellaires[k].planete5->taille = 2;
					}
					else if(systemeStellaires[k].planete5->taille <= 70) {
						systemeStellaires[k].planete5->taille = 3;
					}
					else if(systemeStellaires[k].planete5->taille <= 90) {
						systemeStellaires[k].planete5->taille = 4;
					}
					else {
						systemeStellaires[k].planete5->taille = 5;
					}
					
					if(randInt(0, 1) == 1) {
						systemeStellaires[k].planete5->y = Y_CENTRE_SYSTEME - (systemeStellaires[k].planete5->y - Y_CENTRE_SYSTEME);
					}
					systemeStellaires[k].planete5->villes = NULL;
				
				case 4:
					systemeStellaires[k].planete4->habitable = 0;
					systemeStellaires[k].planete4->rayonOrbite = randInt(RAYON_PLANETE4, RAYON_PLANETE4 + 10);
					systemeStellaires[k].planete4->x = randInt(X_CENTRE_SYSTEME - systemeStellaires[k].planete4->rayonOrbite, X_CENTRE_SYSTEME + systemeStellaires[k].planete4->rayonOrbite);
					systemeStellaires[k].planete4->y = sqrt(pow((double)systemeStellaires[k].planete4->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete4->x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME;	
					
					systemeStellaires[k].planete4->type = randInt(1, 100);
					if(systemeStellaires[k].planete4->type <= 30) {
						systemeStellaires[k].planete4->type = 14; //geante gazeuse
						systemeStellaires[k].planete4->habitable = 0;
					}
					else if (systemeStellaires[k].planete4->type <= 70){
						systemeStellaires[k].planete4->type = 11; //monde aride froid
						systemeStellaires[k].planete4->habitable = 0;
					}
					else if (systemeStellaires[k].planete4->type <= 75){
						systemeStellaires[k].planete4->type = 4; //monde Alpine habitable
						systemeStellaires[k].planete4->habitable = 1;
					}
					else if (systemeStellaires[k].planete4->type <= 80){
						systemeStellaires[k].planete4->type = 5; //monde Arctic froid
						systemeStellaires[k].planete4->habitable = 1;
					}
					else if (systemeStellaires[k].planete4->type <= 85){
						systemeStellaires[k].planete4->type = 6; //monde Tundra habitable
						systemeStellaires[k].planete4->habitable = 1;
					}
					else if (systemeStellaires[k].planete4->type <= 90){
						systemeStellaires[k].planete4->type = 7; //monde continental froid
						systemeStellaires[k].planete4->habitable = 1;
					}
					else {
						systemeStellaires[k].planete4->type = 12; //monde brisé
						systemeStellaires[k].planete4->habitable = 0;
					}
					
					if(systemeStellaires[k].planete4->habitable == 1) {
						strcpy(systemeStellaires[k].planete4->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						// strcpy(systemeStellaires[k].planete4->nom, "habitabl");
					}
					
					systemeStellaires[k].planete4->taille = randInt(1, 100);
					if(systemeStellaires[k].planete4->taille <= 10) {
						systemeStellaires[k].planete4->taille = 2;
					}
					else if(systemeStellaires[k].planete4->taille <= 70) {
						systemeStellaires[k].planete4->taille = 3;
					}
					else if(systemeStellaires[k].planete4->taille <= 90) {
						systemeStellaires[k].planete4->taille = 4;
					}
					else {
						systemeStellaires[k].planete4->taille = 5;
					}
					if(randInt(0, 1) == 1)
					{
						systemeStellaires[k].planete4->y = Y_CENTRE_SYSTEME - (systemeStellaires[k].planete4->y - Y_CENTRE_SYSTEME);
					}
					systemeStellaires[k].planete4->villes = NULL;
					
				case 3:
					systemeStellaires[k].planete3->habitable = 0;
					systemeStellaires[k].planete3->rayonOrbite = randInt(RAYON_PLANETE3, RAYON_PLANETE3 + 10);
					systemeStellaires[k].planete3->x = randInt(X_CENTRE_SYSTEME - systemeStellaires[k].planete3->rayonOrbite, X_CENTRE_SYSTEME + systemeStellaires[k].planete3->rayonOrbite);
					systemeStellaires[k].planete3->y = sqrt(pow((double)systemeStellaires[k].planete3->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete3->x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME;
					
					systemeStellaires[k].planete3->type = randInt(1, 100);
					if(systemeStellaires[k].planete3->type <= 5) {
						systemeStellaires[k].planete3->type = 14; //geante gazeuse
						systemeStellaires[k].planete3->habitable = 0;
					}
					else if (systemeStellaires[k].planete3->type <= 20){
						systemeStellaires[k].planete3->type = 16; //monde toxique
						systemeStellaires[k].planete3->habitable = 0;
					}					
					else if (systemeStellaires[k].planete3->type <= 60){
						systemeStellaires[k].planete3->type = 11; //monde aride froid
						systemeStellaires[k].planete3->habitable = 0;
					}
					else if (systemeStellaires[k].planete3->type <= 70){
						systemeStellaires[k].planete3->type = 4; //monde Alpine habitable
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 74){
						systemeStellaires[k].planete3->type = 5; //monde Arctic froid
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 78){
						systemeStellaires[k].planete3->type = 6; //monde Tundra habitable
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 82){
						systemeStellaires[k].planete3->type = 7; //monde continental froid
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 86){
						systemeStellaires[k].planete3->type = 8; //monde ocean
						systemeStellaires[k].planete3->habitable = 1;
					}
					else {
						systemeStellaires[k].planete3->type = 12; //monde brisé
						systemeStellaires[k].planete3->habitable = 0;
					}
					
					if(systemeStellaires[k].planete3->habitable == 1){
						strcpy(systemeStellaires[k].planete3->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						//strcpy(systemeStellaires[k].planete3->nom, "habitabl");
					}
					
					systemeStellaires[k].planete3->taille = randInt(1, 100);
					if(systemeStellaires[k].planete3->taille <= 10) {
						systemeStellaires[k].planete3->taille = 2;
					}
					else if(systemeStellaires[k].planete3->taille <= 70) {
						systemeStellaires[k].planete3->taille = 3;
					}
					else if(systemeStellaires[k].planete3->taille <= 90) {
						systemeStellaires[k].planete3->taille = 4;
					}
					else {
						systemeStellaires[k].planete3->taille = 5;
					}
					if(randInt(0, 1) == 1)
					{
						systemeStellaires[k].planete3->y = Y_CENTRE_SYSTEME - (systemeStellaires[k].planete3->y - Y_CENTRE_SYSTEME);
					}
					systemeStellaires[k].planete3->villes = NULL;
					
				case 2:
					systemeStellaires[k].planete2->habitable = 0;
					systemeStellaires[k].planete2->rayonOrbite =  randInt(RAYON_PLANETE2, RAYON_PLANETE2 + 10);
					systemeStellaires[k].planete2->x = randInt(X_CENTRE_SYSTEME - systemeStellaires[k].planete2->rayonOrbite, X_CENTRE_SYSTEME + systemeStellaires[k].planete2->rayonOrbite);
					systemeStellaires[k].planete2->y = sqrt(pow((double)systemeStellaires[k].planete2->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete2->x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME;	
					
					systemeStellaires[k].planete2->type = randInt(1, 100);
					if(systemeStellaires[k].planete2->type <= 2) {
						systemeStellaires[k].planete2->type = 14; //geante gazeuse
						systemeStellaires[k].planete2->habitable = 0;
					}
					else if (systemeStellaires[k].planete2->type <= 30){
						systemeStellaires[k].planete2->type = 16; //monde toxique
						systemeStellaires[k].planete2->habitable = 0;
					}					
					else if (systemeStellaires[k].planete2->type <= 80){
						systemeStellaires[k].planete2->type = 10; //monde arid 
						systemeStellaires[k].planete2->habitable = 0;
					}
					else if (systemeStellaires[k].planete2->type <= 83){
						systemeStellaires[k].planete2->type = 7; //monde continental habitable
						systemeStellaires[k].planete2->habitable = 1;
					}
					else if (systemeStellaires[k].planete2->type <= 86){
						systemeStellaires[k].planete2->type = 8; //monde ocean habitable
						systemeStellaires[k].planete2->habitable = 1;
					}
					else if (systemeStellaires[k].planete2->type <= 89){
						systemeStellaires[k].planete2->type = 9; //monde tropical habitable
						systemeStellaires[k].planete2->habitable = 1;
					}
					else {
						systemeStellaires[k].planete2->type = 12; //monde brisé
						systemeStellaires[k].planete2->habitable = 0;
					}
					
					if(systemeStellaires[k].planete2->habitable == 1){
						strcpy(systemeStellaires[k].planete2->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						//strcpy(systemeStellaires[k].planete2->nom, "habitabl");
					}
					
					systemeStellaires[k].planete2->taille = randInt(1, 100);
					if(systemeStellaires[k].planete2->taille <= 10) {
						systemeStellaires[k].planete2->taille = 2;
					}
					else if(systemeStellaires[k].planete2->taille <= 70) {
						systemeStellaires[k].planete2->taille = 3;
					}
					else if(systemeStellaires[k].planete2->taille <= 90) {
						systemeStellaires[k].planete2->taille = 4;
					}
					else {
						systemeStellaires[k].planete2->taille = 5;
					}
					if(randInt(0, 1) == 1)
					{
						systemeStellaires[k].planete2->y = Y_CENTRE_SYSTEME - (systemeStellaires[k].planete2->y - Y_CENTRE_SYSTEME);
					}
					systemeStellaires[k].planete2->villes = NULL;
				
				case 1:
					systemeStellaires[k].planete1->habitable = 0;
					systemeStellaires[k].planete1->rayonOrbite = randInt(RAYON_PLANETE1, RAYON_PLANETE1 + 10);
					systemeStellaires[k].planete1->x = randInt(X_CENTRE_SYSTEME - systemeStellaires[k].planete1->rayonOrbite, X_CENTRE_SYSTEME + systemeStellaires[k].planete1->rayonOrbite);
					systemeStellaires[k].planete1->y = sqrt(pow((double)systemeStellaires[k].planete1->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete1->x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME;	
					
					systemeStellaires[k].planete1->type = randInt(1, 100);
					if(systemeStellaires[k].planete1->type <= 3) {
						systemeStellaires[k].planete1->type = 1; //monde arid
						systemeStellaires[k].planete1->habitable = 1;
					}
					else if (systemeStellaires[k].planete1->type <= 6){
						systemeStellaires[k].planete1->type = 2; //monde Desert
						systemeStellaires[k].planete1->habitable = 1;
					}					
					else if (systemeStellaires[k].planete1->type <= 9){
						systemeStellaires[k].planete1->type = 3; //monde Savanna
						systemeStellaires[k].planete1->habitable = 1;
					}
					else if (systemeStellaires[k].planete1->type <= 40){
						systemeStellaires[k].planete1->type = 10; //monde aride
						systemeStellaires[k].planete1->habitable = 0;
					}
					else if (systemeStellaires[k].planete1->type <= 50){
						systemeStellaires[k].planete1->type = 12; //monde brisé
						systemeStellaires[k].planete1->habitable = 0;
					}
					else{
						systemeStellaires[k].planete1->type = 15; //monde en fusion
						systemeStellaires[k].planete1->habitable = 0;
					}

					if(systemeStellaires[k].planete1->habitable == 1){
						strcpy(systemeStellaires[k].planete1->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						//strcpy(systemeStellaires[k].planete1->nom, "habitabl");
					}

					systemeStellaires[k].planete1->taille = randInt(1, 100);
					if(systemeStellaires[k].planete1->taille <= 10) {
						systemeStellaires[k].planete1->taille = 2;
					}
					else if(systemeStellaires[k].planete1->taille <= 70) {
						systemeStellaires[k].planete1->taille = 3;
					}
					else if(systemeStellaires[k].planete1->taille <= 90) {
						systemeStellaires[k].planete1->taille = 4;
					}
					else {
						systemeStellaires[k].planete1->taille = 5;
					}
					systemeStellaires[k].planete1->villes = NULL;
			}
			
			
			k++;
			i += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, barreDeChargement += 3, 5);
		i = 0;
		j++;
	}
	free(galaxie);
	
	i = 0;
	//recreation des hyperlanes
	while(i < k) {
		if(systemeStellaires[i].hyperlane1 != 255) {
			systemeStellaires[systemeStellaires[i].hyperlane1].hyperlane3 = i;
		}
		if(systemeStellaires[k].hyperlane2 != 255) {
			systemeStellaires[systemeStellaires[i].hyperlane2].hyperlane4 = i;
		}
		i++;
	}
	
	fin = 1;

	//choix d'un systeme
	while(fin == 1)
	{
		i = randInt(0, k);
		if(((systemeStellaires[i].x >= 160) && (systemeStellaires[i].y >= 120)) && (systemeStellaires[i].etoileType != ETOILE_TYPE_TROU_NOIR))
		{
			fin = 0;
			
			systemeStellaires[i].etoileType = ETOILE_TYPE_K;
			systemeStellaires[i].nombrePlanetesHabitables = 1;
			systemeStellaires[i].nombrePlanetesHabitees = 1;
			systemeStellaires[i].empire = 1;
			systemeStellaires[i].niveauDeConnaissance = TOTAL;

			systemeStellaires[i].station->niveauStation = PORT_STELLAIRE;
			systemeStellaires[i].station->puissance = 450;
			systemeStellaires[i].station->coqueTotal = 10000;
			systemeStellaires[i].station->coqueVie = 10000;
			systemeStellaires[i].station->blindageTotal = 2000;
			systemeStellaires[i].station->blindageVie = 2000;
			systemeStellaires[i].station->bouclierTotal = 4000;
			systemeStellaires[i].station->bouclierVie = 4000;
			systemeStellaires[i].station->module1 = CHANTIER_SPATIAL;
			systemeStellaires[i].station->module2 = CARREFOUR_COMMERCIAL;

			flotte = NouvelleFlotte(flotteJoueur, i, FLOTTE_MILITAIRE, 3, 0, 0, 0);
			flotte->x = X_CENTRE_SYSTEME + 10;
			flotte->y = Y_CENTRE_SYSTEME - 10;

			flotte = NouvelleFlotte(flotteJoueur, i, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);
			flotte->x = X_CENTRE_SYSTEME + 10;
			flotte->y = Y_CENTRE_SYSTEME + 10;

			flotte = NouvelleFlotte(flotteJoueur, i, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);
			flotte->x = X_CENTRE_SYSTEME - 10;
			flotte->y = Y_CENTRE_SYSTEME + 10;

			camera->x = systemeStellaires[i].x;
			camera->y = systemeStellaires[i].y;
			camera->systeme = i;
			camera->systemeSelectione = i;

			switch(randInt(1, systemeStellaires[i].nombrePlanetes))
			{
				case 1:
					planete = systemeStellaires[i].planete1;
					break;
				case 2:
					planete = systemeStellaires[i].planete2;
					break;
				case 3:
					planete = systemeStellaires[i].planete3;
					break;
				case 4:
					planete = systemeStellaires[i].planete4;
					break;
				case 5:
					planete = systemeStellaires[i].planete3;
					break;
			}
			planete->habitable = 1;
			planete->type = 7;
			camera->xSysteme = planete->x - 160;
			camera->ySysteme = planete->y - 120;
			strcpy(planete->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
			planete->villes = calloc(1, sizeof(Villes));
			planete->population = 27;
			planete->villes->districtsUrbains = 4;
			planete->villes->districtsGenerateurs = 3;
			planete->villes->districtsMiniers = 3;
			planete->villes->districtsAgricoles = 3;
			planete->villes->emplois = 27;
			planete->villes->ordreFile = CreerFileOrdres();
			planete->villes->batiment1 = CAPITALE;
			planete->villes->niveauBatiment1 = 3;
			planete->villes->batiment2 = USINE_CIVILE;
			planete->villes->niveauBatiment2 = 1;
			planete->villes->batiment3 = FONDERIE;
			planete->villes->niveauBatiment3 = 1;
		}
	}
	return 1;
}

/**
 *utilisé dans les menus de création de nouvelle partie pour afficher le texte
 */
void PrintText(const char *str, int x, int y, int taille, int color)
{
	int a, longueur, i;
    gfx_TempSprite(ch, 8, 8);

	/*fait un "fond vert" au sprite et le rend transparent*/
	
    gfx_SetTextFGColor(color);
    gfx_SetTextBGColor(TEXT_BG_COLOR);
	gfx_SetTransparentColor(TEXT_BG_COLOR);
	
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
