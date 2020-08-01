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
#include "flottes.h"

#include "locale/locale.h"


/**
 *fonction principale pour dessiner la map
 *
 *contient aussi la gestion des évenements
 */
void StellarisMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur){
	char key2 = 0;
	gfx_SetColor(1);
	if(camera->mapType == NORMAL)
	{
		key2 = 0;
		if((camera->fenetre == MENU_AUCUN) && (fenetre->commandPrompt == false))
		{
			key2 = os_GetCSC();
		}
		//gere les événements des touches
		if(fenetre->commandPrompt == false){
			KeyActionNormalMap(empireListe, systemeStellaires, camera, key, flotteJoueur, date, fenetre, joueur);
			KeyActionNormalMap(empireListe, systemeStellaires, camera, &key2, flotteJoueur, date, fenetre, joueur);
		}

		if (camera->zoom < 1)
		{
			camera->zoom = 1;
			camera->mapType = CARTE;
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

		//fait en sorte que la camera sorte pas des limites		
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
		DessinerVueMap(systemeStellaires, camera, empireListe);
		DessinerFlottesMap(empireListe, joueur, systemeStellaires, camera);
	}
	else if(camera->mapType == CARTE){
		switch(*key){
			case sk_Del:
				camera->mapType = NORMAL; //dezoomer
				break;
		}
		DessinerVueMap(systemeStellaires, camera, empireListe);
	}
	else if(camera->mapType == SYSTEME){
		if(camera->bloque == FALSE){
			switch(*key){
				case sk_Clear:
					camera->mapType = NORMAL; //dezoomer
					camera->vecteurx = 0;
					camera->vecteury = 0;
					break;
				case sk_Up:
					camera->vecteury -= 4;
					break;
				case sk_Down:
					camera->vecteury += 4;
					break;
				case sk_Left:
					camera->vecteurx -= 4;
					break;
				case sk_Right:
					camera->vecteurx += 4;
					break;
			}
		}

		if(camera->vecteury != 0)
		{
			camera->ySysteme += camera->vecteury;
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
			camera->xSysteme += camera->vecteurx;
			if(camera->vecteurx > 0)
			{
				camera->vecteurx--;
			}
			else if(camera->vecteurx < 0)
			{
				camera->vecteurx++;
			}
		}
		if(fenetre->commandPrompt == false){
			KeyActionNormal(empireListe, systemeStellaires, camera, key, flotteJoueur, date, fenetre, joueur);
		}


		//fait en sorte que la camera sorte pas des limites
		while(pow((double)(camera->xSysteme - 320), 2.0) + pow((double)(camera->ySysteme - 240), 2.0) > pow((double)RAYON_DE_VUE_SYSTEME, 2.0)){
			camera->vecteurx = 0;
			camera->vecteury = 0;
			if(camera->xSysteme - 320 > 0){
				camera->xSysteme--;
			}
			if(camera->xSysteme - 320 < 0){
				camera->xSysteme++;
			}
			if(camera->ySysteme - 240 > 0){
				camera->ySysteme--;
			}
			if(camera->ySysteme - 240 < 0){
				camera->ySysteme++;
			}
		}
		DessinerVueSysteme(systemeStellaires, camera, fenetre, empireListe, key);
	}
}


/**
 *dessiner la map en vision normale et galactique
 */
void DessinerVueMap(SystemeStellaire* systemeStellaires, Camera* camera, EmpireListe* empireListe){
	int i = 0, x = 0, y = 0, xLn, yLn, hyperLane1 = 0, hyperLane2 = 0, hyperLane3 = 0, hyperLane4 = 0, systeme = 0;
	//dessiner carte
	gfx_SetTextConfig(gfx_text_clip);
	camera->systemeSelectione = -1;
	while (i < (LARGEUR_GALAXIE * LARGEUR_GALAXIE) - 1)
	{
		hyperLane1 = 255;
		hyperLane2 = 255;
		hyperLane3 = 255;
		
		xLn = 0;
		yLn = 0;
		if(camera->mapType == NORMAL)
		{
			x = systemeStellaires[i].x * camera->zoom - camera->x + 160;
			y = systemeStellaires[i].y * camera->zoom - camera->y + 120;
		}
		else if(camera->mapType == CARTE)
		{
			x = systemeStellaires[i].x / 2.5 + 10;
			y = systemeStellaires[i].y / 2.5 - 30;
		}
		if ( (((0 <= x) && (x <= 320)) && ((0 <= y)&& (y <= 240))) && ((systemeStellaires[i].x != 0) && (systemeStellaires[i].y != 0)) )
		{
			if(systemeStellaires[i].niveauDeConnaissance != INCONNU) {
				if(systemeStellaires[i].empire == 1){
					if(camera->mapType == NORMAL){
						gfx_SetColor(9);
						gfx_Circle(x, y, 20 * camera->zoom);
						gfx_Circle(x, y, (19 * camera->zoom) + 1);
						DessinerPlanetesHabitables(systemeStellaires, i, x, y);
					}
					else {
						gfx_SetColor(9);
						gfx_Circle(x, y, 10);
						gfx_Circle(x, y, 9);
					}
				}
				//dessiner hyperLanes
				hyperLane1 = systemeStellaires[i].hyperlane1;
				if (hyperLane1 != 255){
					xLn = systemeStellaires[hyperLane1].x;
					yLn = systemeStellaires[hyperLane1].y;
					DessinerHyperlane(systemeStellaires[hyperLane1].niveauDeConnaissance, systemeStellaires[i].niveauDeConnaissance, x, y, xLn, yLn, camera);
				}
				
				hyperLane2 = systemeStellaires[i].hyperlane2;
				if (hyperLane2 != 255){
					xLn = systemeStellaires[hyperLane2].x;
					yLn = systemeStellaires[hyperLane2].y;
					DessinerHyperlane(systemeStellaires[hyperLane2].niveauDeConnaissance, systemeStellaires[i].niveauDeConnaissance, x, y, xLn, yLn, camera);
				}

				hyperLane3 = systemeStellaires[i].hyperlane3;
				if (hyperLane3 != 255){
					xLn = systemeStellaires[hyperLane3].x;
					yLn = systemeStellaires[hyperLane3].y;
					DessinerHyperlane(systemeStellaires[hyperLane3].niveauDeConnaissance, systemeStellaires[i].niveauDeConnaissance, x, y, xLn, yLn, camera);
				}
				
				hyperLane4 = systemeStellaires[i].hyperlane4;
				if (hyperLane4 != 255){
					xLn = systemeStellaires[hyperLane4].x;
					yLn = systemeStellaires[hyperLane4].y;
					DessinerHyperlane(systemeStellaires[hyperLane4].niveauDeConnaissance, systemeStellaires[i].niveauDeConnaissance, x, y, xLn, yLn, camera);
				}
			}
			// ecrit le numéro de l'étoile
			// gfx_SetTextXY(x - 4, y + 9);
			// PrintInt(i);

			
			if(camera->mapType == NORMAL){
				switch(systemeStellaires[i].etoileType){
					case ETOILE_TYPE_B: //B
						gfx_SetColor(15);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						break;
					case ETOILE_TYPE_A: //A
						gfx_SetColor(14);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case ETOILE_TYPE_F: //F
						gfx_SetColor(1);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case ETOILE_TYPE_G: //G
						gfx_SetColor(1);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case ETOILE_TYPE_K: //K
						gfx_SetColor(13);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						break;
					case ETOILE_TYPE_M: //M
						gfx_SetColor(13);
						gfx_FillCircle(x, y, 2 * camera->zoom);
						break;
					case ETOILE_TYPE_TROU_NOIR: //trou noir
						gfx_SetColor(13);
						gfx_Circle(x, y, 2 * camera->zoom);
						break;
					case ETOILE_TYPE_PULSAR: //pulsar
						gfx_SetColor(14);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						break;
					case ETOILE_TYPE_ETOILE_A_NEUTRONS: ///toile a neutrons
						gfx_SetColor(14);
						gfx_FillCircle(x, y, 1 * camera->zoom);
						gfx_Circle(x, y, 3 * camera->zoom);
						break;
				}
			}
			else{
				switch(systemeStellaires[i].etoileType){
					case ETOILE_TYPE_B: //B
						gfx_SetColor(15);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_A: //A
						gfx_SetColor(14);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_F: //F
						gfx_SetColor(1);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_G: //G
						gfx_SetColor(1);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_K: //K
						gfx_SetColor(13);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_M: //M
						gfx_SetColor(13);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_TROU_NOIR: //trou noir
						gfx_SetColor(13);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_PULSAR: //pulsar
						gfx_SetColor(14);
						gfx_Circle_NoClip(x, y, 1);
						break;
					case ETOILE_TYPE_ETOILE_A_NEUTRONS: ///toile a neutrons
						gfx_SetColor(14);
						gfx_Circle_NoClip(x, y, 1);
						break;
				}
			}
		}

		//Le if suivant teste si la systeme est celui selectionné
		if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y)))
		{
			gfx_SetColor(9);
			gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
			camera->systemeSelectione = i;
		}
		i++;
	}
	gfx_SetTextConfig(gfx_text_noclip);
}

