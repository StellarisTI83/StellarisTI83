#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>
/*fonts (desinclude)
#include "fonts/fonts.h"
*/
#include "fonts/Standard.h"
#include "gfx/gfx.h"
#include "main.h"
#include "noms.c"

#include "locale/locale.h"

int main(void)
{
	char fin = 1, partie = 0;
	
	Empire* joueur;
	EmpireListe* empireListe;
	Parametres parametres;
	Date date;
	Camera camera;
	SystemeStellaire* systemeStellaires= (SystemeStellaire*) malloc(sizeof(SystemeStellaire) * LARGEUR_GALAXIE * LARGEUR_GALAXIE);
	FlotteListe* flotteJoueur;
	Fenetre fenetre;
	Marche marche;

	ti_CloseAll();

	
	empireListe = EmpireListeCreer();
	joueur = EmpireAjouter(empireListe);
	flotteJoueur = FlotteListeCreer();
	

	memset(systemeStellaires, 0, sizeof(SystemeStellaire) * LARGEUR_GALAXIE * LARGEUR_GALAXIE);
	
	srandom(rtc_Time());
	
	gfx_Begin();
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);

	setLanguage(LC_FR);	
	
	while (fin)
	{
		fin = MainMenu(empireListe, joueur, &parametres, &date, systemeStellaires, &camera, flotteJoueur, &fenetre, &marche);
	}
	
	gfx_End();
	
    return 0;
}

