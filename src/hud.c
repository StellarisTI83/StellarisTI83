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
#include "console.h"
#include "flottes.h"
#include "hud.h"
#include "menus.h"
#include "parametres.h"
#include "time.h"

/* private functions =================================================== */

/**
 *Exprime à l'écran les données envoyées pour la abrre du haut
 */
static void PrintHUD(const unsigned int nombre, const int change, int x, int y) {
	unsigned char longueur, color;
	char nombreStr[10];
	char changeStr[5];

	//ecrire nombre
	if(nombre < 1000){
		sprintf(nombreStr, "%d", nombre);
	} else {
		sprintf(nombreStr, "%d", nombre/1000);
		strcat(nombreStr, "K");
	}

	//ecrire change
	if (change >=0){
		gfx_SetTextFGColor(1);
		sprintf(changeStr, "+%d", change);
	} else {
		gfx_SetTextFGColor(3);
		sprintf(changeStr, "%d", change);
	}

	strcat(nombreStr, changeStr);

	gfx_PrintStringXY(nombreStr, x, y);
}

static void DrawBackgroundHUD(Date *date, Camera *camera){
	//dessine le fond du hud
	gfx_SetColor(6);
	//barre du haut
	gfx_FillRectangle(0, 0 , LCD_WIDTH, 22);
	//barre du coté gauche
	gfx_FillRectangle(0, 0, 20, 200);
	gfx_FillTriangle(0, 200, 20, 200, 0, 220);
	//barre du bas
	if (GetTimeSpeed(date) == 0)
	{
		gfx_SetColor(10);
	}
	gfx_FillRectangle(100, 220, 120, 20);
	gfx_FillTriangle(100, 220, 80, 240, 100, 240);
	gfx_FillTriangle(220, 220, 240, 240, 220, 240);
	//nom selection
	if (((GetCameraViewedSystem(camera) != -1) || (GetCameraMapType(camera) == CARTE)) || (GetCameraMapType(camera) == SYSTEME)) {
		gfx_FillRectangle(110, 210, 100, 10);
		gfx_FillTriangle(110, 210, 100, 220, 110, 220);
		gfx_FillTriangle(210, 210, 210, 220, 220, 220);
	}
}

static void DrawOutlinesHUD(Date *date, Camera *camera){
	//dessine les bord du hud
	gfx_SetColor(7);
	//barre du haut
	gfx_HorizLine_NoClip(20, 22, LCD_WIDTH-20);
	//barre du coté gauche
	gfx_VertLine_NoClip(20, 22, 179);
	gfx_Line_NoClip(20, 200, 0, 220);
	//barre du bas
	if (GetTimeSpeed(date) == 0)
	{
		gfx_SetColor(9);
	}
	gfx_HorizLine_NoClip(100, 220, 120);
	gfx_Line_NoClip(100, 220, 81, 239);
	gfx_Line_NoClip(220, 220, 239, 239);
	//nom selection
	if (((GetCameraViewedSystem(camera) != -1) || (GetCameraMapType(camera) == CARTE)) || (GetCameraMapType(camera) == SYSTEME))
	{
		gfx_HorizLine_NoClip(110, 210, 100);
		gfx_Line_NoClip(110, 210, 100, 220);
		gfx_Line_NoClip(210, 210, 220, 220);
	}
}

static void DrawFlagHUD(){
	//point de l'hexagone
	int drapeau[12] = {
		17, 0,
		0, 10,
		0, 30,
		17, 40,
		35, 30,
		35, 10,
	};
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
}