/**
 *dessine à l'écran quand une planete est habitable en mode carte normale
 */
void DessinerPlanetesHabitables(SystemeStellaire* systemeStellaires, int i, int x, int y){
	char nombrePlanetesHabitablesSysteme = 0, nombrePlanetesHabiteesSysteme = 0;
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
		if(systemeStellaires[i].niveauDeConnaissance >= FAIBLE) {
			
			if(systemeStellaires[i].niveauDeConnaissance == ELEVEE) {
				gfx_SetTextFGColor(1);
			}
			else {
				gfx_SetTextFGColor(11);
			}
			gfx_PrintStringXY(systemeStellaires[i].nom, x - (strlen(systemeStellaires[i].nom) * 4), y + 9);
		}
	}
}

/**
 *dessine les flottes sur la map
 */
void DessinerFlottesMap(EmpireListe* empireListe, Empire* joueur, SystemeStellaire* systemeStellaires, Camera* camera){
	Flotte* flotte;
	int systeme, xFlotte, yFlotte;
	int index = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	//dessiner flottes
	flotte = joueur->flotte->premier;
	while(flotte != NULL) {
		systeme = flotte->systeme;
		xFlotte = systemeStellaires[systeme].x*camera->zoom - camera->x + 165;
		yFlotte = systemeStellaires[systeme].y*camera->zoom - camera->y + 110;
		if(((xFlotte > 0) && (xFlotte < 320)) && ((0 < yFlotte) && (yFlotte < 240))){
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
		//dessiner chemin
		index = flotte->avancementTrajet - 1;
		gfx_SetColor(13);
		while((flotte->chemin[index] != 0) && (flotte->chemin[index + 1] != 0)){
			x1 = systemeStellaires[flotte->chemin[index]].x*camera->zoom - camera->x + 160;
			y1 = systemeStellaires[flotte->chemin[index]].y*camera->zoom - camera->y + 120;
			x2 = systemeStellaires[flotte->chemin[index + 1]].x*camera->zoom - camera->x + 160;
			y2 = systemeStellaires[flotte->chemin[index + 1]].y*camera->zoom - camera->y + 120;
			if((((x1 > 0) && (x1 < 320)) && ((0 < y1) && (y1 < 240))) && (((x2 > 0) && (x2 < 320)) && ((0 < y2) && (y2 < 240)))){
				gfx_Line_NoClip(x1, y1, x2, y2);
			}
			index++;
		}
		flotte = flotte->suivant;
	}
}

/**
 *Dessine une hyperlane
 */
void DessinerHyperlane(int8_t niveauDeConnaissance1, int8_t niveauDeConnaissance2, int16_t x, int16_t y, int16_t xLn, int16_t yLn, Camera* camera){
	if((niveauDeConnaissance1 == INCONNU) || (niveauDeConnaissance2 == INCONNU)) {
		gfx_SetColor(11);
	}
	else {
		gfx_SetColor(12);
	}
	
	if(camera->mapType == NORMAL)
	{
		xLn = xLn*camera->zoom - camera->x + 160;
		yLn = yLn*camera->zoom - camera->y + 120;
	}
	else 
	{
		xLn = xLn / 2.5 + 10;
		yLn = yLn / 2.5 - 30;
	}
	if ( (((0 <= xLn) && (xLn <= 320)) && ((0 <= yLn) && (yLn <= 240))) && (((0 <= x) && (x <= 320)) && ((0 <= y) && (y <= 240))) )
	{
		gfx_Line_NoClip(x, y, xLn, yLn);
	}
}

void KeyActionNormal(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur){
	switch(*key){
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
		case sk_2nd:
			fenetre->commandPrompt = true;
			date->vitesse = 0;
			camera->bloque = true;
			*key = 0;
			break;
	}
}

/**
 *Gère les actions des touches pour la boucle StellarisMap
 */
void KeyActionNormalMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur){
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
			if (((camera->fenetre == MENU_AUCUN) && (camera->systemeSelectione != -1)) && ((camera->bougerFlotte == FALSE) && (systemeStellaires[camera->systemeSelectione].niveauDeConnaissance != INCONNU)))
			{
				camera->bloque = FALSE;
				camera->mapType = SYSTEME;
				if(camera->systeme != camera->systemeSelectione){
					camera->xSysteme = 320;
					camera->ySysteme = 240;
				}
				camera->systeme = camera->systemeSelectione;
			}
			else if(((camera->fenetre == MENU_AUCUN) && (camera->systemeSelectione != -1)) && (camera->bougerFlotte == TRUE)){
				FlotteBouger(camera->flotte, camera->empire, camera->systemeSelectione, camera, empireListe, systemeStellaires);
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
	}
	KeyActionNormal(empireListe, systemeStellaires, camera, key, flotteJoueur, date, fenetre, joueur);
}


