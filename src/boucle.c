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
#include "updating.h"
#include "console.h"
#include "menu.h"
#include "ai.h"

#include "locale/locale.h"

static void PrintHUD(const unsigned int nombre, const int change, int x, int y);
static void CalculerNiveauDeConnaissance(SystemeStellaire *systemeStellaires, EmpireListe *empireListe);


/**********************************************Jeu principal**********************************************/
int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche){
	char finBoucle = 1, key = 0;
	while (finBoucle == 1)
	{
		/**********boucle du jeu**********/
		
		key = os_GetCSC();
		
		/**gere le temps**/
		StellarisTemps(empireListe, date, &key, systemeStellaires, fenetre);
		
		//efface l'ecran
		gfx_ZeroScreen();

		/**dessine la map**/
		StellarisMap(empireListe, systemeStellaires, camera, &key, date, fenetre, joueur, parametres);

		
		/**dessine le hud**/
		finBoucle = StellarisHUD(empireListe, joueur, date, &key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche);
		
		gfx_SwapDraw();
		if((boot_CheckOnPressed()) || (finBoucle == 0))
		{
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			EmpireListeSupprimer(empireListe);
			return 0;
		}
	}
	return 0;
}

/******************gerer le temps******************/
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires, Fenetre *fenetre){
	//différentes actions des touches
	if(fenetre->commandPrompt == false){
		switch(*key){
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
	if((((date->jour == 30) || (date->jour == 10)) || (date->jour == 20)) && (date->horloge == 0)){
		EffectuerActionsFlottes(empireListe, systemeStellaires);
		EffectuerActionsStations(systemeStellaires, empireListe->premier);
		EffectuerActionsPlanetes(systemeStellaires, empireListe->premier);
	}
	CalculerNiveauDeConnaissance(systemeStellaires, empireListe);
}

/**
 * calcule le niveau de connaissance (intel level) du systeme
 * */
static void CalculerNiveauDeConnaissance(SystemeStellaire *systemeStellaires, EmpireListe *empireListe){
	Empire *joueur = empireListe->premier;
	Flotte *flotte = joueur->flotte->premier;
	int numeroSysteme = 0;

	while(numeroSysteme < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
		if(systemeStellaires[numeroSysteme].niveauDeConnaissance >= MOYEN){
			systemeStellaires[numeroSysteme].niveauDeConnaissance = MOYEN;
		}
		if((systemeStellaires[numeroSysteme].station->niveauStation > AUCUN) && (systemeStellaires[numeroSysteme].empire == 1)){
			systemeStellaires[numeroSysteme].niveauDeConnaissance = TOTAL;
		}
		numeroSysteme++;
	}
	while(flotte != NULL){
		systemeStellaires[flotte->systeme].niveauDeConnaissance = TOTAL;
		flotte = flotte->suivant;
	}
}

/**
 *Effectue les actions de station
 */
void EffectuerActionsStations(SystemeStellaire *systemeStellaires, Empire *joueur){
	int numero = 0, nombreDeVaisseaux = 0;
	int nombreDeCorvette = 0, nombreDeDestroyer = 0, nombreDeCroiseur = 0, nombreDeCuirasse = 0;
	Ordre *ordre = NULL;
	while(numero < (LARGEUR_GALAXIE * LARGEUR_GALAXIE) - 1){
		ordre = RecupererOrdre(systemeStellaires[numero].station->ordreFile);
		if(ordre != NULL){
			if(ordre->tempsActuel > 1){
				ordre->tempsActuel--;
			}
			else{
				switch(ordre->ordre){
				case AMELIORER_STATION:
					systemeStellaires[numero].station->niveauStation++;
					break;
				case CONSTRUIRE_MODULE:
					systemeStellaires[numero].station->modules[ordre->info1 - 1] = ordre->info2;
					break;
				case CONSTRUIRE_VAISSEAU:
					nombreDeVaisseaux = ordre->info2;
					switch(ordre->info1){
						case 1:
							ordre->info1 = FLOTTE_SCIENTIFIQUE;
							nombreDeVaisseaux = 1;
							break;
						case 2:
							ordre->info1 = FLOTTE_DE_CONSTRUCTION;
							nombreDeVaisseaux = 1;
							break;
						case 3:
							ordre->info1 = FLOTTE_MILITAIRE;
							nombreDeCorvette = nombreDeVaisseaux;
							break;
						case 4:
							ordre->info1 = FLOTTE_MILITAIRE;
							nombreDeDestroyer = nombreDeVaisseaux;
							break;
						case 5:
							ordre->info1 = FLOTTE_MILITAIRE;
							nombreDeCroiseur = nombreDeVaisseaux;
							break;
						case 6:
							ordre->info1 = FLOTTE_MILITAIRE;
							nombreDeCuirasse = nombreDeVaisseaux;
							break;
					}
					NouvelleFlotte(joueur->flotte, numero, ordre->info1, nombreDeCorvette, nombreDeDestroyer, nombreDeCroiseur, nombreDeCuirasse);
					break;
				}
				//rajoute des points de vie en cas de canons ou missiles
				if((ordre->info2 == CANONS) || (ordre->info2 == MISSILES)){
					systemeStellaires[numero].station->coqueTotal += systemeStellaires[numero].station->coqueTotal/10;
					systemeStellaires[numero].station->coqueVie += systemeStellaires[numero].station->coqueVie/10;
					systemeStellaires[numero].station->blindageTotal += systemeStellaires[numero].station->blindageTotal/10;
					systemeStellaires[numero].station->blindageVie += systemeStellaires[numero].station->blindageVie/10;
				}
				FinirOrdre(systemeStellaires[numero].station->ordreFile);
			}
		}
		numero++;
	}
}


/**
 *Effectue les actions des planetes
 */
void EffectuerActionsPlanetes(SystemeStellaire *systemeStellaires, Empire *joueur){
	int i = 0, j = 0;
	Villes *villes = NULL;
	Batiment *batiment = NULL;
	Ordre *ordre;
	while(i < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
		j = 0;
		while(j < 5){
			villes = systemeStellaires[i].planetes[j]->villes;
			if(villes != NULL){
				ordre = RecupererOrdre(villes->ordreFile);
				if(ordre != NULL){
					if(ordre->tempsActuel > 1){
						ordre->tempsActuel--;
					}
					else if(ordre->tempsActuel == 1){
						switch(ordre->ordre){
							case CONSTRUIRE_DISTRICT_URBAIN:
								villes->districtsUrbains++;
								break;
							case CONSTRUIRE_DISTRICT_GENERATEUR:
								villes->districtsGenerateurs++;
								break;
							case CONSTRUIRE_DISTRICT_MINIER:
								villes->districtsMiniers++;
								break;
							case CONSTRUIRE_DISTRICT_AGRICOLE:
								villes->districtsAgricoles++;
								break;
							case CONSTRUIRE_BATIMENT:
								switch(ordre->info1){
									case 1:
										batiment = &villes->batiment1;
										villes->niveauBatiment1 = 1;
										break;
									case 2:
										batiment = &villes->batiment2;
										villes->niveauBatiment2 = 1;
										break;
									case 3:
										batiment = &villes->batiment3;
										villes->niveauBatiment3 = 1;
										break;
									case 4:
										batiment = &villes->batiment4;
										villes->niveauBatiment4 = 1;
										break;
									case 5:
										batiment = &villes->batiment5;
										villes->niveauBatiment5 = 1;
										break;
									case 6:
										batiment = &villes->batiment6;
										villes->niveauBatiment6 = 1;
										break;
								}
								*batiment = ordre->info2;
								break;
						}
						FinirOrdre(villes->ordreFile);
					}
				}
			}
			j++;
		}
		i++;
	}
}

/******************dessiner le hud******************/
int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche){
	char jourChar[11];
	char moisChar[8];
	char anneeChar[5];
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
	int16_t systeme = 0, menu = 0;
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
	PrintHUD(joueur->credits, 0, 40, 13);
	gfx_TransparentSprite_NoClip(minerai, 100 ,1);
	PrintHUD(joueur->minerais, 0, 85, 13);
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
	if(camera->bougerFlotte == TRUE){
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(80, 28, 160, 11);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(80, 28, 160, 11);
		gfx_PrintStringXY("Choisir destination", 84, 30);
	}
	
	//pause / avance
	switch (date->vitesse){
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
	if (((camera->systemeSelectione != -1) || (camera->systeme != -1)) && (camera->mapType != CARTE)) {
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
	menu = Menus(empireListe, joueur, date, key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche);
	if(fenetre->commandPrompt == true){ 
		AfficherConsole(key, fenetre, empireListe, camera, date, parametres);
	}
	return menu;
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
	if(change >= 0){
		gfx_SetTextFGColor(1);
	}else{
		gfx_SetTextFGColor(3);
	}
	if(nombre < 1000){
		sprintf(nombreStr, "%d", nombre);
		if (change >= 0){
		sprintf(changeStr, "+%d", change);
		}else{
		sprintf(changeStr, "%d", change);
		}
		strcat(nombreStr, changeStr);
	}else{
		sprintf(nombreStr, "%d", nombre/1000);
		if (change >=0){
		sprintf(changeStr, "+%d", change);
		}else{
		sprintf(changeStr, "%d", change);
		}
		strcat(nombreStr, "K");
		strcat(nombreStr, changeStr);
	}
	gfx_PrintStringXY(nombreStr, x, y);
}
