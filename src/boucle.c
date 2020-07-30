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

#include "locale/locale.h"

/**********************************************Jeu principal**********************************************/
int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche, Console *console){
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
		StellarisMap(empireListe, systemeStellaires, camera, &key, flotteJoueur, date, fenetre, joueur, console);

		
		/**dessine le hud**/
		finBoucle = StellarisHUD(empireListe, joueur, date, &key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche, console);
		
		gfx_SwapDraw();
		if((boot_CheckOnPressed()) || (finBoucle == 0))
		{
			// StellarisSauvegarde(sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche);
			return 0;
		}
	}
	return 0;
}

/******************gerer le temps******************/
void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires, Fenetre *fenetre)
{
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
						switch(ordre->info1){
							case 1:
								systemeStellaires[numero].station->module1 = ordre->info2;
								break;
							case 2:
								systemeStellaires[numero].station->module2 = ordre->info2;
								break;
							case 3:
								systemeStellaires[numero].station->module3 = ordre->info2;
								break;
							case 4:
								systemeStellaires[numero].station->module4 = ordre->info2;
								break;
							case 5:
								systemeStellaires[numero].station->module5 = ordre->info2;
								break;
							case 6:
								systemeStellaires[numero].station->module6 = ordre->info2;
								break;
						}
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
		j = 1;
		while(j < 6){
			switch(j){
				case 1:
					villes = systemeStellaires[i].planete1->villes;
					break;
				case 2:
					villes = systemeStellaires[i].planete2->villes;
					break;
				case 3:
					villes = systemeStellaires[i].planete3->villes;
					break;
				case 4:
					villes = systemeStellaires[i].planete4->villes;
					break;
				case 5:
					villes = systemeStellaires[i].planete5->villes;
					break;
			}
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
int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche, Console *console){
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
	menu = Menus(empireListe, joueur, date, key, camera, systemeStellaires, fenetre, parametres, sauvegarde, marche);
	if(fenetre->commandPrompt == true){ 
		AfficherConsole(console, key, fenetre, empireListe, camera, date);
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
	int8_t compteur = 0, compteurFlotte = 0, maximum = 0, fin = 1;
	int8_t populationChar[5];
	int16_t systeme = 0;

	//quitter
	if(camera->fenetre == MENU_QUITTER)
	{
		fin = MenuQuitter(key, sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, marche, fenetre);
		if (fin == 0){
			return 0;
		}
	}
	
	//menu systeme
	else if(camera->fenetre == MENU_SYSTEME)
	{
		MenuSysteme(key, empireListe, joueur, parametres, date, systemeStellaires, camera, marche, fenetre);
	}
	
	//menu marche
	else if(camera->fenetre == MENU_MARCHE)
	{
		MenuMarche(key, joueur ,marche, camera, fenetre);
	}
	
	//menu flotte
	else if (camera->fenetre == MENU_FLOTTE)
	{
		MenuListeFLottes(key, joueur, camera, fenetre);
	}
	
	//menu recherche
	else if (camera->fenetre == MENU_RECHERCHE)
	{
		MenuRecherche(key, camera, fenetre);
	}
	
	//menu contacts
	else if (camera->fenetre == MENU_CONTACTS)
	{
		MenuContacts(key, empireListe, camera, fenetre);
	}
	return 1;
}

/**
 *Dessine le menu lorsqu'on appui sur echap
 */
int8_t MenuQuitter(char* key, ti_var_t* sauvegarde, EmpireListe* empireListe, Empire* joueur, Parametres* parametres, Date* date, SystemeStellaire* systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre){
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
	return 1;
}

void MenuSysteme(char* key, EmpireListe* empireListe, Empire* joueur, Parametres* parametres, Date* date, SystemeStellaire* systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre){
	char niveau = 0;
	int16_t systeme = 0;
	Flotte* flotte = NULL;
	switch(fenetre->ouverte)
	{				
		case MENU_SYSTEME_FLOTTES: //liste flottes
			MenuSystemeFlotte(key, empireListe, joueur, systemeStellaires, camera, fenetre);
			break;
			
		case MENU_SYSTEME_ETOILE: //etoile
			MenuSystemeEtoile(key, systemeStellaires, camera, fenetre);
			break;
			
		case MENU_SYSTEME_PLANETE_RESUME: //planete resume
			MenuSystemePlaneteResume(key, systemeStellaires, camera, fenetre);
			break;
			
		case MENU_SYSTEME_PLANETE_DISTRICT: //planete districts
			MenuSystemePlaneteDistrict(key, systemeStellaires, camera, fenetre, empireListe);
			break;
			
		case MENU_SYSTEME_PLANETE_BATIMENT: //planete planete baiment
			MenuSystemePlaneteBatiments(key, systemeStellaires, camera, fenetre, empireListe);
			break;

		case MENU_SYSTEME_PLANETE_BATIMENT_CHOIX: //menu planete changer batiment
			MenuSystemePlaneteBatimentChoix(key, joueur, systemeStellaires, camera, fenetre);
			break;

		case MENU_SYSTEME_FLOTTE_DETAILS: //menu flotte details
			MenuSystemeFlotteDetails(key, systemeStellaires, empireListe, joueur, camera, fenetre);
			break;

		case MENU_SYSTEME_STATION_RESUME: //menu station infos
			MenuSystemeStationResume(key, joueur, systemeStellaires, camera, fenetre);
			break;

		case MENU_SYSTEME_STATION_MODULES: //menu station module
			MenuSystemeStationModules(key, joueur, systemeStellaires, camera, fenetre);
			break;

		case MENU_SYSTEME_STATION_MODULES_CHOIX: //menu station changer module
			MenuSystemeStationModulesChoix(key, joueur, systemeStellaires, camera, fenetre);
			break;

		case MENU_SYSTEME_STATION_CHANTIER: //menu station chantier
			MenuSystemeStationChantier(key, joueur, systemeStellaires, camera, fenetre);
			break;

		case MENU_SYSTEME_STATION_CHANTIER_CHOIX: //menu station changer module
			MenuSystemeStationChantierChoix(key, joueur, systemeStellaires, camera, fenetre);
			break;
	}

}

/**
 *Dessine le menu des flottes du système
 */
void MenuSystemeFlotte(char* key, EmpireListe* empireListe, Empire* joueur, SystemeStellaire* systemeStellaires, Camera* camera, Fenetre* fenetre){
	char niveau = 0, i = 0;
	int8_t compteur = 0, compteurFlotte = 0;
	Flotte* flotte = NULL;
	Flotte* flotteDuSysteme[20];
	memset(flotteDuSysteme, 0, sizeof(Flotte*) * 20);

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
}

/**
 *Dessine le menu de resumé de l'étoile
 */
void MenuSystemeEtoile(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
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
}

/**
 *Dessine le menu de resumé de planète
 */
void MenuSystemePlaneteResume(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	int8_t populationChar[5], nomPlanete[20];
	int8_t decalage = 0;
	int niveau = 40;
	Planete* planete = NULL;

	switch(fenetre->planete)
	{
		case 1:
			decalage = 186;
			planete = systemeStellaires[camera->systeme].planete1;
			strcpy(nomPlanete, " I");
			break;
		case 2:
			decalage = 182;
			planete = systemeStellaires[camera->systeme].planete2;
			strcpy(nomPlanete, " II");
			break;
		case 3:
			decalage = 178;
			planete = systemeStellaires[camera->systeme].planete3;
			strcpy(nomPlanete, " III");
			break;
		case 4:
			decalage = 182;
			planete = systemeStellaires[camera->systeme].planete4;
			strcpy(nomPlanete, " IV");
			break;
			
		case 5:
			decalage = 186;
			planete = systemeStellaires[camera->systeme].planete5;
			strcpy(nomPlanete, " V");
			break;
	}

	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);

	gfx_SetColor(7);
	gfx_FillRectangle_NoClip(40, 200, strlen("R/sum/") * 8 + 10, 10);
	gfx_PrintStringXY("R/sum/", 45, 201);
	niveau += strlen("R/sum/") * 8 + 10;
	gfx_SetColor(6);

	if(planete->population == 0){
		gfx_SetColor(11);
	}
	gfx_FillRectangle_NoClip(niveau, 200, strlen("Districts") * 8 + 10, 10);
	gfx_PrintStringXY("Districts", niveau + 5, 201);
	niveau += strlen("Districts") * 8 + 10;
	gfx_SetColor(7);
	gfx_VertLine_NoClip(niveau - 1, 201, 8);//barre de séparation
	gfx_SetColor(6);

	if(planete->population == 0){
		gfx_SetColor(11);
	}
	gfx_FillRectangle_NoClip(niveau, 200, strlen("Batiments") * 8 + 10, 10);
	gfx_PrintStringXY("Batiments", niveau + 5, 201);
	niveau += strlen("Batiments") * 8 + 10;


	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_Rectangle_NoClip(40, 199, niveau - 40, 12); //cadre des differents pages

	gfx_HorizLine_NoClip(45, 51, 180); //***
	gfx_Line_NoClip(225, 51, 230, 46); //*Barre sous le titre
	gfx_HorizLine_NoClip(230, 45, 40); //***

	gfx_Rectangle_NoClip(45, 56, 100, 100); //cadre image

	gfx_SetColor(1);
	gfx_SetPixel(270, 45);//point au bout de la ligne du titre

	gfx_SetTextXY(45, 42);
	if(planete->habitable == 1){
		gfx_PrintString(planete->nom);
		gfx_SetTextFGColor(19);
		if(planete->population > 0){
			gfx_PrintStringXY("Habit/e", 150, 62);
		}
		else{
			gfx_PrintStringXY("Habitable", 150, 62);
		}
	}
	else{
		gfx_PrintString(systemeStellaires[camera->systeme].nom);
		gfx_PrintString(nomPlanete);
		gfx_SetTextFGColor(3);
		gfx_PrintStringXY("Non-habitable", 150, 62);
	}

	gfx_SetTextFGColor(1);
	gfx_SetTextXY(157, 74);
	gfx_TransparentSprite_NoClip(pop_icon, 150, 74);
	sprintf(populationChar, "%d", planete->population);
	gfx_PrintString(populationChar);
	
	if(planete->population > 0){
		gfx_SetTextXY(157, 84);
		gfx_TransparentSprite_NoClip(criminality_pop_icon, 150, 84);
		PrintInt(planete->villes->criminatlitee);
		
		gfx_SetTextXY(157, 94);
		gfx_TransparentSprite_NoClip(free_houses_icon, 150, 94);
		PrintInt(planete->villes->districtsUrbains);
		
		gfx_SetTextXY(157, 104);
		gfx_TransparentSprite_NoClip(amienties_icon, 150, 104);
		PrintInt(planete->villes->amienties);

		gfx_SetTextXY(157, 114);
		gfx_TransparentSprite_NoClip(free_job_icon, 150, 114);
		if(planete->population - planete->villes->emplois > 0){
			PrintInt(planete->population - planete->villes->emplois);
		}
		else{
			PrintInt(0);
		}
		
		gfx_SetTextXY(157, 124);
		gfx_TransparentSprite_NoClip(unemployed_pop_icon, 150, 124);
		if(planete->population - planete->villes->emplois < 0) {
			PrintInt(-(planete->population - planete->villes->emplois));
		}
		else{
			PrintInt(0);
		}

		niveau = 158;
		gfx_SetTextXY(45, niveau);
		gfx_PrintString("Production");

		niveau += 14;
		gfx_SetTextXY(45, niveau);
		gfx_PrintString("D/ficit");
	}

	switch(*key)
	{
		case sk_Clear:
			camera->fenetre = MENU_AUCUN;
			fenetre->ouverte = FALSE;
			camera->bloque = FALSE;
			*key = 0;
			break;
		case sk_Right:
			if(planete->population > 0){
				fenetre->ouverte = MENU_SYSTEME_PLANETE_DISTRICT;
				*key = 0;
			}
			break;
	}
}

/**
 *Dessine le menu des districts de planète
 */
void MenuSystemePlaneteDistrict(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, EmpireListe *empireListe){
	int8_t nomPlanete[20];
	int8_t decalage = 0;
	int niveau = 40, maximum = 4;
	Planete* planete = NULL;
	Ordre *ordre = NULL;

	switch(fenetre->planete)
	{
		case 1:
			decalage = 186;
			planete = systemeStellaires[camera->systeme].planete1;
			strcpy(nomPlanete, " I");
			break;
		case 2:
			decalage = 182;
			planete = systemeStellaires[camera->systeme].planete2;
			strcpy(nomPlanete, " II");
			break;
		case 3:
			decalage = 178;
			planete = systemeStellaires[camera->systeme].planete3;
			strcpy(nomPlanete, " III");
			break;
		case 4:
			decalage = 182;
			planete = systemeStellaires[camera->systeme].planete4;
			strcpy(nomPlanete, " IV");
			break;
			
		case 5:
			decalage = 186;
			planete = systemeStellaires[camera->systeme].planete5;
			strcpy(nomPlanete, " V");
			break;
	}

	ordre = RecupererOrdre(planete->villes->ordreFile);	

	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);

	gfx_FillRectangle_NoClip(40, 200, strlen("R/sum/") * 8 + 10, 10);
	gfx_PrintStringXY("R/sum/", 45, 201);
	niveau += strlen("R/sum/") * 8 + 10;

	if(planete->population == 0){
		gfx_SetColor(11);
	}
	gfx_SetColor(7);
	gfx_FillRectangle_NoClip(niveau, 200, strlen("Districts") * 8 + 10, 10);
	gfx_PrintStringXY("Districts", niveau + 5, 201);
	niveau += strlen("Districts") * 8 + 10;
	gfx_SetColor(6);

	if(planete->population == 0){
		gfx_SetColor(11);
	}
	gfx_FillRectangle_NoClip(niveau, 200, strlen("Batiments") * 8 + 10, 10);
	gfx_PrintStringXY("Batiments", niveau + 5, 201);
	niveau += strlen("Batiments") * 8 + 10;


	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_Rectangle_NoClip(40, 199, niveau - 40, 11);

	gfx_HorizLine_NoClip(45, 51, 180); //***
	gfx_Line_NoClip(225, 51, 230, 46); //*Barre sous le titre
	gfx_HorizLine_NoClip(230, 45, 40); //***

	gfx_HorizLine_NoClip(45, 179, 230);//barre du bas au dessus des ordres
	
	gfx_SetColor(1);
	gfx_SetPixel(270, 45);//point au bout de la ligne du titre

	gfx_SetTextXY(45, 42);
	if(planete->habitable == 1){
		gfx_PrintString(planete->nom);
	}
	else{
		gfx_PrintString(systemeStellaires[camera->systeme].nom);
		gfx_PrintString(nomPlanete);
	}
	if(planete->villes != NULL){
		niveau = 55;
		if(fenetre->selection == 1){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(7);
		}
		gfx_Rectangle_NoClip(45, niveau, 190, 21);
		gfx_SetTextFGColor(17);
		gfx_SetTextXY(50, niveau + 2);
		gfx_PrintString("Districts urbains");
		gfx_SetTextXY(50, niveau + 12);
		PrintInt(planete->villes->districtsUrbains);
		gfx_PrintString("|10");

		niveau += 24;
		if(fenetre->selection == 2){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(7);
		}
		gfx_Rectangle_NoClip(45, niveau, 190, 21);
		gfx_SetTextFGColor(18);
		gfx_SetTextXY(50, niveau + 2);
		gfx_PrintString("Districts g/n/rateurs");
		gfx_SetTextXY(50, niveau + 12);
		PrintInt(planete->villes->districtsGenerateurs);
		gfx_PrintString("|10");

		niveau += 24;
		if(fenetre->selection == 3){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(7);
		}
		gfx_Rectangle_NoClip(45, niveau, 190, 21);
		gfx_SetTextFGColor(3);
		gfx_SetTextXY(50, niveau + 2);
		gfx_PrintString("Districts miniers");
		gfx_SetTextXY(50, niveau + 12);
		PrintInt(planete->villes->districtsMiniers);
		gfx_PrintString("|10");

		niveau += 24;
		if(fenetre->selection == 4){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(7);
		}
		gfx_Rectangle_NoClip(45, niveau, 190, 21);
		gfx_SetTextFGColor(19);
		gfx_SetTextXY(50, niveau + 2);
		gfx_PrintString("Districts agricoles");
		gfx_SetTextXY(50, niveau + 12);
		PrintInt(planete->villes->districtsAgricoles);
		gfx_PrintString("|10");
	}
	OrdreDistrictNom(planete->villes);
	
	if(ordre != NULL){
		niveau = 181;
		maximum = 5;
		if(fenetre->selection == 5){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(3);
		}
		gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);
		gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);
	}

	if(fenetre->error == MINERAI_MANQUE){
		fenetre->errorCountDown--;
		gfx_SetTextFGColor(3);
		gfx_PrintStringXY("Pas assez de minerais", 45, 160);
		if(fenetre->errorCountDown == 0){
			fenetre->error = NO_ERROR;
		}
	}

	if(*key == sk_Enter){
		if(fenetre->selection == 5){
			empireListe->premier->minerais += ordre->prix;
			FinirOrdre(planete->villes->ordreFile);
		}
		else{
			if(empireListe->premier->minerais >= 50){
				empireListe->premier->minerais -= 50;
				switch(fenetre->selection){
					case 1:
						NouvelOrdre(planete->villes->ordreFile, CONSTRUIRE_DISTRICT_URBAIN, 12, 0, 0, 50);
						break;
					case 2:
						NouvelOrdre(planete->villes->ordreFile, CONSTRUIRE_DISTRICT_GENERATEUR, 12, 0, 0, 50);
						break;
					case 3:
						NouvelOrdre(planete->villes->ordreFile, CONSTRUIRE_DISTRICT_MINIER, 12, 0, 0, 50);
						break;
					case 4:
						NouvelOrdre(planete->villes->ordreFile, CONSTRUIRE_DISTRICT_AGRICOLE, 12, 0, 0, 50);
						break;
				}
			}
			else{
				fenetre->error = MINERAI_MANQUE;
				fenetre->errorCountDown = 50;
			}
		}
		*key = 0;
	}
	switch(*key)
	{
		case sk_Clear:
			camera->fenetre = MENU_AUCUN;
			fenetre->ouverte = FALSE;
			camera->bloque = FALSE;
			*key = 0;
			break;
		case sk_Left:
			fenetre->ouverte = MENU_SYSTEME_PLANETE_RESUME;
			*key = 0;
			break;
			break;
		case sk_Right:
			fenetre->ouverte = MENU_SYSTEME_PLANETE_BATIMENT;
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
	if(fenetre->selection < 1){
		fenetre->selection = 1;
	}
	if(fenetre->selection > maximum){
		fenetre->selection = maximum;
	}
}