/**
 *dessine le systeme en vision systeme
 */
void DessinerVueSysteme(SystemeStellaire* systemeStellaires, Camera* camera, Fenetre *fenetre, EmpireListe* empireListe, char *key){
	SystemeStellaire* systeme;
	systeme = &systemeStellaires[camera->systeme];
	DessinerEtoile(systeme, camera, fenetre, key);
	DessinerBase(systeme, camera, fenetre, key);
	DessinerFlottesSysteme(empireListe, camera, fenetre, key);
	switch(systeme->nombrePlanetes){
		case 5:
			DessinerPlanete(systeme, systeme->planete5, camera, fenetre, 5, key);
		case 4:
			DessinerPlanete(systeme, systeme->planete4, camera, fenetre, 4, key);
		case 3:
			DessinerPlanete(systeme, systeme->planete3, camera, fenetre, 3, key);
		case 2:
			DessinerPlanete(systeme, systeme->planete2, camera, fenetre, 2, key);
		case 1:
			DessinerPlanete(systeme, systeme->planete1, camera, fenetre, 1, key);
	}
}


/**
 *Dessine l'étoile en vue systeme
 */
void DessinerEtoile(SystemeStellaire* systeme, Camera* camera, Fenetre *fenetre, char* key){
	int xEtoile = X_CENTRE_SYSTEME - camera->xSysteme, yEtoile = Y_CENTRE_SYSTEME - camera->ySysteme, rayon = 0;
	switch(systeme->etoileType){
		case ETOILE_TYPE_B:
			rayon = 6;
			break;
		case ETOILE_TYPE_A:
			rayon = 5;
			break;
		case ETOILE_TYPE_F:
			rayon = 4;
			break;
		case ETOILE_TYPE_G:
			rayon = 3;
			break;
		case ETOILE_TYPE_K:
			rayon = 2;
			break;
		case ETOILE_TYPE_M:
			rayon = 1;
			break;
		case ETOILE_TYPE_TROU_NOIR:
			rayon = 2;
			break;
		case ETOILE_TYPE_PULSAR:
			rayon = 3;
			break;
		case ETOILE_TYPE_ETOILE_A_NEUTRONS:
			rayon = 4;
			break;
	}
	if(systeme->etoileType < 6){
		CouleurEtoile(systeme->etoileType);
		gfx_FillCircle(xEtoile, yEtoile, rayon);
	}
	else{
		switch(systeme->etoileType){
			case ETOILE_TYPE_TROU_NOIR: //trou noir
				gfx_SetColor(13);
				gfx_Circle(xEtoile, yEtoile, 4);
				break;
			case ETOILE_TYPE_PULSAR: //pulsar
				gfx_SetColor(14);
				gfx_FillCircle(xEtoile, yEtoile, 1);
				break;
			case ETOILE_TYPE_ETOILE_A_NEUTRONS: ///toile a neutrons
				gfx_SetColor(14);
				gfx_FillCircle(xEtoile, yEtoile, 1);
				gfx_Circle(xEtoile, yEtoile, 8);
				break;
		}
	}
	if(((150 <= xEtoile) && (170 >= xEtoile)) && ((110 <= yEtoile) && (130 >= yEtoile)))
	{
		gfx_SetColor(9);
		gfx_Rectangle_NoClip(xEtoile - 8, yEtoile - 8, 16, 16);
		if((*key == sk_Enter) && (camera->bloque == false)){
			camera->fenetre = MENU_SYSTEME;
			fenetre->ouverte = MENU_SYSTEME_ETOILE;
			camera->bloque = TRUE;
			*key = 0;
		}
	}
}

