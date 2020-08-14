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

#include "ai.h"
#include "boucle.h"
#include "camera.h"
#include "console.h"
#include "flottes.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "parametres.h"
#include "systemes.h"
#include "time.h"

#include "locale/locale.h"

/* private variables =================================================== */

char * nomGalaxies[] = {
	"Yekt",
	"Veax",
	"Uthau",
	"Hep",
	"Xons",
	"Zigdioks",
	"Pluekreft",
	"Qauduks",
	"Sriucros",
	"Wuelsurs",
	"Ni",
	"Gree",
	"Etrooc",
	"Ro",
	"Ozraay",
	"Ekruacsus",
	"Ephaezde",
	"Yuazlen",
	"Zrusre",
	"Sroiruts",
	"Toik",
	"Efe",
	"Qiacs",
	"Oreak",
	"Xo",
	"Vlotaes",
	"Vreuzrab",
	"Praothal",
	"Iguezla",
	"Aflaovikt",
	"Kuad",
	"Zut",
	"Shias",
	"Traakt",
	"Ref",
	"Thumdeu",
	"Ebreocod",
	"Taaki",
	"Inertar",
	"Choaco",
	"Acu",
	"Vruth",
	"Afleen",
	"Su",
	"Ivriab",
	"Afablut",
	"Sleoclu",
	"Qeorit",
	"Vleastruc",
	"Struntar"
	"Clax",
	"Phab",
	"Zeop",
	"Imoig",
	"Vraash",
	"Qentael",
	"Hepheob",
	"Baokrap",
	"Uphiartal",
	"Sropsau",
	"Ulers",
	"Israef",
	"Vruey",
	"Fli",
	"Thio",
	"Zliokluh",
	"Fofeir",
	"Ufeepsuns",
	"Eslauprek",
	"Slebli",
	"Athent",
	"Iveaph",
	"Azleem",
	"Puets",
	"Avors",
	"Troaholt",
	"Strobrims",
	"Pluebsups",
	"Xucdiult",
	"Urimdin",
	"Thi",
	"Eclair",
	"Flio",
	"Tiud",
	"Poals",
	"Vleuzlars",
	"Gramaob",
	"Driwex",
	"Utrinsaa",
	"Flezlauft",
	"Xoab",
	"Chur",
	"Ijoor",
	"Oplaap",
	"Uzluak",
	"Esreeclu",
	"Ukraammiy",
	"Loissekt",
	"Klaezzash",
	"Strivo",
	"Zuel",
	"Lo",
	"Ekruem",
	"Osi",
	"Teo",
	"Yoizolt",
	"Cheddeel",
	"Iyiubsud",
	"Braddaub",
	"Koiploc"
};


char * nomPlanetes[] = {
	"Egnepra",
	"Aceonerth",
	"Tochade",
	"Pemapus",
	"Ruliv",
	"Chuturn",
	"Gidetera",
	"Gneputani",
	"Nora",
	"Gao",
	"Vunubos",
	"Zolveiril",
	"Ostriea",
	"Hibbillon",
	"Cheruta",
	"Puthea",
	"Cuinia",
	"Gachuruta",
	"Gone",
	"Thade",
	"Bevotov",
	"Chavivis",
	"Yuvore",
	"Biseron",
	"Zauter",
	"Sony",
	"Michethea",
	"Gravutis",
	"Lille",
	"Strade",
	"Kidririli",
	"Bonvanus",
	"Icharvis",
	"Zilminda",
	"Yutune",
	"Thaelia",
	"Luchazuno",
	"Truutis",
	"Lleshan",
	"Cyke",
	"Themeanov",
	"Miccoiria",
	"Gichurn",
	"Nibronoe",
	"Nieliv",
	"Roirus",
	"Bamipra",
	"Cregocaro",
	"Brypso",
	"Deron",
	"Kindounus",
	"Danniula",
	"Ondosie",
	"Yangomia",
	"Hephus",
	"Haiturn",
	"Crumuturn",
	"Loxistea",
	"Brade",
	"Tryria",
	"Yovoanide",
	"Ellaclite",
	"Naccone",
	"Relnoth",
	"Beuliv",
	"Thauyama",
	"Chayolara",
	"Phugemia",
	"Larth",
	"Griri"
};

