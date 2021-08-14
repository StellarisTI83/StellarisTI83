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

#include "menus.h"

#include "boucle.h"
#include "console.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"
#include "updating.h"

#include "locale/locale.h"

/* private functions =================================================== */

/**
 * Base pour dessiner les menus
 */
static void DrawMenuBase(char planetePopulation, char niveauMenu, Fenetre *fenetre) {
	int niveau = 40;
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_HorizLine_NoClip(45, 51, 180); //***
	gfx_Line_NoClip(225, 51, 230, 46); //*Barre sous le titre
	gfx_HorizLine_NoClip(231, 45, 39); //***
	gfx_SetColor(1);
	gfx_SetPixel(270, 45);//point au bout de la ligne du titre
	
	if(GetOpenedMenuClass(fenetre) == MENU_SYSTEME) {
		gfx_SetColor(7);
		gfx_Rectangle_NoClip(40, 40, 240, 160);
		gfx_Rectangle_NoClip(40, 199, niveau - 40, 12); //cadre des differents pages

		if(niveauMenu == 0)
			gfx_SetColor(7);
		else
			gfx_SetColor(6);
		gfx_FillRectangle_NoClip(40, 200, strlen("R/sum/") * 8 + 10, 10);
		gfx_PrintStringXY("R/sum/", 45, 201);
		niveau += strlen("R/sum/") * 8 + 10;
		gfx_SetColor(6);

		if(planetePopulation == 0){
			gfx_SetColor(11);
		}
		else if(niveauMenu == 1)
			gfx_SetColor(7);
		else
			gfx_SetColor(6);
		gfx_FillRectangle_NoClip(niveau, 200, strlen("Districts") * 8 + 10, 10);
		gfx_PrintStringXY("Districts", niveau + 5, 201);
		niveau += strlen("Districts") * 8 + 10;
		gfx_VertLine_NoClip(niveau - 1, 201, 8);//barre de séparation
		gfx_SetColor(6);

		if(planetePopulation == 0){
			gfx_SetColor(11);
		}
		else if(niveauMenu == 2)
			gfx_SetColor(7);
		else
			gfx_SetColor(6);
		gfx_FillRectangle_NoClip(niveau, 200, strlen("Batiments") * 8 + 10, 10);
		gfx_PrintStringXY("Batiments", niveau + 5, 201);
		niveau += strlen("Batiments") * 8 + 10;
	}
}

/**
 * Dessine le menu lorsqu'on appui sur echap
 */
static int8_t MenuQuitter(char* key, ti_var_t* sauvegarde, EmpireListe* empireListe, Parametres* parametres, Date* date, SystemeStellaire **systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre){
	switch(*key) {
		default:
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			UnpauseGame(date);
			break;
	}
	if (GetWindowSelection(fenetre) > 5) {
		SetWindowSelection(fenetre, 1);
	} else if (GetWindowSelection(fenetre) < 1) {
		SetWindowSelection(fenetre, 5);
	}
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(100, 40, 120, 160);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(100, 40, 120, 160);
	
	if(GetWindowSelection(fenetre) == 1) {
		gfx_SetColor(13);
	}
	gfx_Rectangle_NoClip(110, 50, 100, 18);
	gfx_SetColor(7);

	if(GetWindowSelection(fenetre) == 2) {
		gfx_SetColor(13);
	}
	gfx_Rectangle_NoClip(110, 80, 100, 18);
	gfx_SetColor(7);

	if(GetWindowSelection(fenetre) == 3) {
		gfx_SetColor(13);
	}
	gfx_Rectangle_NoClip(110, 110, 100, 18);
	gfx_SetColor(7);

	if(GetWindowSelection(fenetre) == 4) {
		gfx_SetColor(13);
	}
	gfx_Rectangle_NoClip(110, 140, 100, 18);
	gfx_SetColor(7);
	
	if(GetWindowSelection(fenetre) == 5) {
		gfx_SetColor(13);
	}
	gfx_Rectangle_NoClip(110, 170, 100, 18);
	gfx_SetColor(7);

	gfx_PrintStringXY(_(lc_back), 160 - strlen(_(lc_back)) * 4, 55);
	gfx_PrintStringXY(_(lc_save), 160 - strlen(_(lc_save)) * 4, 85);
	gfx_PrintStringXY(_(lc_load), 160 - strlen(_(lc_load)) * 4, 115);
	gfx_PrintStringXY(_(lc_settings), 160 - strlen(_(lc_settings)) * 4, 145);
	gfx_PrintStringXY(_(lc_exit), 160 - strlen(_(lc_exit)) * 4, 175);
	if(*key == sk_Enter)
	{
		switch(GetWindowSelection(fenetre))
		{
			case 1:
				CloseMenu(fenetre, camera);
				break;
			case 2:
				CloseMenu(fenetre, camera);
				break;
			case 5:
				return 0;
				break;
			default:
				break;
		}
	}
	return 1;
}

/**
 * Dessine le menu des flottes du système
 */
static void MenuSystemeFlotte(char* key, EmpireListe* empireListe, SystemeStellaire **systemeStellaires, Camera* camera, Fenetre* fenetre){
	int niveau = 0, i = 0;
	int8_t compteur = 0, compteurFlotte = 0;
	int fleetIndex = 0;
	int sizeFleet = 0;
	Flotte* flotte = NULL;
	Flotte* flotteDuSysteme[20];
	Empire *joueur = EmpireNumero(empireListe, 0);
	memset(flotteDuSysteme, 0, sizeof(Flotte*) * 20);

	switch(*key) {
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Left:
			SetWindowSelection(fenetre, 0);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
	}

	//dessiner fenetre
	gfx_PrintStringXY("Vue flottes", 116, 42);
	niveau = 57;
	flotte = FlotteNumero(EmpireFleetGetArray(joueur), 1);
	compteur = 0;
	compteurFlotte = 0;
	fleetIndex = 1;
	sizeFleet = FleetArraySize(EmpireFleetGetArray(joueur));
	while(fleetIndex < sizeFleet){
		if(GetFleetSystem(flotte) == GetCameraSystem(camera)) {
			flotteDuSysteme[compteur] = flotte;
			compteur++;
		}
		fleetIndex++;
		flotte = FlotteNumero(EmpireFleetGetArray(joueur), fleetIndex);
	}

	if (GetWindowSelection(fenetre) > compteur) {
		SetWindowSelection(fenetre, 0);
	}
	else if (GetWindowSelection(fenetre) < 0) {
		SetWindowSelection(fenetre, compteur);
	}
	flotte = flotteDuSysteme[0];
	i = 0;
	while(i < compteur) {
		if(GetWindowSelection(fenetre) == (i + 1)) {
			gfx_SetTextFGColor(13);
		}
		else {
			gfx_SetTextFGColor(1);
		}
		switch(GetFleetType(flotte)){
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
		PrintInt(RecupererFlotteNumero(EmpireFleetGetArray(joueur), flotteDuSysteme[i]) + 1);
		gfx_HorizLine_NoClip(50, niveau + 11, 220);
		niveau += 17;
		i++;
		compteurFlotte++;
		flotte = flotteDuSysteme[i];
	}
	if (*key == sk_Enter) {
		*key = 0;
		SetWindowSelectedFleet(fenetre, RecupererFlotteNumero(EmpireFleetGetArray(joueur), flotteDuSysteme[GetWindowSelection(fenetre) - 1]) + 1);
		OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_FLOTTE_DETAILS);
		SetWindowPrevious(fenetre, 1);
	}
}

/**
 * Dessine le menu de resumé de l'étoile
 */
static void MenuSystemeEtoile(char *key, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	switch(*key) {
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
	}
	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_SetColor(0);
	gfx_FillRectangle_NoClip(45, 56, 100, 100);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(45, 56, 100, 100);

	gfx_SetTextXY(45, 42);
	gfx_PrintString("Proxima ");
	gfx_PrintString(GetSystemName(systemeStellaires[GetCameraSystem(camera)]));
	gfx_SetTextXY(160, 62);
	gfx_PrintString("Classe : ");
	switch(GetSystemStarType(systemeStellaires[GetCameraSystem(camera)])) {
		case ETOILE_TYPE_B:
			gfx_PrintString("B");
			gfx_SetColor(1);
			gfx_FillCircle_NoClip(95, 106, 45);				
			gfx_SetColor(15);
			gfx_Circle_NoClip(95, 106, 45);
			break;
		case ETOILE_TYPE_A:
			gfx_PrintString("A");
			gfx_SetColor(1);
			gfx_FillCircle_NoClip(95, 106, 40);				
			gfx_SetColor(14);
			gfx_Circle_NoClip(95, 106, 40);
			break;
		case ETOILE_TYPE_F:
			gfx_PrintString("F");
			gfx_SetColor(1);
			gfx_FillCircle_NoClip(95, 106, 35);				
			break;
		case ETOILE_TYPE_G:
			gfx_PrintString("G");
			gfx_SetColor(1);
			gfx_FillCircle_NoClip(95, 106, 32);				
			gfx_SetColor(13);
			gfx_Circle_NoClip(95, 106, 32);
			break;
		case ETOILE_TYPE_K:
			gfx_PrintString("K");
			gfx_SetColor(13);
			gfx_FillCircle_NoClip(95, 106, 30);				
			gfx_SetColor(9);
			gfx_Circle_NoClip(95, 106, 30);
			break;
		case ETOILE_TYPE_M:
			gfx_PrintString("M");
			gfx_SetColor(9);
			gfx_FillCircle_NoClip(95, 106, 10);				
			gfx_SetColor(13);
			gfx_Circle_NoClip(95, 106, 10);
			break;
		case ETOILE_TYPE_TROU_NOIR:
			gfx_PrintString("trou noir");
			gfx_SetColor(1);
			gfx_Circle_NoClip(95, 106, 40);	
			gfx_SetColor(1);
			gfx_Circle_NoClip(95, 106, 41);	
			gfx_SetColor(14);
			gfx_Circle_NoClip(95, 106, 42);				
			gfx_SetColor(13);
			gfx_Circle_NoClip(95, 106, 43);			
			gfx_SetColor(0);
			gfx_Circle_NoClip(95, 106, 44);			
			gfx_SetColor(11);
			gfx_Circle_NoClip(95, 106, 45);				
			gfx_SetColor(14);
			gfx_Circle_NoClip(95, 106, 46);
			gfx_SetColor(13);
			gfx_Circle_NoClip(95, 106, 47);
			gfx_SetColor(0);
			gfx_FillCircle_NoClip(105, 110, 38);
			gfx_SetColor(13);
			gfx_HorizLine_NoClip(45, 106, 100);
			gfx_HorizLine_NoClip(48, 105, 94);
			gfx_HorizLine_NoClip(48, 106, 94);
			break;
		case ETOILE_TYPE_PULSAR:
			gfx_PrintString("pulsar");
			gfx_SetColor(14);
			gfx_FillCircle_NoClip(95, 106, 4);
			gfx_Line_NoClip(85, 96, 105, 116);
			gfx_SetColor(1);
			gfx_Circle_NoClip(95, 106, 4);
			break;
		case ETOILE_TYPE_ETOILE_A_NEUTRONS:
			gfx_PrintString("/toile a neutrons");
			gfx_SetColor(14);
			gfx_FillCircle_NoClip(95, 106, 4);	
			gfx_Circle_NoClip(95, 106, 11);
			gfx_Circle_NoClip(95, 106, 9);
			gfx_SetColor(1);
			gfx_Circle_NoClip(95, 106, 4);
			gfx_Circle_NoClip(95, 106, 10);
			break;
	}
}