/**
 *set la couleur pour une étoile
 */
void CouleurEtoile(int type){	
	//dessiner etoile
	switch(type)
	{
		case ETOILE_TYPE_B: //B
			gfx_SetColor(15);
			break;
		case ETOILE_TYPE_A: //A
			gfx_SetColor(14);
			break;
		case ETOILE_TYPE_F: //F
			gfx_SetColor(1);
			break;
		case ETOILE_TYPE_G: //G
			gfx_SetColor(1);
			break;
		case ETOILE_TYPE_K: //K
			gfx_SetColor(13);
			break;
		case ETOILE_TYPE_M: //M
			gfx_SetColor(13);
			break;
	}
}

/**
 *Dessine une planète pour le menu systeme de la fonction StellarisHUD
 */
void DessinerPlanete(SystemeStellaire* systeme, Planete* planete, Camera* camera, Fenetre *fenetre, int numero, char* key){
	int x, y;
	char nomNumero[10], decalage = 0;
	x = planete->x - camera->xSysteme;
	y = planete->y - camera->ySysteme;
	gfx_SetColor(11);
	gfx_Circle(X_CENTRE_SYSTEME - camera->xSysteme, Y_CENTRE_SYSTEME - camera->ySysteme, planete->rayonOrbite);
	CouleurPlanete(planete->type);
	gfx_FillCircle(x, y, planete->taille);
	gfx_SetTextFGColor(1);
	if(((50 < x) && (x < 310)) && ((10 < y) && (y < 220))){
		if(planete->population > 0){
			gfx_SetColor(6);
			gfx_FillRectangle(x - strlen(planete->nom) * 4 - 2, y + 8, strlen(planete->nom) * 8 + 4, 10);
			gfx_SetColor(7);
			gfx_Rectangle(x - strlen(planete->nom) * 4 - 2, y + 8, strlen(planete->nom) * 8 + 4, 10);
			gfx_PrintStringXY(planete->nom, x - (strlen(planete->nom) * 4), y + 9);
		}
		else{
			switch(numero){
				case 1:
					decalage = 8;
					strcpy(nomNumero, " I");
					break;
				case 2:
					decalage = 12;
					strcpy(nomNumero, " II");
					break;
				case 3:
					decalage = 16;
					strcpy(nomNumero, " III");
					break;
				case 4:
					decalage = 12;
					strcpy(nomNumero, " IV");
					break;
				case 5:
					decalage = 8;
					strcpy(nomNumero, " V");
					break;
			}
			gfx_SetColor(0);
			gfx_FillRectangle(x - strlen(systeme->nom) * 4 - 2 - decalage, y + 8, strlen(systeme->nom) * 8 + 4 + (decalage * 2), 10);
			
			gfx_SetTextXY(x - (strlen(systeme->nom) * 4) - decalage, y + 9);
			gfx_PrintString(systeme->nom);
			gfx_PrintString(nomNumero);
		}
	}

	if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y)))
	{
		gfx_SetColor(9);
		gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
		camera->selection = numero;
		if((*key == sk_Enter) && (camera->bloque == false)){
			camera->fenetre = MENU_SYSTEME;
			fenetre->ouverte = MENU_SYSTEME_PLANETE_RESUME;
			fenetre->planete = numero;
			fenetre->selection = 1;
			camera->bloque = TRUE;
			*key = 0;
		}
	}
}