/*main menu*/
int MainMenu(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche)
{
	char choix = 0, fin = 0, key = 0, erreur = 0;
	unsigned char car = 'è';
	unsigned int carInt = (int)car;
	gfx_SetDrawScreen();
	/*imprimer tout*/
	gfx_FillScreen(255);
    //fontlib_SetFont(test_font, 0);
    PrintCentered("Stellaris", 30, 4, 0, 0);
	gfx_SetMonospaceFont(8);
	gfx_SetTextBGColor(255);
	gfx_PrintStringXY(__DATE__, 10, 205);
	gfx_PrintStringXY(VERSION_LOGICIEL, LCD_WIDTH-strlen(VERSION_LOGICIEL)*8 - 20, 205);
	/*faire le choix*/
	while((key = os_GetCSC()) != sk_Enter)
	{
		switch (key)
		{
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
		if(boot_CheckOnPressed())	
		{
			return fin;
		}
		if (choix > 3) {choix = 0;}
		if (choix < 0) {choix = 3;}
		/*dessiner le choix*/
		switch (choix)
		{
			case 0:
				PrintCentered(_(LC_CHARGER), 80, 2, 4, 0);
				PrintCentered(_(LC_NOUVELLE_PARTIE), 110, 2, 0, 0);
				PrintCentered(_(LC_OPTIONS), 140, 2, 0, 0);
				PrintCentered(_(LC_QUITTER), 170, 2, 0, 0);
				break;
			case 1:
				PrintCentered(_(LC_CHARGER), 80, 2, 0, 0);
				PrintCentered(_(LC_NOUVELLE_PARTIE), 110, 2, 4, 0);
				PrintCentered(_(LC_OPTIONS), 140, 2, 0, 0);
				PrintCentered(_(LC_QUITTER), 170, 2, 0, 0);
				break;
			case 2:
				PrintCentered(_(LC_CHARGER), 80, 2, 0, 0);
				PrintCentered(_(LC_NOUVELLE_PARTIE), 110, 2, 0, 0);
				PrintCentered(_(LC_OPTIONS), 140, 2, 4, 0);
				PrintCentered(_(LC_QUITTER), 170, 2, 0, 0);
				break;
			case 3:
				PrintCentered(_(LC_CHARGER), 80, 2, 0, 0);
				PrintCentered(_(LC_NOUVELLE_PARTIE), 110, 2, 0, 0);
				PrintCentered(_(LC_OPTIONS), 140, 2, 0, 0);
				PrintCentered(_(LC_QUITTER), 170, 2, 4, 0);
				break;
		}	
	}
	switch (choix)
	{
		case 0:
			erreur = ChargementAnciennePartie(empireListe, joueur, parametres, date, systemeStellaires, camera, flotteJoueur, fenetre, marche);
			if(erreur == 1)
			{
				gfx_FillScreen(255);
				PrintCentered("Aucune sauvegarde", 96, 2, 0, 0);
				while(!os_GetCSC());
				fin = 1;
			}
			break;
		case 1:
			fin = NouvellePartieAvertissement(joueur, parametres);
			if (fin == 0)
			{
				/*lancer la nouvelle partie*/
				ChargementNouvellePartie(empireListe, joueur, parametres, date, systemeStellaires, camera, flotteJoueur, fenetre, marche);
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

void Options(){
	char key = 0, choix = 0, fin = 1;
	char language = getLanguage();
	char* languageStr = "Francais";
	while(fin)
	{
		do
		{
			gfx_SwapDraw();
			
			gfx_FillScreen(255);
			PrintCentered("Parametres", 20, 3, 0, 0);
			switch(key)
			{
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
			if (choix > 0) {choix = 0;}
			if (choix < 0) {choix = 0;}
			
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

/*message d'avertissement*/
int NouvellePartieAvertissement(Empire *joueur, Parametres *parametres)
{
	char key = 0, choix = 1, fin = 1, nouvellePartie = 1;
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
			if (((lettre != '.') && (key &&  29 >= curseur)) && ((key != sk_Del && key != sk_Alpha) && key != sk_Enter))
			{
				joueur->nom[curseur] = lettre;
				lettre = '.';
				curseur++;
			}
			else if ((key == sk_Del) && (curseur > 0))
			{
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
	int compteur = 0, compteurEmpires = 0;;
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
	date->horloge = 0;
	
	camera->x = 380;
	camera->y = 360;
	camera->vecteurx = 0;
	camera->vecteury = 0;
	camera->zoom = 1;
	camera->mapType = 1;
	camera->fenetre = MENU_AUCUN;
	camera->bloque = FALSE;
	camera->bougerFlotte = FALSE;

	marche->valeurMinerai = 50;

	ti_Write(parametres, sizeof(Parametres), 1, sauvegarde);
	ti_Write(date, sizeof(Date), 1, sauvegarde);
	ti_Write(camera, sizeof(Camera), 1, sauvegarde);
	ti_Write(marche, sizeof(Marche), 1, sauvegarde);
	fin = ChargementNouvellePartieGalaxie(parametres, &sauvegarde, systemeStellaires, flotteJoueur, camera);
	if(fin == 0)
	{
		return ;
	}

	compteur = 0;
	compteurEmpires = 0;

	empire = empireListe->premier;
	while(empire != NULL) {
		compteur++;
		empire = empire->suivant;
	}

	ti_Write(&compteur, sizeof(int), 1, sauvegarde);

	empire = empireListe->premier;
	while(empire != NULL) {
		ti_Write(empire, sizeof(Empire), 1, sauvegarde);

		//enregistrer flottes
		compteur = 0;
		flotte = empire->flotte->premier;
		while(flotte != NULL) {
			compteur++;
			flotte = flotte->suivant;
		}
		ti_Write(&compteur, sizeof(int), 1, sauvegarde);
		flotte = empire->flotte->premier;
		while(flotte != NULL) {
			ti_Write(flotte, sizeof(Flotte), 1, sauvegarde);
			flotte = flotte->suivant;
		}

		empire = empire->suivant;
	}
	
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
	
	
	
	//enregistrer matrice et generer hyperlane
	i = 0;
	j = 0;
	while(j < LARGEUR_GALAXIE)
	{
		while(i < LARGEUR_GALAXIE*2)
		{
			x = galaxie[j*LARGEUR_GALAXIE*2+i];
			y = galaxie[j*LARGEUR_GALAXIE*2+i+1];
			systemeStellaires[k].x = x;
			systemeStellaires[k].y = y;
			etoile = randInt(1, 100);
			trouNoir = 0;
			if(etoile <= 10)
			{
				etoile = 1;
			}
			else if(etoile <= 30)
			{
				etoile = 2;
			}
			else if(etoile <= 44)
			{
				etoile = 3;
			}
			else if(etoile <= 57)
			{
				etoile = 4;
			}
			else if(etoile <= 70)
			{
				etoile = 5;
			}
			else if(etoile <= 85)
			{
				etoile = 6;
			}
			else if(etoile <= 90)
			{
				etoile = 7;
				trouNoir = 1;
			}
			else if(etoile <= 95)
			{
				etoile = 8;
			}
			else
			{
				etoile = 9;
			}
			systemeStellaires[i].niveauStation = 0;
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
			
			if (((systemeStellaires[k].etoileType == 3) || (systemeStellaires[k].etoileType == 4)) || (systemeStellaires[k].etoileType == 5))
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
			}			
			if(nombrePlanetes >= 2)
			{
				systemeStellaires[k].planete2 = (Planete*)malloc(sizeof(Planete));
			}			
			if(nombrePlanetes >= 3)
			{
				systemeStellaires[k].planete3 = (Planete*)malloc(sizeof(Planete));
			}			
			if(nombrePlanetes >= 4)
			{
				systemeStellaires[k].planete4 = (Planete*)malloc(sizeof(Planete));
			}			
			if(nombrePlanetes == 5)
			{
				systemeStellaires[k].planete5 = (Planete*)malloc(sizeof(Planete));
			}
			
			switch(nombrePlanetes) //initialiser les planetes
			{
				case 5:
					systemeStellaires[k].planete5->habitable = 0;
					systemeStellaires[k].planete5->rayonOrbite = randInt(62, 70);
					systemeStellaires[k].planete5->x = randInt(114 - systemeStellaires[k].planete5->rayonOrbite, 114 + systemeStellaires[k].planete5->rayonOrbite); //aleatoire de x
					systemeStellaires[k].planete5->y = sqrt(pow((double)systemeStellaires[k].planete5->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete5->x - 114), 2.0)) + 126; //calcule de y pour ce x
					
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
						systemeStellaires[k].planete5->y = 126 - (systemeStellaires[k].planete5->y - 126);
					}
					systemeStellaires[k].planete5->population = 0;
				
				case 4:
					systemeStellaires[k].planete4->habitable = 0;
					systemeStellaires[k].planete4->rayonOrbite = randInt(50,56);
					systemeStellaires[k].planete4->x = randInt(114 - systemeStellaires[k].planete4->rayonOrbite, 114 + systemeStellaires[k].planete4->rayonOrbite);
					systemeStellaires[k].planete4->y = sqrt(pow((double)systemeStellaires[k].planete4->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete4->x - 114), 2.0)) + 126;	
					
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
						systemeStellaires[k].planete4->y = 126 - (systemeStellaires[k].planete4->y - 126);
					}
					systemeStellaires[k].planete4->population = 0;
					
				case 3:
					systemeStellaires[k].planete3->habitable = 0;
					systemeStellaires[k].planete3->rayonOrbite = randInt(40, 46);
					systemeStellaires[k].planete3->x = randInt(114 - systemeStellaires[k].planete3->rayonOrbite, 114 + systemeStellaires[k].planete3->rayonOrbite);
					systemeStellaires[k].planete3->y = sqrt(pow((double)systemeStellaires[k].planete3->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete3->x - 114), 2.0)) + 126;
					
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
						systemeStellaires[k].planete3->y = 126 - (systemeStellaires[k].planete3->y - 126);
					}
					systemeStellaires[k].planete3->population = 0;
					
				case 2:
					systemeStellaires[k].planete2->habitable = 0;
					systemeStellaires[k].planete2->rayonOrbite =  randInt(30, 36);
					systemeStellaires[k].planete2->x = randInt(114 - systemeStellaires[k].planete2->rayonOrbite, 114 + systemeStellaires[k].planete2->rayonOrbite);
					systemeStellaires[k].planete2->y = sqrt(pow((double)systemeStellaires[k].planete2->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete2->x - 114), 2.0)) + 126;	
					
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
						systemeStellaires[k].planete2->y = 126 - (systemeStellaires[k].planete2->y - 126);
					}
					systemeStellaires[k].planete2->population = 0;
				
				case 1:
					systemeStellaires[k].planete1->habitable = 0;
					systemeStellaires[k].planete1->rayonOrbite = randInt(20, 26);
					systemeStellaires[k].planete1->x = randInt(114 - systemeStellaires[k].planete1->rayonOrbite, 114 + systemeStellaires[k].planete1->rayonOrbite);
					systemeStellaires[k].planete1->y = sqrt(pow((double)systemeStellaires[k].planete1->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete1->x - 114), 2.0)) + 126;	
					
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
					systemeStellaires[k].planete1->population = 0;
			}
			
			systemeStellaires[k].niveauConnaissance = 0;
			
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
		if(systemeStellaires[k].hyperlane1 != 255) {
			systemeStellaires[systemeStellaires[k].hyperlane1].hyperlane3 = k;
		}
		if(systemeStellaires[k].hyperlane2 != 255) {
			systemeStellaires[systemeStellaires[k].hyperlane3].hyperlane4 = k;
		}
		i++;
	}
	
	fin = 1;

	//choix d'un systeme
	while(fin == 1)
	{
		i = randInt(0, k);
		if(((systemeStellaires[i].x >= 160) && (systemeStellaires[i].y >= 120)) && (systemeStellaires[i].etoileType != 7))
		{
			systemeStellaires[i].etoileType = 5;
			systemeStellaires[i].nombrePlanetesHabitables = 1;
			systemeStellaires[i].nombrePlanetesHabitees = 1;
			systemeStellaires[i].empire = 1;
			systemeStellaires[i].niveauStation = 1;
			systemeStellaires[i].niveauConnaissance = 4;

			flotte = FlotteAjouter(flotteJoueur);
			flotte->systeme = i;
			flotte->nombreVaisseaux = 3;
			flotte->puissance = 1600;
			flotte->type = FLOTTE_MILITAIRE;
			flotte->coqueVie = 300;
			flotte->coqueTotal = 300;
			flotte->blindageVie = 50;
			flotte->blindageTotal = 50;
			flotte->bouclierVie = 100;
			flotte->bouclierTotal = 100;

			flotte = FlotteAjouter(flotteJoueur);
			flotte->systeme = i;
			flotte->nombreVaisseaux = 1;
			flotte->puissance = 0;
			flotte->type = FLOTTE_DE_CONSTRUCTION;
			flotte->coqueVie = 300;
			flotte->coqueTotal = 300;
			flotte->blindageVie = 0;
			flotte->blindageTotal = 0;
			flotte->bouclierVie = 0;
			flotte->bouclierTotal = 0;

			flotte = FlotteAjouter(flotteJoueur);
			flotte->systeme = i;
			flotte->nombreVaisseaux = 1;
			flotte->puissance = 0;
			flotte->type = FLOTTE_SCIENTIFIQUE;
			flotte->coqueVie = 300;
			flotte->coqueTotal = 300;
			flotte->blindageVie = 0;
			flotte->blindageTotal = 0;
			flotte->bouclierVie = 0;
			flotte->bouclierTotal = 0;

			camera->x = systemeStellaires[i].x;
			camera->y = systemeStellaires[i].y;

			fin = 0;
			switch(randInt(1, systemeStellaires[i].nombrePlanetes))
			{
				case 1:
					systemeStellaires[i].planete1->habitable = 1;
					systemeStellaires[i].planete1->type = 7;
					systemeStellaires[i].planete1->population = 28;
					strcpy(systemeStellaires[i].planete1->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
					break;
				case 2:
					systemeStellaires[i].planete2->habitable = 1;
					systemeStellaires[i].planete2->type = 7;
					systemeStellaires[i].planete2->population = 28;
					strcpy(systemeStellaires[i].planete2->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
					break;
				case 3:
					systemeStellaires[i].planete3->habitable = 1;
					systemeStellaires[i].planete3->type = 7;
					systemeStellaires[i].planete3->population = 28;
					strcpy(systemeStellaires[i].planete3->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
					break;
				case 4:
					systemeStellaires[i].planete4->habitable = 1;
					systemeStellaires[i].planete4->type = 7;
					systemeStellaires[i].planete4->population = 28;
					strcpy(systemeStellaires[i].planete4->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
					break;
				case 5:
					systemeStellaires[i].planete3->habitable = 1;
					systemeStellaires[i].planete3->type = 7;
					systemeStellaires[i].planete3->population = 28;
					strcpy(systemeStellaires[i].planete3->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
					break;
			}
		}
	}
	
	i = 0;
	//enregistrer
	while(i <= LARGEUR_GALAXIE * LARGEUR_GALAXIE)
	{
		ti_Write(&systemeStellaires[i], sizeof(SystemeStellaire), 1, *sauvegarde);
		
		if(systemeStellaires[i].nombrePlanetes >= 1)
		{
			ti_Write(systemeStellaires[i].planete1, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 2)
		{
			ti_Write(systemeStellaires[i].planete2, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 3)
		{
			ti_Write(systemeStellaires[i].planete3, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 4)
		{
			ti_Write(systemeStellaires[i].planete4, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes == 5)
		{
			ti_Write(systemeStellaires[i].planete5, sizeof(Planete), 1, *sauvegarde);
		}
		i++;
	}
	return 1;
}

/******************chargement ancienne partie******************/
int ChargementAnciennePartie(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche)
{
	ti_var_t sauvegarde;
	int i = 0, j = 0, compteur = 0, compteurFlottes = 0;
	Flotte* flotte = NULL, *flottePrecedente = NULL;
	Empire* empire = NULL, *empirePrecedent = NULL;
	gfx_FillScreen(255);
	PrintCentered("Chargement de la partie..." ,60 ,1 , 0, 0);
	/*ouvrir sauvegarde*/
	ti_CloseAll();
	sauvegarde = ti_Open("sauv", "r+");
	if (sauvegarde == 0)
	{
		return 1;
	}
	ti_Read(parametres, sizeof(Parametres), 1, sauvegarde);
	ti_Read(date, sizeof(Date), 1, sauvegarde);
	ti_Read(camera, sizeof(Camera), 1, sauvegarde);
	ti_Read(marche, sizeof(Marche), 1, sauvegarde);
	
	i = 0;
	
	//ourvir
	while(i <= LARGEUR_GALAXIE * LARGEUR_GALAXIE)
	{
		ti_Read(&systemeStellaires[i], sizeof(SystemeStellaire), 1, sauvegarde);
		
		if(systemeStellaires[i].nombrePlanetes >= 1)
		{
			systemeStellaires[i].planete1 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete1, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 2)
		{
			systemeStellaires[i].planete2 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete2, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 3)
		{
			systemeStellaires[i].planete3 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete3, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 4)
		{
			systemeStellaires[i].planete4 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete4, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes == 5)
		{
			systemeStellaires[i].planete5 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete5, sizeof(Planete), 1, sauvegarde);
		}
		i++;
	}
	ti_Read(&compteur, sizeof(int), 1, sauvegarde);

	i = 0;
	while(i < compteur){
		empire = EmpireAjouter(empireListe);
		ti_Read(empire, sizeof(Empire), 1, sauvegarde);
		empire->flotte = FlotteListeCreer();
		j = 0;
		compteurFlottes = 0;
		ti_Read(&compteurFlottes, sizeof(int), 1, sauvegarde);
		while(j < compteurFlottes){
			flotte = FlotteAjouter(empire->flotte);
			ti_Read(flotte, sizeof(flotte), 1, sauvegarde);
			j++;
		}
		i++;
	}
	joueur = EmpireNumero(empireListe, 1);
	flotteJoueur = joueur->flotte;

	

	PrintCentered(joueur->nom ,90 ,1 , 0, 0);
	camera->fenetre = MENU_AUCUN;
	camera->bloque = FALSE;
	StellarisBoucle(&sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, flotteJoueur, fenetre, marche);
	return 0;
}

/**********************************************Jeu principal**********************************************/
int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche)
{
	char finBoucle = 1, key = 0;
	while (finBoucle == 1)
	{
		/**********boucle du jeu**********/
		
		key = os_GetCSC();
		
		/**gere le temps**/
		StellarisTemps(joueur, date, &key);
		
		//efface l'ecran
		gfx_ZeroScreen();

		/**dessine la map**/
		StellarisMap(empireListe, systemeStellaires, camera, &key, flotteJoueur, date, fenetre, joueur);

		
		/**dessine le hud**/
		finBoucle = StellarisHUD(empireListe, joueur, date, &key, camera, systemeStellaires, fenetre, flotteJoueur, parametres, sauvegarde, marche);
		
		gfx_SwapDraw();
		if((boot_CheckOnPressed()) || (finBoucle == 0))
		{
			StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, flotteJoueur, marche);
			return 0;
		}
	}
	return 0;
}

/******************dessiner le hud******************/
int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, FlotteListe *flotteJoueur, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche)
{
	char jourChar[10];
	char moisChar[7];
	char anneeChar[4];
	//point de l'hexagone
	int drapeau[12] = {
		17, 0,
		0, 10,
		0, 30,
		17, 40,
		35, 30,
		35, 10,
	};
	char valeurMineraiStr[4];
	char nomPlanete[20];
	char population[5];
	char niveau = 0;
	int8_t compteur = 0, compteurFlotte = 0, maximum = 0;
	Flotte* flotte = NULL;
	//gcvt(marche->valeurMinerai , 2, valeurMineraiStr);
	
	//dessine le fond du hud
	gfx_SetColor(6);
	//barre du haut
	gfx_FillRectangle(0, 0 , LCD_WIDTH, 22);
	//barre du coté gauche
	gfx_FillRectangle(0, 0, 20, 200);
	gfx_FillTriangle(0, 200, 20, 200, 0, 220);
	//barre du bas
	if (date->vitesse == 0)
	{
		gfx_SetColor(10);
	}
	gfx_FillRectangle(100, 220, 120, 20);
	gfx_FillTriangle(100, 220, 80, 240, 100, 240);
	gfx_FillTriangle(220, 220, 240, 240, 220, 240);
	//nom selection
	if ((camera->selection != -1) || (camera->mapType == 2))
	{
		gfx_FillRectangle(110, 210, 100, 10);
		gfx_FillTriangle(110, 210, 100, 220, 110, 220);
		gfx_FillTriangle(210, 210, 210, 220, 220, 220);
	}
	
	
	//dessine les bord du hud
	gfx_SetColor(7);
	//barre du haut
	gfx_HorizLine_NoClip(20, 22, LCD_WIDTH-20);
	//barre du coté gauche
	gfx_VertLine_NoClip(20, 22, 179);
	gfx_Line_NoClip(20, 200, 0, 220);
	//barre du bas
	if (date->vitesse == 0)
	{
		gfx_SetColor(9);
	}
	gfx_HorizLine_NoClip(100, 220, 120);
	gfx_Line_NoClip(100, 220, 81, 239);
	gfx_Line_NoClip(220, 220, 239, 239);
	//nom selection
	if ((camera->selection != -1) || (camera->mapType == 2))
	{
		gfx_HorizLine_NoClip(110, 210, 100);
		gfx_Line_NoClip(110, 210, 100, 220);
		gfx_Line_NoClip(210, 210, 220, 220);
	}
	
	//drapeau intérieur
	gfx_SetColor(6);
	gfx_FillTriangle_NoClip(17, 30, 35, 30, 17, 40);
	gfx_FillRectangle_NoClip(17, 20, 18,10);
	
	//drapeau bords
	gfx_SetColor(7);
	gfx_Polygon_NoClip(drapeau, 6);
	gfx_FloodFill(17, 1, 0);
	
	//dessiner drapeau
	gfx_SetColor(1);
	gfx_Circle_NoClip(20, 15, 6);
	gfx_Line_NoClip(10, 30, 20, 15);
	gfx_FillCircle_NoClip(20, 15, 4);
	gfx_Circle_NoClip(10, 30, 3);
	gfx_SetColor(0);
	gfx_FillCircle_NoClip(20, 15, 2);
	
	//texte
	gfx_SetTextFGColor(8);
	//barre du haut
	gfx_TransparentSprite_NoClip(credit, 55 ,1);
	PrintHUD(joueur->credits, 0, 40, 13);
	gfx_TransparentSprite_NoClip(minerai, 100 ,1);
	PrintHUD(joueur->minerais, 0, 85, 13);
	gfx_TransparentSprite_NoClip(food, 145 ,1);
	PrintHUD(joueur->nourriture, 0, 130, 13);
	gfx_TransparentSprite_NoClip(fer, 190 ,1);
	PrintHUD(joueur->acier, 0, 175, 13);
	
	//barre de gauche
	gfx_TransparentSprite_NoClip(contact, 5, 50);
	gfx_TransparentSprite_NoClip(market, 5, 70);
	gfx_TransparentSprite_NoClip(science, 5, 90);
	gfx_TransparentSprite_NoClip(alliedFleet, 5, 110);
	
	//barre du bas
	//date
	if (date->jour < 10)
	{
		sprintf(jourChar, "0%d.", date->jour);
	}
	else 
	{
		sprintf(jourChar, "%d.", date->jour);
	}
	if (date->mois < 10)
	{
		sprintf(moisChar, "0%d.", date->mois);
	}
	else 
	{
		sprintf(moisChar, "%d.", date->mois);
	}
	sprintf(anneeChar, "%d", date->annee);
	strcat(moisChar, anneeChar);
	strcat(jourChar, moisChar);
	gfx_PrintStringXY(jourChar, 125, 225);
	
	//pause / avance
	switch (date->vitesse)
	{
		case -2:
			gfx_SetColor(1);
			gfx_FillRectangle_NoClip(100, 225, 3, 9);
			gfx_FillTriangle_NoClip(109, 225, 109, 233, 105, 229);
			gfx_FillTriangle_NoClip(114, 225, 114, 233, 110, 229);
			break;
		case -1:
			gfx_SetColor(1);
			gfx_FillRectangle_NoClip(100, 225, 3, 9);
			gfx_FillTriangle_NoClip(109, 225, 109, 233, 105, 229);
			break;
		case 0:
			gfx_SetColor(9);
			gfx_FillRectangle_NoClip(105, 225, 3, 9);
			gfx_FillRectangle_NoClip(110, 225, 3, 9);
			break;
		case 1:
			gfx_SetColor(1);
			gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
			break;	
		case 2:
			gfx_SetColor(1);
			gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
			gfx_FillTriangle_NoClip(110, 225, 110, 233, 114, 229);
			break;
		case 3:
			gfx_SetColor(1);
			gfx_FillTriangle_NoClip(105, 225, 105, 233, 109, 229);
			gfx_FillTriangle_NoClip(110, 225, 110, 233, 114, 229);
			gfx_FillTriangle_NoClip(115, 225, 115, 233, 119, 229);
			break;
	}
	gfx_SetColor(11);
	
	//pointeur
	if (camera->mapType == 1) {
		gfx_Line_NoClip(150, 120, 170, 120);
		gfx_Line_NoClip(160, 110, 160, 130);
		gfx_FillCircle_NoClip(160, 120, 2);
		gfx_Circle_NoClip(160, 120, 4);
		gfx_Circle_NoClip(160, 120, 8);
	}
	else {
		gfx_Line_NoClip(camera->x / 2.5 + 5, camera->y / 2.5 - 30, camera->x / 2.5 + 15, camera->y / 2.5 - 30);
		gfx_Line_NoClip(camera->x / 2.5 + 10, camera->y / 2.5 - 25, camera->x / 2.5 + 10, camera->y / 2.5 - 35);
		gfx_FillCircle_NoClip(camera->x / 2.5 + 10, camera->y / 2.5 - 30, 3);
	}
	//nom galaxie
	if ((camera->selection != -1) || (camera->mapType == 2)) {
		if (camera->mapType == 1) {
			if (systemeStellaires[camera->selection].niveauConnaissance == 0) {
				gfx_PrintStringXY("Inconnu", 132, 211);
			}
			else {
				gfx_PrintStringXY(systemeStellaires[camera->selection].nom, 160 - strlen(systemeStellaires[camera->selection].nom) * 4, 211);
			}
		}
		else {
			gfx_PrintStringXY("Carte", 140, 211);
		}
	}
	
	/*************MENUS*************/
	//quitter
	if(camera->fenetre == MENU_QUITTER)
	{
		switch(*key)
		{
			case sk_Up:
				fenetre->selection--;
				break;
			case sk_Down:
				fenetre->selection++;
				break;
			case sk_Clear:
				camera->fenetre = 0;
				camera->bloque = FALSE;
				break;
		}
		if (fenetre->selection > 5)
		{
			fenetre->selection = 1;
		}
		else if (fenetre->selection < 1)
		{
			fenetre->selection = 5;
		}
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(100, 40, 120, 160);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(100, 40, 120, 160);
		
		if(fenetre->selection == 1) {
			gfx_SetColor(gfx_Lighten(7, 200));
		}
		gfx_Rectangle_NoClip(110, 50, 100, 18);
		gfx_SetColor(7);

		if(fenetre->selection == 2) {
			gfx_SetColor(gfx_Lighten(7, 200));
		}
		gfx_Rectangle_NoClip(110, 80, 100, 18);
		gfx_SetColor(7);

		if(fenetre->selection == 3) {
			gfx_SetColor(gfx_Lighten(7, 200));
		}
		gfx_Rectangle_NoClip(110, 110, 100, 18);
		gfx_SetColor(7);

		if(fenetre->selection == 4) {
			gfx_SetColor(gfx_Lighten(7, 200));
		}
		gfx_Rectangle_NoClip(110, 140, 100, 18);
		gfx_SetColor(7);
		
		if(fenetre->selection == 5) {
			gfx_SetColor(gfx_Lighten(7, 200));
		}
		gfx_Rectangle_NoClip(110, 170, 100, 18);
		gfx_SetColor(7);

		gfx_SetColor(1);
		gfx_PrintStringXY(_(LC_RETOUR), 160 - strlen(_(LC_RETOUR)) * 4, 55);
		gfx_PrintStringXY(_(LC_SAUVEGARDER), 160 - strlen(_(LC_SAUVEGARDER)) * 4, 85);
		gfx_PrintStringXY(_(LC_CHARGER), 160 - strlen(_(LC_CHARGER)) * 4, 115);
		gfx_PrintStringXY(_(LC_OPTIONS), 160 - strlen(_(LC_OPTIONS)) * 4, 145);
		gfx_PrintStringXY(_(LC_QUITTER), 160 - strlen(_(LC_QUITTER)) * 4, 175);
		if(*key == sk_Enter)
		{
			switch(fenetre->selection)
			{
				case 1:
					camera->fenetre = MENU_AUCUN;
					camera->bloque = FALSE;
					break;
				case 2:
					camera->fenetre = MENU_AUCUN;
					camera->bloque = FALSE;
					StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, flotteJoueur, marche);
					break;
				case 5:
					return 0;
					break;
			}
		}
	}
	
	//menu systeme
	else if(camera->fenetre == MENU_SYSTEME)
	{
		switch(fenetre->ouverte)
		{
			case 0: //vue systeme
				switch(*key)
				{
					case sk_Up:
						fenetre->selection--;
						break;
					case sk_Down:
						fenetre->selection++;
						break;
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						camera->bloque = FALSE;
						*key = 0;
						break;
					case sk_Right:
						fenetre->ouverte = 1;
						*key = 0;
						break;
				}
				if (fenetre->selection > systemeStellaires[camera->selection].nombrePlanetes + 2)
				{
					fenetre->selection = systemeStellaires[camera->selection].nombrePlanetes + 2;
				}
				else if (fenetre->selection < 1 + (!systemeStellaires[camera->selection].niveauStation))
				{
					fenetre->selection = 1 + (!systemeStellaires[camera->selection].niveauStation);
				}
				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 11);
				gfx_FillRectangle_NoClip(200, 51, 80, 149); //barre de coté
				gfx_FillRectangle_NoClip(110, 200, 70, 12); //barre du bas
				gfx_SetColor(0);
				gfx_FillRectangle_NoClip(40, 52, 160, 148);
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_VertLine_NoClip(200, 57, 138); //barre de coté
				gfx_FillRectangle_NoClip(40, 200, 70, 12);
				gfx_Rectangle_NoClip(40, 199, 140, 13);
				gfx_PrintStringXY("Vue syst~me", 116, 42);
				gfx_PrintStringXY("Syst~me", 47, 202);
				gfx_PrintStringXY("Flotte", 121, 202);
				
				
				
				//dessiner etoile
				switch(systemeStellaires[camera->selection].etoileType)
				{
					case 1: //B
						gfx_SetColor(15);
						gfx_FillCircle(114, 126, 3);
						break;
					case 2: //A
						gfx_SetColor(14);
						gfx_FillCircle(114, 126, 4);
						break;
					case 3: //F
						gfx_SetColor(1);
						gfx_FillCircle(114, 126, 4);
						break;
					case 4: //G
						gfx_SetColor(1);
						gfx_FillCircle(114, 126, 4);
						break;
					case 5: //K
						gfx_SetColor(13);
						gfx_FillCircle(114, 126, 2);
						break;
					case 6: //M
						gfx_SetColor(13);
						gfx_FillCircle(114, 126, 4);
						break;
					case 7: //trou noir
						gfx_SetColor(13);
						gfx_Circle(114, 126, 4);
						break;
					case 8: //pulsar
						gfx_SetColor(14);
						gfx_FillCircle(114, 126, 1);
						break;
					case 9: ///toile a neutrons
						gfx_SetColor(14);
						gfx_FillCircle(114, 126, 1);
						gfx_Circle(114, 126, 8);
						break;
				}
				//gerer selection
				
				/*******station*******/
				niveau = 57;
				if(systemeStellaires[camera->selection].niveauStation > 0) {
					gfx_SetColor(11);
					gfx_FillCircle_NoClip(110, 115, 1);
					if(fenetre->selection == 1) {
						gfx_SetTextFGColor(13);
						gfx_SetColor(9);
						gfx_Circle_NoClip(110, 110, 15);
					}
					gfx_PrintStringXY("Station", 212, niveau);
					gfx_SetTextFGColor(1);
					gfx_SetColor(7);
					gfx_HorizLine(205, niveau + 14, 70);
					niveau += 20;
				}
				
				/*******etoile*******/
				if(fenetre->selection == 2) {
					gfx_SetTextFGColor(13);
					gfx_SetColor(9);
					gfx_Circle_NoClip(114, 126, 15);
				}
				gfx_PrintStringXY("Etoile", 216, niveau);
				gfx_SetTextFGColor(1);
				
				gfx_SetColor(7);
				gfx_HorizLine(205, niveau + 14, 70);
				
				niveau += 20;
				
				/*******planete1*******/
				if(systemeStellaires[camera->selection].planete1 != 0) {
					gfx_SetColor(11);
					gfx_Circle_NoClip(114, 126, systemeStellaires[camera->selection].planete1->rayonOrbite);

					CouleurPlanete(systemeStellaires[camera->selection].planete1->type);

					gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete1->x, systemeStellaires[camera->selection].planete1->y, systemeStellaires[camera->selection].planete1->taille);
					if(systemeStellaires[camera->selection].planete1->type == 12){
						gfx_SetColor(0);
						gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete1->x + 5, systemeStellaires[camera->selection].planete1->y + 5, systemeStellaires[camera->selection].planete1->taille);
					}
					else if(systemeStellaires[camera->selection].planete1->type == 14){
						gfx_SetColor(26);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete1->x, systemeStellaires[camera->selection].planete1->y, systemeStellaires[camera->selection].planete1->taille - 1);
					}
					if(fenetre->selection == 3) {
						gfx_SetTextFGColor(13);
						gfx_SetColor(9);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete1->x, systemeStellaires[camera->selection].planete1->y, 15);
					}
					if(systemeStellaires[camera->selection].planete1->habitable == 1){
						gfx_SetTextXY(240 - strlen(systemeStellaires[camera->selection].planete1->nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].planete1->nom);
					}
					else{
						gfx_SetTextXY(232 - strlen(systemeStellaires[camera->selection].nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].nom);
						gfx_PrintString(" I");
					}
					gfx_SetColor(7);
					gfx_HorizLine(205, niveau + 14, 70);//barre de séparation
					gfx_SetTextFGColor(1);
					if(systemeStellaires[camera->selection].planete1->population)
					{
						gfx_SetColor(16);
						gfx_FillRectangle(systemeStellaires[camera->selection].planete1->x - (strlen(systemeStellaires[camera->selection].planete1->nom) * 4), systemeStellaires[camera->selection].planete1->y + 8, strlen(systemeStellaires[camera->selection].planete1->nom) * 8, 10);
						gfx_PrintStringXY(systemeStellaires[camera->selection].planete1->nom, systemeStellaires[camera->selection].planete1->x - (strlen(systemeStellaires[camera->selection].planete1->nom) * 4), systemeStellaires[camera->selection].planete1->y + 9);
					}
				}
				
				niveau += 20;
				
				/********planete 2*********/
				if(systemeStellaires[camera->selection].planete2 != 0) {
					gfx_SetColor(11);
					gfx_Circle_NoClip(114, 126, systemeStellaires[camera->selection].planete2->rayonOrbite);

					CouleurPlanete(systemeStellaires[camera->selection].planete2->type);

					gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete2->x, systemeStellaires[camera->selection].planete2->y, systemeStellaires[camera->selection].planete2->taille);
					if(systemeStellaires[camera->selection].planete2->type == 12){
						gfx_SetColor(0);
						gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete2->x + 5, systemeStellaires[camera->selection].planete2->y + 5, systemeStellaires[camera->selection].planete2->taille);
					}
					else if(systemeStellaires[camera->selection].planete2->type == 14){
						gfx_SetColor(26);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete2->x, systemeStellaires[camera->selection].planete2->y, systemeStellaires[camera->selection].planete2->taille - 1);
					}
					if(fenetre->selection == 4) {
						gfx_SetTextFGColor(13);
						gfx_SetColor(9);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete2->x, systemeStellaires[camera->selection].planete2->y, 15);
					}
					if(systemeStellaires[camera->selection].planete2->habitable == 1){
						gfx_SetTextXY(240 - strlen(systemeStellaires[camera->selection].planete2->nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].planete2->nom);
					}
					else{
						gfx_SetTextXY(228 - strlen(systemeStellaires[camera->selection].nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].nom);
						gfx_PrintString(" II");
					}
					gfx_SetColor(7);
					gfx_HorizLine(205, niveau + 14, 70);//barre de séparation
					gfx_SetTextFGColor(1);
					if(systemeStellaires[camera->selection].planete2->population)
					{
						gfx_SetColor(16);
						gfx_FillRectangle(systemeStellaires[camera->selection].planete2->x - (strlen(systemeStellaires[camera->selection].planete2->nom) * 4), systemeStellaires[camera->selection].planete2->y + 8, strlen(systemeStellaires[camera->selection].planete2->nom) * 8, 10);
						gfx_PrintStringXY(systemeStellaires[camera->selection].planete2->nom, systemeStellaires[camera->selection].planete2->x - (strlen(systemeStellaires[camera->selection].planete2->nom) * 4), systemeStellaires[camera->selection].planete2->y + 9);
					}
				}
				
				niveau += 20;
				
				/*********planete 3*********/
				if(systemeStellaires[camera->selection].planete3 != 0) {
					gfx_SetColor(11);
					gfx_Circle_NoClip(114, 126, systemeStellaires[camera->selection].planete3->rayonOrbite);

					CouleurPlanete(systemeStellaires[camera->selection].planete3->type);

					gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete3->x, systemeStellaires[camera->selection].planete3->y, systemeStellaires[camera->selection].planete3->taille);
					if(systemeStellaires[camera->selection].planete3->type == 12){
						gfx_SetColor(0);
						gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete3->x + 5, systemeStellaires[camera->selection].planete3->y + 5, systemeStellaires[camera->selection].planete3->taille);
					}
					else if(systemeStellaires[camera->selection].planete3->type == 14){
						gfx_SetColor(26);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete3->x, systemeStellaires[camera->selection].planete3->y, systemeStellaires[camera->selection].planete3->taille - 1);
					}
					if(fenetre->selection == 5) {
						gfx_SetTextFGColor(13);
						gfx_SetColor(9);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete3->x, systemeStellaires[camera->selection].planete3->y, 15);
					}
					if(systemeStellaires[camera->selection].planete3->habitable == 1){
						gfx_SetTextXY(240 - strlen(systemeStellaires[camera->selection].planete3->nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].planete3->nom);
					}
					else{
						gfx_SetTextXY(224 - strlen(systemeStellaires[camera->selection].nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].nom);
						gfx_PrintString(" III");
					}
					gfx_SetColor(7);
					gfx_HorizLine(205, niveau + 14, 70);//barre de séparation
					gfx_SetTextFGColor(1);
					if(systemeStellaires[camera->selection].planete3->population)
					{
						gfx_SetColor(16);
						gfx_FillRectangle(systemeStellaires[camera->selection].planete3->x - (strlen(systemeStellaires[camera->selection].planete3->nom) * 4), systemeStellaires[camera->selection].planete3->y + 8, strlen(systemeStellaires[camera->selection].planete3->nom) * 8, 10);
						gfx_PrintStringXY(systemeStellaires[camera->selection].planete3->nom, systemeStellaires[camera->selection].planete3->x - (strlen(systemeStellaires[camera->selection].planete3->nom) * 4), systemeStellaires[camera->selection].planete3->y + 9);
					}
				}
				
				niveau += 20;
				
				/*********planete 4********/
				if(systemeStellaires[camera->selection].planete4 != 0) {
					gfx_SetColor(11);
					gfx_Circle_NoClip(114, 126, systemeStellaires[camera->selection].planete4->rayonOrbite);

					CouleurPlanete(systemeStellaires[camera->selection].planete4->type);

					gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete4->x, systemeStellaires[camera->selection].planete4->y, systemeStellaires[camera->selection].planete4->taille);
					if(systemeStellaires[camera->selection].planete4->type == 12){
						gfx_SetColor(0);
						gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete4->x + 5, systemeStellaires[camera->selection].planete4->y + 5, systemeStellaires[camera->selection].planete4->taille);
					}
					else if(systemeStellaires[camera->selection].planete4->type == 14){
						gfx_SetColor(26);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete4->x, systemeStellaires[camera->selection].planete4->y, systemeStellaires[camera->selection].planete4->taille - 1);
					}
					if(fenetre->selection == 6) {
						gfx_SetTextFGColor(13);
						gfx_SetColor(9);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete4->x, systemeStellaires[camera->selection].planete4->y, 15);
					}
					if(systemeStellaires[camera->selection].planete4->habitable == 1){
						gfx_SetTextXY(240 - strlen(systemeStellaires[camera->selection].planete4->nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].planete4->nom);
					}
					else{
						gfx_SetTextXY(228 - strlen(systemeStellaires[camera->selection].nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].nom);
						gfx_PrintString(" IV");
					}
					gfx_SetColor(7);
					gfx_HorizLine(205, niveau + 14, 70);//barre de séparation
					gfx_SetTextFGColor(1);
					if(systemeStellaires[camera->selection].planete4->population)
					{
						gfx_SetColor(16);
						gfx_FillRectangle(systemeStellaires[camera->selection].planete4->x - (strlen(systemeStellaires[camera->selection].planete4->nom) * 4), systemeStellaires[camera->selection].planete4->y + 8, strlen(systemeStellaires[camera->selection].planete4->nom) * 8, 10);
						gfx_PrintStringXY(systemeStellaires[camera->selection].planete4->nom, systemeStellaires[camera->selection].planete4->x - (strlen(systemeStellaires[camera->selection].planete4->nom) * 4), systemeStellaires[camera->selection].planete4->y + 9);
					}
				}
				
				niveau += 20;
				
				/********planete 5*******/
				if(systemeStellaires[camera->selection].planete5 != 0) {
					gfx_SetColor(11);
					gfx_Circle_NoClip(114, 126, systemeStellaires[camera->selection].planete5->rayonOrbite);

					CouleurPlanete(systemeStellaires[camera->selection].planete5->type);

					gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete5->x, systemeStellaires[camera->selection].planete5->y, systemeStellaires[camera->selection].planete5->taille);
					if(systemeStellaires[camera->selection].planete5->type == 12){
						gfx_SetColor(0);
						gfx_FillCircle_NoClip(systemeStellaires[camera->selection].planete5->x + 5, systemeStellaires[camera->selection].planete5->y + 5, systemeStellaires[camera->selection].planete5->taille);
					}
					else if(systemeStellaires[camera->selection].planete5->type == 14){
						gfx_SetColor(26);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete5->x, systemeStellaires[camera->selection].planete5->y, systemeStellaires[camera->selection].planete5->taille - 1);
					}
					if(fenetre->selection == 7) {
						gfx_SetTextFGColor(13);
						gfx_SetColor(9);
						gfx_Circle_NoClip(systemeStellaires[camera->selection].planete5->x, systemeStellaires[camera->selection].planete5->y, 15);
					}
					if(systemeStellaires[camera->selection].planete5->habitable == 1){
						gfx_SetTextXY(240 - strlen(systemeStellaires[camera->selection].planete5->nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].planete5->nom);
					}
					else{
						gfx_SetTextXY(232 - strlen(systemeStellaires[camera->selection].nom) * 4, niveau);
						gfx_PrintString(systemeStellaires[camera->selection].nom);
						gfx_PrintString(" V");
					}
					gfx_SetColor(7);
					gfx_HorizLine(205, niveau + 14, 70);//barre de séparation
					gfx_SetTextFGColor(1);
					if(systemeStellaires[camera->selection].planete5->population)
					{
						gfx_SetColor(16);
						gfx_FillRectangle(systemeStellaires[camera->selection].planete5->x - (strlen(systemeStellaires[camera->selection].planete5->nom) * 4), systemeStellaires[camera->selection].planete5->y + 8, strlen(systemeStellaires[camera->selection].planete5->nom) * 8, 10);
						gfx_PrintStringXY(systemeStellaires[camera->selection].planete5->nom, systemeStellaires[camera->selection].planete5->x - (strlen(systemeStellaires[camera->selection].planete5->nom) * 4), systemeStellaires[camera->selection].planete5->y + 9);
					}
				}
				if(*key == sk_Enter)
				{
					if(fenetre->selection == 2)
					{
						fenetre->ouverte = 2;
						fenetre->selection = 1;
					}
					else
					{
						fenetre->ouverte = 3;
					}
				}
				break;
				
			case 1: //liste flottes
				switch(*key)
				{
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						camera->bloque = FALSE;
						*key = 0;
						break;
					case sk_Left:
						fenetre->ouverte = 0;
						*key = 0;
						break;
					case sk_Down:
						fenetre->selection++;
						*key = 0;
						break;
					case sk_Up:
						fenetre->selection--;
						*key = 0;
						break;
				}

				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 160);
				gfx_FillRectangle_NoClip(40, 200, 70, 12); //barre du bas
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_FillRectangle_NoClip(110, 200, 70, 12); //barre du bas
				gfx_Rectangle_NoClip(40, 199, 140, 13);
				gfx_PrintStringXY("Vue flottes", 116, 42);
				gfx_PrintStringXY("Syst~me", 47, 202);
				gfx_PrintStringXY("Flotte", 121, 202);
				niveau = 57;
				flotte = joueur->flotte->premier;
				compteur = 0;
				compteurFlotte = 1;
				while(flotte != NULL){
					if(flotte->systeme == camera->selection) {
						compteur++;
					}
					flotte = flotte->suivant;
				}

				if (fenetre->selection > compteur) {
					fenetre->selection = 1;
				}
				else if (fenetre->selection < 1) {
					fenetre->selection = compteur;
				}
				flotte = joueur->flotte->premier;
				while(flotte != NULL) {
					if(flotte->systeme == camera->selection) {
						if(fenetre->selection == compteurFlotte) {
							gfx_SetTextFGColor(13);
						}
						else {
							gfx_SetTextFGColor(1);
						}
						switch(flotte->type){
							case FLOTTE_MILITAIRE:
								gfx_PrintStringXY("militaire", 208, niveau);
								break;
							case FLOTTE_DE_CONSTRUCTION:
								gfx_PrintStringXY("construction", 184, niveau);
								break;
							case FLOTTE_SCIENTIFIQUE:
								gfx_PrintStringXY("scientifique", 184, niveau);
								break;
						}
						gfx_SetTextXY(45, niveau);
						gfx_PrintString("Flotte ");
						PrintInt(compteurFlotte);
						gfx_HorizLine_NoClip(50, niveau + 11, 220);
						niveau += 17;
					}
					compteurFlotte++;
					flotte = flotte->suivant;
				}
				if (*key == sk_Enter) {
					fenetre->ouverte = 6;
					fenetre->flotteSelectionee = fenetre->selection;
					fenetre->selection = 1;
					fenetre->precedente = 1;
				}
				break;
				
			case 2: //etoile
				switch(*key) {
					case sk_Clear:
						fenetre->ouverte = 0;
						*key = 0;
						break;
				}
				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 160);
				gfx_SetColor(0);
				gfx_FillRectangle_NoClip(45, 56, 110, 110);
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_VertLine_NoClip(100, 42, 8);
				gfx_Rectangle_NoClip(45, 56, 110, 110);
				gfx_PrintStringXY("Retour", 48, 42);
				gfx_SetTextXY(158 - strlen(systemeStellaires[camera->selection].nom)*4, 42);
				gfx_PrintString("Proxima ");
				gfx_PrintString(systemeStellaires[camera->selection].nom);
				gfx_SetTextXY(160, 62);
				gfx_PrintString("Classe : ");
				switch(systemeStellaires[camera->selection].etoileType)
				{
					case 1:
						gfx_PrintString("B");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 45);				
						gfx_SetColor(15);
						gfx_Circle_NoClip(100, 111, 45);
						break;
					case 2:
						gfx_PrintString("A");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 40);				
						gfx_SetColor(14);
						gfx_Circle_NoClip(100, 111, 40);
						break;
					case 3:
						gfx_PrintString("F");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 35);				
						break;
					case 4:
						gfx_PrintString("G");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 32);				
						gfx_SetColor(13);
						gfx_Circle_NoClip(100, 111, 32);
						break;
					case 5:
						gfx_PrintString("K");
						gfx_SetColor(13);
						gfx_FillCircle_NoClip(100, 111, 30);				
						gfx_SetColor(9);
						gfx_Circle_NoClip(100, 111, 30);
						break;
					case 6:
						gfx_PrintString("M");
						gfx_SetColor(9);
						gfx_FillCircle_NoClip(100, 111, 10);				
						gfx_SetColor(13);
						gfx_Circle_NoClip(100, 111, 10);
						break;
					case 7:
						gfx_PrintString("trou noir");
						gfx_SetColor(1);
						gfx_Circle_NoClip(100, 111, 40);	
						gfx_SetColor(1);
						gfx_Circle_NoClip(100, 111, 41);	
						gfx_SetColor(14);
						gfx_Circle_NoClip(100, 111, 42);				
						gfx_SetColor(13);
						gfx_Circle_NoClip(100, 111, 43);			
						gfx_SetColor(0);
						gfx_Circle_NoClip(100, 111, 44);			
						gfx_SetColor(11);
						gfx_Circle_NoClip(100, 111, 45);				
						gfx_SetColor(14);
						gfx_Circle_NoClip(100, 111, 46);
						gfx_SetColor(13);
						gfx_Circle_NoClip(100, 111, 47);
						gfx_SetColor(0);
						gfx_FillCircle_NoClip(105, 115, 40);
						gfx_SetColor(13);
						gfx_HorizLine_NoClip(45, 111, 110);
						gfx_HorizLine_NoClip(48, 110, 104);
						gfx_HorizLine_NoClip(48, 112, 104);
						break;
					case 8:
						gfx_PrintString("pulsar");
						gfx_SetColor(14);
						gfx_FillCircle_NoClip(100, 111, 4);
						gfx_Line_NoClip(90, 101, 110, 121);
						gfx_SetColor(1);
						gfx_Circle_NoClip(100, 111, 4);
						break;
					case 9:
						gfx_PrintString("/toile a neutrons");
						gfx_SetColor(14);
						gfx_FillCircle_NoClip(100, 111, 4);	
						gfx_Circle_NoClip(100, 111, 11);
						gfx_Circle_NoClip(100, 111, 9);
						gfx_SetColor(1);
						gfx_Circle_NoClip(100, 111, 4);
						gfx_Circle_NoClip(100, 111, 10);
						break;
				}
				break;
				
			case 3: //planete resume
				switch(*key)
				{
					case sk_Clear:
						fenetre->ouverte = 0;
						*key = 0;
						break;
					case sk_Right:
						fenetre->ouverte = 4;
						*key = 0;
						break;
				}
				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 160);
				gfx_FillRectangle_NoClip(110, 200, 90, 12); //barre du bas
				gfx_FillRectangle_NoClip(200, 200, 70, 12);
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_VertLine_NoClip(100, 42, 8);
				gfx_Rectangle_NoClip(45, 56, 100, 100);
				gfx_Rectangle_NoClip(40, 199, 230, 12); //barre du bas
				gfx_FillRectangle_NoClip(40, 200, 70, 12);
				gfx_VertLine_NoClip(200, 201, 8);
				gfx_PrintStringXY("R/sum/ ", 51, 201);
				gfx_PrintStringXY("Population", 115, 201);
				gfx_PrintStringXY("Arm/e", 215, 201);
				gfx_PrintStringXY("Retour", 48, 42);
				switch(fenetre->selection)
				{
					case 3:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete1->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete1->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" I");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete1->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 4:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete2->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete2->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" II");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete2->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 5:
						gfx_SetTextXY(178 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete3->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete3->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" III");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete3->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 6:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete4->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete4->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" IV");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete4->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
						
					case 7:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete5->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete5->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" V");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete5->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
				}
				break;
				
			case 4: //planete poplation
				switch(*key)
				{
					case sk_Clear:
						fenetre->ouverte = 0;
						*key = 0;
						break;
					case sk_Right:
						fenetre->ouverte = 5;
						*key = 0;
						break;					
					case sk_Left:
						fenetre->ouverte = 3;
						*key = 0;
						break;
				}
				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 160);
				gfx_FillRectangle_NoClip(40, 200, 70, 12); //barre du bas
				gfx_FillRectangle_NoClip(200, 200, 70, 12);
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_VertLine_NoClip(100, 42, 8);
				gfx_Rectangle_NoClip(45, 56, 100, 100);
				gfx_Rectangle_NoClip(40, 199, 230, 12); //barre du bas
				gfx_FillRectangle_NoClip(110, 200, 90, 12);
				gfx_VertLine_NoClip(200, 201, 8);
				gfx_PrintStringXY("R/sum/ ", 51, 201);
				gfx_PrintStringXY("Population", 115, 201);
				gfx_PrintStringXY("Arm/e", 215, 201);
				gfx_PrintStringXY("Retour", 48, 42);
				switch(fenetre->selection)
				{
					case 3:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete1->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete1->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" I");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete1->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 4:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete2->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete2->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" II");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete2->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 5:
						gfx_SetTextXY(178 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete3->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete3->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" III");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete3->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 6:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete4->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete4->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" IV");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete4->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
						
					case 7:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete5->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete5->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" V");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete5->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
				}
				break;
				
			case 5: //planete armee
				switch(*key)
				{
					case sk_Clear:
						fenetre->ouverte = 0;
						*key = 0;
						break;				
					case sk_Left:
						fenetre->ouverte = 4;
						*key = 0;
						break;
				}
				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 160);
				gfx_FillRectangle_NoClip(40, 200, 70, 12); //barre du bas
				gfx_FillRectangle_NoClip(110, 200, 90, 12);
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_VertLine_NoClip(100, 42, 8);
				gfx_Rectangle_NoClip(45, 56, 100, 100);
				gfx_Rectangle_NoClip(40, 199, 230, 12); //barre du bas
				gfx_FillRectangle_NoClip(200, 200, 70, 12);
				gfx_VertLine_NoClip(200, 201, 8);
				gfx_PrintStringXY("R/sum/ ", 51, 201);
				gfx_PrintStringXY("Population", 115, 201);
				gfx_PrintStringXY("Arm/e", 215, 201);
				gfx_PrintStringXY("Retour", 48, 42);
				switch(fenetre->selection)
				{
					case 3:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete1->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete1->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" I");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete1->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 4:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete2->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete2->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" II");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete2->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 5:
						gfx_SetTextXY(178 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete3->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete3->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" III");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete3->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 6:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete4->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete4->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" IV");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete4->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
						
					case 7:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->selection].nom), 42);
						if(systemeStellaires[camera->selection].planete5->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->selection].planete5->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->selection].nom);
							gfx_PrintString(" V");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->selection].planete5->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
				}
				break;
			case 6: //menu flotte details
				switch(*key)
				{
					case sk_Clear:
						switch(fenetre->precedente){
							case 1:
								fenetre->ouverte = 1;
								fenetre->selection = fenetre->flotteSelectionee;
								break;
							case 2:
								camera->fenetre = MENU_FLOTTE;
								fenetre->selection = fenetre->flotteSelectionee;
								break;
						}
						*key = 0;
						break;
					case sk_Up:
						fenetre->selection -= 4;
						break;
					case sk_Down:
						fenetre->selection += 4;
						break;
					case sk_Left:
						fenetre->selection--;
						break;
					case sk_Right:
						fenetre->selection++;
						break;
				}
				//dessiner fenetre
				gfx_SetColor(6);
				gfx_FillRectangle_NoClip(40, 40, 240, 160);
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_HorizLine_NoClip(45, 67, 180); //*barre en dessous du nom
				gfx_Line_NoClip(225, 67, 233, 55); //*
				gfx_HorizLine_NoClip(235, 55, 40); //*
				gfx_HorizLine_NoClip(45, 162, 230);//barre du bas au dessus des ordres
				gfx_VertLine_NoClip(100, 42, 8); //barre a coté de retour
				gfx_SetColor(1);
				gfx_SetPixel(275, 55);
				gfx_SetPixel(275, 51);
				gfx_PrintStringXY("Retour", 48, 42);
				gfx_SetTextXY(150, 42);
				flotte = FlotteNumero(joueur->flotte, fenetre->flotteSelectionee);
				if(flotte->type == FLOTTE_DE_CONSTRUCTION){
					if((fenetre->selection == 0) || (fenetre->selection >= 10)){
						fenetre->selection = 2;
					}
					else if((fenetre->selection == 3) || (fenetre->selection == 9)){
						fenetre->selection = 1;
					}
					else if((fenetre->selection == 7) || (fenetre->selection <= -3)){
						fenetre->selection = 5;
					}
					else if((fenetre->selection == 4) || (fenetre->selection == -2)){
						fenetre->selection = 6;
					}
				}
				else{
					if(fenetre->selection == 0){
						fenetre->selection = 2;
					}
					else if((fenetre->selection == 3) || (fenetre->selection > 6)){
						fenetre->selection = 1;
					}
					else if(((fenetre->selection == 6) || (fenetre->selection == 4)) || (fenetre->selection < 0)){
						fenetre->selection = 5;
					}
				}

				gfx_PrintString("Flotte ");
				if (fenetre->flotteSelectionee > 10) {
					gfx_PrintInt(fenetre->flotteSelectionee, 2);
				}
				else {
					gfx_PrintInt(fenetre->flotteSelectionee, 1);
				}
				niveau = 57;
				gfx_SetTextXY(45, niveau);
				niveau += 14;
				gfx_SetTextFGColor(17);
				switch(flotte->type){
					case FLOTTE_MILITAIRE:
						gfx_PrintString("Flotte militaire");
						break;
					case FLOTTE_DE_CONSTRUCTION:
						gfx_PrintString("Flotte de construction");
						break;
					case FLOTTE_SCIENTIFIQUE:
						gfx_PrintString("Flotte scientifique");
						break;
				}
				gfx_SetTextFGColor(1);
				gfx_SetTextXY(45, niveau);
				niveau += 14;
				gfx_PrintString("Puissance : ");
				PrintInt(flotte->puissance);
				gfx_SetTextXY(45, niveau);
				niveau += 14;
				gfx_PrintString("Coque : ");
				PrintInt(flotte->coqueVie);
				gfx_PrintString("|");
				PrintInt(flotte->coqueTotal);
				gfx_PrintString("(");
				PrintInt((flotte->coqueVie * 100) / flotte->coqueTotal);
				gfx_PrintString("%)");
				if(flotte->type == FLOTTE_MILITAIRE){
					gfx_SetTextXY(45, niveau);
					niveau += 14;
					gfx_PrintString("Blindage : ");
					PrintInt(flotte->blindageVie);
					gfx_PrintString("|");
					PrintInt(flotte->blindageTotal);
					gfx_PrintString("(");
					PrintInt((flotte->blindageVie * 100) / flotte->blindageTotal);
					gfx_PrintString("%)");
					gfx_SetTextXY(45, niveau);
					gfx_PrintString("Bouclier : ");
					PrintInt(flotte->bouclierVie);
					gfx_PrintString("|");
					PrintInt(flotte->bouclierTotal);
					gfx_PrintString("(");
					PrintInt((flotte->bouclierVie * 100) / flotte->bouclierTotal);
					gfx_PrintString("%)");
				}
				niveau += 20;
				gfx_SetTextXY(45, niveau);
				if(fenetre->selection == FLOTTE_BOUGER){
					gfx_SetTextFGColor(13);
				}
				gfx_PrintString("Bouger");
				gfx_SetTextFGColor(1);
				gfx_SetTextXY(165, niveau);
				niveau += 14;
				switch(flotte->type){
					case FLOTTE_MILITAIRE:
						if(fenetre->selection == FLOTTE_ATTAQUER){
							gfx_SetTextFGColor(13);
						}
						gfx_PrintString("Attaquer");
						gfx_SetTextFGColor(1);

						if(fenetre->selection == FLOTTE_BOMBARDER){
							gfx_SetTextFGColor(13);
						}
						gfx_SetTextXY(45, niveau);
						niveau += 14;
						gfx_PrintString("Bombarder");
						gfx_SetTextFGColor(1);
						break;

					case FLOTTE_SCIENTIFIQUE:
						if(fenetre->selection == FLOTTE_INSPECTER){
							gfx_SetTextFGColor(13);
						}
						gfx_PrintString("Inspecter");
						gfx_SetTextFGColor(1);

						if(fenetre->selection == FLOTTE_RECHERCHER){
							gfx_SetTextFGColor(13);
						}
						gfx_SetTextXY(45, niveau);
						niveau += 14;
						gfx_PrintString("Rechercher");
						gfx_SetTextFGColor(1);
						break;

					case FLOTTE_DE_CONSTRUCTION:
						if(fenetre->selection == FLOTTE_CONSTRUIRE_BASE){
							gfx_SetTextFGColor(13);
						}
						gfx_PrintString("Station");
						gfx_SetTextFGColor(1);

						if(fenetre->selection == FLOTTE_COSNTRUIRE_MINE){
							gfx_SetTextFGColor(13);
						}
						gfx_SetTextXY(45, niveau);
						gfx_PrintString("Base mini/re");
						gfx_SetTextFGColor(1);

						if(fenetre->selection == FLOTTE_CONSTRUIRE_BASE_SCIENTIFIQUE){
							gfx_SetTextFGColor(13);
						}
						gfx_SetTextXY(165, niveau);
						gfx_PrintString("Base scientifique");
						gfx_SetTextFGColor(1);
						break;
				}
				gfx_SetTextXY(45, 167);
				switch(flotte->action){
					case FLOTTE_AUCUNE_ACTION:
						gfx_PrintString("Aucun ordre");
						break;
					case FLOTTE_BOUGER:
						gfx_PrintString("Se d/place vers");
						gfx_SetTextFGColor(13);
						gfx_PrintString(systemeStellaires[flotte->systemeArrive].nom);
						break;
					case FLOTTE_ATTAQUER:
						gfx_PrintString("Attaque");
						break;
				}
				if(*key = sk_Enter){ //effectuer l'action
					if(fenetre->selection == FLOTTE_BOUGER){
						FlotteBouger(fenetre->flotteSelectionee, 1, 0, camera, empireListe);
					}
					else{
						switch(flotte->type){
							case FLOTTE_MILITAIRE:
								switch(fenetre->selection){
									case FLOTTE_ATTAQUER:
										break; 
								}
						}
					}
				}
				break;
		}
	}
	
	//menu marche
	else if(camera->fenetre == MENU_MARCHE)
	{
		if(*key == sk_Clear)
		{
			camera->fenetre = MENU_AUCUN;
			camera->bloque = FALSE;
			*key = 0;
		}
		//dessiner fenetre
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(40, 40, 240, 160);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(40, 40, 240, 160);
		gfx_HorizLine_NoClip(50, 50, 220);
		gfx_SetColor(1);
		switch(fenetre->selection)
		{
			case 1:
				gfx_PrintStringXY("Argent", 136, 41);
				gfx_PrintStringXY("Non /changeable", 100, 100);
				break;
				
			case 2:
				gfx_PrintStringXY("Minerai", 132, 41);
				gfx_TransparentSprite_NoClip(credit, 70, 60);
				gfx_PrintStringXY("=" , 80, 60);
				gfx_SetTextXY(90, 60);
				if(marche->valeurMinerai < 100)
				{
					gfx_PrintString("0,");
					gfx_PrintInt(marche->valeurMinerai, 1);
				}
				else
				{
					gfx_PrintInt(marche->valeurMinerai / 100, 2);
					gfx_PrintString(",");
					gfx_PrintInt((marche->valeurMinerai - marche->valeurMinerai / 100)*100, 2);
				}
				gfx_TransparentSprite_NoClip(minerai, 130, 60);
				break;

			case 3:
				gfx_PrintStringXY("Nourriture", 132, 41);
				gfx_TransparentSprite_NoClip(credit, 70, 60);
				gfx_PrintStringXY("=" , 80, 60);
				gfx_SetTextXY(90, 60);
				if(marche->valeurMinerai < 100)
				{
					gfx_PrintString("0,");
					gfx_PrintInt(marche->valeurNourriture, 1);
				}
				else
				{
					gfx_PrintInt(marche->valeurNourriture / 100, 2);
					gfx_PrintString(",");
					gfx_PrintInt((marche->valeurNourriture - marche->valeurNourriture / 100)*100, 2);
				}
				gfx_TransparentSprite_NoClip(food, 130, 60);
				break;

			case 4:
				gfx_PrintStringXY("Fer", 132, 41);
				gfx_TransparentSprite_NoClip(credit, 70, 60);
				gfx_PrintStringXY("=" , 80, 60);
				gfx_SetTextXY(90, 60);
				if(marche->valeurFer < 100)
				{
					gfx_PrintString("0,");
					gfx_PrintInt(marche->valeurFer, 1);
				}
				else
				{
					gfx_PrintInt(marche->valeurFer / 100, 2);
					gfx_PrintString(",");
					gfx_PrintInt((marche->valeurFer - marche->valeurFer / 100)*100, 2);
				}
				gfx_TransparentSprite_NoClip(fer, 130, 60);
				break;
				break;
			case 5:
				gfx_PrintStringXY("Autre", 148, 41);
				break;
		}
	}
	
	//menu flotte
	else if (camera->fenetre == MENU_FLOTTE)
	{
		switch (*key) {
			case sk_Clear:
				camera->fenetre = MENU_AUCUN;
				camera->bloque = FALSE;
				*key = 0;
				break;
			case sk_Down:
				fenetre->selection++;
				*key = 0;
				break;
			case sk_Up:
				fenetre->selection--;
				*key = 0;
				break;
		}
		//dessiner fenetre
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(40, 40, 240, 160);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(40, 40, 240, 160);
		gfx_HorizLine_NoClip(50, 50, 220);
		gfx_PrintStringXY("Flotte", 136, 41);
		niveau = 57;
		flotte = joueur->flotte->premier;
		compteur = 0;
		compteurFlotte = 1;
		while(flotte != NULL){
			compteur++;
			flotte = flotte->suivant;
		}

		if (fenetre->selection > compteur) {
			fenetre->selection = 1;
		}
		else if (fenetre->selection < 1) {
			fenetre->selection = compteur;
		}

		flotte = joueur->flotte->premier;
		while(flotte != NULL) {
			if(fenetre->selection == compteurFlotte) {
				gfx_SetTextFGColor(13);
			}
			else {
				gfx_SetTextFGColor(1);
			}
			switch(flotte->type){
				case FLOTTE_MILITAIRE:
					gfx_PrintStringXY("militaire", 208, niveau);
					break;
				case FLOTTE_DE_CONSTRUCTION:
					gfx_PrintStringXY("construction", 184, niveau);
					break;
				case FLOTTE_SCIENTIFIQUE:
					gfx_PrintStringXY("scientifique", 184, niveau);
					break;
			}
			gfx_SetTextXY(45, niveau);
			gfx_PrintString("Flotte ");
			PrintInt(compteurFlotte);
			gfx_HorizLine_NoClip(50, niveau + 11, 220);
			niveau += 17;
			compteurFlotte++;
			flotte = flotte->suivant;
		}
		if(*key == sk_Enter){
			camera->fenetre = MENU_SYSTEME;
			fenetre->flotteSelectionee = fenetre->selection;
			fenetre->selection = 1;
			fenetre->ouverte = 6;
			fenetre->precedente = 2;
		}
	}
	
	//menu recherche
	else if (camera->fenetre == MENU_RECHERCHE)
	{
		if(*key == sk_Clear)
		{
			camera->fenetre = MENU_AUCUN;
			camera->bloque = FALSE;
			*key = 0;
		}
		//dessiner fenetre
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(40, 40, 240, 160);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(40, 40, 240, 160);
		gfx_HorizLine_NoClip(50, 50, 220);
		gfx_SetColor(1);
		gfx_PrintStringXY("Recherche", 124, 41);
	}
	
	//menu contacts
	else if (camera->fenetre == MENU_CONTACTS)
	{
		if(*key == sk_Clear)
		{
			camera->fenetre = MENU_AUCUN;
			camera->bloque = FALSE;
			*key = 0;
		}
		//dessiner fenetre
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(40, 40, 240, 160);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(40, 40, 240, 160);
		gfx_HorizLine_NoClip(50, 50, 220);
		gfx_SetColor(1);
		gfx_PrintStringXY("Contacts", 128, 41);
	}
	return 1;
}