/**
 *Ecrie le nom de l'ordre de district
 */
void OrdreDistrictNom(Villes *villes){
	Ordre *ordre;
	int nombredOrdres = 0;
	int niveau = 181;
	ordre = RecupererOrdre(villes->ordreFile);
	gfx_SetTextXY(45, niveau);
	gfx_SetTextFGColor(1);
	if(ordre != NULL){
		switch(ordre->ordre){
			case AUCUN:
				gfx_PrintString("Aucun ordre");
				break;
			case CONSTRUIRE_DISTRICT_URBAIN:
				gfx_PrintString("Construit district urbain");
				break;
			case CONSTRUIRE_DISTRICT_GENERATEUR:
				gfx_PrintString("Construit district g/n/rateur");
				break;
			case CONSTRUIRE_DISTRICT_MINIER:
				gfx_PrintString("Construit district minier");
				break;
			case CONSTRUIRE_DISTRICT_AGRICOLE:
				gfx_PrintString("Construit district agicole");
				break;
				break;
			case CONSTRUIRE_BATIMENT:
				gfx_PrintString("Construit batiment ");
				PrintInt(ordre->info1);
				break;
		}
		if(ordre->ordre != AUCUN){
			gfx_SetTextXY(45, niveau += 9);
			gfx_PrintString("(");
			gfx_SetTextFGColor(13);
			PrintInt((ordre->tempsTotal - ordre->tempsActuel) * 100 / 12);
			gfx_PrintString("%");
			gfx_SetTextFGColor(1);
			gfx_PrintString(")");
			nombredOrdres = NombredOrdres(villes->ordreFile);
			if(nombredOrdres > 1){
				gfx_PrintString(" (+");
				PrintInt(nombredOrdres - 1);
				gfx_PrintString(")");
			}
		}
	}
	else{
		gfx_PrintString("Aucun ordre");
	}
}

