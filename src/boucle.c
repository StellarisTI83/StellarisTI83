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
#include "sauvegarde.h"

#include "locale/locale.h"

/**********************************************Jeu principal**********************************************/
int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche)
{
	char finBoucle = 1, key = 0;
	while (finBoucle == 1)
	{
		/**********boucle du jeu**********/
		
		key = os_GetCSC();
		
		/**gere le temps**/
		StellarisTemps(empireListe, date, &key, systemeStellaires);
		
		//efface l'ecran
		gfx_ZeroScreen();

		/**dessine la map**/
		StellarisMap(empireListe, systemeStellaires, camera, &key, flotteJoueur, date, fenetre, joueur);

		
		/**dessine le hud**/
		finBoucle = StellarisHUD(empireListe, joueur, date, &key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche);
		
		gfx_SwapDraw();
		if((boot_CheckOnPressed()) || (finBoucle == 0))
		{
			StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			return 0;
		}
	}
	return 0;
}

/******************gerer le temps******************/
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires)
{
	//différentes actions des touches
	switch(*key)
	{
		case sk_0://pause / dépause
			if(date->vitesse == 0){
				date->vitesse = date->vitesseSauvegardee;
			}
			else{
				date->vitesseSauvegardee = date->vitesse;
				date->vitesse = 0;
			}
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
	if ((date->horloge == 0) && (date->vitesse !=0))
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
	if(((date->jour == 0) || (date->jour == 10)) || (date->jour == 20)){
		EffectuerActionsFlottes(empireListe, systemeStellaires);
	}
}

/******************dessiner le hud******************/
int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche)
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
	int16_t systeme = 0;
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
	if (((camera->systemeSelectione != -1) || (camera->mapType == CARTE)) || (camera->mapType == SYSTEME))
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
	if (((camera->systemeSelectione != -1) || (camera->mapType == CARTE)) || (camera->mapType == SYSTEME))
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
	PrintHUD(camera->xSysteme, 0, 40, 13);
	gfx_TransparentSprite_NoClip(minerai, 100 ,1);
	PrintHUD(camera->ySysteme, 0, 85, 13);
	gfx_TransparentSprite_NoClip(food, 145 ,1);
	PrintHUD(joueur->nourriture, 0, 130, 13);
	gfx_TransparentSprite_NoClip(fer, 190 ,1);
	PrintHUD(joueur->acier, 0, 175, 13);
	
	//barre de gauche
	gfx_TransparentSprite_NoClip(contact, 5, 50);
	gfx_TransparentSprite_NoClip(market_icon, 5, 70);
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
	if (camera->mapType == CARTE) {
		gfx_Line_NoClip(camera->x / 2.5 + 5, camera->y / 2.5 - 30, camera->x / 2.5 + 15, camera->y / 2.5 - 30);
		gfx_Line_NoClip(camera->x / 2.5 + 10, camera->y / 2.5 - 25, camera->x / 2.5 + 10, camera->y / 2.5 - 35);
		gfx_Circle_NoClip(camera->x / 2.5 + 10, camera->y / 2.5 - 30, 3);
	}
	else {
		gfx_Line_NoClip(150, 120, 170, 120);
		gfx_Line_NoClip(160, 110, 160, 130);
		gfx_FillCircle_NoClip(160, 120, 2);
		gfx_Circle_NoClip(160, 120, 4);
		gfx_Circle_NoClip(160, 120, 8);
	}

	//nom galaxie
	if ((camera->systemeSelectione != -1) || (camera->systeme != -1)) {
		if (camera->mapType == NORMAL) {
			systeme = camera->systemeSelectione;
		}
		else if(camera->mapType == SYSTEME){
			systeme = camera->systeme;
		}
		if (systemeStellaires[systeme].niveauDeConnaissance == INCONNU) {
			gfx_PrintStringXY("Inconnu", 132, 211);
		}
		else {
			gfx_PrintStringXY(systemeStellaires[systeme].nom, 160 - strlen(systemeStellaires[systeme].nom) * 4, 211);
		}
	}
	
	if (camera->mapType == CARTE) {
		gfx_PrintStringXY("Carte", 140, 211);
	}	
	return Menus(empireListe, joueur, date, key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche);
}

/**
 *Exprime à l'écran les données envoyées pour la abrre du haut
 */
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

/**
 *Fonction gérant les differents menus in-game
 */