static void NumeroPlanete(int numero, int *decalage, Planete *planete, char *nomPlanete, SystemeStellaire *systeme){
	switch(numero){
	case 0:
		*decalage = 186;
		planete = GetPlanet(systeme, 0);
		strcpy(nomPlanete, " I");
		break;
	case 1:
		*decalage = 182;
		planete = GetPlanet(systeme, 1);
		strcpy(nomPlanete, " II");
		break;
	case 2:
		*decalage = 178;
		planete = GetPlanet(systeme, 2);
		strcpy(nomPlanete, " III");
		break;
	case 3:
		*decalage = 182;
		planete = GetPlanet(systeme, 3);
		strcpy(nomPlanete, " IV");
		break;
	case 4:
		*decalage = 186;
		planete = GetPlanet(systeme, 4);
		strcpy(nomPlanete, " V");
		break;
	default:
		*decalage = 178;
		planete = GetPlanet(systeme, 0);
		strcpy(nomPlanete, " ERR");
		break;
	}
}

/**
 *Dessine le menu de resumé de planète
 */
static void MenuSystemePlaneteResume(char *key, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	char populationChar[5], nomPlanete[20];
	int decalage = 0;
	int niveau = 0;
	Planete* planete = NULL;
	planete = GetPlanet(systemeStellaires[GetCameraSystem(camera)], GetWindowPlanet(fenetre));
	NumeroPlanete(GetWindowPlanet(fenetre), &decalage, planete, nomPlanete, systemeStellaires[GetCameraSystem(camera)]);

	//dessiner fenetre
	DrawMenuBase(GetPlanetCityPopulation(planete), 0, fenetre);

	gfx_Rectangle_NoClip(45, 56, 100, 100); //cadre image

	gfx_SetTextXY(45, 42);
	if(GetPlanetHabitability(planete) == 1){
		gfx_PrintString(GetPlanetName(planete));
		gfx_SetTextFGColor(19);
		if(GetPlanetCityPopulation(planete) > 0){
			gfx_PrintStringXY("Habit/e", 150, 62);
		}
		else{
			gfx_PrintStringXY("Habitable", 150, 62);
		}
	}
	else{
		gfx_PrintString(GetSystemName(systemeStellaires[GetCameraSystem(camera)]));
		gfx_PrintString(nomPlanete);
		gfx_SetTextFGColor(3);
		gfx_PrintStringXY("Non-habitable", 150, 62);
	}

	gfx_SetTextFGColor(1);
	gfx_SetTextXY(157, 74);
	gfx_TransparentSprite_NoClip(pop_icon, 150, 74);
	sprintf(populationChar, "%d", GetPlanetCityPopulation(planete));
	gfx_PrintString(populationChar);
	
	if(GetPlanetCityPopulation(planete) > 0){
		gfx_SetTextXY(157, 84);
		gfx_TransparentSprite_NoClip(criminality_pop_icon, 150, 84);
		PrintInt(GetPlanetCityCrimianity(planete));
		
		gfx_SetTextXY(157, 94);
		gfx_TransparentSprite_NoClip(free_houses_icon, 150, 94);
		PrintInt(GetPlanetCityUrbanDistrictNumber(planete));
		
		gfx_SetTextXY(157, 104);
		gfx_TransparentSprite_NoClip(amienties_icon, 150, 104);
		PrintInt(GetPlanetCityAmienties(planete));

		gfx_SetTextXY(157, 114);
		gfx_TransparentSprite_NoClip(free_job_icon, 150, 114);
		if(GetPlanetCityPopulation(planete) - GetPlanetCityJob(planete) > 0){
			PrintInt(GetPlanetCityPopulation(planete) - GetPlanetCityJob(planete));
		} else {
			PrintInt(0);
		}
		
		gfx_SetTextXY(157, 124);
		gfx_TransparentSprite_NoClip(unemployed_pop_icon, 150, 124);
		if(GetPlanetCityPopulation(planete) - GetPlanetCityJob(planete) < 0) {
			PrintInt(-(GetPlanetCityPopulation(planete) - GetPlanetCityJob(planete)));
		} else {
			PrintInt(0);
		}

		niveau = 158;
		gfx_SetTextXY(45, niveau);
		gfx_PrintString("Production");

		niveau += 14;
		gfx_SetTextXY(45, niveau);
		gfx_PrintString("D/ficit");
	}

	switch(*key) {
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Right:
			if(GetPlanetCityPopulation(planete) > 0){
				OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_DISTRICT);
				*key = 0;
			}
			break;
	}
}

/**
 *Ecrie le nom de l'ordre de district
 */
static void OrdreDistrictNom(Villes *villes){
	OrdreFile *ordreQueue;
	int nombredOrdres = 0;
	int niveau = 181;
	ordreQueue = GetCityOrderQueue(villes);
	gfx_SetTextXY(45, niveau);
	gfx_SetTextFGColor(1);
	if(RecupererOrdre(ordreQueue) != NULL){
		switch(GetOrder(ordreQueue)){
			case AUCUN_ORDRE_VILLE:
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
				PrintInt(GetOrderInfo1(ordreQueue));
				break;
		}
		if(GetOrder(ordreQueue) != AUCUN_ORDRE_VILLE){
			gfx_SetTextXY(45, niveau += 9);
			gfx_PrintString("(");
			gfx_SetTextFGColor(13);
			PrintInt((GetOrderTotalTime(ordreQueue) - GetOrderProgress(ordreQueue)) * 100 / 12);
			gfx_PrintString("%");
			gfx_SetTextFGColor(1);
			gfx_PrintString(")");
			nombredOrdres = NombredOrdres(ordreQueue);
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
 *Dessine le menu des districts de planète
 */
static void MenuSystemePlaneteDistrict(char *key, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre, EmpireListe *empireListe){
	int8_t nomPlanete[20];
	int decalage = 0;
	int maximum = 4;
	int niveau = 0;
	Planete* planete = NULL;
	Ordre *ordre = NULL;
	planete = GetPlanet(systemeStellaires[GetCameraSystem(camera)], GetWindowPlanet(fenetre));
	NumeroPlanete(GetWindowPlanet(fenetre), &decalage, planete, nomPlanete, systemeStellaires[GetCameraSystem(camera)]);

	ordre = GetCityOrderElement(GetPlanetCity(planete));	

	//dessiner fenetre
	DrawMenuBase(GetPlanetCityPopulation(planete), 1, fenetre);

	gfx_HorizLine_NoClip(45, 179, 230);//barre du bas au dessus des ordres

	gfx_SetTextXY(45, 42);
	if(GetPlanetHabitability(planete) == 1){
		gfx_PrintString(GetPlanetName(planete));
	}
	else{
		OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_RESUME);
	}
	if(GetPlanetCity(planete) != NULL){
		niveau = 55;
		if(GetWindowSelection(fenetre) == 1){
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
		PrintInt(GetPlanetCityUrbanDistrictNumber(planete));
		gfx_PrintString("|10");

		niveau += 24;
		if(GetWindowSelection(fenetre) == 2){
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
		PrintInt(GetPlanetCityGeneratorDistrictNumber(planete));
		gfx_PrintString("|10");

		niveau += 24;
		if(GetWindowSelection(fenetre) == 3){
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
		PrintInt(GetPlanetCityMiningDistrictNumber(planete));
		gfx_PrintString("|10");

		niveau += 24;
		if(GetWindowSelection(fenetre) == 4){
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
		PrintInt(GetPlanetCityAgricultureDistrictNumber(planete));
		gfx_PrintString("|10");
	}
	OrdreDistrictNom(GetPlanetCity(planete));
	
	if(ordre != NULL){
		niveau = 181;
		maximum = 5;
		if(GetWindowSelection(fenetre) == 5){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(3);
		}
		gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);
		gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);
	}

	if(GetWindowError(fenetre) == MINERAI_MANQUE){
		UnincrementWindowErrorCountdown(fenetre);
		gfx_SetTextFGColor(3);
		gfx_PrintStringXY("Pas assez de minerais", 45, 160);
		if(GetWindowErrorCountDown(fenetre) == 0){
			SetWindowError(fenetre, NO_ERROR);
		}
	}

	if(*key == sk_Enter && GetSystemEmpire(systemeStellaires[GetCameraSystem(camera)]) != -1){
		if(GetWindowSelection(fenetre) == 5){
			AddEmpireMinerals(EmpireNumero(empireListe, 0), GetOrderPrice(GetCityOrderQueue(GetPlanetCity(planete))));
			FinirOrdre(GetCityOrderQueue(GetPlanetCity(planete)));
		}
		else{
			if(GetEmpireMinerals(EmpireNumero(empireListe, 0)) >= 50){
				AddEmpireMinerals(EmpireNumero(empireListe, 0), -50);
				switch(GetWindowSelection(fenetre)){
					case 1:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_URBAIN, 1, 12, 0, 0, 50);
						break;
					case 2:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_GENERATEUR, 1, 12, 0, 0, 50);
						break;
					case 3:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_MINIER, 1, 12, 0, 0, 50);
						break;
					case 4:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_AGRICOLE, 1, 12, 0, 0, 50);
						break;
				}
			}
			else{
				SetWindowError(fenetre, MINERAI_MANQUE);
				SetWindowErrorCountdown(fenetre, 50);
			}
		}
		*key = 0;
	}
	switch(*key) {
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Left:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_RESUME);
			*key = 0;
			break;
		case sk_Right:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_BATIMENT);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
	}
	if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, 1);
	}
	if(GetWindowSelection(fenetre) > maximum){
		SetWindowSelection(fenetre, maximum);
	}
}