static void WriteTextHUD(Empire *joueur, Date *date, Camera *camera){
    char jourChar[11];
	char moisChar[8];
	char anneeChar[5];
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
	
	//texte
	gfx_SetTextFGColor(8);
	//barre du haut
	gfx_TransparentSprite_NoClip(credit, 55 ,1);
	PrintHUD(GetEmpireCredit(joueur), GetEmpireCreditChange(joueur), 40, 13);
	gfx_TransparentSprite_NoClip(minerai, 100 ,1);
	PrintHUD(GetEmpireMinerals(joueur), GetEmpireMineralsChange(joueur), 85, 13);
	gfx_TransparentSprite_NoClip(food, 145 ,1);
	PrintHUD(GetEmpireFood(joueur), GetEmpireFoodChange(joueur), 130, 13);
	gfx_TransparentSprite_NoClip(fer, 190 ,1);
	PrintHUD(GetEmpireAlloys(joueur), GetEmpireAlloysChange(joueur), 175, 13);
	
	//barre de gauche
	gfx_TransparentSprite_NoClip(contact, 5, 50);
	gfx_TransparentSprite_NoClip(market_icon, 5, 70);
	gfx_TransparentSprite_NoClip(science, 5, 90);
	gfx_TransparentSprite_NoClip(alliedFleet, 5, 110);
	
	//barre du bas
	//date
	if (GetTimeDay(date) < 10) {
		sprintf(jourChar, "0%d.", GetTimeDay(date));
	} else {
		sprintf(jourChar, "%d.", GetTimeDay(date));
	}
	if (GetTimeMonth(date) < 10) {
		sprintf(moisChar, "0%d.", GetTimeMonth(date));
	} else {
		sprintf(moisChar, "%d.", GetTimeMonth(date));
	}
	sprintf(anneeChar, "%d", GetTimeYear(date));
	strcat(moisChar, anneeChar);
	strcat(jourChar, moisChar);
	gfx_PrintStringXY(jourChar, 125, 225);
	if(IsCameraMoveFleet(camera) == TRUE){
		gfx_SetColor(6);
		gfx_FillRectangle_NoClip(80, 28, 160, 11);
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(80, 28, 160, 11);
		gfx_PrintStringXY("Choisir destination", 84, 30);
	}
}

static void DrawPauseHUD(Date *date){
	//pause / avance
	switch (GetTimeSpeed(date)){
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
}

static void DrawPointerHUD(Camera *camera){
	//pointeur
	if (GetCameraMapType(camera) == CARTE) {
		gfx_Line_NoClip(GetCameraX(camera) / 2.5 + 5, GetCameraY(camera) / 2.5 - 30, GetCameraX(camera) / 2.5 + 15, GetCameraY(camera) / 2.5 - 30);
		gfx_Line_NoClip(GetCameraX(camera) / 2.5 + 10, GetCameraY(camera) / 2.5 - 25, GetCameraX(camera) / 2.5 + 10, GetCameraY(camera) / 2.5 - 35);
		gfx_Circle_NoClip(GetCameraX(camera) / 2.5 + 10, GetCameraY(camera) / 2.5 - 30, 3);
	} else {
		gfx_Line_NoClip(150, 120, 170, 120);
		gfx_Line_NoClip(160, 110, 160, 130);
		gfx_FillCircle_NoClip(160, 120, 2);
		gfx_Circle_NoClip(160, 120, 4);
		gfx_Circle_NoClip(160, 120, 8);
	}
}

static void WriteGalaxieNameHUD(Camera *camera, SystemeStellaire **systemeStellaires){
    int16_t systeme = 0;
	//nom galaxie
	if (((GetCameraViewedSystem(camera) != -1) || (GetCameraMapType(camera) == CARTE)) || (GetCameraMapType(camera) == SYSTEME)) {
		if (GetCameraMapType(camera) == NORMAL) {
			systeme = GetCameraViewedSystem(camera);
		} 
		else {
			systeme = GetCameraSystem(camera);
		}
		if (GetSystemIntelLevel(systemeStellaires[systeme]) == INCONNU) {
			gfx_PrintStringXY("Inconnu", 132, 211);
		}
		else {
			gfx_PrintStringXY(GetSystemName(systemeStellaires[systeme]), 160 - strlen(GetSystemName(systemeStellaires[systeme])) * 4, 211);
		}
	}
}

/* entry points ======================================================== */

int DrawHUD(EmpireListe *empireListe, Date *date, char *key, Camera *camera, SystemeStellaire **systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche){

	char valeurMineraiStr[4];
	char nomPlanete[20];
	int menu = 0;
    Empire *joueur = EmpireNumero(empireListe, 1);
	
	DrawBackgroundHUD(date, camera);
	DrawOutlinesHUD(date, camera);
	DrawFlagHUD();

	WriteTextHUD(joueur, date, camera);
	
    DrawPauseHUD(date);
	
    DrawPointerHUD(camera);

    WriteGalaxieNameHUD(camera, systemeStellaires);
	
	if (GetCameraMapType(camera) == CARTE) {
		gfx_PrintStringXY("Carte", 140, 211);
	}

	if(GetCommandPromptStatus(fenetre) == true){ 
		AfficherConsole(key, fenetre, empireListe, camera, date, parametres);
	}
	return menu;
}
