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
#include "flottes.h"

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
	gfx_SetMonospaceFont(8);
	gfx_SetTextBGColor(255);

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
	int espacement = 0, niveau = 0;

	gfx_SetDrawScreen();

	/*imprimer tout*/
	gfx_ZeroScreen();
    PrintCentered("Stellaris", 40, 3, 1, -30);
	gfx_SetMonospaceFont(8);
	gfx_SetTextBGColor(255);
	gfx_SetTextFGColor(1);
	gfx_SetColor(1);
	gfx_PrintStringXY(VERSION_LOGICIEL, 5, 205);
	gfx_SetTextXY(LCD_WIDTH - strlen(__DATE__) * 8 - strlen(__TIME__) * 8 - 36, 205);
	gfx_PrintString(" ");
	gfx_PrintString(__DATE__);
	gfx_PrintString(" ");
	gfx_PrintString(__TIME__);

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
		gfx_SetTextXY(5, 140);
		niveau = 100;
		espacement = 15;
		/*dessiner le choix*/
		switch (choix){
		case 0:
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
			gfx_PrintStringXY(_(LC_CHARGER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_CHARGER)) * 8);
			gfx_SetTextFGColor(1);
			gfx_SetColor(1);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_NOUVELLE_PARTIE), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_NOUVELLE_PARTIE)) * 8);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_OPTIONS), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_OPTIONS)) * 8);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_QUITTER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_QUITTER)) * 8);
			break;
		case 1:
			gfx_PrintStringXY(_(LC_CHARGER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_CHARGER)) * 8);

			niveau += espacement;
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
			gfx_PrintStringXY(_(LC_NOUVELLE_PARTIE), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_NOUVELLE_PARTIE)) * 8);
			gfx_SetTextFGColor(1);
			gfx_SetColor(1);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_OPTIONS), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_OPTIONS)) * 8);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_QUITTER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_QUITTER)) * 8);
			break;
		case 2:
			gfx_PrintStringXY(_(LC_CHARGER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_CHARGER)) * 8);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_NOUVELLE_PARTIE), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_NOUVELLE_PARTIE)) * 8);

			niveau += espacement;
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
			gfx_PrintStringXY(_(LC_OPTIONS), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_OPTIONS)) * 8);
			gfx_SetTextFGColor(1);
			gfx_SetColor(1);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_QUITTER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_QUITTER)) * 8);
			break;
		case 3:
			gfx_PrintStringXY(_(LC_CHARGER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_CHARGER)) * 8);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_NOUVELLE_PARTIE), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_NOUVELLE_PARTIE)) * 8);

			niveau += espacement;
			gfx_PrintStringXY(_(LC_OPTIONS), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_OPTIONS)) * 8);

			niveau += espacement;
			gfx_SetTextFGColor(13);
			gfx_SetColor(13);
			gfx_PrintStringXY(_(LC_QUITTER), 5, niveau);
			gfx_HorizLine_NoClip(5, niveau + 10, strlen(_(LC_QUITTER)) * 8);
			gfx_SetTextFGColor(1);
			gfx_SetColor(1);
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
 *Renvoie la taille du int envoyé
 */
int tailleInt(int nombre){
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
	Empire *pointeur = 0;
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

//crèe une file d'ordre
OrdreFile *CreerFileOrdres(){
	OrdreFile *ordreFile = NULL;
	ordreFile = malloc(sizeof(OrdreFile));
	ordreFile->premierOrdre = NULL;
	return ordreFile;
}

//supprime une file d'ordre
void SupprimerFileOrdres(OrdreFile *ordreFile){
	Ordre *ordre = NULL, *ordreSuivant = NULL;
	ordre = ordreFile->premierOrdre;
	while(ordre != NULL){
		ordreSuivant = ordre->ordreSuivant;
		free(ordre);
		ordre = ordreSuivant;
	}
	free(ordreFile);
}

//crèe un ordre
void NouvelOrdre(OrdreFile* ordreFile, int ordre, int tempsTotal, int info1, int info2, int prix){
	Ordre *ordreElement = NULL, *ordrePrecedent = NULL;

	if(ordreFile->premierOrdre == NULL){
		ordreFile->premierOrdre = calloc(1, sizeof(Ordre));
		ordreElement = ordreFile->premierOrdre;
	}
	else{
		ordreElement = ordreFile->premierOrdre;
		while(ordreElement->ordreSuivant != NULL){
			ordreElement = ordreElement->ordreSuivant;
		}
		ordreElement->ordreSuivant = calloc(1, sizeof(Ordre));
		ordreElement = ordreElement->ordreSuivant;
	}

	ordreElement->ordre = ordre;
	ordreElement->tempsTotal = tempsTotal;
	ordreElement->tempsActuel = tempsTotal;
	ordreElement->info1 = info1;
	ordreElement->info2 = info2;
	ordreElement->prix = prix;
	ordreElement->ordreSuivant = NULL;
}

//fini le dernier ordre
void FinirOrdre(OrdreFile *ordreFile){
	Ordre *ordre;
	ordre = ordreFile->premierOrdre;
	if(ordre != NULL){
		ordreFile->premierOrdre = ordre->ordreSuivant;
		free(ordre);
	}
}

//renvoie un ordre
Ordre* RecupererOrdre(OrdreFile *ordreFile){
	Ordre *ordre = NULL;
	ordre = ordreFile->premierOrdre;
	return ordre;
}

//renvoie le nombre d'ordres
int NombredOrdres(OrdreFile *ordreFile){
	int nombre = 0;
	Ordre *ordre = NULL;
	ordre = ordreFile->premierOrdre;
	while(ordre != NULL){
		ordre = ordre->ordreSuivant;
		nombre++;
	}
	return(nombre);
}