int Menus(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t* sauvegarde, Marche *marche){
	char niveau = 0, i = 0;
	int8_t compteur = 0, compteurFlotte = 0, maximum = 0;
	int8_t population[5];
	int16_t systeme = 0;
	Flotte* flotte = NULL;
	Flotte* flotteDuSysteme[20];
	memset(flotteDuSysteme, 0, sizeof(Flotte*) * 20);

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
					StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
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
			case MENU_SYSTEME_FLOTTES: //liste flottes
				switch(*key)
				{
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						fenetre->ouverte = FALSE;
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
				gfx_SetColor(7);
				gfx_Rectangle_NoClip(40, 40, 240, 160);
				gfx_HorizLine_NoClip(45, 51, 230);
				gfx_PrintStringXY("Vue flottes", 116, 42);
				niveau = 57;
				flotte = joueur->flotte->premier;
				compteur = 0;
				compteurFlotte = 1;
				while(flotte != NULL){
					if(flotte->systeme == camera->systeme) {
						flotteDuSysteme[compteur] = flotte;
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
				flotte = flotteDuSysteme[0];
				i = 0;
				while(flotte != NULL) {
					if(fenetre->selection == (i + 1)) {
						gfx_SetTextFGColor(13);
					}
					else {
						gfx_SetTextFGColor(1);
					}
					switch(flotte->type){
						case FLOTTE_MILITAIRE:
							gfx_PrintStringXY("militaire", 203, niveau);
							break;
						case FLOTTE_DE_CONSTRUCTION:
							gfx_PrintStringXY("construction", 179, niveau);
							break;
						case FLOTTE_SCIENTIFIQUE:
							gfx_PrintStringXY("scientifique", 179, niveau);
							break;
					}
					gfx_SetTextXY(45, niveau);
					gfx_PrintString("Flotte ");
					PrintInt(FlotteNumeroRecuperer(joueur->flotte, flotteDuSysteme[i]) + 1);
					gfx_HorizLine_NoClip(50, niveau + 11, 220);
					niveau += 17;
					i++;
					compteurFlotte++;
					flotte = flotteDuSysteme[i];
				}
				if (*key == sk_Enter) {
					*key = 0;
					fenetre->ouverte = 6;
					fenetre->flotteSelectionee = FlotteNumeroRecuperer(joueur->flotte, flotteDuSysteme[fenetre->selection - 1]) + 1;
					fenetre->selection = 1;
					fenetre->precedente = 1;
				}
				break;
				
			case MENU_SYSTEME_ETOILE: //etoile
				switch(*key) {
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						fenetre->ouverte = FALSE;
						camera->bloque = FALSE;
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
				gfx_SetTextXY(158 - strlen(systemeStellaires[camera->systeme].nom)*4, 42);
				gfx_PrintString("Proxima ");
				gfx_PrintString(systemeStellaires[camera->systeme].nom);
				gfx_SetTextXY(160, 62);
				gfx_PrintString("Classe : ");
				switch(systemeStellaires[camera->systeme].etoileType)
				{
					case ETOILE_TYPE_B:
						gfx_PrintString("B");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 45);				
						gfx_SetColor(15);
						gfx_Circle_NoClip(100, 111, 45);
						break;
					case ETOILE_TYPE_A:
						gfx_PrintString("A");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 40);				
						gfx_SetColor(14);
						gfx_Circle_NoClip(100, 111, 40);
						break;
					case ETOILE_TYPE_F:
						gfx_PrintString("F");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 35);				
						break;
					case ETOILE_TYPE_G:
						gfx_PrintString("G");
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(100, 111, 32);				
						gfx_SetColor(13);
						gfx_Circle_NoClip(100, 111, 32);
						break;
					case ETOILE_TYPE_K:
						gfx_PrintString("K");
						gfx_SetColor(13);
						gfx_FillCircle_NoClip(100, 111, 30);				
						gfx_SetColor(9);
						gfx_Circle_NoClip(100, 111, 30);
						break;
					case ETOILE_TYPE_M:
						gfx_PrintString("M");
						gfx_SetColor(9);
						gfx_FillCircle_NoClip(100, 111, 10);				
						gfx_SetColor(13);
						gfx_Circle_NoClip(100, 111, 10);
						break;
					case ETOILE_TYPE_TROU_NOIR:
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
					case ETOILE_TYPE_PULSAR:
						gfx_PrintString("pulsar");
						gfx_SetColor(14);
						gfx_FillCircle_NoClip(100, 111, 4);
						gfx_Line_NoClip(90, 101, 110, 121);
						gfx_SetColor(1);
						gfx_Circle_NoClip(100, 111, 4);
						break;
					case ETOILE_TYPE_ETOILE_A_NEUTRONS:
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
				
			case MENU_SYSTEME_PLANETE_RESUME: //planete resume
				switch(*key)
				{
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						fenetre->ouverte = FALSE;
						camera->bloque = FALSE;
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
					case 1:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete1->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete1->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" I");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete1->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 2:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete2->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete2->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" II");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete2->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 3:
						gfx_SetTextXY(178 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete3->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete3->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" III");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete3->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 4:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete4->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete4->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" IV");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete4->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
						
					case 5:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete5->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete5->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" V");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete5->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
				}
				break;
				
			case MENU_SYSTEME_PLANETE_POPULATION: //planete poplation
				switch(*key)
				{
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						fenetre->ouverte = FALSE;
						camera->bloque = FALSE;
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
					case 1:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete1->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete1->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" I");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete1->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 2:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete2->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete2->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" II");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete2->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 3:
						gfx_SetTextXY(178 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete3->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete3->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" III");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete3->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 4:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete4->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete4->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" IV");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete4->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
						
					case 5:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete5->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete5->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" V");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete5->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
				}
				break;
				
			case MENU_SYSTEME_PLANETE_ARMEE: //planete armee
				switch(*key)
				{
					case sk_Clear:
						camera->fenetre = MENU_AUCUN;
						fenetre->ouverte = FALSE;
						camera->bloque = FALSE;
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
					case 1:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete1->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete1->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" I");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete1->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 2:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete2->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete2->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" II");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete2->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 3:
						gfx_SetTextXY(178 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete3->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete3->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" III");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete3->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
					case 4:
						gfx_SetTextXY(182 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete4->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete4->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" IV");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete4->population);
						gfx_PrintStringXY(population, 150, 86);
						break;
						
					case 5:
						gfx_SetTextXY(186 - strlen(systemeStellaires[camera->systeme].nom), 42);
						if(systemeStellaires[camera->systeme].planete5->habitable == 1){
							gfx_PrintString(systemeStellaires[camera->systeme].planete5->nom);
							gfx_SetTextFGColor(19);
							gfx_PrintStringXY("Habitable", 150, 62);
						}
						else{
							gfx_PrintString(systemeStellaires[camera->systeme].nom);
							gfx_PrintString(" V");
							gfx_SetTextFGColor(3);
							gfx_PrintStringXY("Non-habitable", 150, 62);
						}
						gfx_SetTextFGColor(1);
						gfx_PrintStringXY("Population", 150, 74);
						sprintf(population, "%d", systemeStellaires[camera->systeme].planete5->population);
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
				gfx_SetTextFGColor(34);
				gfx_SetTextXY(50, niveau);
				gfx_TransparentSprite_NoClip(life_icon, 45, niveau + 1);
				PrintInt((flotte->coqueVie * 100) / flotte->coqueTotal);
				gfx_PrintString("%");
				gfx_SetTextXY(170, niveau);
				gfx_TransparentSprite_NoClip(blindage_icon, 45, niveau + 1);
				PrintInt((flotte->blindageVie * 100) / flotte->blindageTotal);
				gfx_PrintString("%");
				niveau += 14;
				gfx_SetTextXY(50, niveau);
				gfx_TransparentSprite_NoClip(bouclier_icon, 45, niveau);
				PrintInt((flotte->bouclierVie * 100) / flotte->bouclierTotal);
				gfx_PrintString("%");
				if(flotte->type == FLOTTE_MILITAIRE){
					gfx_SetTextFGColor(18);
					gfx_SetTextXY(170, niveau);
					gfx_TransparentSprite_NoClip(fleet_power_icon, 165, niveau + 2);
					PrintInt(flotte->puissance);
				}
				gfx_SetTextFGColor(1);
				
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
						gfx_PrintString("Se d/place vers ");
						gfx_SetTextFGColor(13);
						gfx_PrintString(systemeStellaires[flotte->systemeArrive].nom);
						break;
					case FLOTTE_ATTAQUER:
						gfx_PrintString("Attaque");
						break;
				}
				if(*key == sk_Enter){ //effectuer l'action
					*key = 0;
					if(fenetre->selection == FLOTTE_BOUGER){
						FlotteBouger(fenetre->flotteSelectionee, 1, 0, camera, empireListe, systemeStellaires);
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
					gfx_PrintStringXY("militaire", 203, niveau);
					break;
				case FLOTTE_DE_CONSTRUCTION:
					gfx_PrintStringXY("construction", 179, niveau);
					break;
				case FLOTTE_SCIENTIFIQUE:
					gfx_PrintStringXY("scientifique", 179, niveau);
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
			*key = 0;
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