/**
 *Set la bonne couleur suivant le type de la planète
 */
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

/**
 *Dessine la base du systeme
 */
void DessinerBase(SystemeStellaire* systeme, Camera* camera, Fenetre* fenetre, char* key){
	int x, y;
	if(systeme->station->niveauStation != AUCUNE){
		gfx_SetColor(1);
		x = 465 - camera->xSysteme;
		y = 345 - camera->ySysteme;
		
		//verifie que la base soit dans l'écran
		if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
			switch(systeme->station->niveauStation){ //dessine la station
				case AVANT_POSTE:
					gfx_SetPixel(x, y);
					break;
				case PORT_STELLAIRE:
					gfx_Line_NoClip(x - 1, y - 1, x + 1, y + 1);
					break;
				case REDOUTE_STELLAIRE:
					gfx_Line_NoClip(x - 1, y - 1, x + 1, y + 1);
					gfx_Line_NoClip(x - 1, y + 1, x + 1, y - 1);
					break;
				case FORTERESSE_STELLAIRE:
					gfx_Line_NoClip(x - 2, y, x, y - 2);
					gfx_Line_NoClip(x - 2, y, x, y + 2);
					gfx_Line_NoClip(x + 2, y, x, y - 2);
					gfx_Line_NoClip(x + 2, y, x, y + 2);
					gfx_SetPixel(x, y);
					break;
				case CITADELLE:
					gfx_Line_NoClip(x - 2, y, x, y - 2);
					gfx_Line_NoClip(x - 2, y, x, y + 2);
					gfx_Line_NoClip(x + 2, y, x, y - 2);
					gfx_Line_NoClip(x + 2, y, x, y + 2);
					gfx_SetPixel(x, y);
					gfx_SetPixel(x - 2, y - 2);//coins
					gfx_SetPixel(x - 2, y + 2);
					gfx_SetPixel(x + 2, y - 2);
					gfx_SetPixel(x + 2, y + 2);
					break;
			}

			if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (camera->bloque == FALSE))))
			{
				gfx_SetColor(9);
				gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
				if((*key == sk_Enter) && (camera->bloque == false)){
					camera->fenetre = MENU_SYSTEME;
					camera->bloque = TRUE;
					fenetre->ouverte = MENU_SYSTEME_STATION_RESUME;
					fenetre->selection = 1;
					*key = 0;
				}
			}
		}
	}
}