/******************gerer le temps******************/
void StellarisTemps(Empire *joueur, Date *date, char *key)
{
	switch(*key)
	{
		case sk_0:
			date->vitesse = 0;
			break;
		case sk_Add:
			date->vitesse += 1;
			break;
		case sk_Sub:
			date->vitesse -= 1;
			break;
	}
	if(date->vitesse > 3)
	{
		date->vitesse = 3;
	}
	else if(date->vitesse < -2)
	{
		date->vitesse = -2;
	}
	switch (date->vitesse)
	{
		case -2:
			date->horloge ++;
			break;
		case -1:
			date->horloge += 2;
			break;
		case 1:
			date->horloge += 3;
			break;
		case 2:
			date->horloge += 4;
			break;
		case 3:
			date->horloge += 6;
			break;
	}
	if (date->horloge >= 24)
	{
		date->horloge = 0;
	}
	if ((date->horloge == 0) &&(date->vitesse !=0))
	{
		date->jour++;
		if(date->jour > 30)
		{
				date->jour = 1;
				date->mois++;
		}
		if(date->mois > 12)
		{
				date->mois = 1;
				date->annee++;
		}
	}
}

/********dessiner la map********/
void StellarisMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur)
{
	int i = 0, x = 0, y = 0, xLn = 0, yLn = 0, hyperLane1 = 0, hyperLane2 = 0, hyperLane3 = 0, systeme = 0;
	int xFlotte = 0, yFlotte = 0;
	char nombrePlanetesHabitablesSysteme = 0, nombrePlanetesHabiteesSysteme = 0, key2 = 0;
	Flotte* flotte = NULL;
	gfx_SetColor(1);
	if(camera->mapType == 1)
	{
		key2 = 0;
		if(camera->fenetre == MENU_AUCUN)
		{
			key2 = os_GetCSC();
		}

		KeyAction(empireListe, systemeStellaires, camera, key, flotteJoueur, date, fenetre, joueur);
		KeyAction(empireListe, systemeStellaires, camera, &key2, flotteJoueur, date, fenetre, joueur);

		if (camera->zoom < 1)
		{
			camera->zoom = 1;
			camera->mapType = 2;
		}
		else if (camera->zoom > 2)
		{
			camera->zoom = 2;
		}
		
		if(camera->vecteury != 0)
		{
			camera->y += camera->vecteury;
			if(camera->vecteury > 0)
			{
				camera->vecteury--;
			}
			else if(camera->vecteury < 0)
			{
				camera->vecteury++;
			}
		}
		if(camera->vecteurx != 0)
		{
			camera->x += camera->vecteurx;
			if(camera->vecteurx > 0)
			{
				camera->vecteurx--;
			}
			else if(camera->vecteurx < 0)
			{
				camera->vecteurx++;
			}
		}
		
		if (camera->x < 160)
		{
			camera->x = 160;
			camera->vecteurx = 0;
		}
		else if ((LARGEUR_GALAXIE + 1) * 50  * camera->zoom < camera->x)
		{
			camera->x = (LARGEUR_GALAXIE + 1) * 50 * camera->zoom;
			camera->vecteurx = 0;
		}
		if (camera->y < 120)
		{
			camera->y = 120;
			camera->vecteury = 0;
		}
		else if((LARGEUR_GALAXIE + 1) * 50  * camera->zoom < camera->y)
		{
			camera->y = (LARGEUR_GALAXIE + 1) * 50  * camera->zoom;
			camera->vecteury = 0;
		}
		
		camera->selection = -1;
	}
	else
	{
		if(camera->mapType != 2)
		{
			camera->mapType = 2;
		}
		switch(*key)
		{
			case sk_Del:
				camera->mapType = 1;
				break;
		}
	}

	while (i < (LARGEUR_GALAXIE * LARGEUR_GALAXIE) - 1)
	{
		hyperLane1 = 255;
		hyperLane2 = 255;
		hyperLane3 = 255;
		
		nombrePlanetesHabitablesSysteme = 0;
		nombrePlanetesHabiteesSysteme = 0;
		
		xLn = 0;
		yLn = 0;
		if(camera->mapType == 1)
		{
			x = systemeStellaires[i].x * camera->zoom - camera->x + 160;
			y = systemeStellaires[i].y * camera->zoom - camera->y + 120;
		}
		else if(camera->mapType == 2)
		{
			x = systemeStellaires[i].x / 2.5 + 10;
			y = systemeStellaires[i].y / 2.5 - 30;
		}
		if ( (((0 <= x) && (x <= 320)) && ((0 <= y)&& (y <= 240))) && ((systemeStellaires[i].x != 0) && (systemeStellaires[i].y != 0)) )
		{
			if(systemeStellaires[i].niveauConnaissance != 0) {
				if(systemeStellaires[i].empire == 1)
				{
					if(camera->mapType == 1)
					{
						gfx_SetColor(9);
						gfx_Circle(x, y, 20 * camera->zoom);
						gfx_Circle(x, y, (19 * camera->zoom) + 1);
					}
					else {
						gfx_SetColor(9);
						gfx_Circle(x, y, 10);
						gfx_Circle(x, y, 9);
					}
				}
				//dessiner hyperLanes
				hyperLane1 = systemeStellaires[i].hyperlane1;
				if (hyperLane1 != 255)
				{
					if((systemeStellaires[hyperLane1].niveauConnaissance == 0) || (systemeStellaires[i].niveauConnaissance == 0)) {
						gfx_SetColor(11);
					}
					else {
						gfx_SetColor(12);
					}
					
					if(camera->mapType == 1)
					{
						xLn = systemeStellaires[hyperLane1].x*camera->zoom - camera->x + 160;
						yLn = systemeStellaires[hyperLane1].y*camera->zoom - camera->y + 120;
					}
					else 
					{
						xLn = systemeStellaires[hyperLane1].x / 2.5 + 10;
						yLn = systemeStellaires[hyperLane1].y / 2.5 - 30;
					}
					if ( (((0 <= xLn) && (xLn <= 320)) && ((0 <= yLn) && (yLn <= 240))) && (((0 <= x) && (x <= 320)) && ((0 <= y) && (y <= 240))) )
					{
						gfx_Line_NoClip(x, y, xLn, yLn);
					}
				}
				
				xLn = 0;
				yLn = 0;
				hyperLane2 = systemeStellaires[i].hyperlane2;
				if (hyperLane2 != 255)
				{
					if((systemeStellaires[hyperLane1].niveauConnaissance == 0) || (systemeStellaires[i].niveauConnaissance == 0)) {
						gfx_SetColor(11);
					}
					else {
						gfx_SetColor(12);
					}
					
					if(camera->mapType == 1)
					{
						xLn = systemeStellaires[hyperLane2].x*camera->zoom - camera->x + 160;
						yLn = systemeStellaires[hyperLane2].y*camera->zoom - camera->y + 120;
					}
					else 
					{
						xLn = systemeStellaires[hyperLane2].x / 2.5 + 10;
						yLn = systemeStellaires[hyperLane2].y / 2.5 - 30;
					}
					if ( (((0 <= xLn) && (xLn <= 320)) && ((0 <= yLn) && (yLn <= 240))) && (((0 <= x) && (x <= 320)) && ((0 <= y) && (y <= 240))) )		
					{
						gfx_Line_NoClip(x, y, xLn, yLn);
					}
				}
				
				if(camera->mapType == 1)
				{
					gfx_SetTextFGColor(1);
					gfx_SetColor(16);
					//dessiner planetes habitables
					switch(systemeStellaires[i].nombrePlanetes)
					{
						case 5:
							if(systemeStellaires[i].planete5->habitable == 1) {
								if(systemeStellaires[i].planete5->population == 0) {
									nombrePlanetesHabitablesSysteme++;
								}
								else {
									gfx_FillRectangle(x - strlen(systemeStellaires[i].nom) * 4, y + 8, strlen(systemeStellaires[i].nom) * 8, 10);
									gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
									nombrePlanetesHabiteesSysteme++;
								}
							}
						case 4:
							if(systemeStellaires[i].planete4->habitable == 1) {
								if(systemeStellaires[i].planete4->population == 0) {
									nombrePlanetesHabitablesSysteme++;
								}
								else if(nombrePlanetesHabiteesSysteme == 0){
									gfx_FillRectangle(x - strlen(systemeStellaires[i].nom) * 4, y + 8, strlen(systemeStellaires[i].nom) * 8, 10);
									gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
									nombrePlanetesHabiteesSysteme++;
								}
							}
						case 3:
							if(systemeStellaires[i].planete3->habitable == 1) {
								if(systemeStellaires[i].planete3->population == 0) {
									nombrePlanetesHabitablesSysteme++;
								}
								else if(nombrePlanetesHabiteesSysteme == 0) {
									gfx_FillRectangle(x - strlen(systemeStellaires[i].nom) * 4, y + 8, strlen(systemeStellaires[i].nom) * 8, 10);
									gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
									nombrePlanetesHabiteesSysteme++;
								}
							}
						case 2:
							if(systemeStellaires[i].planete2->habitable == 1) {
								if(systemeStellaires[i].planete2->population == 0) {
									nombrePlanetesHabitablesSysteme++;
								}
								else if(nombrePlanetesHabiteesSysteme == 0) {
									gfx_FillRectangle(x - strlen(systemeStellaires[i].nom) * 4, y + 8, strlen(systemeStellaires[i].nom) * 8, 10);
									gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
									nombrePlanetesHabiteesSysteme++;
								}
							}
						case 1:
							if(systemeStellaires[i].planete1->habitable == 1) {
								if(systemeStellaires[i].planete1->population == 0) {
									nombrePlanetesHabitablesSysteme++;
								}
								else if(nombrePlanetesHabiteesSysteme == 0) {
									gfx_FillRectangle(x - strlen(systemeStellaires[i].nom) * 4, y + 8, strlen(systemeStellaires[i].nom) * 8, 10);
									gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
									nombrePlanetesHabiteesSysteme++;
								}
							}
					}
					if(nombrePlanetesHabitablesSysteme > 0) {
						gfx_TransparentSprite(highHabitablePlanet, x - 10, y);
					}
					if(nombrePlanetesHabiteesSysteme == 0) {
						if(systemeStellaires[i].niveauConnaissance >= 1) {
							
							if(systemeStellaires[i].niveauConnaissance == 3) {
								gfx_SetTextFGColor(1);
							}
							else {
								gfx_SetTextFGColor(11);
							}
							gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
						}
					}
				}
			}
			
			if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y)))
			{
				gfx_SetColor(9);
				gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
				camera->selection = i;
			}
			
			if(camera->mapType == 1)
			{
				switch(systemeStellaires[i].etoileType)
				{
					case 1: //B
						gfx_SetColor(15);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						break;
					case 2: //A
						gfx_SetColor(14);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case 3: //F
						gfx_SetColor(1);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case 4: //G
						gfx_SetColor(1);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case 5: //K
						gfx_SetColor(13);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						break;
					case 6: //M
						gfx_SetColor(13);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case 7: //trou noir
						gfx_SetColor(13);
						gfx_Circle(x, y, 2 * camera->zoom);
						break;
					case 8: //pulsar
						gfx_SetColor(14);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						break;
					case 9: ///toile a neutrons
						gfx_SetColor(14);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						gfx_Circle(x, y, 3 * camera->zoom);
						break;
				}
			}
			else
			{
				switch(systemeStellaires[i].etoileType)
				{
					case 1: //B
						gfx_SetColor(15);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 2: //A
						gfx_SetColor(14);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 3: //F
						gfx_SetColor(1);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 4: //G
						gfx_SetColor(1);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 5: //K
						gfx_SetColor(13);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 6: //M
						gfx_SetColor(13);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 7: //trou noir
						gfx_SetColor(13);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 8: //pulsar
						gfx_SetColor(14);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case 9: ///toile a neutrons
						gfx_SetColor(14);
						gfx_Circle_NoClip(x, y, 1);
						break;
				}
			}
		}
		i++;
	}
	
	if(camera->mapType == 1)
	{
		//dessiner flottes
		flotte = joueur->flotte->premier;
		while(flotte != NULL) {
			systeme = flotte->systeme;
			xFlotte = systemeStellaires[systeme].x*camera->zoom - camera->x + 165;
			yFlotte = systemeStellaires[systeme].y*camera->zoom - camera->y + 110;
			if(((xFlotte > 0) && (xFlotte < 320)) && ((0 < yFlotte) && (yFlotte < 240)))
			{
				while(gfx_GetPixel(xFlotte + 4, yFlotte + 1) == 19){
					xFlotte += 11;
				}
				gfx_TransparentSprite(ourFleet, xFlotte, yFlotte);
				switch(flotte->type){
					case FLOTTE_MILITAIRE:
						gfx_TransparentSprite(force_our, xFlotte + 6, yFlotte - 1);
						if (flotte->puissance > 500)
						{
							gfx_TransparentSprite(force_our, xFlotte + 10, yFlotte - 1);
						}
						if (flotte->puissance > 1500)
						{
							gfx_TransparentSprite(force_our, xFlotte + 8, yFlotte - 4);
						}
						break;
					case FLOTTE_DE_CONSTRUCTION:
						gfx_TransparentSprite(construction_ship_our_icon, xFlotte + 6, yFlotte - 4);
						break;
					case FLOTTE_SCIENTIFIQUE:
						gfx_TransparentSprite(science_ship_our_icon, xFlotte + 6, yFlotte - 4);
						break;
				}
			}
			flotte = flotte->suivant;
		}
	}
}