/* private functions =================================================== */
static int QuitterNouvellePartieAvertissement();
static int NouvellePartieEspece(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieGouvernement(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartiePrincipes(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieNom(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieDrapeau(EmpireListe *empireListe, Parametres *parametres);
static int NouvellePartieParametres(EmpireListe *empireListe, Parametres *parametres);

/**
 * avertissement lorsqu'on veut quitter la creation d'une nouvelle partie
 */
int QuitterNouvellePartieAvertissement(){
	char key = 0, choix = 1, fin = 1;
	do {
		gfx_FillScreen(255);
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
	case 0: // si non
		return 0;
		break;
	case 1: // si oui
		return 1;
		break;
	}
	return 1;
}

/**
 * choix de l'espece
 */
int NouvellePartieEspece(EmpireListe *empireListe, Parametres *parametres){
	char key = 0, fin = 1, i = 0;
	int choix = 0;
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);
	while(fin) {
		choix = GetEmpireSpecies(joueur);
		do {
			gfx_SwapDraw();		
			gfx_FillScreen(255);
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
				SetEmpireGouvernement(joueur, choix);
				fin = NouvellePartieNom(empireListe, parametres);
				break;
		}
	}
	return 0;
}

/*nouvelle partie principes*/
int NouvellePartiePrincipes(EmpireListe *empireListe, Parametres *parametres) {
	char key = 0, fin = 1, selection = 2, point = 3;
	char principe1 = -1, principe2 = -1, principe3 = -1, principe4 = -1, principe5 =-1;
	while(fin) {
		selection = 2;
		
		do {
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
	char key = 0, fin = 1, lettre = 0, majuscule = 1, curseur = 0, i = 0, finBoucle = 0, erreur = 0;
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);
	while(fin) {
		finBoucle = 0;
		do {
			if(erreur > 0) {
				erreur--;
			}
			gfx_SwapDraw();
			gfx_FillScreen(255);
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

/**
 * Choix du drapeau
 */
int NouvellePartieDrapeau(EmpireListe *empireListe, Parametres *parametres) {
	char key = 0, choix = 0, fin = 1;
	Empire *joueur = NULL;
	joueur = EmpireNumero(empireListe, 1);
	while(fin) {
		do {
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
		} while(((key = os_GetCSC()) != sk_Enter) && (key != sk_Clear));
		switch (key) {
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
static bool InitializeNewGame(EmpireListe **empireListe, Date **date, Camera **camera, Fenetre **fenetre, Marche **marche, Parametres **parametres, ti_var_t *sauvegarde){
	Empire *joueur = NULL;
	joueur = EmpireNumero(*empireListe, 1);
	ti_CloseAll();
	*sauvegarde = ti_Open("sauv", "w");
	SetEmpireCredit(joueur, 100);
	SetEmpireMinerals(joueur, 100);
	SetEmpireFood(joueur, 200);
	SetEmpireAlloys(joueur, 100);
	SetEmpireConsumerGoods(joueur, 100);

	*parametres = AllocParametres();

	SetSeeAll(*parametres, false);

	*date = AllocDate();
	SetTime(*date, 1, 1, 2200);
	SetTimeSpeed(*date, 0, 1);

	*camera = AllocCamera();
	SetCameraX(*camera, 380);
	SetCameraY(*camera, 360);
	SetCameraXSystem(*camera, 320);
	SetCameraYSystem(*camera, 240);
	SetCameraXVector(*camera, 0);
	SetCameraYVector(*camera, 0);

	SetCameraZoom(*camera, 1);
	SetCameraMapType(*camera, SYSTEME);
	
	*fenetre = AllocFenetre();
	CloseMenu(*fenetre, *camera);

	CloseCommandPrompt(*fenetre, *camera, *date);
	return 0;
}

/**
 * Create matrix
 */
static void CreateGalacticMatrix(int *galaxie, int espaceEntreEtoiles, int *barreDeChargement){
	int hauteur = 0, largeur = 0;
	int x = LIMITE_GAUCHE;
	int y = LIMITE_HAUT;
	while(hauteur < LARGEUR_GALAXIE) {
		while(largeur < LARGEUR_GALAXIE*2) {
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur] = x;
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1] = y;
			x += espaceEntreEtoiles;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		x = LIMITE_GAUCHE;
		y += espaceEntreEtoiles;
		hauteur++;
	}
}

/**
 * Randomise matrix position
 */
static void RandomGalacticMatrix(int *galaxie, int coefficientDeplacementStellaire, int *barreDeChargement){
	int hauteur = 0, largeur = 0;
	int coefficientX = 0, coefficientY = 0;
	int x = 0, y = 0;
	while(hauteur < LARGEUR_GALAXIE) {
		while(largeur < LARGEUR_GALAXIE*2) {
			x = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur];
			y = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1];
			coefficientX = randInt (-coefficientDeplacementStellaire, coefficientDeplacementStellaire);
			coefficientY = randInt (-coefficientDeplacementStellaire, coefficientDeplacementStellaire);
			x = x + (coefficientX / 5);
			y = y + (coefficientY / 5);
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur] = x;
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1] = y;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		hauteur++;
	}
}

static void RoundGalacticMatrix(int *galaxie, int espaceEntreEtoiles, int rayonExterieur, int rayonInterieur, int *barreDeChargement){
	int hauteur = 0, largeur = 0;
	int x = 0, y = 0;
	while(hauteur < LARGEUR_GALAXIE)
	{
		while(largeur < LARGEUR_GALAXIE * 2)
		{
			x = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur];
			y = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1];
			if(((pow(x - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2) + pow(y - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2)) > (pow(rayonExterieur,2))) || ((pow(x - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2) + pow(y - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2)) < (pow(rayonInterieur = 50,2))))
			{
				x = 0;
				y = 0;
			}
			galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur] = x;
			galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur + 1] = y;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		hauteur++;
	}
}

static void CreatePlanetSystem(SystemeStellaire *systemeStellaire, int numeroPlanete, int habitable){
	int rayonOrbite = 0, taille = 0, type = 0;
    int x = 0;
    int y = 0;

	type = randInt(1, 100);
	switch(numeroPlanete + 1){
	case 5:
		if(type <= 60) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 90){
			type = COLD_ARID; //monde aride froid
			habitable = 0;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}
		break;
	case 4:
		if(type <= 30) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 70){
			type = COLD_ARID; //monde aride froid
			habitable = 0;
		} else if (type <= 75){
			type = HABITABLE_ALPINE; //monde Alpine habitable
			habitable = 1;
		} else if (type <= 80){
			type = HABITABLE_ARCTIC; //monde Arctic froid
			habitable = 1;
		} else if (type <= 85){
			type = HABITABLE_TUNDRA; //monde Tundra habitable
			habitable = 1;
		} else if (type <= 90){
			type = HABITABLE_CONTINENTAL; //monde continental froid
			habitable = 1;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}	
		break;
	case 3:
		if(type <= 5) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 20){
			type = TOXIC; //monde toxique
			habitable = 0;
		} else if (type <= 60){
			type = COLD_ARID; //monde aride froid
			habitable = 0;
		} else if (type <= 70){
			type = HABITABLE_ALPINE; //monde Alpine habitable
			habitable = 1;
		} else if (type <= 74){
			type = HABITABLE_ARCTIC; //monde Arctic froid
			habitable = 1;
		} else if (type <= 78){
			type = HABITABLE_TUNDRA; //monde Tundra habitable
			habitable = 1;
		} else if (type <= 82){
			type = HABITABLE_CONTINENTAL; //monde continental froid
			habitable = 1;
		} else if (type <= 86){
			type = HABITABLE_OCEAN; //monde ocean
			habitable = 1;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}
		break;
	case 2:
		if(type <= 2) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 30){
			type = TOXIC; //monde toxique
			habitable = 0;
		} else if (type <= 80){
			type = ARID; //monde arid 
			habitable = 0;
		} else if (type <= 83){
			type = HABITABLE_CONTINENTAL; //monde continental habitable
			habitable = 1;
		} else if (type <= 86){
			type = HABITABLE_OCEAN; //monde ocean habitable
			habitable = 1;
		} else if (type <= 89){
			type = HABITABLE_TROPICAL; //monde tropical habitable
			habitable = 1;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}
		break;
	case 1:
		if(type <= 3) {
			type = HABITABLE_ARID; //monde arid
			habitable = 1;
		} else if (type <= 6){
			type = HABITABLE_DESERT; //monde Desert
			habitable = 1;
		} else if (type <= 9){
			type = HABITABLE_SAVANNA; //monde Savanna
			habitable = 1;
		} else if (type <= 40){
			type = ARID; //monde aride
			habitable = 0;
		} else if (type <= 50){
			type = BROKE; //monde brisé
			habitable = 0;
		} else{
			type = MOLTEN; //monde en fusion
			habitable = 0;
		}
		break;
	}

	taille = randInt(1, 100);
	switch(numeroPlanete + 1){
	case 1:
		rayonOrbite = randInt(RAYON_PLANETE1, RAYON_PLANETE1 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 2:
		rayonOrbite = randInt(RAYON_PLANETE2, RAYON_PLANETE2 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 3:
		rayonOrbite = randInt(RAYON_PLANETE3, RAYON_PLANETE3 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 4:
		rayonOrbite = randInt(RAYON_PLANETE4, RAYON_PLANETE4 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 5:
		rayonOrbite = randInt(RAYON_PLANETE5, RAYON_PLANETE5 + 10);
		if(type == 14) {
			taille = 5;
		}
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	}

    SetSystemPlanetHabitability(systemeStellaire, numeroPlanete, habitable);
    SetSystemPlanetOrbitRadius(systemeStellaire, numeroPlanete, rayonOrbite);

    x = randInt(X_CENTRE_SYSTEME - rayonOrbite, X_CENTRE_SYSTEME + rayonOrbite); //aleatoire de x
    y = sqrt(pow((double)rayonOrbite, 2.0) - pow((double)(x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME; //calcule de y pour ce x
    if(randInt(0, 1) == 1) {
    y = Y_CENTRE_SYSTEME - (y - Y_CENTRE_SYSTEME);
}
    SetSystemPlanetXY(systemeStellaire, numeroPlanete, x, y);

    SetSystemPlanetType(systemeStellaire, numeroPlanete, type);
    SetSystemPlanetRadius(systemeStellaire, numeroPlanete, taille);

	if(habitable == 1) {
        SetSystemPlanetName(systemeStellaire, numeroPlanete, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
	}
}

/**
 * generate the struct of the system
 */
static int GenerateSystemeStruct(int *galaxie, SystemeStellaire **systemeStellaires, int *barreDeChargement){//enregistrer matrice et generer hyperlane
	int largeur = 0;
	int hauteur = 0;
	int x = 0;
	int y = 0;
    int k = 0;

    int nombrePlanetes = 0;
    int nombrePlanetesHabitables = 0;
    int planeteHabitable[5] = 0;
    int nombrePlanetesHabitee = 0;
    int nombreHyperlanes = 0;
    int nomInt = 0;

    int planetIndex = 0;

    int hyperLane1 = 0;
    int hyperLane2 = 0;
    int hyperLane3 = 0;
    int hyperLaneSup1 = 0;
    int hyperLaneSup2 = 0;

    EtoileType etoile = 0;
    int trouNoir = 0;
	while(hauteur < LARGEUR_GALAXIE) {
		while(largeur < LARGEUR_GALAXIE * 2) {
			systemeStellaires[k] = AllocSystem();

			x = galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur];
			y = galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur + 1];

            SetSystemXY(systemeStellaires[k], x, y);
            
            CreateSystemStation(systemeStellaires[k]);
            
			etoile = randInt(1, 100);
			trouNoir = 0;

            SetSystemIntelLevel(systemeStellaires[k], INCONNU);
			if(etoile <= 10) {
				etoile = ETOILE_TYPE_B;
			} else if(etoile <= 30) {
				etoile = ETOILE_TYPE_A;
			} else if(etoile <= 44) {
				etoile = ETOILE_TYPE_F;
			} else if(etoile <= 57) {
				etoile = ETOILE_TYPE_G;
			} else if(etoile <= 70) {
				etoile = ETOILE_TYPE_K;
			} else if(etoile <= 85) {
				etoile = ETOILE_TYPE_M;
			} else if(etoile <= 90) {
				etoile = ETOILE_TYPE_TROU_NOIR;
				trouNoir = TRUE;
			} else if(etoile <= 95) {
				etoile = ETOILE_TYPE_PULSAR;
			} else {
				etoile = ETOILE_TYPE_ETOILE_A_NEUTRONS;
			}
            SetSystemStationLevel(systemeStellaires[k], AUCUNE);
			nombrePlanetes = 0;
			nombrePlanetes = randInt(1, 100);
			if(nombrePlanetes <= 8) {
				nombrePlanetes = 1;
			} else if(nombrePlanetes <= 20) {
				nombrePlanetes = 2;
			} else if(nombrePlanetes <= 50) {
				nombrePlanetes = 3;
			} else if(nombrePlanetes <= 80) {
				nombrePlanetes = 4;
			} else {
				nombrePlanetes = 5;
			}
			if(trouNoir == 1) {
				nombrePlanetes = 0;
			}
			SetSystemStarType(systemeStellaires[k], etoile);
            
            SetSystemPlanetNumber(systemeStellaires[k], 0);
			
            if((GetSystemStarType(systemeStellaires[k]) == ETOILE_TYPE_M) ||
            ((GetSystemStarType(systemeStellaires[k]) == ETOILE_TYPE_G) || 
            (GetSystemStarType(systemeStellaires[k]) == ETOILE_TYPE_K))){
				nombrePlanetesHabitables = randInt(0, nombrePlanetes);
                SetSystemPlanetHabitableNumber(systemeStellaires[k], nombrePlanetesHabitables);
			}
			nombreHyperlanes = randInt(1, 15);
			
			hyperLane1 = 1;
			hyperLane2 = 1;
			hyperLane3 = 1;
			
			if(nombreHyperlanes == 1)
			{
				do
				{
					hyperLaneSup1 = randInt(1, 2);
					hyperLaneSup2 = randInt(1, 2);
				} while(hyperLaneSup1 == hyperLaneSup2);
				switch(hyperLaneSup1)
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
				hyperLaneSup1 = randInt(1, 3);
				switch(hyperLaneSup1)
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
			
            SetHyperlaneDestination(systemeStellaires[k], 0, 255);
            SetHyperlaneDestination(systemeStellaires[k], 1, 255);
            SetHyperlaneDestination(systemeStellaires[k], 2, 255);
            SetHyperlaneDestination(systemeStellaires[k], 3, 255);
            
			if((GetSystemX(systemeStellaires[k - LARGEUR_GALAXIE]) != 0) && (hyperLane1)) {
				if((k - LARGEUR_GALAXIE > 0) && (k - LARGEUR_GALAXIE < 255))
            		SetHyperlaneDestination(systemeStellaires[k], 0, k - LARGEUR_GALAXIE);
			}
			
			if((GetSystemX(systemeStellaires[k - 1]) != 0) && (hyperLane2)) {
				if((k - 1 > 0) && (k - 1 < 255))
                	SetHyperlaneDestination(systemeStellaires[k], 1, k - 1);
			}
			
			nomInt = randInt(0, (sizeof(nomGalaxies)/sizeof(nomGalaxies[0])) - 1);
            SetSystemName(systemeStellaires[k], nomGalaxies[nomInt]);
			
			//gestion des planetes
			switch(nombrePlanetes){
			case 5:
				planeteHabitable[4] = randInt(1, 10);
			case 4:
				planeteHabitable[3] = randInt(1, 10);
			case 3:
				planeteHabitable[2] = randInt(1, 10);
			case 2:
				planeteHabitable[1] = randInt(1, 10);
			case 1:
				planeteHabitable[0] = randInt(1, 10);
			}

			planetIndex = 0;
			while(planetIndex < nombrePlanetes){
                CreateSystemPlanet(systemeStellaires[k], planetIndex);
				planetIndex++;
			}	

			planetIndex = 0;	
			while(planetIndex < nombrePlanetes){
				gfx_SetTextXY(50, 60);
				PrintInt(planetIndex);
				CreatePlanetSystem(systemeStellaires[k], planetIndex, randInt(1, 10));
				planetIndex++;
			}
			SetSystemPlanetNumber(systemeStellaires[k], nombrePlanetes);

			k++;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		hauteur++;
	}
	return k;
}

/**
 * Function to recreate the hyperlanes
 */
static void RecreateHyperlanes(SystemeStellaire **systemeStellaires, int *barreDeChargement, int k){
	int i = 0;
	int j = 0;
	//recreation des hyperlanes
	while(i < k) {
		SystemeStellaire *systeme;
		systeme = systemeStellaires[i];
		if(GetHyperlaneDestination(systeme, 0) != 255) {
			SetHyperlaneDestination(systemeStellaires[GetHyperlaneDestination(systeme, 0)], 2, i);
		}
		if(GetHyperlaneDestination(systeme, 1) != 255) {
			SetHyperlaneDestination(systemeStellaires[GetHyperlaneDestination(systeme, 1)], 3, i);
		}
		i++;
	}

	i = 0;
	j = 0;
	while(i < k){
		SystemeStellaire *systeme;
        int x = 0, y = 0;
		systeme = systemeStellaires[i];
		//calcul des positions de sortie
		j = 0;
		while(j < 4){
			if(GetHyperlaneDestination(systeme, j) != 255){
				if((GetSystemX(systemeStellaires[GetHyperlaneDestination(systeme, j)]) != 0) && (GetSystemY(systemeStellaires[GetHyperlaneDestination(systeme, j)]) != 0)){
					double angle = 0;

					angle = atan2(GetSystemY(systemeStellaires[GetHyperlaneDestination(systeme, j)]) - GetSystemY(systeme), GetSystemX(systemeStellaires[GetHyperlaneDestination(systeme, j)]) - GetSystemX(systeme));
					
					x = X_CENTRE_SYSTEME + ((RAYON_DE_VUE_SYSTEME + 5) * cos(angle));

					y = Y_CENTRE_SYSTEME + ((RAYON_DE_VUE_SYSTEME + 5) * sin(angle));
					SetHyperlaneXY(systeme, j, x, y);
				}
			}
			j++;
		}
		i++;
	}
}

/**
 * Function to create the galaxy
 */
static int ChargementNouvellePartieGalaxie(Parametres *parametres, ti_var_t *sauvegarde, SystemeStellaire **systemeStellaires){
	int i = 0, j = 0, espaceEntreEtoiles = 50, barreDeChargement = 1, etoile = 0, nombrePlanetes = 0, nombrePlanetesHabitables = 0, trouNoir = 0, fin = 0, nomInt = 0;
	int coefficientDeplacementStellaire = 100, coefficientX = 0, coefficientY = 0, rayon = ((espaceEntreEtoiles * LARGEUR_GALAXIE) - espaceEntreEtoiles) / 2 - 25, lane = 255, rayonInterieur = 50;
	int x = LIMITE_GAUCHE;
	int y = LIMITE_HAUT;
	int nombreHyperlanes = 0, hyperLane1 = 0, hyperLane2 = 0, hyperLane3 = 0, hyperlaneSup1, hyperlaneSup2;
	int planeteHabitable[5] = {0}, nombreAleatoire = 0;
	int planetIndex = 0;
    int k = LARGEUR_GALAXIE * LARGEUR_GALAXIE;
	Flotte* flotte = NULL;
	Planete *planete = NULL;

	int *galaxie = NULL;
	galaxie = malloc(LARGEUR_GALAXIE * LARGEUR_GALAXIE * 2 * sizeof(int));

	if(galaxie == NULL)
	{
		exit(EXIT_FAILURE);
	}

	gfx_SetDrawScreen();
	gfx_FillScreen(255);
	PrintCentered(_(LC_CREATE_GALAXIE), 120, 1, 0, 0);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(49, 159, 222, 7);
	gfx_SetColor(4);
	gfx_SetTextXY(50, 50);
	gfx_PrintString("1");
	CreateGalacticMatrix(galaxie, espaceEntreEtoiles, &barreDeChargement);

	gfx_PrintString("2");
	RandomGalacticMatrix(galaxie, coefficientDeplacementStellaire, &barreDeChargement);
	
	gfx_PrintString("3");
	RoundGalacticMatrix(galaxie, espaceEntreEtoiles, rayon, rayonInterieur, &barreDeChargement);

	gfx_PrintString("4");
	k = GenerateSystemeStruct(galaxie, systemeStellaires, &barreDeChargement);
	free(galaxie);
	
	gfx_PrintString("5");
	RecreateHyperlanes(systemeStellaires, &barreDeChargement, k);

	
	fin = 1;
	return 1;
}

/**
 * Creer Empires
 */
static void CreerEmpires(Parametres *parametres, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Camera *camera){
	int i = 0, fin = 1, j = 0;
	int k = LARGEUR_GALAXIE * LARGEUR_GALAXIE;
	int planete = 0;
	Empire *joueur = EmpireNumero(empireListe, 1);
	//creation joueur
	while(fin == 1) { // choix du systeme
		i = randInt(0, k - 1);
		gfx_SetTextXY(50, 70);
		PrintInt(i);
		if(((GetSystemX(systemeStellaires[i]) >= 160) && (GetSystemY(systemeStellaires[i]) >= 120)) && (GetSystemStarType(systemeStellaires[i]) != ETOILE_TYPE_TROU_NOIR))
			fin = 0;
	}
	SetEmpireColor(joueur, 9);
	SetSystemStarType(systemeStellaires[i], ETOILE_TYPE_K);
	SetSystemPlanetHabitableNumber(systemeStellaires[i], 1);
	SetSystemPlanetInhabitedNumber(systemeStellaires[i], 1);
	SetSystemEmpire(systemeStellaires[i], 1);
	SetSystemIntelLevel(systemeStellaires[i], TOTAL);

	SetSystemStationLevel(systemeStellaires[i], PORT_STELLAIRE);
	SetSystemStationModule(systemeStellaires[i], 0, CHANTIER_SPATIAL);
	SetSystemStationModule(systemeStellaires[i], 1, CARREFOUR_COMMERCIAL);
	
	EmpireNouvelleFlotte(joueur, i, FLOTTE_MILITAIRE, 3, 0, 0, 0);
	EmpireNouvelleFlotte(joueur, i, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);
	EmpireNouvelleFlotte(joueur, i, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);
	
	SetCameraX(camera, GetSystemX(systemeStellaires[i])); // centre la vue sur le systeme
	SetCameraY(camera, GetSystemY(systemeStellaires[i]));
	SetCameraSystem(camera, i);

	planete = randInt(0, GetSystemPlanetNumber(systemeStellaires[i]) - 1);

	SetSystemPlanetHabitability(systemeStellaires[i], planete, true);
	SetSystemPlanetType(systemeStellaires[i], planete, HABITABLE_CONTINENTAL);

	SetCameraXSystem(camera, GetSystemPlanetX(systemeStellaires[i], planete) - 160);
	SetCameraYSystem(camera, GetSystemPlanetY(systemeStellaires[i], planete) - 120);
	SetSystemPlanetName(systemeStellaires[i], planete, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);

	CreateSystemPlanetCity(systemeStellaires[i], planete);
	SetSystemPlanetCityPopulation(systemeStellaires[i], planete, 27);
	SetSystemPlanetCityDistrict(systemeStellaires[i], planete, 4, 3, 3, 3);

	CalculateSystemPlanetCityJob(systemeStellaires[i], planete);

	SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 1, CAPITALE, 3);
	SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 2, USINE_CIVILE, 1);
	SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 3, FONDERIE, 1);
	gfx_SetTextXY(90, 90);
	for(j = 2; j <= GetEmpireNumber(parametres); j++){
		Empire *empire = NULL;
		fin = 1;
		i = 0;
		while(fin == 1) { // choix du systeme
			i = randInt(0, k - 1);
			if(((GetSystemX(systemeStellaires[i]) >= 160) && (GetSystemY(systemeStellaires[i]) >= 120)) && (GetSystemStarType(systemeStellaires[i]) != ETOILE_TYPE_TROU_NOIR))
				fin = 0;
		}
		empire = EmpireAjouter(empireListe);
		CreerEmpireFlotte(empire);
		EmpireGenerateRandomName(empire);
		SetEmpireSpecies(empire, randInt(0, 3));
		SetEmpireClothes(empire, randInt(0, 2));

		gfx_PrintString("1 ");
		PrintInt(j);
		SetEmpireColor(empire, randInt(20, 29));
		SetSystemStarType(systemeStellaires[i], ETOILE_TYPE_K);
		SetSystemPlanetHabitableNumber(systemeStellaires[i], 1);
		SetSystemPlanetInhabitedNumber(systemeStellaires[i], 1);
		SetSystemEmpire(systemeStellaires[i], j);
		SetSystemIntelLevel(systemeStellaires[i], INCONNU);

		SetSystemStationLevel(systemeStellaires[i], PORT_STELLAIRE);
		SetSystemStationModule(systemeStellaires[i], 0, CHANTIER_SPATIAL);
		SetSystemStationModule(systemeStellaires[i], 1, CARREFOUR_COMMERCIAL);

		EmpireNouvelleFlotte(empire, i, FLOTTE_MILITAIRE, 3, 0, 0, 0);
		EmpireNouvelleFlotte(empire, i, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);
		EmpireNouvelleFlotte(empire, i, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);

		gfx_PrintString("2");
		planete = randInt(0, GetSystemPlanetNumber(systemeStellaires[i])- 1);

		SetSystemPlanetHabitability(systemeStellaires[i], planete, true);
		SetSystemPlanetType(systemeStellaires[i], planete, HABITABLE_CONTINENTAL);
		SetSystemPlanetName(systemeStellaires[i], planete, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);

		CreateSystemPlanetCity(systemeStellaires[i], planete);
		SetSystemPlanetCityPopulation(systemeStellaires[i], planete, 27);
		SetSystemPlanetCityDistrict(systemeStellaires[i], planete, 4, 3, 3, 3);

		CalculateSystemPlanetCityJob(systemeStellaires[i], planete);

		SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 1, CAPITALE, 3);
		SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 2, USINE_CIVILE, 1);
		SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 3, FONDERIE, 1);
		
		CalculateEmpireFleetPower(empire);

		gfx_PrintString("3");
	}
}

/* entry points ======================================================== */

/**
 * Message d'avertissement avant une nouvelle partie. 
 * Cela supprimera toute sauvegarde
 */
int NouvellePartieAvertissement(EmpireListe *empireListe, Parametres *parametres){
	char key = 0, choix = 1, fin = 1, nouvellePartie = 1;
	while(fin){
		do{
			gfx_FillScreen(255);
			PrintCentered("ATTENTION", 50, 2, 3, 0);
			PrintCentered("Cela supprimera", 80, 2, 0, 0);
			PrintCentered("toute sauvegarde", 100, 2, 0, 0);
			PrintCentered("Continuer?", 140, 2, 0, 0);
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
				PrintCentered("    non",180, 2, 0, 0);
				break;
			case 1:
				PrintCentered("oui    ", 180, 2, 0, 0);
				PrintCentered("    non", 180, 2, 4, 0);
				break;
			}
			gfx_SwapDraw();
		} while((key = os_GetCSC()) != sk_Enter);
		switch (choix){
		case 0: // si "non"
			while (nouvellePartie){
				fin = NouvellePartieEspece(empireListe, parametres);
				if (fin){
					nouvellePartie = QuitterNouvellePartieAvertissement();
				}else{
					return 0;
				}
			}
			return 1;
			break;
		case 1: // si "oui"
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
	Flotte* flotte = NULL;
	Empire* empire = NULL;
	Date *date = NULL;
	Camera *camera = NULL;
	Fenetre *fenetre = NULL;
	Marche *marche = NULL;
	SystemeStellaire *systemeStellaires[LARGEUR_GALAXIE * LARGEUR_GALAXIE];
	
	InitializeNewGame(&empireListe, &date, &camera, &fenetre, &marche, &parametres, &sauvegarde);

	fin = ChargementNouvellePartieGalaxie(parametres, &sauvegarde, systemeStellaires);
	
	gfx_PrintString("666");
	SetEmpireNumber(parametres, 4);
	CreerEmpires(parametres, empireListe, systemeStellaires, camera);
	
	gfx_PrintStringXY("1000102102003", 80, 80);

	PauseGame(date);

	gfx_SetDrawBuffer();
	StellarisBoucle(&sauvegarde, empireListe, parametres, date, systemeStellaires, camera, fenetre, marche);
	// StellarisBoucle(&sauvegarde, empireListe, parametres, date, systemeStellaires, camera, fenetre, marche);
}

/**
 *utilisé dans les menus de création de nouvelle partie pour afficher le texte
 */
void PrintText(const char *str, int x, int y, int taille, int color) {
	int a, longueur, i;
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