/**
 *Dessine les flottes du systeme
 */
void DessinerFlottesSysteme(EmpireListe *empireListe, Camera *camera, Fenetre *fenetre, char* key){
	Empire* empire = NULL;
	Flotte* flotte = NULL;
	int x, y;
	gfx_SetColor(11);
	empire = empireListe->premier;
	while(empire != NULL){
		flotte = empire->flotte->premier;
		while(flotte != NULL){
			if(flotte->systeme == camera->systeme){
				x = flotte->x - camera->xSysteme - 3;
				y = flotte->y - camera->ySysteme - 3;
				if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
					if(flotte->avancement == 0){
						gfx_TransparentSprite(ourFleet, x, y);
						switch(flotte->type){
						case FLOTTE_MILITAIRE:
							gfx_TransparentSprite(force_our, x + 6, y - 1);
							if (flotte->puissance > 500)
							{
								gfx_TransparentSprite(force_our, x + 10, y - 1);
							}
							if (flotte->puissance > 1500)
							{
								gfx_TransparentSprite(force_our, x + 8, y - 4);
							}
							break;
						case FLOTTE_DE_CONSTRUCTION:
							gfx_TransparentSprite(construction_ship_our_icon, x + 6, y - 4);
							break;
						case FLOTTE_SCIENTIFIQUE:
							gfx_TransparentSprite(science_ship_our_icon, x + 6, y - 4);
							break;
						}
					} else if(flotte->avancement == 1) {//dessinne l'hypervitesse
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(x + 3, y, 5);
						flotte->avancement = 2;
					}
					x += 3;
					y += 3;
					if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (camera->bloque == FALSE)))){
						gfx_SetColor(9);
						gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);			
						if((*key == sk_Enter) && (camera->bloque == false)){
							camera->fenetre = MENU_SYSTEME;
							camera->bloque = TRUE;
							fenetre->ouverte = MENU_SYSTEME_FLOTTE_DETAILS;
							fenetre->selection = 1;
							fenetre->flotteSelectionee = FlotteNumeroRecuperer(empire->flotte, flotte) + 1;
							fenetre->precedente = 1;
							*key = 0;
						}
					}
				}
			}
			flotte = flotte->suivant;
		}
		empire = empire->suivant;
	}
}