/**
 *Dessine le menu de les batiments de planète
 */
void MenuSystemePlaneteBatiments(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, EmpireListe *empireListe){
	int8_t nomPlanete[20], nom[50];
	int8_t decalage = 0;
	int niveau = 40, nombreDeBatiment = 0;
	int supprimer = 0;
	Planete* planete = NULL;
	Ordre *ordre = NULL;

	switch(fenetre->planete)
	{
		case 1:
			decalage = 186;
			planete = systemeStellaires[camera->systeme].planete1;
			strcpy(nomPlanete, " I");
			break;
		case 2:
			decalage = 182;
			planete = systemeStellaires[camera->systeme].planete2;
			strcpy(nomPlanete, " II");
			break;
		case 3:
			decalage = 178;
			planete = systemeStellaires[camera->systeme].planete3;
			strcpy(nomPlanete, " III");
			break;
		case 4:
			decalage = 182;
			planete = systemeStellaires[camera->systeme].planete4;
			strcpy(nomPlanete, " IV");
			break;
			
		case 5:
			decalage = 186;
			planete = systemeStellaires[camera->systeme].planete5;
			strcpy(nomPlanete, " V");
			break;
	}

	ordre = RecupererOrdre(planete->villes->ordreFile);	

	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);

	gfx_FillRectangle_NoClip(40, 200, strlen("R/sum/") * 8 + 10, 10);
	gfx_PrintStringXY("R/sum/", 45, 201);
	niveau += strlen("R/sum/") * 8 + 10;
	gfx_SetColor(7);
	gfx_VertLine_NoClip(niveau - 1, 201, 7);//barre de séparation
	gfx_SetColor(6);

	gfx_FillRectangle_NoClip(niveau, 200, strlen("Districts") * 8 + 10, 10);
	gfx_PrintStringXY("Districts", niveau + 5, 201);
	niveau += strlen("Districts") * 8 + 10;

	gfx_SetColor(7);
	gfx_FillRectangle_NoClip(niveau, 200, strlen("Batiments") * 8 + 10, 10);
	gfx_PrintStringXY("Batiments", niveau + 5, 201);
	niveau += strlen("Batiments") * 8 + 10;


	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_Rectangle_NoClip(40, 199, niveau - 40, 11);

	gfx_HorizLine_NoClip(45, 51, 180); //***
	gfx_Line_NoClip(225, 51, 230, 46); //*Barre sous le titre
	gfx_HorizLine_NoClip(230, 45, 40); //***

	gfx_HorizLine_NoClip(45, 179, 230);//barre du bas au dessus des ordres

	gfx_SetColor(1);
	gfx_SetPixel(270, 45);//point au bout de la ligne du titre

	gfx_SetTextXY(45, 42);//écrie le nom de la planète
	if(planete->habitable == 1){
		gfx_PrintString(planete->nom);
	}
	else{
		gfx_PrintString(systemeStellaires[camera->systeme].nom);
		gfx_PrintString(nomPlanete);
	}
	niveau = 55;
	if(planete->villes != NULL){
		gfx_SetTextXY(45, niveau);
		if(planete->population >= 0){
			gfx_SetTextFGColor(1);
			if(fenetre->selection == 1){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(planete->villes->batiment1, planete->villes->niveauBatiment1, nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("0");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(planete->population >= 10){
			gfx_SetTextFGColor(1);
			if(fenetre->selection == 2){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(planete->villes->batiment2, planete->villes->niveauBatiment2, nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("10");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(planete->population >= 20){
			gfx_SetTextFGColor(1);
			if(fenetre->selection == 3){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(planete->villes->batiment3, planete->villes->niveauBatiment3, nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("20");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(planete->population >= 30){
			gfx_SetTextFGColor(1);
			if(fenetre->selection == 4){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(planete->villes->batiment4, planete->villes->niveauBatiment4, nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("30");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(planete->population >= 40){
			gfx_SetTextFGColor(1);
			if(fenetre->selection == 5){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(planete->villes->batiment5, planete->villes->niveauBatiment5, nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("40");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(planete->population >= 50){
			gfx_SetTextFGColor(1);
			if(fenetre->selection == 6){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(planete->villes->batiment6, planete->villes->niveauBatiment6, nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("50");
		}
		niveau += 14;
	}
	OrdreDistrictNom(planete->villes);
	
	if(ordre != NULL){
		niveau = 181;
		if(fenetre->selection == 7){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(3);
		}
		supprimer = 1;
		gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);
		gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);
	}
	
	if(*key == sk_Enter){
		if((supprimer == 1) && (fenetre->selection == 7)){
			empireListe->premier->minerais += ordre->prix;
			FinirOrdre(planete->villes->ordreFile);
		}
		else if (fenetre->selection != 1){
			fenetre->ouverte = MENU_SYSTEME_PLANETE_BATIMENT_CHOIX;
			fenetre->precedente = fenetre->selection;
			fenetre->selection = 1;
			fenetre->villes = planete->villes;
		}
		*key = 0;
	}
	switch(*key)
	{
		case sk_Clear:
			camera->fenetre = MENU_AUCUN;
			fenetre->ouverte = FALSE;
			camera->bloque = FALSE;
			*key = 0;
			break;
		case sk_Left:
			fenetre->ouverte = MENU_SYSTEME_PLANETE_DISTRICT;
			*key = 0;
			break;
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
	if(fenetre->selection < 1){
		fenetre->selection = 1;
	}
	if((fenetre->selection == 6) && (nombreDeBatiment != 6)){
		fenetre->selection = nombreDeBatiment;
	}
	if((fenetre->selection > nombreDeBatiment) && (supprimer == 0)){
		fenetre->selection = nombreDeBatiment;
	}
	if((fenetre->selection > nombreDeBatiment) && (supprimer == 1)){
		fenetre->selection = 7;
	}
}

/**
 *Choisi un batiment
 */
void MenuSystemePlaneteBatimentChoix(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	int niveau = 53;
	char nomDuBatiment[50];
	int i = 0, prix = 0;
	Batiment *batiment = NULL;
	int niveauDuBatiment = 0;
	switch(*key){
		case sk_Clear:
			fenetre->ouverte = MENU_SYSTEME_PLANETE_BATIMENT;
			fenetre->selection = fenetre->precedente;
			fenetre->precedente = 1;
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
	if(fenetre->selection > 5){
		fenetre->selection = 5;
	}
	else if(fenetre->selection < 1){
		fenetre->selection = 1;
	}
	if(fenetre->villes == NULL){
		fenetre->ouverte = MENU_SYSTEME_PLANETE_BATIMENT;
		return;
	}
	switch(fenetre->precedente){
		case 1:
			batiment = &fenetre->villes->batiment1;
			niveauDuBatiment = fenetre->villes->niveauBatiment1;
			break;
		case 2:
			batiment = &fenetre->villes->batiment2;
			niveauDuBatiment = fenetre->villes->niveauBatiment2;
			break;
		case 3:
			batiment = &fenetre->villes->batiment3;
			niveauDuBatiment = fenetre->villes->niveauBatiment3;
			break;
		case 4:
			batiment = &fenetre->villes->batiment4;
			niveauDuBatiment = fenetre->villes->niveauBatiment4;
			break;
		case 5:
			batiment = &fenetre->villes->batiment5;
			niveauDuBatiment = fenetre->villes->niveauBatiment5;
			break;
		case 6:
			batiment = &fenetre->villes->batiment6;
			niveauDuBatiment = fenetre->villes->niveauBatiment6;
			break;
	}
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);

	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);

	gfx_HorizLine_NoClip(45, 51, 180);
	gfx_Line_NoClip(225, 51, 230, 46);
	gfx_HorizLine_NoClip(230, 45, 40);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Batiment ");
	PrintInt(fenetre->precedente);
	
	gfx_SetTextFGColor(1);
	if(fenetre->selection == 1){
		gfx_SetTextFGColor(13);
		prix = 0;
	}
	gfx_PrintStringXY("D/manteler", 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	PrintInt(0);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 2){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(FONDERIE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 3){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(LABORATOIRE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 4){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(USINE_CIVILE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 5){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(THEATRE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	if(*key == sk_Enter){
		fenetre->ouverte = MENU_SYSTEME_PLANETE_BATIMENT;
		if(fenetre->selection != 0){
			fenetre->selection++;
		}
		if(joueur->acier >= prix){
			joueur->acier -= prix;
			NouvelOrdre(fenetre->villes->ordreFile,
				CONSTRUIRE_BATIMENT,
				10,
				fenetre->precedente,
				fenetre->selection,
				prix
			);
			fenetre->selection = fenetre->precedente;
			fenetre->precedente = 1;
		}
		*key = 0;
	}
}

/**
 * Ecrit le nom du batiment
 * */
char *PlaneteBatimentNom(Batiment batiment, int niveau, char* nom){
	char levelStr[10];
	if(batiment != AUCUN){
		switch(batiment){
			case CAPITALE:
				strcpy(nom, "Capitale");
				break;
			case FONDERIE:
				strcpy(nom, "Fonderie");
				break;
			case LABORATOIRE:
				strcpy(nom, "Laboratoire");
				break;
			case USINE_CIVILE:
				strcpy(nom, "Usine civile");
				break;
			case THEATRE:
				strcpy(nom, "Theatre");
		}
		strcat(nom, " lvl.");
		sprintf(levelStr, "%d", niveau);
		strcat(nom, levelStr);
	}
	else{
		strcpy(nom, "Aucun batiment");
	}
	return nom;
}

/**
 *Dessine le menu des détails sur la flotte sélectionnée par les menus MenuSystemeFlotte et MenuListeFlottes
 */
void MenuSystemeFlotteDetails(char *key, SystemeStellaire *systemeStellaires, EmpireListe *empireListe, Empire* joueur, Camera *camera, Fenetre *fenetre){
	char niveau = 0;
	Flotte* flotte = NULL;
	switch(*key)
	{
		case sk_Clear:
			switch(fenetre->precedente){
				case 1:
					fenetre->ouverte = MENU_SYSTEME_FLOTTES;
					fenetre->selection = fenetre->flotteSelectionee;
					break;
				case 2:
					camera->fenetre = MENU_FLOTTE;
					fenetre->selection = fenetre->flotteSelectionee;
					break;
				default:
					camera->fenetre = AUCUN;
					camera->bloque = FALSE;
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
			niveau += 20;
			gfx_PrintString("Bombarder");
			gfx_SetTextFGColor(1);

			//ecrire la liste des vaisseaux
			gfx_SetTextXY(45, niveau);
			PrintInt(flotte->nombreDeCorvette);
			gfx_PrintString(" Corvette(s)");
			gfx_SetTextFGColor(1);

			gfx_SetTextXY(165, niveau);
			niveau += 14;
			PrintInt(flotte->nombreDeDestroyer);
			gfx_PrintString(" Destroyer(s)");
			gfx_SetTextFGColor(1);
			
			gfx_SetTextXY(45, niveau);
			PrintInt(flotte->nombreDeCroiseur);
			gfx_PrintString(" Croiseur(s)");
			gfx_SetTextFGColor(1);

			
			gfx_SetTextXY(165, niveau);
			niveau += 14;
			PrintInt(flotte->nombreDeCuirasse);
			gfx_PrintString(" Cuirass/(s)");
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
}

/**
 *Dessine le menu de la station du systeme
 */
void MenuSystemeStationResume(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	char evolution[25] = {0};
	char ordreStation[50];
	int prixAmelioration = 0, tempsAmelioration = 0;
	int maximum = 2;
	Ordre *ordre = RecupererOrdre(systemeStellaires[camera->systeme].station->ordreFile);
	int niveau = 120;
	if(ordre != NULL){
		maximum = 3;
	}
	switch(*key){
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
		case sk_Right:
		fenetre->selection = 1;
			fenetre->ouverte = MENU_SYSTEME_STATION_MODULES;
			fenetre->precedente = 0;
			*key = 0;
			break;
	}
	if(fenetre->selection > maximum){
		fenetre->selection = maximum;
	}
	else if(fenetre->selection < 1){
		fenetre->selection = 1;
	}
	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);
	gfx_FillRectangle_NoClip(92, 200, 60, 10);
	gfx_FillRectangle_NoClip(152, 200, 68, 10);

	gfx_SetColor(0);
	gfx_FillRectangle_NoClip(45, 56, 100, 100); //fond de l'image

	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_HorizLine_NoClip(45, 51, 180);
	gfx_Line_NoClip(225, 51, 230, 46);
	gfx_HorizLine_NoClip(230, 45, 40);
	gfx_Rectangle_NoClip(45, 56, 100, 100); //cadre de l'image
	gfx_FillRectangle_NoClip(40, 200, 52, 10);//barre du bas
	gfx_Rectangle_NoClip(40, 199, 180, 11);
	gfx_VertLine_NoClip(152, 201, 8);

	gfx_HorizLine_NoClip(45, 178, 230);//barre du bas au dessus des ordres

	gfx_Rectangle_NoClip(148, 77, 129, 38);//cadre ameliorer
	gfx_Rectangle_NoClip(148, 125, 129, 18);//cadre retrograder

	gfx_SetColor(1);
	gfx_SetPixel(270, 45);

	gfx_PrintStringXY("R/sum/", 42, 201);
	gfx_PrintStringXY("Modules", 94, 201);
	gfx_PrintStringXY("Chantier", 154, 201);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Station de ");
	gfx_PrintString(systemeStellaires[camera->systeme].nom);
	gfx_SetTextXY(150, 62);
	switch(systemeStellaires[camera->systeme].station->niveauStation){
		case AVANT_POSTE:
			gfx_PrintString("Base stellaire");
			strcpy(evolution, "Port stellaire");
			tempsAmelioration = 36;
			prixAmelioration = 200;
			break;
		case PORT_STELLAIRE:
			gfx_PrintString("Port stellaire");
			strcpy(evolution, "Redoute stellaire");
			tempsAmelioration = 72;
			prixAmelioration = 500;
			break;
		case REDOUTE_STELLAIRE:
			gfx_PrintString("Redoute stellaire");
			strcpy(evolution, "Forteresse stellaire");
			tempsAmelioration = 108;
			prixAmelioration = 1200;
			break;
		case FORTERESSE_STELLAIRE:
			gfx_PrintString("Forteresse stellaire");
			strcpy(evolution, "Citadelle");
			tempsAmelioration = 144;
			prixAmelioration = 3500;
			break;
		case CITADELLE:
			gfx_PrintString("Citadelle");
			break;
	}
	gfx_SetColor(11);
	switch(systemeStellaires[camera->systeme].station->niveauStation){
		case CITADELLE:
			gfx_FillRectangle_NoClip(63, 74, 3, 3);//bras haut haut gauche gauche
			gfx_FillRectangle_NoClip(124, 74, 3, 3);//bras haut haut droite droite
			gfx_FillRectangle_NoClip(63, 135, 3, 3);//bras bas bas gauche gauche
			gfx_FillRectangle_NoClip(124, 135, 3, 3);//bras bas bas droite droite

			gfx_FillRectangle_NoClip(55, 66, 8, 8);//plateforme haut haut gauche gauche
			gfx_FillRectangle_NoClip(127, 66, 8, 8);//plateforme haut haut droite droite
			gfx_FillRectangle_NoClip(55, 138, 8, 8);//plateforme bas bas gauche gauche
			gfx_FillRectangle_NoClip(127, 138, 8, 8);//plateforme bas bas droite droite
		case FORTERESSE_STELLAIRE:
			gfx_FillRectangle_NoClip(78, 78, 11, 2);// bras haut haut gauche
			gfx_FillRectangle_NoClip(101, 78, 11, 2);// bras haut haut droite
			gfx_FillRectangle_NoClip(78, 132, 11, 2);// bras bas bas gauche
			gfx_FillRectangle_NoClip(101, 132, 11, 2);// bras bas bas droite

			gfx_FillRectangle_NoClip(67, 89, 2, 11);// bras haut gauche gauche
			gfx_FillRectangle_NoClip(121, 89, 2, 11);// bras haut droite droite
			gfx_FillRectangle_NoClip(67, 112, 2, 11);// bras bas gauche gauche
			gfx_FillRectangle_NoClip(121, 112, 2, 11);// bras bas droite droite

			gfx_FillRectangle_NoClip(82, 93, 3, 3);//bras 1 haut gauche
			gfx_FillRectangle_NoClip(105, 93, 3, 3);//bras 1 haut droite
			gfx_FillRectangle_NoClip(82, 116, 3, 3);//bras 1 bas gauche
			gfx_FillRectangle_NoClip(105, 116, 3, 3);//bras 1 bas droite

			gfx_FillRectangle_NoClip(78, 89, 4, 4);//bras 2 haut gauche
			gfx_FillRectangle_NoClip(108, 89, 4, 4);//bras 2 haut droite
			gfx_FillRectangle_NoClip(78, 119, 4, 4);//bras 2 bas gauche
			gfx_FillRectangle_NoClip(108, 119, 4, 4);//bras 2 bas droite

			gfx_FillRectangle_NoClip(66, 77, 12, 12);// plateforme diagonale haut gauche
			gfx_FillRectangle_NoClip(112, 77, 12, 12);// plateforme diagonale haut droite
			gfx_FillRectangle_NoClip(66, 123, 12, 12);// plateforme diagonale bas gauche
			gfx_FillRectangle_NoClip(112, 123, 12, 12);// plateforme diagonale bas droite
		case REDOUTE_STELLAIRE:
			gfx_FillRectangle_NoClip(93, 85, 4, 11);//bras haut
			gfx_FillRectangle_NoClip(93, 116, 4, 11);//bras bas
			gfx_FillRectangle_NoClip(89, 74, 12, 11);//platforme haut
			gfx_FillRectangle_NoClip(89, 127, 12, 11);//platforme bas
		case PORT_STELLAIRE:
			gfx_FillRectangle_NoClip(74, 104, 11, 4);//bras gauche
			gfx_FillRectangle_NoClip(105, 104, 11, 4);//bras droit
			gfx_FillRectangle_NoClip(63, 100, 11, 12);//platforme gauche
			gfx_FillRectangle_NoClip(116, 100, 11, 12);//platforme droite
		case AVANT_POSTE:
			gfx_FillRectangle_NoClip(85, 96, 20, 20);//centre
	}
	if(fenetre->selection == 1){
		gfx_SetTextFGColor(13);
	}
	if(systemeStellaires[camera->systeme].station->niveauStation < CITADELLE){
		gfx_SetTextXY(212 - strlen("Am/liorer") * 4, 80);
		gfx_PrintString("Am/liorer");
		gfx_SetTextXY(212 - strlen(evolution) * 4, 92);
		gfx_PrintString(evolution);
		gfx_SetTextXY(227 - tailleInt(prixAmelioration) * 4, 104);
		gfx_TransparentSprite_NoClip(fer, 212 - tailleInt(prixAmelioration) * 4, 103);
		if(joueur->acier < prixAmelioration){
			gfx_SetTextFGColor(3);
		}
		PrintInt(prixAmelioration);
	}
	else{
		gfx_SetTextXY(212 - strlen("Am/lioration max") * 4, 92);
		gfx_PrintString("Am/lioration max");
	}
	gfx_SetTextFGColor(1);

	if(fenetre->selection == 2){
		gfx_SetTextFGColor(13);
	}
	if(systemeStellaires[camera->systeme].station->niveauStation > AVANT_POSTE){
		gfx_SetTextXY(212 - strlen("R/etrogader") * 4, 130);
		gfx_PrintString("R/trogader");
	}
	else{
		gfx_SetTextXY(212 - strlen("Detruire") * 4, 130);
		gfx_PrintString("D~truire");
	}
	gfx_SetTextFGColor(34);
	niveau = 160;
	//ecrire les statistiques
	gfx_SetTextXY(50, niveau);
	gfx_TransparentSprite_NoClip(life_icon, 45, niveau + 1);
	PrintInt((systemeStellaires[camera->systeme].station->coqueVie * 100) / systemeStellaires[camera->systeme].station->coqueTotal);
	gfx_PrintString("%");
	gfx_SetTextXY(170, niveau);
	gfx_TransparentSprite_NoClip(blindage_icon, 165, niveau + 1);
	PrintInt((systemeStellaires[camera->systeme].station->blindageVie * 100) / systemeStellaires[camera->systeme].station->blindageTotal);
	gfx_PrintString("%");
	niveau += 9;
	gfx_SetTextXY(50, niveau);
	gfx_TransparentSprite_NoClip(bouclier_icon, 45, niveau);
	PrintInt((systemeStellaires[camera->systeme].station->bouclierVie * 100) / systemeStellaires[camera->systeme].station->bouclierTotal);
	gfx_PrintString("%");
	gfx_SetTextFGColor(18);
	gfx_SetTextXY(170, niveau);
	gfx_TransparentSprite_NoClip(fleet_power_icon, 165, niveau + 2);
	PrintInt(systemeStellaires[camera->systeme].station->puissance);
	
	niveau = 181;
	
	if(ordre != NULL){
		if(fenetre->selection == 3){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(3);
		}
		gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);// croix
		gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);// croix
	}

	//ecrire ordre
	gfx_SetTextFGColor(1);
	gfx_SetTextXY(45, 190);
	OrdreStationNom(systemeStellaires[camera->systeme].station, ordre->info1, ordreStation, niveau);

	//effectuer action
	if(*key == sk_Enter){
		switch(fenetre->selection){
			case 1:
				if((joueur->acier >= prixAmelioration) && (systemeStellaires[camera->systeme].station->niveauStation < CITADELLE)){
					joueur->acier -= prixAmelioration;
					NouvelOrdre(systemeStellaires[camera->systeme].station->ordreFile,
						AMELIORER_STATION,
						tempsAmelioration,
						0,
						0,
						prixAmelioration
					);
				}
				break;
			case 2:
				if(systemeStellaires[camera->systeme].station->niveauStation > AVANT_POSTE){
					switch(systemeStellaires[camera->systeme].station->niveauStation){
						case PORT_STELLAIRE:
							systemeStellaires[camera->systeme].station->module1 = AUCUN;
							systemeStellaires[camera->systeme].station->module2 = AUCUN;
						case REDOUTE_STELLAIRE:
							systemeStellaires[camera->systeme].station->module3 = AUCUN;
							systemeStellaires[camera->systeme].station->module4 = AUCUN;
						case FORTERESSE_STELLAIRE:
							systemeStellaires[camera->systeme].station->module5 = AUCUN;
							systemeStellaires[camera->systeme].station->module6 = AUCUN;
					}
					systemeStellaires[camera->systeme].station->niveauStation--;
				}
				else {
					camera->fenetre = MENU_AUCUN;
					fenetre->ouverte = 0;
					camera->bloque = false;
					systemeStellaires[camera->systeme].station->niveauStation = AUCUNE;
					memset(systemeStellaires[camera->systeme].station, 0, sizeof(Station));
				}
				break;
			case 3:
				joueur->acier += ordre->prix;
				FinirOrdre(systemeStellaires[camera->systeme].station->ordreFile);
				break;
		}
	}
}

/**
 *Ecrie et renvoie le nom de l'ordre
 */
char* OrdreStationNom(Station *station, int numeroDuModule, char* nomDeOrdre, int niveau){
	char numero[20];
	int travail = 0, nombredOrdres = 0;
	Ordre *ordre = RecupererOrdre(station->ordreFile);
	if(ordre == NULL){
		gfx_PrintStringXY("Aucun ordre", 55, niveau);
	}
	else{
		switch(ordre->ordre){
			case AUCUN:
				gfx_PrintStringXY("Aucun ordre", 55, niveau);
				break;
			case AMELIORER_STATION:
				gfx_PrintStringXY("S'am/liore en ", 55, niveau);
				gfx_SetTextFGColor(13);
				switch(station->niveauStation){
					case AVANT_POSTE:
						strcpy(nomDeOrdre, "port stellaire");
						break;
					case PORT_STELLAIRE:
						strcpy(nomDeOrdre, "redoute stellaire");
						break;
					case REDOUTE_STELLAIRE:
						strcpy(nomDeOrdre, "forteresse stellaire");
						break;
					case FORTERESSE_STELLAIRE:
						strcpy(nomDeOrdre, "citadelle");
						break;
				}
				niveau += 9;
				gfx_SetTextXY(55, niveau);
				gfx_PrintString(nomDeOrdre);
				sprintf(numero, "(%d", ((ordre->tempsTotal - ordre->tempsActuel) * 100) / ordre->tempsTotal);
				gfx_PrintString(numero);
				gfx_PrintString("%)");
				break;
			case CONSTRUIRE_MODULE:
				strcpy(nomDeOrdre, "Construit le module ");
				sprintf(numero, "%d", numeroDuModule);
				strcat(nomDeOrdre, numero);
				sprintf(numero, "(%d)", (ordre->tempsTotal - ordre->tempsActuel) * 8);
				strcat(nomDeOrdre, numero);
				gfx_PrintStringXY(nomDeOrdre, 55, niveau);
				break;
			case CONSTRUIRE_PLATEFORME:
				strcpy(nomDeOrdre, "Construit une plateforme");
				gfx_PrintStringXY(nomDeOrdre, 55, niveau);
				break;
			case CONSTRUIRE_VAISSEAU:
				strcpy(nomDeOrdre, "Construit un vaisseau ");
				gfx_PrintStringXY(nomDeOrdre, 55, niveau);
				niveau += 9;
				memset(nomDeOrdre, 0, sizeof(nomDeOrdre));
				switch(ordre->info1) {
					case 1:
						strcat(nomDeOrdre, "Scientifique");
						travail = 3;
						break;
					case 2:
						strcat(nomDeOrdre, "Construction");
						travail = 3;
						break;
					case 3:
						strcat(nomDeOrdre, "Corvette");
						travail = 3;
						break;
					case 4:
						strcat(nomDeOrdre, "Destroyer");
						travail = 6;
						break;
					case 5:
						strcat(nomDeOrdre, "Croiseur");
						travail = 12;
						break;
					case 6:
						strcat(nomDeOrdre, "Cuirass/");
						travail = 24;
						break;
				}
				gfx_SetTextXY(55, niveau);
				sprintf(numero, "(%d", ((ordre->tempsTotal - ordre->tempsActuel) * 100) / ordre->tempsTotal);
				strcat(nomDeOrdre, numero);
				gfx_PrintString(nomDeOrdre);
				gfx_PrintString("%)");
				break;
		}
		nombredOrdres = NombredOrdres(station->ordreFile);
		if(nombredOrdres > 1){
			gfx_PrintString("(+");
			gfx_SetTextFGColor(13);
			PrintInt(nombredOrdres - 1);
			gfx_SetTextFGColor(1);
			gfx_PrintString(")");
		}
	}
	return nomDeOrdre;
}

/**
 *Dessine le menu des modules de la station du systeme
 */
void MenuSystemeStationModules(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	Ordre *ordre = RecupererOrdre(systemeStellaires[camera->systeme].station->ordreFile);
	int niveau = 120;
	char ordreStation[50];
	int modules = 0, supprimer = 0;
	Module *module = NULL;
	int numero = 0;
	char nomDuModule[50];
	switch(*key){
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
		case sk_Right:
			fenetre->ouverte = MENU_SYSTEME_STATION_CHANTIER;
			fenetre->selection = 1;
			*key = 0;
			break;
		case sk_Left:
			fenetre->ouverte = MENU_SYSTEME_STATION_RESUME;
			*key = 0;
			break;
	}
	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);
	gfx_FillRectangle_NoClip(40, 200, 52, 10);
	gfx_FillRectangle_NoClip(152, 200, 68, 10);

	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_HorizLine_NoClip(45, 51, 180);
	gfx_Line_NoClip(225, 51, 230, 46);
	gfx_HorizLine_NoClip(230, 45, 40);
	gfx_FillRectangle_NoClip(92, 200, 60, 10);//barre du bas
	gfx_Rectangle_NoClip(40, 199, 180, 11);

	gfx_HorizLine_NoClip(45, 178, 230);//barre du bas au dessus des ordres

	gfx_SetColor(1);
	gfx_SetPixel(270, 45);

	gfx_PrintStringXY("R/sum/", 42, 201);
	gfx_PrintStringXY("Modules", 94, 201);
	gfx_PrintStringXY("Chantier", 154, 201);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Station de ");
	gfx_PrintString(systemeStellaires[camera->systeme].nom);
	gfx_SetTextXY(150, 62);

	switch(systemeStellaires[camera->systeme].station->niveauStation){
		case CITADELLE:
		case FORTERESSE_STELLAIRE:
			modules = 6;
			break;
		case REDOUTE_STELLAIRE:
			modules = 4;
			break;
		case PORT_STELLAIRE:
			modules = 2;
			break;
		case AVANT_POSTE:
			modules = 0;
			break;
	}
	if(ordre != NULL){
		supprimer = 1;
	}
	if(fenetre->selection < 1){
		fenetre->selection = 1;
	}
	if((fenetre->selection == 6) && (modules != 6)){
		fenetre->selection = modules;
	}
	if((fenetre->selection > modules) && (supprimer == 0)){
		fenetre->selection = modules;
	}
	if((fenetre->selection > modules) && (supprimer == 1)){
		fenetre->selection = 7;
	}
	if(fenetre->precedente != 0){
		switch(fenetre->selection){
			case 1:
				module = &(systemeStellaires[camera->systeme].station->module1);
				numero = 1;
				break;
			case 2:
				module = &(systemeStellaires[camera->systeme].station->module2);
				numero = 2;
				break;
			case 3:
				module = &(systemeStellaires[camera->systeme].station->module3);
				numero = 3;
				break;
			case 4:
				module = &(systemeStellaires[camera->systeme].station->module4);
				numero = 4;
				break;
			case 5:
				module = &(systemeStellaires[camera->systeme].station->module5);
				numero = 5;
				break;
			case 6:
				module = &(systemeStellaires[camera->systeme].station->module6);
				numero = 6;
				break;
		}
		if(*module != fenetre->precedente){
			if(fenetre->precedente == 8){
				fenetre->precedente = 0;
				*module = AUCUN;
			}
			if(fenetre->precedente != 0){
				if(joueur->acier >= 50){
					joueur->acier -= 50;
					NouvelOrdre(systemeStellaires[camera->systeme].station->ordreFile,
						CONSTRUIRE_MODULE, 
						12,
						numero,
						fenetre->precedente,
						50
						);
				}
			}
		}
		fenetre->precedente = 0;
	}
	niveau = 55;
	if(modules >= 2){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 1){gfx_SetTextFGColor(13);}
		gfx_SetTextXY(144 - strlen("Module") * 4, niveau);
		gfx_PrintString("Module");
		gfx_PrintString(" 1 :");
		niveau += 10;
		ModuleNom(systemeStellaires[camera->systeme].station->module1, nomDuModule);
		gfx_SetTextXY(160 - strlen(nomDuModule) * 4, niveau);
		gfx_PrintString(nomDuModule);
		niveau += 15;
		//module 2
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 2){gfx_SetTextFGColor(13);}
		gfx_SetTextXY(144 - strlen("Module") * 4, niveau);
		gfx_PrintString("Module");
		gfx_PrintString(" 2 :");
		niveau += 10;
		ModuleNom(systemeStellaires[camera->systeme].station->module2, nomDuModule);
		gfx_SetTextXY(160 - strlen(nomDuModule) * 4, niveau);
		gfx_PrintString(nomDuModule);
		niveau += 15;
	}
	if(modules >= 4){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 3){gfx_SetTextFGColor(13);}
		gfx_SetTextXY(144 - strlen("Module") * 4, niveau);
		gfx_PrintString("Module");
		gfx_PrintString(" 3 :");
		niveau += 10;
		ModuleNom(systemeStellaires[camera->systeme].station->module3, nomDuModule);
		gfx_SetTextXY(160 - strlen(nomDuModule) * 4, niveau);
		gfx_PrintString(nomDuModule);
		niveau += 15;
		//module 4
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 4){gfx_SetTextFGColor(13);}
		gfx_SetTextXY(144 - strlen("Module") * 4, niveau);
		gfx_PrintString("Module");
		gfx_PrintString(" 4 :");
		niveau += 10;
		ModuleNom(systemeStellaires[camera->systeme].station->module4, nomDuModule);
		gfx_SetTextXY(160 - strlen(nomDuModule) * 4, niveau);
		gfx_PrintString(nomDuModule);
		niveau += 15;
	}
	if(modules >= 6){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 5){gfx_SetTextFGColor(13);}
		gfx_SetTextXY(144 - strlen("Module") * 4, niveau);
		gfx_PrintString("Module");
		gfx_PrintString(" 5 :");
		niveau += 10;
		ModuleNom(systemeStellaires[camera->systeme].station->module5, nomDuModule);
		gfx_SetTextXY(160 - strlen(nomDuModule) * 4, niveau);
		gfx_PrintString(nomDuModule);
		niveau += 15;
		//module 6
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 6){gfx_SetTextFGColor(13);}
		gfx_SetTextXY(144 - strlen("Module") * 4, niveau);
		gfx_PrintString("Module");
		gfx_PrintString(" 6 :");
		niveau += 10;
		ModuleNom(systemeStellaires[camera->systeme].station->module6, nomDuModule);
		gfx_SetTextXY(160 - strlen(nomDuModule) * 4, niveau);
		gfx_PrintString(nomDuModule);
		niveau += 15;
	}

	//écrire ordre
	niveau = 181;
	if(ordre != NULL){
		if(fenetre->selection == 7){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(3);
		}
		gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);// croix
		gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);// croix
	}
	gfx_SetTextFGColor(1);
	gfx_SetTextXY(45, 190);
	OrdreStationNom(systemeStellaires[camera->systeme].station, ordre->info1, ordreStation, niveau);
	
	if(*key == sk_Enter){
		if((supprimer == 1) && (fenetre->selection == 7)){
			joueur->acier += ordre->prix;
			FinirOrdre(systemeStellaires[camera->systeme].station->ordreFile);
		}
		else{
			fenetre->ouverte = MENU_SYSTEME_STATION_MODULES_CHOIX;
			fenetre->precedente = fenetre->selection;
			fenetre->selection = 0;
			fenetre->scroll = 1;
			*key = 0;
		}
	}
}

/**
 *Renvoie le nom du module
 */
char* ModuleNom(Module module, char* nomDuModule){
	switch(module){
		case AUCUN:
			strcpy(nomDuModule, "Aucun");
			break;
		case CHANTIER_SPATIAL:
			strcpy(nomDuModule, "Chantier Spatial");
			break;
		case ANCRAGE:
			strcpy(nomDuModule, "Ancrage");
			break;
		case CANONS:
			strcpy(nomDuModule, "Batterie de canons");
			break;
		case MISSILES:
			strcpy(nomDuModule, "Batterie lance-missiles");
			break;
		case HANGAR:
			strcpy(nomDuModule, "Hangar");
			break;
		case CARREFOUR_COMMERCIAL:
			strcpy(nomDuModule, "Carrefour commercial");
			break;
		case PANNEAUX_SOLAIRES:
			strcpy(nomDuModule, "Panneaux solaires");
			break;
	}
	return nomDuModule;
}

/**
 *Choisi un module
 */
void MenuSystemeStationModulesChoix(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	int niveau = 53;
	char nomDuModule[50];
	int i = 0;
	switch(*key){
		case sk_Clear:
			fenetre->ouverte = MENU_SYSTEME_STATION_MODULES;
			fenetre->selection = 0;
			fenetre->precedente = 0;
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
	if(fenetre->selection > 7){
		fenetre->selection = 0;
	}
	if(fenetre->selection < 0){
		fenetre->selection = 7;
	}
	if(fenetre->selection == 0){
		fenetre->scroll = 1;
	}
	if((fenetre->selection == 1) && (fenetre->scroll == 3)){
		fenetre->scroll = 2;
	}
	if((fenetre->selection == 6) && (fenetre->scroll == 1)){
		fenetre->scroll = 3;
	}
	if(fenetre->selection == 7){
		fenetre->scroll = 3;
	}
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);

	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);

	gfx_HorizLine_NoClip(45, 51, 180);
	gfx_Line_NoClip(225, 51, 230, 46);
	gfx_HorizLine_NoClip(230, 45, 40);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Module ");
	PrintInt(fenetre->precedente);
	
	if(fenetre->scroll <= 1){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 0){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY("D/manteler", 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		PrintInt(0);
		niveau += 15;
	}

	if(fenetre->scroll <= 2){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 1){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY(ModuleNom(CHANTIER_SPATIAL, nomDuModule), 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		if(joueur->acier < 50){
			gfx_SetTextFGColor(3);
		}
		PrintInt(50);
		niveau += 15;
	}

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 2){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(ANCRAGE, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 3){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(CANONS, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 4){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(MISSILES, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(fenetre->selection == 5){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(HANGAR, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(joueur->acier < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	if(fenetre->scroll >= 2){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 6){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY(ModuleNom(CARREFOUR_COMMERCIAL, nomDuModule), 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		if(joueur->acier < 50){
			gfx_SetTextFGColor(3);
		}
		PrintInt(50);
		niveau += 15;
	}

	if(fenetre->scroll >= 3){
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 7){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY(ModuleNom(PANNEAUX_SOLAIRES, nomDuModule), 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		if(joueur->acier < 50){
			gfx_SetTextFGColor(3);
		}
		PrintInt(50);
		niveau += 15;
	}

	if(*key == sk_Enter){
		if((joueur->acier >= 50) && (fenetre->selection != 1)){
			if(fenetre->selection == 0){
				fenetre->selection = 8;
			}
			fenetre->ouverte = MENU_SYSTEME_STATION_MODULES;
			i = fenetre->precedente;
			fenetre->precedente = fenetre->selection;	
			fenetre->selection = i;
			*key = 0;
		}
	}
}

/**
 *Dessine le menu des modules de la station du systeme
 */
void MenuSystemeStationChantier(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	char evolution[25] = {0};
	int prixAmelioration = 0;
	char ordreStation[50];
	int niveau = 55, nombreDeChantiers = 0, travail = 0, prix = 0;
	Ordre *ordre = RecupererOrdre(systemeStellaires[camera->systeme].station->ordreFile);
	switch(*key){
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
		case sk_Left:
			fenetre->ouverte = MENU_SYSTEME_STATION_MODULES;
			fenetre->precedente = 0;
			*key = 0;
			break;
	}
	if(fenetre->selection < 1){
		fenetre->selection = 1;
	}
	else if((fenetre->selection > 7) && (ordre != NULL)){
		fenetre->selection = 7;
	}
	else if(fenetre->selection > 6){
		fenetre->selection = 6;
	}
	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);
	gfx_FillRectangle_NoClip(40, 200, 52, 10);
	gfx_FillRectangle_NoClip(92, 200, 60, 10);

	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_HorizLine_NoClip(45, 51, 180);
	gfx_Line_NoClip(225, 51, 230, 46);
	gfx_HorizLine_NoClip(230, 45, 40);
	gfx_FillRectangle_NoClip(152, 200, 68, 10);//barre du bas
	gfx_Rectangle_NoClip(40, 199, 180, 11);
	gfx_VertLine_NoClip(92, 201, 8);

	gfx_HorizLine_NoClip(45, 178, 230);//barre du bas au dessus des ordres

	gfx_SetColor(1);
	gfx_SetPixel(270, 45);

	gfx_PrintStringXY("R/sum/", 42, 201);
	gfx_PrintStringXY("Modules", 94, 201);
	gfx_PrintStringXY("Chantier", 154, 201);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Station de ");
	gfx_PrintString(systemeStellaires[camera->systeme].nom);
	if(systemeStellaires[camera->systeme].station->module1 == CHANTIER_SPATIAL){
		nombreDeChantiers++;
	}
	if(systemeStellaires[camera->systeme].station->module2 == CHANTIER_SPATIAL){
		nombreDeChantiers++;
	}
	if(systemeStellaires[camera->systeme].station->module3 == CHANTIER_SPATIAL){
		nombreDeChantiers++;
	}
	if(systemeStellaires[camera->systeme].station->module4 == CHANTIER_SPATIAL){
		nombreDeChantiers++;
	}
	if(systemeStellaires[camera->systeme].station->module5 == CHANTIER_SPATIAL){
		nombreDeChantiers++;
	}
	if(systemeStellaires[camera->systeme].station->module6 == CHANTIER_SPATIAL){
		nombreDeChantiers++;
	}

	//verifie que la station soit avec un chantier spatial
	if(nombreDeChantiers > 0){
		gfx_SetTextXY(45, niveau);
		gfx_PrintString("Chantiers : ");
		gfx_PrintInt(nombreDeChantiers, 1);

		if(fenetre->selection == 1){
			gfx_SetTextFGColor(13);
			travail = 3;
			prix = 100;
		}
		niveau += 20;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Scientifique ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(joueur->acier < 100){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("100");
		
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 2){
			gfx_SetTextFGColor(13);
			travail = 3;
			prix = 100;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Construction ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(joueur->acier < 100){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("100");
		
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 3){
			gfx_SetTextFGColor(13);
			travail = 3;
			prix = 100;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Corvette ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(joueur->acier < 100){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("100");
		
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 4){
			gfx_SetTextFGColor(13);
			travail = 6;
			prix = 200;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Destroyer ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(joueur->acier < 200){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("200");
		
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 5){
			gfx_SetTextFGColor(13);
			travail = 12;
			prix = 500;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Croiseur ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(joueur->acier < 500){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("500");
		
		gfx_SetTextFGColor(1);
		if(fenetre->selection == 6){
			gfx_SetTextFGColor(13);
			travail = 24;
			prix = 1000;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Cuirass/ "); //battleship
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(joueur->acier < 1000){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("1000");
		
		//écrire ordre
		niveau = 181;
		if(ordre != NULL){
			if(fenetre->selection == 7){
				gfx_SetColor(13);
			}
			else{
				gfx_SetColor(3);
			}
			gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);// croix
			gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);// croix
		}
		gfx_SetTextFGColor(1);
		gfx_SetTextXY(45, 190);
		OrdreStationNom(systemeStellaires[camera->systeme].station, ordre->info1, ordreStation, niveau);
		

		if(*key == sk_Enter){
			if(fenetre->selection == 7){
				joueur->acier += ordre->prix;
				FinirOrdre(systemeStellaires[camera->systeme].station->ordreFile);
			}
			else if(fenetre->selection >= 3){
				fenetre->ouverte = MENU_SYSTEME_STATION_CHANTIER_CHOIX;
				fenetre->nombreDeVaisseaux = 1;
			}
			else{
				if(joueur->acier >= prix){
					joueur->acier -= prix;
					NouvelOrdre(systemeStellaires[camera->systeme].station->ordreFile,
						CONSTRUIRE_VAISSEAU,
						travail,
						fenetre->selection,
						1,
						prix
					);
				}
			}
		}
	

	}
	else{
		gfx_PrintStringXY("Pas de chantier spatial", 68, 90);
	}
}


/**
 *Choisi un module
 */
void MenuSystemeStationChantierChoix(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre){
	int niveau = 53;
	int i = 0, prix = 0, travail = 0;
	char nom[50];
	switch(*key){
		case sk_Clear:
			fenetre->ouverte = MENU_SYSTEME_STATION_CHANTIER;
			*key = 0;
			break;
		case sk_Right:
			fenetre->nombreDeVaisseaux++;
			*key = 0;
			break;
		case sk_Left:
			fenetre->nombreDeVaisseaux--;
			*key = 0;
			break;
		case sk_Add:
			fenetre->nombreDeVaisseaux++;
			*key = 0;
			break;
		case sk_Sub:
			fenetre->nombreDeVaisseaux--;
			*key = 0;
			break;
		case sk_1:
			fenetre->nombreDeVaisseaux = 1;
			*key = 0;
			break;
		case sk_2:
			fenetre->nombreDeVaisseaux = 2;
			*key = 0;
			break;
		case sk_3:
			fenetre->nombreDeVaisseaux = 3;
			*key = 0;
			break;
		case sk_4:
			fenetre->nombreDeVaisseaux = 4;
			*key = 0;
			break;
		case sk_5:
			fenetre->nombreDeVaisseaux = 5;
			*key = 0;
			break;
		case sk_6:
			fenetre->nombreDeVaisseaux = 6;
			*key = 0;
			break;
		case sk_7:
			fenetre->nombreDeVaisseaux = 7;
			*key = 0;
			break;
		case sk_8:
			fenetre->nombreDeVaisseaux = 8;
			*key = 0;
			break;
		case sk_9:
			fenetre->nombreDeVaisseaux = 9;
			*key = 0;
			break;
	}
	if(fenetre->nombreDeVaisseaux < 0){
		fenetre->nombreDeVaisseaux = 0;
	}
	if(fenetre->nombreDeVaisseaux > 20){
		fenetre->nombreDeVaisseaux = 20;
	}
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);

	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);

	gfx_HorizLine_NoClip(45, 51, 180);
	gfx_Line_NoClip(225, 51, 230, 46);
	gfx_HorizLine_NoClip(230, 45, 40);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Constuire ");
	switch(fenetre->selection){
		case 3:
			strcpy(nom, "Corvette");
			travail = 3;
			prix = 100;
			break;
		case 4:
			strcpy(nom, "Destroyer");
			travail = 6;
			prix = 200;
			break;
		case 5:
			strcpy(nom, "Croiseur");
			travail = 12;
			prix = 500;
			break;
		case 6:
			strcpy(nom, "Cuirass/");
			travail = 24;
			prix = 1000;
			break;
	}
	gfx_PrintString(nom);
	gfx_SetTextXY(45, 60);
	gfx_PrintString("Nombre de vaisseaux : ");
	PrintInt(fenetre->nombreDeVaisseaux);
	gfx_SetTextXY(60, 74);
	gfx_TransparentSprite_NoClip(fer, 45, 74);
	if(fenetre->nombreDeVaisseaux * prix > joueur->acier){
		gfx_SetTextFGColor(3);
	}
	PrintInt(fenetre->nombreDeVaisseaux * prix);

	if(*key == sk_Enter){
		if(joueur->acier >= prix * fenetre->nombreDeVaisseaux){
			joueur->acier -= prix * fenetre->nombreDeVaisseaux;
			NouvelOrdre(systemeStellaires[camera->systeme].station->ordreFile,
				CONSTRUIRE_VAISSEAU,
				travail * fenetre->nombreDeVaisseaux,
				fenetre->selection,
				fenetre->nombreDeVaisseaux,
				prix * fenetre->nombreDeVaisseaux
			);
			fenetre->ouverte = MENU_SYSTEME_STATION_RESUME;
		}
		*key = 0;
	}
}

/**
 *Dessine le menu du marché galactique
 */
void MenuMarche(char *key, Empire* joueur, Marche *marche, Camera *camera, Fenetre *fenetre){
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

/**
 *Dessine le menu avec la lste des flottes du joueur
 */
void MenuListeFLottes(char *key, Empire *joueur, Camera *camera, Fenetre *fenetre){
	char niveau = 0;
	int8_t compteur = 0, compteurFlotte = 0;
	Flotte* flotte = NULL;

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

/**
 *Dessine le menu de recherche
 */
void MenuRecherche(char *key, Camera *camera, Fenetre *fenetre){
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

/**
 *Dessine le menu avec les contacts
 */
void MenuContacts(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre){
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