int StellarisSauvegarde(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Marche *marche)
{
	int i = 0;
	Flotte* flotte = NULL, *flottePrecedente = NULL;
	Empire* empire = NULL, *empirePrecedent = NULL;
	int compteur = 0;
	ti_CloseAll();
	*sauvegarde = ti_Open("sauv", "w");
	ti_Write(parametres, sizeof(Parametres), 1, *sauvegarde);
	ti_Write(date, sizeof(Date), 1, *sauvegarde);
	ti_Write(camera, sizeof(Camera), 1, *sauvegarde);
	ti_Write(marche, sizeof(Marche), 1, *sauvegarde);
	i = 0;
	
	//enregistrer
	while(i <= LARGEUR_GALAXIE * LARGEUR_GALAXIE)
	{
		ti_Write(&systemeStellaires[i], sizeof(SystemeStellaire), 1, *sauvegarde);
		
		if(systemeStellaires[i].nombrePlanetes >= 1)
		{
			ti_Write(systemeStellaires[i].planete1, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 2)
		{
			ti_Write(systemeStellaires[i].planete2, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 3)
		{
			ti_Write(systemeStellaires[i].planete3, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 4)
		{
			ti_Write(systemeStellaires[i].planete4, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes == 5)
		{
			ti_Write(systemeStellaires[i].planete5, sizeof(Planete), 1, *sauvegarde);
		}
		i++;
	}
	
	compteur = 0;

	empire = empireListe->premier;
	while(empire != NULL) {
		compteur++;
		empire = empire->suivant;
	}

	ti_Write(&compteur, sizeof(int), 1, *sauvegarde);

	empire = empireListe->premier;
	while(empire != NULL) {
		ti_Write(empire, sizeof(Empire), 1, *sauvegarde);
		
		//enregistrer flottes
		compteur = 0;
		flotte = empire->flotte->premier;
		while(flotte != NULL) {
			compteur++;
			flotte = flotte->suivant;
		}
		ti_Write(&compteur, sizeof(int), 1, *sauvegarde);
		flotte = empire->flotte->premier;
		while(flotte != NULL) {
			ti_Write(flotte, sizeof(Flotte), 1, *sauvegarde);
			flotte = flotte->suivant;
		}

		empire = empire->suivant;
	}
	
	return 1;
}

/**********************************************Fonctions generales**********************************************/

/*print centered fonction*/
void PrintCentered(const char *str, int y, int taille, int color, int differenceX)
{
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

void PrintHUD(const unsigned int nombre, const int change, int x, int y)
{
	unsigned char longueur, color;
	char nombreStr[10];
	char changeStr[5];
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
	if(change >= 0)
	{
		gfx_SetTextFGColor(1);
	}
	else
	{
		gfx_SetTextFGColor(3);
	}
	if(nombre < 1000)
	{
		sprintf(nombreStr, "%d", nombre);
		if (change >= 0)
		{
		sprintf(changeStr, "+%d", change);
		}
		else
		{
		sprintf(changeStr, "%d", change);
		}
		strcat(nombreStr, changeStr);
	}
	else
	{
		sprintf(nombreStr, "%d", nombre/1000);
		if (change >=0)
		{
		sprintf(changeStr, "+%d", change);
		}
		else
		{
		sprintf(changeStr, "%d", change);
		}
		strcat(nombreStr, "K");
		strcat(nombreStr, changeStr);
	}
	gfx_PrintStringXY(nombreStr, x, y);
}

void PrintText(const char *str, int x, int y, int taille, int color)
{
	int a, longueur, i;
    gfx_TempSprite(ch, 8, 8);
	gfx_SetFontData(font_logo);

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


FlotteListe* FlotteListeCreer() {
	FlotteListe* flotteliste;
	flotteliste = malloc(sizeof(FlotteListe));
	flotteliste->premier = NULL;
	return flotteliste;
}

void FlotteListeSupprimer(FlotteListe* flotteliste) {
	Flotte *pointeur = 0, *pointeursuivant = 0;
	int compteur = 0;
	pointeur = flotteliste->premier;
	while(pointeur->suivant != NULL) {
		pointeursuivant = pointeur->suivant;
		free(pointeur);
		pointeur = pointeursuivant;
	}
	free(flotteliste);
}

Flotte* FlotteNumero(FlotteListe* flotteliste, int numero) {
	Flotte *pointeur = 0;
	int compteur = 1;
	pointeur = flotteliste->premier;
	while(compteur < numero) {
		if(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	return pointeur;
}

Flotte* FlotteAjouter(FlotteListe* flotteliste) {
	Flotte *pointeur = 0, fin = 0;
	pointeur = flotteliste->premier;
	if(flotteliste->premier != NULL) {
		while(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		pointeur->suivant = malloc(sizeof(Flotte));
		pointeur = pointeur->suivant;
		pointeur->suivant = NULL;
	}
	else {
		flotteliste->premier = malloc(sizeof(Flotte));
		pointeur = flotteliste->premier;
	}
	return pointeur;
}

void FlotteSupprimer(FlotteListe* flotteliste, int numero) {
	Flotte *pointeur = 0, *pointeurPrecedent = 0;
	int compteur = 0;
	pointeur = flotteliste->premier;
	while(compteur <= numero) {
		if(pointeur->suivant != NULL) {
			pointeurPrecedent = pointeur;
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	if(pointeur == flotteliste->premier){
		flotteliste->premier = pointeur->suivant;
	}
	else {
		pointeurPrecedent->suivant = pointeur->suivant;
	}
	free(pointeur);
}



EmpireListe* EmpireListeCreer() {
	EmpireListe* empireListe;
	empireListe = malloc(sizeof(EmpireListe));
	empireListe->premier = NULL;
	return empireListe;
}

void EmpireListeSupprimer(EmpireListe* empireListe) {
	Empire *pointeur = 0, *pointeursuivant = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	while(pointeur->suivant != NULL) {
		pointeursuivant = pointeur->suivant;
		free(pointeur);
		pointeur = pointeursuivant;
	}
	free(empireListe);
}

Empire* EmpireNumero(EmpireListe* empireListe, int numero) {
	Empire *pointeur = 0;
	int compteur = 1;
	pointeur = empireListe->premier;
	while(compteur < numero) {
		if(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	return pointeur;
}

Empire* EmpireAjouter(EmpireListe* empireListe) {
	Empire *pointeur = 0, fin = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	if(empireListe->premier != NULL) {
		while(pointeur->suivant != NULL) {
			pointeur = pointeur->suivant;
		}
		pointeur->suivant = malloc(sizeof(Empire));
		pointeur = pointeur->suivant;
		pointeur->suivant = NULL;
	}
	else {
		empireListe->premier = malloc(sizeof(Empire));
		pointeur = empireListe->premier;
	}
	return pointeur;
}

void EmpireSupprimer(EmpireListe* empireListe, int numero) {
	Empire *pointeur = 0, *pointeurPrecedent = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	while(compteur <= numero) {
		if(pointeur->suivant != NULL) {
			pointeurPrecedent = pointeur;
			pointeur = pointeur->suivant;
		}
		compteur++;
	}
	if(pointeur == empireListe->premier){
		empireListe->premier = pointeur->suivant;
	}
	else {
		pointeurPrecedent->suivant = pointeur->suivant;
	}
	free(pointeur);
}

void KeyAction(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur){
	switch(*key){
		case sk_Up:
			if (camera->bloque != TRUE) {
				camera->vecteury -= 5;
			}
			break;
		case sk_Down:
			if (camera->bloque != TRUE) {
				camera->vecteury += 5;
			}
			break;
		case sk_Left:
			if (camera->bloque != TRUE) {
				camera->vecteurx -= 5;
			}
			break;
		case sk_Right:
			if (camera->bloque != TRUE) {
				camera->vecteurx += 5;
			}
			break;
		case sk_Mode:
			if (camera->bloque != TRUE) {
				camera->zoom -= 1;
				if(camera->zoom >= 1)
				{
					camera->x *= 0.5;
					camera->y *= 0.5;
				}
			}
			break;
		case sk_Del:
			if (camera->bloque != TRUE) {
				camera->zoom += 1;
				if (camera->zoom < 3 && camera->zoom >= 1)
				{
					camera->x *= 2;
					camera->y *= 2;
				}
			}
			break;
		case sk_Enter:
			if (((camera->fenetre == MENU_AUCUN) && (camera->selection != -1)) && (camera->bougerFlotte = FALSE))
			{
				camera->fenetre = MENU_SYSTEME;
				camera->bloque = TRUE;
				fenetre->selection = 1;
				fenetre->ouverte = 0;
				*key = 0;
			}
			else if((camera->selection != -1) && (camera->bougerFlotte = TRUE)){
				FlotteBouger(camera->flotte, camera->empire, fenetre->selection, camera, empireListe);
			}
			break;
		case sk_Clear:
			if (camera->fenetre == MENU_AUCUN)
			{
				camera->fenetre = MENU_QUITTER;
				camera->bloque = TRUE;
				date->vitesse = 0;
				fenetre->selection = 1;
				*key = 0;
			}
			break;
		case sk_Yequ :
			if ((camera->fenetre == MENU_AUCUN) || (fenetre->selection != 1))
			{
				camera->fenetre = MENU_MARCHE;
				fenetre->selection = 1;
				camera->bloque = TRUE;
			}
			else if ((camera->fenetre == MENU_MARCHE) && (fenetre->selection != 1))
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Window :
			if ((camera->fenetre == MENU_AUCUN) || (fenetre->selection != 2))
			{
				camera->fenetre = MENU_MARCHE;
				fenetre->selection = 2;
				camera->bloque = TRUE;
			}
			else if ((camera->fenetre == MENU_MARCHE) && (fenetre->selection != 2))
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Zoom :
			if ((camera->fenetre == MENU_AUCUN) || (fenetre->selection != 3))
			{
				camera->fenetre = MENU_MARCHE;
				fenetre->selection = 3;
				camera->bloque = TRUE;
			}
			else if ((camera->fenetre == MENU_MARCHE) && (fenetre->selection != 3))
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Trace :
			if ((camera->fenetre == MENU_AUCUN) || (fenetre->selection != 4))
			{
				camera->fenetre = MENU_MARCHE;
				fenetre->selection = 4;
				camera->bloque = TRUE;
			}
			else if ((camera->fenetre == MENU_MARCHE) && (fenetre->selection != 4))
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Graph :
			if ((camera->fenetre == MENU_AUCUN) || (fenetre->selection != 5))
			{
				camera->fenetre = MENU_MARCHE;
				fenetre->selection = 5;
				camera->bloque = TRUE;
			}
			else if ((camera->fenetre == MENU_MARCHE) && (fenetre->selection != 5))
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Recip:
			if ((camera->fenetre != MENU_MARCHE) && (camera->fenetre != MENU_QUITTER))
			{
				camera->fenetre = MENU_MARCHE;
				fenetre->selection = 2;
				camera->bloque = TRUE;
			}
			else if (camera->fenetre == MENU_MARCHE)
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Math:
			if ((camera->fenetre != MENU_CONTACTS) && (camera->fenetre != MENU_QUITTER))
			{
				camera->fenetre = MENU_CONTACTS;
				fenetre->selection = 0;
				camera->bloque = TRUE;
			}
			else if (camera->fenetre == MENU_CONTACTS)
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Square:
			if ((camera->fenetre != MENU_RECHERCHE) && (camera->fenetre != MENU_QUITTER))
			{
				camera->fenetre = MENU_RECHERCHE;
				fenetre->selection = 0;
				camera->bloque = TRUE;
			}
			else if (camera->fenetre == MENU_RECHERCHE)
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
		case sk_Log:
			if ((camera->fenetre != MENU_FLOTTE) && (camera->fenetre != MENU_QUITTER))
			{
				camera->fenetre = MENU_FLOTTE;
				fenetre->selection = 1;
				camera->bloque = TRUE;
			}
			else if (camera->fenetre == MENU_FLOTTE)
			{
				camera->fenetre = MENU_AUCUN;
				fenetre->selection = 0;
				camera->bloque = FALSE;
			}
			break;
	}
}

void CouleurPlanete(char type){
	switch(type){
		case 1:
			gfx_SetColor(20);
			break;
		case 2:
			gfx_SetColor(21);
			break;
		case 3:
			gfx_SetColor(22);
			break;
		case 4:
			gfx_SetColor(23);
			break;
		case 5:
			gfx_SetColor(23);
			break;
		case 6:
			gfx_SetColor(24);
			break;
		case 7:
			gfx_SetColor(25);
			break;
		case 8:
			gfx_SetColor(26);
			break;
		case 9:
			gfx_SetColor(25);
			break;
		case 10:
			gfx_SetColor(20);
			break;
		case 11:
			gfx_SetColor(23);
			break;
		case 12:
			gfx_SetColor(20);
			break;
		case 13:
			gfx_SetColor(23);
			break;
		case 14:
			gfx_SetColor(29);
			break;
		case 15:
			gfx_SetColor(27);
			break;
		case 16:
			gfx_SetColor(28);
			break;
	}
}

void FlotteBouger(int numeroDeFlotte, int numeroDeEmpire, char systeme, Camera *camera, EmpireListe *empireListe){
	Empire* empire;
	Flotte* flotte;
	empire = EmpireNumero(empireListe, numeroDeEmpire);
	flotte = FlotteNumero(empire->flotte, numeroDeFlotte);
	if(camera->bougerFlotte == FALSE){
		camera->bougerFlotte = TRUE;
		camera->bloque = FALSE;
		camera->fenetre = MENU_AUCUN;
		camera->empire = numeroDeEmpire;
		camera->flotte = numeroDeFlotte;
	}
	else{
		camera->bougerFlotte = FALSE;
		flotte->systeme = systeme;
	}
}