/**
 * Ecrit le nom du batiment
 * */
static char *PlaneteBatimentNom(Batiment batiment, int niveau, char* nom){
	char levelStr[10];
	if(batiment != AUCUN_DISTRICT_VILLE){
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
 *Choisi un batiment
 */
static void MenuSystemePlaneteBatimentChoix(char *key, Empire *joueur, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	int niveau = 53;
	char nomDuBatiment[50];
	int i = 0, prix = 0;
	// Batiment *batiment = NULL;
	// int niveauDuBatiment = 0;
	switch(*key) {
		default:
			break;
		case sk_Clear:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_BATIMENT);
			SetWindowSelection(fenetre, GetWindowPrevious(fenetre));
			SetWindowPrevious(fenetre, 1);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
	}

	if(GetWindowSelection(fenetre) > 5) {
		SetWindowSelection(fenetre, 5);
	} else if(GetWindowSelection(fenetre) < 1) {
		SetWindowSelection(fenetre, 1);
	}

	if(GetWindowCity(fenetre) == NULL){
		OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_BATIMENT);
		return;
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
	PrintInt(GetWindowPrevious(fenetre));
	
	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 1){
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
	if(GetWindowSelection(fenetre) == 2){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(FONDERIE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 3){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(LABORATOIRE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 4){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(USINE_CIVILE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 5){
		gfx_SetTextFGColor(13);
		prix = 400;
	}
	gfx_PrintStringXY(PlaneteBatimentNom(THEATRE, 1, nomDuBatiment), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 400){
		gfx_SetTextFGColor(3);
	}
	PrintInt(400);
	niveau += 15;

	if(*key == sk_Enter && GetSystemEmpire(systemeStellaires[GetCameraSystem(camera)]) != -1){
		OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_BATIMENT);
		if(GetWindowSelection(fenetre) != 0){
			IncrementWindowSelection(fenetre);
		}
		if(GetEmpireAlloys(joueur) >= prix){
			AddEmpireAlloys(joueur, -prix);
			NouvelOrdre(GetCityOrderQueue(GetWindowCity(fenetre)),
				CONSTRUIRE_BATIMENT,
				1, 
				10,
				GetWindowPrevious(fenetre),
				GetWindowSelection(fenetre),
				prix
			);
			SetWindowSelection(fenetre, GetWindowPrevious(fenetre));
			SetWindowPrevious(fenetre, 1);
		}
		*key = 0;
	}
}

/**
 *Dessine le menu de les batiments de planète
 */
static void MenuSystemePlaneteBatiments(char *key, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre, EmpireListe *empireListe){
	char nomPlanete[20], nom[50];
	int decalage = 0;
	int nombreDeBatiment = 0;
	int supprimer = 0;
	int niveau = 0;
	Planete* planete = NULL;
	Ordre *ordre = NULL;
	planete = GetPlanet(systemeStellaires[GetCameraSystem(camera)], GetWindowPlanet(fenetre));
	NumeroPlanete(GetWindowPlanet(fenetre), &decalage, planete, nomPlanete, systemeStellaires[GetCameraSystem(camera)]);

	ordre = GetCityOrderElement(GetPlanetCity(planete));	

	//dessiner fenetre
	DrawMenuBase(GetPlanetCityPopulation(planete), 2, fenetre);

	gfx_HorizLine_NoClip(45, 179, 230);//barre du bas au dessus des ordres

	gfx_SetColor(1);
	gfx_SetPixel(270, 45);//point au bout de la ligne du titre

	gfx_SetTextXY(45, 42);//écrie le nom de la planète
	if(GetPlanetHabitability(planete) == 1){
		gfx_PrintString(GetPlanetName(planete));
	}
	else{
		OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_RESUME);
	}
	niveau = 55;
	if(GetPlanetCity(planete) != NULL){
		gfx_SetTextXY(45, niveau);
		if(GetPlanetCityPopulation(planete) >= 0){
			gfx_SetTextFGColor(1);
			if(GetWindowSelection(fenetre) == 1){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(GetPlanetCityBuildingNumber(planete, 0), GetPlanetCityBuildingLevel(planete, 0), nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("0");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(GetPlanetCityPopulation(planete) >= 10){
			gfx_SetTextFGColor(1);
			if(GetWindowSelection(fenetre) == 2){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(GetPlanetCityBuildingNumber(planete, 1), GetPlanetCityBuildingLevel(planete, 1), nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("10");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(GetPlanetCityPopulation(planete) >= 20){
			gfx_SetTextFGColor(1);
			if(GetWindowSelection(fenetre) == 3){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(GetPlanetCityBuildingNumber(planete, 2), GetPlanetCityBuildingLevel(planete, 2), nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("20");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(GetPlanetCityPopulation(planete) >= 30){
			gfx_SetTextFGColor(1);
			if(GetWindowSelection(fenetre) == 4){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(GetPlanetCityBuildingNumber(planete, 3), GetPlanetCityBuildingLevel(planete, 3), nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("30");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(GetPlanetCityPopulation(planete) >= 40){
			gfx_SetTextFGColor(1);
			if(GetWindowSelection(fenetre) == 5){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(GetPlanetCityBuildingNumber(planete, 4), GetPlanetCityBuildingLevel(planete, 4), nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("40");
		}
		niveau += 14;
		gfx_SetTextXY(45, niveau);
		if(GetPlanetCityPopulation(planete) >= 50){
			gfx_SetTextFGColor(1);
			if(GetWindowSelection(fenetre) == 6){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString(PlaneteBatimentNom(GetPlanetCityBuildingNumber(planete, 5), GetPlanetCityBuildingLevel(planete, 5), nom));
			nombreDeBatiment++;
		}
		else{
			gfx_SetTextFGColor(3);
			gfx_PrintString("50");
		}
		niveau += 14;
	}
	OrdreDistrictNom(GetPlanetCity(planete));
	
	if(ordre != NULL){
		niveau = 181;
		if(GetWindowSelection(fenetre) == 7){
			gfx_SetColor(13);
		}
		else{
			gfx_SetColor(3);
		}
		supprimer = 1;
		gfx_Line_NoClip(265, niveau + 2, 275, niveau + 12);
		gfx_Line_NoClip(265, niveau + 12, 275, niveau + 2);
	}
	
	if(*key == sk_Enter && GetSystemEmpire(systemeStellaires[GetCameraSystem(camera)]) != -1){
		if(GetWindowSelection(fenetre) == 5){
			AddEmpireMinerals(EmpireNumero(empireListe, 0), GetOrderPrice(GetCityOrderQueue(GetPlanetCity(planete))));
			FinirOrdre(GetCityOrderQueue(GetPlanetCity(planete)));
		}
		else{
			if(GetEmpireMinerals(EmpireNumero(empireListe, 0)) >= 50){
				AddEmpireMinerals(EmpireNumero(empireListe, 0), -50);
				switch(GetWindowSelection(fenetre)){
					case 1:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_URBAIN, 1, 12, 0, 0, 50);
						break;
					case 2:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_GENERATEUR, 1, 12, 0, 0, 50);
						break;
					case 3:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_MINIER, 1, 12, 0, 0, 50);
						break;
					case 4:
						NouvelOrdre(GetCityOrderQueue(GetPlanetCity(planete)), CONSTRUIRE_DISTRICT_AGRICOLE, 1, 12, 0, 0, 50);
						break;
				}
			}
			else{
				SetWindowError(fenetre, MINERAI_MANQUE);
				SetWindowErrorCountdown(fenetre, 50);
			}
		}
		*key = 0;
	}
	if(*key == sk_Enter && GetSystemEmpire(systemeStellaires[GetCameraSystem(camera)]) != -1){
		if((supprimer == 1) && (GetWindowSelection(fenetre) == 7)){
			AddEmpireMinerals(EmpireNumero(empireListe, 0), GetOrderPrice(GetCityOrderQueue(GetPlanetCity(planete))));
			FinirOrdre(GetCityOrderQueue(GetPlanetCity(planete)));
		}
		else if (GetWindowSelection(fenetre) != 1){
			SetWindowPrevious(fenetre, GetWindowSelection(fenetre));
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_BATIMENT_CHOIX);
			SetWindowCity(fenetre, GetPlanetCity(planete));
		}
		*key = 0;
	}
	switch(*key) {
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Left:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_DISTRICT);
			*key = 0;
			break;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
	}
	if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, 1);
	}
	if((GetWindowSelection(fenetre) == 6) && (nombreDeBatiment != 6)){
		SetWindowSelection(fenetre, nombreDeBatiment);
	}
	if((GetWindowSelection(fenetre) > nombreDeBatiment) && (supprimer == 0)){
		SetWindowSelection(fenetre, nombreDeBatiment);
	}
	if((GetWindowSelection(fenetre) > nombreDeBatiment) && (supprimer == 1)){
		SetWindowSelection(fenetre, 7);
	}
}


/**
 *Dessine le menu des détails sur la flotte sélectionnée par les menus MenuSystemeFlotte et MenuListeFlottes
 */
static void MenuSystemeFlotteDetails(char *key, SystemeStellaire **systemeStellaires, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre){
	char niveau = 0;
	Flotte* flotte = NULL;
	switch(*key) {
		default:
			break;
		case sk_Clear:
			switch(GetWindowPrevious(fenetre)){
				case 1:
					CloseMenu(fenetre, camera);
					break;
				case 2:
					OpenMenu(fenetre, camera, MENU_FLOTTE, 0);
					SetWindowSelection(fenetre, GetWindowSelectedFleet(fenetre));
					break;
				default:
					CloseMenu(fenetre, camera);
					break;
			}
			*key = 0;
			break;
		case sk_Up:
			AddWindowSelection(fenetre, -4);
			break;
		case sk_Down:
			AddWindowSelection(fenetre, +4);
			break;
		case sk_Left:
			UnincrementWindowSelection(fenetre);
			break;
		case sk_Right:
			IncrementWindowSelection(fenetre);
			break;
	}
	//dessiner fenetre
	gfx_SetColor(6);
	gfx_FillRectangle_NoClip(40, 40, 240, 160);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(40, 40, 240, 160);
	gfx_HorizLine_NoClip(45, 51, 225); // barre sous le titre et retour
	gfx_HorizLine_NoClip(45, 67, 180); //***
	gfx_Line_NoClip(225, 67, 230, 62); //*Barre sous le sous-titre
	gfx_HorizLine_NoClip(231, 62, 38); //***
	gfx_HorizLine_NoClip(45, 162, 230);//barre du bas au dessus des ordres
	gfx_VertLine_NoClip(100, 42, 8); //barre a coté de retour
	gfx_SetColor(1);
	gfx_SetPixel(270, 62);
	gfx_SetPixel(270, 51);
	gfx_PrintStringXY("Retour", 48, 42);
	gfx_SetTextXY(150, 42);
	flotte = FlotteNumero(EmpireFleetGetArray(EmpireNumero(empireListe, 0)), GetWindowSelectedFleet(fenetre));
	if(GetFleetType(flotte) == FLOTTE_DE_CONSTRUCTION){
		if((GetWindowSelection(fenetre) == 0) || (GetWindowSelection(fenetre) >= 10)){
			SetWindowSelection(fenetre, 2);
		}
		else if((GetWindowSelection(fenetre) == 3) || (GetWindowSelection(fenetre) == 9)){
			SetWindowSelection(fenetre, 1);
		}
		else if((GetWindowSelection(fenetre) == 7) || (GetWindowSelection(fenetre) <= -3)){
			SetWindowSelection(fenetre, 5);
		}
		else if((GetWindowSelection(fenetre) == 4) || (GetWindowSelection(fenetre) == -2)){
			SetWindowSelection(fenetre, 6);
		}
	}
	else{
		if(GetWindowSelection(fenetre) == 0){
			SetWindowSelection(fenetre, 2);
		}
		else if((GetWindowSelection(fenetre) == 3) || (GetWindowSelection(fenetre) > 6)){
			SetWindowSelection(fenetre, 1);
		}
		else if(((GetWindowSelection(fenetre) == 6) || (GetWindowSelection(fenetre) == 4)) || (GetWindowSelection(fenetre) < 0)){
			SetWindowSelection(fenetre, 5);
		}
	}

	gfx_PrintString("Flotte ");
	if (GetWindowSelectedFleet(fenetre) > 10) {
		gfx_PrintInt(GetWindowSelectedFleet(fenetre), 2);
	}
	else {
		gfx_PrintInt(GetWindowSelectedFleet(fenetre), 1);
	}
	niveau = 57;
	gfx_SetTextXY(45, niveau);
	niveau += 14;
	gfx_SetTextFGColor(17);
	switch(GetFleetType(flotte)){
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
	PrintInt(GetFleetHullPourcent(flotte));
	gfx_PrintString("%");
	gfx_SetTextXY(170, niveau);
	gfx_TransparentSprite_NoClip(blindage_icon, 45, niveau + 1);
	PrintInt(GetFleetArmorPourcent(flotte));
	gfx_PrintString("%");
	niveau += 14;
	gfx_SetTextXY(50, niveau);
	gfx_TransparentSprite_NoClip(bouclier_icon, 45, niveau);
	PrintInt(GetFleetShieldPourcent(flotte));
	gfx_PrintString("%");
	if(GetFleetType(flotte) == FLOTTE_MILITAIRE){
		gfx_SetTextFGColor(18);
		gfx_SetTextXY(170, niveau);
		gfx_TransparentSprite_NoClip(fleet_power_icon, 165, niveau + 2);
		PrintInt(GetFleetPower(flotte));
	}
	gfx_SetTextFGColor(1);
	
	niveau += 20;
	gfx_SetTextXY(45, niveau);
	if(GetWindowSelection(fenetre) == FLOTTE_BOUGER){
		gfx_SetTextFGColor(13);
	}
	gfx_PrintString("Bouger");
	gfx_SetTextFGColor(1);
	gfx_SetTextXY(165, niveau);
	niveau += 14;
	switch(GetFleetType(flotte)){
		case FLOTTE_MILITAIRE:
			if(GetWindowSelection(fenetre) == FLOTTE_ATTAQUER){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString("Attaquer");
			gfx_SetTextFGColor(1);

			if(GetWindowSelection(fenetre) == FLOTTE_BOMBARDER){
				gfx_SetTextFGColor(13);
			}
			gfx_SetTextXY(45, niveau);
			niveau += 20;
			gfx_PrintString("Bombarder");
			gfx_SetTextFGColor(1);

			//ecrire la liste des vaisseaux
			gfx_SetTextXY(45, niveau);
			PrintInt(GetFleetCorvetteNumber(flotte));
			gfx_PrintString(" Corvette(s)");
			gfx_SetTextFGColor(1);

			gfx_SetTextXY(165, niveau);
			niveau += 14;
			PrintInt(GetFleetDestroyerNumber(flotte));
			gfx_PrintString(" Destroyer(s)");
			gfx_SetTextFGColor(1);
			
			gfx_SetTextXY(45, niveau);
			PrintInt(GetFleetCruiserNumber(flotte));
			gfx_PrintString(" Croiseur(s)");
			gfx_SetTextFGColor(1);

			
			gfx_SetTextXY(165, niveau);
			niveau += 14;
			PrintInt(GetFleetBattleshipNumber(flotte));
			gfx_PrintString(" Cuirass/(s)");
			gfx_SetTextFGColor(1);

			gfx_SetTextXY(45, 167);
			switch(GetFleetAction(flotte)){
			case FLOTTE_ATTAQUER:
				gfx_PrintString("Attaque");
				break;
			}
			break;

		case FLOTTE_SCIENTIFIQUE:
			if(GetWindowSelection(fenetre) == FLOTTE_INSPECTER){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString("Inspecter");
			gfx_SetTextFGColor(1);

			if(GetWindowSelection(fenetre) == FLOTTE_RECHERCHER){
				gfx_SetTextFGColor(13);
			}
			gfx_SetTextXY(45, niveau);
			niveau += 14;
			gfx_PrintString("Rechercher");
			gfx_SetTextFGColor(1);
			gfx_SetTextXY(45, 167);
			break;

		case FLOTTE_DE_CONSTRUCTION:
			if(GetWindowSelection(fenetre) == FLOTTE_CONSTRUIRE_BASE){
				gfx_SetTextFGColor(13);
			}
			gfx_PrintString("Station");
			gfx_SetTextFGColor(1);

			if(GetWindowSelection(fenetre) == FLOTTE_COSNTRUIRE_MINE){
				gfx_SetTextFGColor(13);
			}
			gfx_SetTextXY(45, niveau);
			gfx_PrintString("Base mini/re");
			gfx_SetTextFGColor(1);

			if(GetWindowSelection(fenetre) == FLOTTE_CONSTRUIRE_BASE_SCIENTIFIQUE){
				gfx_SetTextFGColor(13);
			}
			gfx_SetTextXY(165, niveau);
			gfx_PrintString("Base scientifique");
			gfx_SetTextFGColor(1);
			gfx_SetTextXY(45, 167);
			switch(GetFleetAction(flotte)){
			case FLOTTE_CONSTRUIRE_BASE:
				gfx_PrintString("Construit une base sur");
				gfx_SetTextXY(45, 177);
				gfx_SetTextFGColor(13);
				gfx_PrintString(GetSystemName(systemeStellaires[GetFleetArriveSystem(flotte)]));
				break;
			}
			break;
	}
	gfx_SetTextXY(45, 167);
	switch(GetFleetAction(flotte)){
		case FLOTTE_AUCUNE_ACTION:
			gfx_PrintString("Aucun ordre");
			break;
		case FLOTTE_BOUGER:
			gfx_PrintString("Se d/place vers ");
			gfx_SetTextFGColor(13);
			gfx_PrintString(GetSystemName(systemeStellaires[GetFleetArriveSystem(flotte)]));
			break;
	}
	if(*key == sk_Enter){ //effectuer l'action
		*key = 0;
		if(GetWindowSelection(fenetre) == FLOTTE_BOUGER){
			SetFleetAction(flotte, FLOTTE_BOUGER);
			BougerFlotte(GetWindowSelectedFleet(fenetre), 0, 0, fenetre, camera, empireListe, systemeStellaires);
		}
		else{
			switch(GetFleetType(flotte)){
			case FLOTTE_MILITAIRE:
				switch(GetWindowSelection(fenetre)){
				case FLOTTE_ATTAQUER:
					break; 
				}
				break;
			case FLOTTE_DE_CONSTRUCTION:
				switch(GetWindowSelection(fenetre)){
				case FLOTTE_CONSTRUIRE_BASE:
					SetFleetAction(flotte, FLOTTE_CONSTRUIRE_BASE);
					BougerFlotte(GetWindowSelectedFleet(fenetre), 0, 0, fenetre, camera, empireListe, systemeStellaires);
					break;
				}
				break;
			}
		}
	}
}

/**
 *Ecrie et renvoie le nom de l'ordre
 */
static char* OrdreStationNom(Station *station, int numeroDuModule, char* nomDeOrdre, int niveau){
	char numero[20];
	int travail = 0, nombredOrdres = 0;
	Ordre *ordre = RecupererOrdre(GetStationOrderQueue(station));
	if(ordre == NULL){
		gfx_PrintStringXY("Aucun ordre", 55, niveau);
	}
	else{
		switch(GetOrder(GetStationOrderQueue(station))){
			case AUCUN_ORDRE_STATION:
				gfx_PrintStringXY("Aucun ordre", 55, niveau);
				break;
			case AMELIORER_STATION:
				gfx_PrintStringXY("S'am/liore en ", 55, niveau);
				gfx_SetTextFGColor(13);
				switch(GetStationLevel(station)){
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
					default:
						break;
				}
				niveau += 9;
				gfx_SetTextXY(55, niveau);
				gfx_PrintString(nomDeOrdre);
				sprintf(numero, "(%d", GetOrderProgressPourcent(ordre));
				gfx_PrintString(numero);
				gfx_PrintString("%)");
				break;
			case CONSTRUIRE_MODULE:
				strcpy(nomDeOrdre, "Construit le module ");
				sprintf(numero, "%d", numeroDuModule);
				strcat(nomDeOrdre, numero);
				sprintf(numero, "(%d)", GetOrderProgressPourcent(ordre));
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
				memset(nomDeOrdre, 0, sizeof(char*));
				switch(GetOrderInfo1(GetStationOrderQueue(station))) {
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
				sprintf(numero, "(%d", GetOrderProgressPourcent(ordre));
				strcat(nomDeOrdre, numero);
				gfx_PrintString(nomDeOrdre);
				gfx_PrintString("%)");
				break;
		}
		nombredOrdres = NombredOrdres(GetStationOrderQueue(station));
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
 *Dessine le menu de la station du systeme
 */
static void MenuSystemeStationResume(char *key, Empire *joueur, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	char evolution[25] = {0};
	char ordreStation[50];
	int prixAmelioration = 0, tempsAmelioration = 0;
	int maximum = 2;
	Station *station = GetSystemStation(systemeStellaires[GetCameraSystem(camera)]);
	Ordre *ordre = RecupererOrdre(GetStationOrderQueue(station));
	int niveau = 120;
	if(ordre != NULL){
		maximum = 3;
	}
	switch(*key){
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Right:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_MODULES);
			SetWindowPrevious(fenetre, 0);
			*key = 0;
			break;
	}
	if(GetWindowSelection(fenetre) > maximum){
		SetWindowSelection(fenetre, maximum);
	}
	else if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, 1);
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
	gfx_PrintString(GetSystemName(systemeStellaires[GetCameraSystem(camera)]));
	gfx_SetTextXY(150, 62);
	switch(GetStationLevel(station)){
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
		default:
			break;
	}
	gfx_SetColor(11);
	switch(GetStationLevel(station)){
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
		default:
			break;
	}
	if(GetWindowSelection(fenetre) == 1){
		gfx_SetTextFGColor(13);
	}
	if(GetStationLevel(station) < CITADELLE){
		gfx_SetTextXY(212 - strlen("Am/liorer") * 4, 80);
		gfx_PrintString("Am/liorer");
		gfx_SetTextXY(212 - strlen(evolution) * 4, 92);
		gfx_PrintString(evolution);
		gfx_SetTextXY(227 - TailleInt(prixAmelioration) * 4, 104);
		gfx_TransparentSprite_NoClip(fer, 212 - TailleInt(prixAmelioration) * 4, 103);
		if(GetEmpireAlloys(joueur) < prixAmelioration){
			gfx_SetTextFGColor(3);
		}
		PrintInt(prixAmelioration);
	}
	else{
		gfx_SetTextXY(212 - strlen("Am/lioration max") * 4, 92);
		gfx_PrintString("Am/lioration max");
	}
	gfx_SetTextFGColor(1);

	if(GetWindowSelection(fenetre) == 2){
		gfx_SetTextFGColor(13);
	}
	if(GetStationLevel(station) > AVANT_POSTE){
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
	PrintInt(GetStationHullPourcent(station));
	gfx_PrintString("%");
	gfx_SetTextXY(170, niveau);
	gfx_TransparentSprite_NoClip(blindage_icon, 165, niveau + 1);
	PrintInt(GetStationArmorPourcent(station));
	gfx_PrintString("%");
	niveau += 9;
	gfx_SetTextXY(50, niveau);
	gfx_TransparentSprite_NoClip(bouclier_icon, 45, niveau);
	PrintInt(GetStationShieldPourcent(station));
	gfx_PrintString("%");
	gfx_SetTextFGColor(18);
	gfx_SetTextXY(170, niveau);
	gfx_TransparentSprite_NoClip(fleet_power_icon, 165, niveau + 2);
	PrintInt(GetStationPower(station));
	
	niveau = 181;
	
	if(ordre != NULL){
		if(GetWindowSelection(fenetre) == 3){
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
	OrdreStationNom(station, GetStationOrderInfo1(station), ordreStation, niveau);

	//effectuer action
	if(*key == sk_Enter){
		switch(GetWindowSelection(fenetre)){
			case 1:
				if((GetEmpireAlloys(joueur) >= prixAmelioration) && (GetStationLevel(station) < CITADELLE)){
					AddEmpireAlloys(joueur, -prixAmelioration);
					NouvelOrdre(GetStationOrderQueue(station),
						AMELIORER_STATION,
						1, 
						tempsAmelioration,
						0,
						0,
						prixAmelioration
					);
				}
				break;
			case 2:
				if(GetStationLevel(station) > AVANT_POSTE){
					switch(GetStationLevel(station)){
						case PORT_STELLAIRE:
							SetStationModule(station, 0, AUCUN_MODULE);
							SetStationModule(station, 1, AUCUN_MODULE);
						case REDOUTE_STELLAIRE:
							SetStationModule(station, 2, AUCUN_MODULE);
							SetStationModule(station, 3, AUCUN_MODULE);
						case FORTERESSE_STELLAIRE:
							SetStationModule(station, 4, AUCUN_MODULE);
							SetStationModule(station, 5, AUCUN_MODULE);
						default:
							break;
					}
					SetStationLevel(station, GetStationLevel(station) - 1);
				}
				else {
					CloseMenu(fenetre, camera);
					SetStationLevel(station, AUCUNE_STATION);
					memsetStation(station);
				}
				break;
			case 3:
				AddEmpireAlloys(joueur, GetOrderPrice(GetStationOrderQueue(station)));
				FinirOrdre(GetStationOrderQueue(station));
				break;
		}
	}
}

/**
 *Renvoie le nom du module
 */
static char* ModuleNom(Module module, char* nomDuModule){
	switch(module){
		case AUCUN_MODULE:
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
 * Ecrire module
 * */
static void EcrireModule(Module module, int selection, int numero, int *niveau){
	char nomDuModule[50];
	gfx_SetTextFGColor(1);
	if(selection == numero){gfx_SetTextFGColor(13);}
	gfx_SetTextXY(144 - strlen("Module ") * 4, *niveau);
	gfx_PrintString("Module ");
	PrintInt(numero);
	*niveau += 10;
	ModuleNom(module, nomDuModule);
	gfx_SetTextXY(160 - strlen(nomDuModule) * 4, *niveau);
	gfx_PrintString(nomDuModule);
	*niveau += 15;
}

/**
 *Dessine le menu des modules de la station du systeme
 */
static void MenuSystemeStationModules(char *key, Empire *joueur, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	Station *station = GetSystemStation(systemeStellaires[GetCameraSystem(camera)]);
	OrdreFile *ordreQueue = GetStationOrderQueue(station);
	int niveau = 120;
	char ordreStation[50];
	int modules = 0, supprimer = 0;
	Module *module = NULL;
	int numero = 0;
	int i = 0;
	switch(*key){
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Right:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_CHANTIER);
			*key = 0;
			break;
		case sk_Left:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_RESUME);
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
	gfx_PrintString(GetSystemName(systemeStellaires[GetCameraSystem(camera)]) );
	gfx_SetTextXY(150, 62);

	switch(GetStationLevel(station)){
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
		default:
			break;
	}
	if(RecupererOrdre(ordreQueue) != NULL){
		supprimer = 1;
	}
	if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, 1);
	}
	if((GetWindowSelection(fenetre) == 6) && (modules != 6)){
		SetWindowSelection(fenetre, modules);
	}
	if((GetWindowSelection(fenetre) > modules) && (supprimer == 0)){
		SetWindowSelection(fenetre, modules);
	}
	if((GetWindowSelection(fenetre) > modules) && (supprimer == 1)){
		SetWindowSelection(fenetre, 7);
	}
	if(GetWindowPrevious(fenetre) != 0){
		module = GetStationModulePointer(station, GetWindowSelection(fenetre));
		if(*module != (Module) GetWindowPrevious(fenetre)){
			if(GetWindowPrevious(fenetre) == 8){
				SetWindowPrevious(fenetre, 0);
				*module = AUCUN_MODULE;
			}
			if(GetWindowPrevious(fenetre) != 0){
				if(GetEmpireAlloys(joueur) >= 50){
					AddEmpireAlloys(joueur, -50);
					NouvelOrdre(ordreQueue,
						CONSTRUIRE_MODULE, 
						1, 
						12,
						numero,
						GetWindowPrevious(fenetre),
						50
						);
				}
			}
		}
		SetWindowPrevious(fenetre, 0);
	}
	niveau = 55;
	i = 1;
	while(i <= modules){
		EcrireModule(GetStationModule(station, i - 1), GetWindowSelection(fenetre), i, &niveau);
		i++;
	}

	//écrire ordre
	niveau = 181;
	if(RecupererOrdre(ordreQueue) != NULL){
		if(GetWindowSelection(fenetre) == 7){
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
	OrdreStationNom(station, GetOrderInfo1(ordreQueue), ordreStation, niveau);
	
	if(*key == sk_Enter){
		if((supprimer == 1) && (GetWindowSelection(fenetre) == 7)) {
			AddEmpireAlloys(joueur, GetOrderPrice(ordreQueue));
			FinirOrdre(ordreQueue);
		} else {
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_MODULES_CHOIX);
			SetWindowPrevious(fenetre, GetWindowSelection(fenetre));
			SetWindowSelection(fenetre, 0);
			SetWindowScroll(fenetre, 1);
			*key = 0;
		}
	}
}

/**
 *Choisi un module
 */
static void MenuSystemeStationModulesChoix(char *key, Empire *joueur, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	int niveau = 53;
	char nomDuModule[50];
	int i = 0;
	switch(*key){
		default:
			break;
		case sk_Clear:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_MODULES);
			SetWindowSelection(fenetre, 0);
			SetWindowPrevious(fenetre, 0);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
	}
	if(GetWindowSelection(fenetre) > 7){
		SetWindowSelection(fenetre, 0);
	}
	if(GetWindowSelection(fenetre) < 0){
		SetWindowSelection(fenetre, 7);
	}
	if(GetWindowSelection(fenetre) == 0){
		SetWindowScroll(fenetre, 1);
	}
	if((GetWindowSelection(fenetre) == 1) && (GetWindowScroll(fenetre)== 3)){
		SetWindowScroll(fenetre, 2);
	}
	if((GetWindowSelection(fenetre) == 6) && (GetWindowScroll(fenetre)== 1)){
		SetWindowScroll(fenetre, 3);
	}
	if(GetWindowSelection(fenetre) == 7){
		SetWindowScroll(fenetre, 3);
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
	PrintInt(GetWindowPrevious(fenetre));
	
	if(GetWindowScroll(fenetre)<= 1){
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 0){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY("D/manteler", 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		PrintInt(0);
		niveau += 15;
	}

	if(GetWindowScroll(fenetre)<= 2){
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 1){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY(ModuleNom(CHANTIER_SPATIAL, nomDuModule), 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		if(GetEmpireAlloys(joueur) < 50){
			gfx_SetTextFGColor(3);
		}
		PrintInt(50);
		niveau += 15;
	}

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 2){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(ANCRAGE, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 3){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(CANONS, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 4){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(MISSILES, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	gfx_SetTextFGColor(1);
	if(GetWindowSelection(fenetre) == 5){gfx_SetTextFGColor(13);}
	gfx_PrintStringXY(ModuleNom(HANGAR, nomDuModule), 45, niveau);
	niveau += 10;
	gfx_SetTextXY(60, niveau);
	gfx_TransparentSprite_NoClip(fer, 45, niveau);
	if(GetEmpireAlloys(joueur) < 50){
		gfx_SetTextFGColor(3);
	}
	PrintInt(50);
	niveau += 15;

	if(GetWindowScroll(fenetre)>= 2){
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 6){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY(ModuleNom(CARREFOUR_COMMERCIAL, nomDuModule), 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		if(GetEmpireAlloys(joueur) < 50){
			gfx_SetTextFGColor(3);
		}
		PrintInt(50);
		niveau += 15;
	}

	if(GetWindowScroll(fenetre)>= 3){
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 7){gfx_SetTextFGColor(13);}
		gfx_PrintStringXY(ModuleNom(PANNEAUX_SOLAIRES, nomDuModule), 45, niveau);
		niveau += 10;
		gfx_SetTextXY(60, niveau);
		gfx_TransparentSprite_NoClip(fer, 45, niveau);
		if(GetEmpireAlloys(joueur) < 50){
			gfx_SetTextFGColor(3);
		}
		PrintInt(50);
		niveau += 15;
	}

	if(*key == sk_Enter){
		if(GetEmpireAlloys(joueur) >= 50){
			if(GetWindowSelection(fenetre) == 0){
				SetWindowSelection(fenetre, 8);
			}
			i = GetWindowPrevious(fenetre);
			SetWindowPrevious(fenetre, GetWindowSelection(fenetre));
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_MODULES);
			SetWindowSelection(fenetre, i);
			*key = 0;
		}
	}
}

/**
 *Dessine le menu des modules de la station du systeme
 */
static void MenuSystemeStationChantier(char *key, Empire *joueur, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	char ordreStation[50];
	int niveau = 55, nombreDeChantiers = 0, travail = 0, prix = 0;
	int i = 0;
	Station *station = GetSystemStation(systemeStellaires[GetCameraSystem(camera)]);
	OrdreFile *ordreQueue = GetStationOrderQueue(station);
	switch(*key){
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Left:
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_MODULES);
			SetWindowPrevious(fenetre, 0);
			*key = 0;
			break;
	}
	if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, 1);
	}
	else if((GetWindowSelection(fenetre) > 7) && (RecupererOrdre(ordreQueue) != NULL)){
		SetWindowSelection(fenetre, 7);
	}
	else if(GetWindowSelection(fenetre) > 6){
		SetWindowSelection(fenetre, 6);
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
	gfx_PrintString(GetSystemName(systemeStellaires[GetCameraSystem(camera)]));
	i = 0;
	while(i < 6){
		if(GetStationModule(station, i) == CHANTIER_SPATIAL){
			nombreDeChantiers++;
		}
		i++;
	}

	//verifie que la station soit avec un chantier spatial
	if(nombreDeChantiers > 0){
		gfx_SetTextXY(45, niveau);
		gfx_PrintString("Chantiers : ");
		gfx_PrintInt(nombreDeChantiers, 1);

		if(GetWindowSelection(fenetre) == 1){
			gfx_SetTextFGColor(13);
			travail = 3;
			prix = 100;
		}
		niveau += 20;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Scientifique ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(GetEmpireAlloys(joueur) < 100){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("100");
		
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 2){
			gfx_SetTextFGColor(13);
			travail = 3;
			prix = 100;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Construction ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(GetEmpireAlloys(joueur) < 100){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("100");
		
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 3){
			gfx_SetTextFGColor(13);
			travail = 3;
			prix = 100;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Corvette ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(GetEmpireAlloys(joueur) < 100){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("100");
		
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 4){
			gfx_SetTextFGColor(13);
			travail = 6;
			prix = 200;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Destroyer ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(GetEmpireAlloys(joueur) < 200){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("200");
		
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 5){
			gfx_SetTextFGColor(13);
			travail = 12;
			prix = 500;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Croiseur ");
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(GetEmpireAlloys(joueur) < 500){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("500");
		
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == 6){
			gfx_SetTextFGColor(13);
			travail = 24;
			prix = 1000;
		}
		niveau += 14;
    	gfx_SetTextXY(45, niveau);
		gfx_PrintString("Cuirass/ "); //battleship
		gfx_TransparentSprite_NoClip(fer, 190, niveau);
    	gfx_SetTextXY(205, niveau);
		if(GetEmpireAlloys(joueur) < 1000){
			gfx_SetTextFGColor(3);
		}
		gfx_PrintString("1000");
		
		//écrire ordre
		niveau = 181;
		if(RecupererOrdre(ordreQueue) != NULL){
			if(GetWindowSelection(fenetre) == 7){
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
		OrdreStationNom(station, GetOrderInfo1(ordreQueue), ordreStation, niveau);
		

		if(*key == sk_Enter){
			if(GetWindowSelection(fenetre) == 7){
				AddEmpireAlloys(joueur, GetOrderPrice(ordreQueue));
				FinirOrdre(ordreQueue);
			}
			else if(GetWindowSelection(fenetre) >= 3){
				OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_CHANTIER_CHOIX);
				SetWindowFleetSize(fenetre, 1);
			}
			else{
				if(GetEmpireAlloys(joueur) >= prix){
					AddEmpireAlloys(joueur, -prix);
					NouvelOrdre(ordreQueue,
						CONSTRUIRE_VAISSEAU,
						1, 
						travail,
						GetWindowSelection(fenetre),
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
static void MenuSystemeStationChantierChoix(char *key, Empire *joueur, SystemeStellaire **systemeStellaires, Camera *camera, Fenetre *fenetre){
	int prix = 0, travail = 0;
	char nom[50];
	switch(*key){
		default:
			break;
		case sk_Clear:
			SetWindowMenuSystem(fenetre, MENU_SYSTEME_STATION_CHANTIER);
			*key = 0;
			break;
		case sk_Right:
			AddWindowFleetSize(fenetre, +1);
			*key = 0;
			break;
		case sk_Left:
			AddWindowFleetSize(fenetre, -1);
			*key = 0;
			break;
		case sk_Add:
			AddWindowFleetSize(fenetre, +1);
			*key = 0;
			break;
		case sk_Sub:
			AddWindowFleetSize(fenetre, -1);
			*key = 0;
			break;
		case sk_1:
			SetWindowFleetSize(fenetre, 1);
			*key = 0;
			break;
		case sk_2:
			SetWindowFleetSize(fenetre, 2);
			*key = 0;
			break;
		case sk_3:
			SetWindowFleetSize(fenetre, 3);
			*key = 0;
			break;
		case sk_4:
			SetWindowFleetSize(fenetre, 4);
			*key = 0;
			break;
		case sk_5:
			SetWindowFleetSize(fenetre, 5);
			*key = 0;
			break;
		case sk_6:
			SetWindowFleetSize(fenetre, 6);
			*key = 0;
			break;
		case sk_7:
			SetWindowFleetSize(fenetre, 7);
			*key = 0;
			break;
		case sk_8:
			SetWindowFleetSize(fenetre, 8);
			*key = 0;
			break;
		case sk_9:
			SetWindowFleetSize(fenetre, 9);
			*key = 0;
			break;
	}
	if(GetWindowFleetSize(fenetre) < 0){
			SetWindowFleetSize(fenetre, 0);
	}
	if(GetWindowFleetSize(fenetre) > 20){
			SetWindowFleetSize(fenetre, 20);
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
	switch(GetWindowSelection(fenetre)){
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
	PrintInt(GetWindowFleetSize(fenetre));
	gfx_SetTextXY(60, 74);
	gfx_TransparentSprite_NoClip(fer, 45, 74);
	if(GetWindowFleetSize(fenetre) * prix > GetEmpireAlloys(joueur)){
		gfx_SetTextFGColor(3);
	}
	PrintInt(GetWindowFleetSize(fenetre) * prix);

	if(*key == sk_Enter){
		if(GetEmpireAlloys(joueur) >= prix * GetWindowFleetSize(fenetre)){
			AddEmpireAlloys(joueur, -(prix * GetWindowFleetSize(fenetre)));
			NouvelOrdre(GetStationOrderQueue(GetSystemStation(systemeStellaires[GetCameraSystem(camera)])),
				CONSTRUIRE_VAISSEAU,
				1, 
				travail * GetWindowFleetSize(fenetre),
				GetWindowSelection(fenetre),
				GetWindowFleetSize(fenetre),
				prix * GetWindowFleetSize(fenetre)
			);
			SetWindowMenuSystem(fenetre, MENU_SYSTEME_STATION_RESUME);
		}
		*key = 0;
	}
}

/**
 * Dessine le menu systeme
 */
static void MenuSysteme(char* key, EmpireListe* empireListe, Parametres* parametres, Date* date, SystemeStellaire **systemeStellaires, Camera* camera, Fenetre* fenetre){
	Empire *joueur = EmpireNumero(empireListe, 0);
	switch(GetOpenedMenuDetails(fenetre))
	{				
		default:
			break;
		case MENU_SYSTEME_FLOTTES: //liste flottes
			MenuSystemeFlotte(key, empireListe, systemeStellaires, camera, fenetre);
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
			MenuSystemeFlotteDetails(key, systemeStellaires, empireListe, camera, fenetre);
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
 *Dessine le menu du marché galactique
 */
static void MenuMarche(char *key, Marche *marche, Camera *camera, Fenetre *fenetre){
	if(*key == sk_Clear){
		CloseMenu(fenetre, camera);
		*key = 0;
	}
	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_SetColor(1);
	switch(GetWindowSelection(fenetre))
	{
		case 1:
			gfx_PrintStringXY("Argent", 45, 42);
			gfx_PrintStringXY("Non /changeable", 100, 100);
			break;
			
		case 2:
			gfx_PrintStringXY("Minerai", 45, 42);
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
			gfx_PrintStringXY("Nourriture", 45, 42);
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
			gfx_PrintStringXY("Fer", 45, 42);
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
			gfx_PrintStringXY("Autre", 45, 42);
			break;
	}
}

/**
 *Dessine le menu avec la lste des flottes du joueur
 */
static void MenuListeFLottes(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre){
	char niveau = 0;
	int sizeFleet = 0, compteurFlotte = 0;
	Flotte* flotte = NULL;
	Empire *joueur = NULL;

	joueur = EmpireNumero(empireListe, 0);

	switch (*key) {
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Down:
			IncrementWindowSelection(fenetre);
			*key = 0;
			break;
		case sk_Up:
			UnincrementWindowSelection(fenetre);
			*key = 0;
			break;
	}
	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_PrintStringXY("Flotte", 45, 42);
	niveau = 57;
	sizeFleet = FleetArraySize(EmpireFleetGetArray(joueur));
	
	if (GetWindowSelection(fenetre) >= sizeFleet) {
		SetWindowSelection(fenetre, 0);
	} else if (GetWindowSelection(fenetre) < 0) {
		SetWindowSelection(fenetre, sizeFleet);
	}
	
	compteurFlotte = 0;
	flotte = FlotteNumero(EmpireFleetGetArray(joueur), 0);
	while(compteurFlotte < sizeFleet) {
		if(GetWindowSelection(fenetre) == compteurFlotte) {
			gfx_SetTextFGColor(13);
		}
		else {
			gfx_SetTextFGColor(1);
		}
		switch(GetFleetType(flotte)){
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
		PrintInt(compteurFlotte + 1);
		gfx_SetColor(7);
		gfx_HorizLine_NoClip(50, niveau + 12, 220);
		niveau += 18;
		compteurFlotte++;
		flotte = FlotteNumero(EmpireFleetGetArray(joueur), compteurFlotte);
	}

	if(*key == sk_Enter){
		*key = 0;
		SetWindowSelectedFleet(fenetre, GetWindowSelection(fenetre));
		OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_FLOTTE_DETAILS);
		SetWindowPrevious(fenetre, 2);
	}
}

/**
 *Dessine le menu de recherche
 */
static void MenuRecherche(char *key, Camera *camera, Fenetre *fenetre){
	if(*key == sk_Clear) {
		CloseMenu(fenetre, camera);
	}
	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_PrintStringXY("Recherche", 45, 42);
}

/**
 *Dessine le menu avec les contacts
 */
static void MenuContacts(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre){
	int nombreEmpire, empireSelectionne = 1;
	Empire *empire = NULL;
	int niveau = 57;
	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_PrintStringXY("Contacts", 45, 42);
	nombreEmpire = EmpireArraySize(empireListe);
	if(GetWindowSelection(fenetre) >= nombreEmpire){
		SetWindowSelection(fenetre, 1);
	}
	if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, nombreEmpire);
	}
	
	while(empireSelectionne < nombreEmpire){
		empire = EmpireNumero(empireListe, empireSelectionne);
		gfx_SetTextFGColor(1);
		if(GetWindowSelection(fenetre) == empireSelectionne)
			gfx_SetTextFGColor(13);
		gfx_PrintStringXY(GetEmpireNameString(empire), 45, niveau);
		gfx_SetColor(7);
		gfx_HorizLine_NoClip(50, niveau + 12, 220);
		niveau += 18;
		empireSelectionne++;
	}
	switch(*key) {
		default:
			break;
		case sk_Clear:
			CloseMenu(fenetre, camera);
			*key = 0;
			break;
		case sk_Up:
			AddWindowSelection(fenetre, -1);
			*key = 0;
			break;
		case sk_Down:
			AddWindowSelection(fenetre, 1);
			*key = 0;
			break;
		case sk_Enter:
			SetWindowPrevious(fenetre, GetWindowSelection(fenetre));
			SetWindowSelection(fenetre, 0);
			OpenMenu(fenetre, camera, MENU_CONTACTS_DETAILS, 0);
			*key = 0;
			break;
	}
}

static void MenuContactsDetails(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre) {
	Empire *empire = EmpireNumero(empireListe, GetWindowPrevious(fenetre));
	Empire *joueur = EmpireNumero(empireListe, 0);
	int boutonY = 55;
	int boutonIndex = 1;
	int niveauX = 45;
	Relations *relations = RelationNumero(EmpireRelationGetArray(empire), GetWindowPrevious(fenetre));

	if(GetWindowSelection(fenetre) > 6){
		SetWindowSelection(fenetre, 1);
	}
	if(GetWindowSelection(fenetre) < 1){
		SetWindowSelection(fenetre, 6);
	}

	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_SetColor(0);
	gfx_FillRectangle_NoClip(45, 55, 120, 60); //fond rectangle du perso
	gfx_SetColor(7);
	gfx_PrintStringXY(GetEmpireNameString(empire), 45, 42);
	
	switch(GetEmpireSpecies(empire)){
		case 0:
			gfx_ScaledTransparentSprite_NoClip(leader_head_human, 69, 67, 3, 3);
			break;
		case 1:
			gfx_ScaledTransparentSprite_NoClip(leader_head_mammalian, 63, 67, 3, 3);
			break;
		case 2:
			gfx_ScaledTransparentSprite_NoClip(leader_head_molluscoid, 63, 61, 3, 3);
			break;
		case 3:
			gfx_ScaledTransparentSprite_NoClip(leader_head_reptilian, 66, 64, 3, 3);
			break;
		default:
			gfx_SetTextXY(66, 64);
			gfx_PrintString("Sprite Error :");
			PrintInt(GetEmpireSpecies(empire));
			break;
	}
	switch(GetEmpireClothes(empire)){
		case 0:
			gfx_ScaledTransparentSprite_NoClip(leader_clothes_1, 60, 88, 3, 3);
			break;
		case 1:
			gfx_ScaledTransparentSprite_NoClip(leader_clothes_2, 60, 82, 3, 3);
			break;
		case 2:
			gfx_ScaledTransparentSprite_NoClip(leader_clothes_3, 57, 88, 3, 3);
			break;
		default:
			gfx_SetTextXY(57, 88);
			gfx_PrintString("Sprite Error :");
			PrintInt(GetEmpireClothes(empire));
			break;
	}
	
	gfx_Rectangle_NoClip(45, 55, 120, 61); //rectangle du perso


	//opinion
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(45, 119, 120, 13); //rectangle
	gfx_TransparentSprite_NoClip(opinion_icon, 48, 121);
	gfx_SetTextXY(58, 122);
	if(RelationGetOpinion(relations) < -750){
		gfx_SetTextFGColor(3);
		PrintInt(RelationGetOpinion(relations));
		gfx_PrintString(" Terribles");
	}
	else if(RelationGetOpinion(relations) < -300){
		gfx_SetTextFGColor(9);
		PrintInt(RelationGetOpinion(relations));
		gfx_PrintString(" Tendues");
	}
	else if(RelationGetOpinion(relations) < 300){
		gfx_SetTextFGColor(13);
		PrintInt(RelationGetOpinion(relations));
		gfx_PrintString(" Neutres");
	}
	else if(RelationGetOpinion(relations) < 750){
		gfx_SetTextFGColor(19);
		PrintInt(RelationGetOpinion(relations));
		gfx_PrintString(" Positives");
	}
	else{
		gfx_SetTextFGColor(17);
		PrintInt(RelationGetOpinion(relations));
		gfx_PrintString(" Excellentes");
	}
	

	//puissance
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(45, 135, 38, 13);
	gfx_TransparentSprite_NoClip(alliedFleet, 50, 139);
	gfx_SetTextXY(73, 138);
	if(GetEmpirePowerMilitary(empire) > GetEmpirePowerMilitary(joueur) + (GetEmpirePowerMilitary(joueur) / 10)){
		gfx_SetTextFGColor(3);
		gfx_PrintString(">");
	}
	else if(GetEmpirePowerMilitary(empire) < GetEmpirePowerMilitary(joueur) - (GetEmpirePowerMilitary(joueur) / 10)){
		gfx_SetTextFGColor(19);
		gfx_PrintString("<");
	}
	else{
		gfx_SetTextFGColor(13);
		gfx_PrintString("=");
	}
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(86, 135, 38, 13);
	gfx_TransparentSprite_NoClip(credit, 91, 136);
	gfx_SetTextXY(114, 138);
	if(GetEmpirePowerEconomic(empire) > GetEmpirePowerEconomic(joueur) + (GetEmpirePowerEconomic(joueur) / 10)){
		gfx_SetTextFGColor(3);
		gfx_PrintString(">");
	}
	else if(GetEmpirePowerEconomic(empire) < GetEmpirePowerEconomic(joueur) - (GetEmpirePowerEconomic(joueur) / 10)){
		gfx_SetTextFGColor(19);
		gfx_PrintString("<");
	}
	else{
		gfx_SetTextFGColor(13);
		gfx_PrintString("=");
	}
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(127, 135, 38, 13);
	gfx_TransparentSprite_NoClip(science, 131, 137);
	gfx_SetTextXY(155, 138);
	if(GetEmpirePowerScientific(empire) > GetEmpirePowerScientific(joueur) + (GetEmpirePowerScientific(joueur) / 10)){
		gfx_SetTextFGColor(3);
		gfx_PrintString(">");
	}
	else if(GetEmpirePowerScientific(empire) < GetEmpirePowerScientific(joueur) - (GetEmpirePowerScientific(joueur) / 10)){
		gfx_SetTextFGColor(19);
		gfx_PrintString("<");
	}
	else{
		gfx_SetTextFGColor(13);
		gfx_PrintString("=");
	}

	//attitude
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(45, 151, 120, 13);
	gfx_TransparentSprite_NoClip(attitude_icon, 48, 152);
	gfx_SetTextXY(58, 154);
	if(RelationGetAttitude(relations) < ATTITUDE_NEUTRAL){
		gfx_SetTextFGColor(3);
	}
	else if(RelationGetAttitude(relations) > ATTITUDE_NEUTRAL){
		gfx_SetTextFGColor(19);
	}
	else{
		gfx_SetTextFGColor(13);
	}
	switch(RelationGetAttitude(relations)) {
		default:
			gfx_PrintString("Erreur_inconnu");
			break;
		case ATTITUDE_HOSTILE:
			gfx_PrintString("Hostile");
			break;
		case ATTITUDE_SUSPICIOUS:
			gfx_PrintString("Suspicious");
			break;
		case ATTITUDE_NEUTRAL:
			gfx_PrintString("Neutral");
			break;
		case ATTITUDE_CORDIAL:
			gfx_PrintString("Cordial");
			break;
		case ATTITUDE_FRIENDLY:
			gfx_PrintString("Friendly");
			break;
		case ATTITUDE_PROTECTIVE:
			gfx_PrintString("Protective");
			break;
	}
	
	//attitude
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(45, 167, 120, 13);
	gfx_TransparentSprite_NoClip(pact_icon, 48, 169);
	if(RelationGetAttitude(relations) < ATTITUDE_NEUTRAL){
		gfx_SetTextFGColor(3);
	}
	else if(RelationGetAttitude(relations) > ATTITUDE_NEUTRAL){
		gfx_SetTextFGColor(19);
	}
	else{
		gfx_SetTextFGColor(13);
	}
	niveauX = 58;
	if(RelationGetPacteStatus(relations, PACTE_NON_AGRESSION)){
		gfx_TransparentSprite_NoClip(attitude_icon, niveauX, 168);
		niveauX += 13;
	}
	if(RelationGetPacteStatus(relations, PACTE_RECHERCHE)){
		gfx_TransparentSprite_NoClip(science, niveauX, 168);
		niveauX += 13;
	}
	//boutons actions
	gfx_SetColor(7);
	gfx_SetTextFGColor(1);
	while(boutonY + 16 < 200 && boutonIndex < 7) {
		gfx_Rectangle_NoClip(169, boutonY, 106, 13);
		if(GetWindowSelection(fenetre) == boutonIndex) {
			gfx_SetTextFGColor(13);
		}
		switch(boutonIndex) {
			default:
				break;
			case 1:
				gfx_TransparentSprite_NoClip(up_icon, 176, boutonY + 2);
				gfx_PrintStringXY("relations", 184, boutonY + 2);
				break;
			case 2:
				gfx_TransparentSprite_NoClip(down_icon, 176, boutonY + 2);
				gfx_PrintStringXY("relations", 184, boutonY + 2);
				break;
			case 3:
				gfx_TransparentSprite_NoClip(attitude_icon, 172, boutonY + 2);
				gfx_PrintStringXY("guerre", 184, boutonY + 2);
				break;
			case 4:
				gfx_TransparentSprite_NoClip(insult_icon, 174, boutonY + 4);
				gfx_PrintStringXY("insulter", 184, boutonY + 2);
				break;
			case 5:
				gfx_TransparentSprite_NoClip(pact_icon, 172, boutonY + 2);
				gfx_PrintStringXY("non-agress.", 184, boutonY + 2);
				break;
			case 6:
				gfx_TransparentSprite_NoClip(pact_icon, 172, boutonY + 2);
				gfx_PrintStringXY("recherche", 184, boutonY + 2);
				break;
		}
		if(GetWindowSelection(fenetre) == boutonIndex) {
			gfx_SetTextFGColor(1);
		}
		boutonY += 16;
		boutonIndex++;
	}
	switch(*key){
		default:
			break;
		case sk_Clear:
			OpenMenu(fenetre, camera, MENU_CONTACTS, 0);
			SetWindowSelection(fenetre, GetWindowPrevious(fenetre));
			*key = 0;
			break;
		case sk_Up:
			AddWindowSelection(fenetre, -1);
			*key = 0;
			break;
		case sk_Down:
			AddWindowSelection(fenetre, +1);
			*key = 0;
			break;
		case sk_Enter:
			switch (GetWindowSelection(fenetre)) {
				case 1:
					RelationAmeliorer(relations);
					break;
				case 2:
					RelationDegrader(relations);
					break;
				case 3:
					RelationGuerreDeclarer(relations);
					break;
				case 4:
					SetWindowEmpire(fenetre, GetWindowSelection(fenetre));
					boutonIndex = GetWindowSelection(fenetre);
					OpenMenu(fenetre, camera, MENU_CONTACTS_EFFECTUER_ACTION, 0);
					SetWindowSelection(fenetre, boutonIndex);
					*key = 0;
					break;
				case 5:
					RelationSetPacte(relations, PACTE_NON_AGRESSION);
					break;
				case 6:
					RelationSetPacte(relations, PACTE_RECHERCHE);
					break;
				default:
					break;
			}
			break;
	}
}

static void MenuContactsEffectuerAction(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre) {
	Empire *empire = EmpireNumero(empireListe, GetWindowPrevious(fenetre));
	int boutonIndex = 1;
	Relations *relations = RelationNumero(EmpireRelationGetArray(empire), GetWindowPrevious(fenetre));

	//dessiner fenetre
	DrawMenuBase(0, 0, fenetre);
	gfx_SetColor(0);
	gfx_FillRectangle_NoClip(45, 55, 120, 60); //fond rectangle du perso
	gfx_SetColor(7);
	gfx_PrintStringXY(GetEmpireNameString(empire), 45, 42);
	
	switch(GetEmpireSpecies(empire)){
		case 0:
			gfx_ScaledTransparentSprite_NoClip(leader_head_human, 69, 67, 3, 3);
			break;
		case 1:
			gfx_ScaledTransparentSprite_NoClip(leader_head_mammalian, 63, 67, 3, 3);
			break;
		case 2:
			gfx_ScaledTransparentSprite_NoClip(leader_head_molluscoid, 63, 61, 3, 3);
			break;
		case 3:
			gfx_ScaledTransparentSprite_NoClip(leader_head_reptilian, 66, 64, 3, 3);
			break;
		default:
			gfx_SetTextXY(66, 64);
			gfx_PrintString("Sprite Error :");
			PrintInt(GetEmpireSpecies(empire));
			break;
	}
	switch(GetEmpireClothes(empire)){
		case 0:
			gfx_ScaledTransparentSprite_NoClip(leader_clothes_1, 60, 88, 3, 3);
			break;
		case 1:
			gfx_ScaledTransparentSprite_NoClip(leader_clothes_2, 60, 82, 3, 3);
			break;
		case 2:
			gfx_ScaledTransparentSprite_NoClip(leader_clothes_3, 57, 88, 3, 3);
			break;
		default:
			gfx_SetTextXY(57, 88);
			gfx_PrintString("Sprite Error :");
			PrintInt(GetEmpireClothes(empire));
			break;
	}
	
	gfx_Rectangle_NoClip(45, 55, 120, 61); //rectangle du perso

	gfx_SetTextXY(50, 120);
	switch(GetWindowSelection(fenetre)) {
		case(4):
			PrintMultipleLines("Are you in great physical pain, or is that your thinking expression ?");
			gfx_SetTextXY(60, 187);
			gfx_SetTextFGColor(13);
			gfx_PrintString("Confirmer");
			
			break;
	}
	
	switch(*key){
		default:
			break;
		case sk_Clear:
			boutonIndex = GetWindowSelection(fenetre);
			OpenMenu(fenetre, camera, MENU_CONTACTS_DETAILS, 0);
			SetWindowSelection(fenetre, boutonIndex);
			*key = 0;
			break;
		case sk_Enter:
			boutonIndex = GetWindowSelection(fenetre);
			OpenMenu(fenetre, camera, MENU_CONTACTS_DETAILS, 0);
			SetWindowSelection(fenetre, boutonIndex);
			RelationInsulter(relations);
			*key = 0;
			break;
	}
}


/* entry points ======================================================== */

/**
 * Fonction gérant les differents menus in-game
 */
int DrawMenu(EmpireListe *empireListe, Date *date, char *key, Camera *camera, SystemeStellaire **systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t* sauvegarde, Marche *marche){
	int8_t fin = 1;

	switch(GetOpenedMenuClass(fenetre)){
		case MENU_QUITTER:
			fin = MenuQuitter(key, sauvegarde, empireListe, parametres, date, systemeStellaires, camera, marche, fenetre);

			if (!fin){
				return 0;
			}
			break;
		case MENU_SYSTEME:
			MenuSysteme(key, empireListe, parametres, date, systemeStellaires, camera, fenetre);
			break;
		case MENU_FLOTTE:
			MenuListeFLottes(key, empireListe, camera, fenetre);
			break;
		case MENU_MARCHE:
			MenuMarche(key ,marche, camera, fenetre);
			break;
		case MENU_RECHERCHE:
			MenuRecherche(key, camera, fenetre);
			break;
		case MENU_CONTACTS:
			MenuContacts(key, empireListe, camera, fenetre);
			break;
		case MENU_CONTACTS_DETAILS:
			MenuContactsDetails(key, empireListe, camera, fenetre);
			break;
		case MENU_CONTACTS_EFFECTUER_ACTION:
			MenuContactsEffectuerAction(key, empireListe, camera, fenetre);
			break;
		default:
			break;
	}
	return 1;
}






