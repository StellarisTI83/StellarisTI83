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
#include "boucle.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"

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
	gfx_SetFontData(font_logo);

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

/**
 *Imprime un entier de la bonne longueur suivant sa taille
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
 *Crée une list de flottes
 */
FlotteListe* FlotteListeCreer() {
	FlotteListe* flotteliste;
	flotteliste = malloc(sizeof(FlotteListe));
	flotteliste->premier = NULL;
	return flotteliste;
}

/**
 *Supprime une liste des flottes
 */
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

/**
 *Renvoi un pointeur vers l'empire numero x
 */
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

/**
 *Renvoi le numéro de la flotte suivant son pointeur
 */
int FlotteNumeroRecuperer(FlotteListe* flotteliste, Flotte* flotte) {
	Flotte *pointeur = 0;
	int compteur = 1;
	pointeur = flotteliste->premier;
	while((pointeur != flotte) && (pointeur != NULL)) {
		pointeur = pointeur->suivant;
		compteur++;
	}
	compteur--;
	return compteur;
}

/**
 *Rajoute une flotte à la liste des flotte envoyée
 */
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

/**
 *Supprime la flotte numero x à la liste de flottes envoyée
 */
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


/**
 *Crée une liste d'empires
 */
EmpireListe* EmpireListeCreer() {
	EmpireListe* empireListe;
	empireListe = malloc(sizeof(EmpireListe));
	empireListe->premier = NULL;
	return empireListe;
}

/**
 *Supprime une liste d'empire
 */
void EmpireListeSupprimer(EmpireListe* empireListe) {
	Empire *pointeur = 0, *pointeursuivant = 0;
	int compteur = 0;
	pointeur = empireListe->premier;
	while(pointeur->suivant != NULL) {
		pointeursuivant = pointeur->suivant;
		FlotteListeSupprimer(pointeur->flotte);
		free(pointeur);
		pointeur = pointeursuivant;
	}
	free(empireListe);
}

/**
 *Renvoi un pointeur vers l'empire numero x
 */
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

/**
 *Rajoute un empire à la liste des empire
 */
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

/**
 *Supprime l'empire numero x à la liste des empires
 */
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
	FlotteListeSupprimer(pointeur->flotte);
	free(pointeur);
}

/**
 *Donne l'ordre de faire bouger la flotte numero x
 */
void FlotteBouger(int numeroDeFlotte, int numeroDeEmpire, int systeme, Camera *camera, EmpireListe *empireListe, SystemeStellaire* systemeStellaires){
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
	else if(camera->bougerFlotte == TRUE){
		camera->bougerFlotte = FALSE;
		flotte->systemeArrive = systeme;
		flotte->action = FLOTTE_BOUGER;
		flotte->avancement = 5;
		camera->flotte = 0;
		camera->empire = 0;
	}
}

/**
 *Fait effectuer les action des flottes
 */
void EffectuerActionsFlottes(EmpireListe* empireListe, SystemeStellaire* systemeStellaires){
	Empire* empire = NULL;
	Flotte* flotte = NULL;
	empire = empireListe->premier;
	while(empire != NULL){
		flotte = empire->flotte->premier;
		while(flotte != NULL){
			if(flotte->avancement > 0){
				flotte->avancement--;
			}
			else if(flotte->action == FLOTTE_BOUGER){
				flotte->systeme = flotte->systemeArrive;
				systemeStellaires[flotte->systeme].niveauDeConnaissance = FAIBLE;
				flotte->systemeArrive = 0;
				flotte->action = FLOTTE_AUCUNE_ACTION;
			}
			flotte = flotte->suivant;
		}
		empire = empire->suivant;
	}
}
