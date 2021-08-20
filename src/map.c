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
#include "colors.h"

#include "main.h"

#include "map.h"
#include "galaxy.h"

#include "locale/locale.h"

/* private functions =================================================== */

static void CouleurEtoile(int type);

/**
 *dessine à l'écran quand une planete est habitable en mode carte normale
 */
static void DessinerPlanetesHabitables(StarSystem **systemeStellaires, Settings *parametres, int i, int x, int y){
	char nombrePlanetesHabitablesSysteme = 0, nombrePlanetesHabiteesSysteme = 0;
	int j = 0;
	gfx_SetTextFGColor(1);
	gfx_SetColor(16);
	//dessiner planetes habitables
	while(j < 5){
		if(starSystem_PlanetHabitabilityGet(systemeStellaires[i], j) == 1) {
			nombrePlanetesHabitablesSysteme++;
			if(starSystem_PlanetCityPopulationGet(systemeStellaires[i], j) != 0){
				nombrePlanetesHabiteesSysteme++;
			}
		}
		j++;
	}
	if(nombrePlanetesHabiteesSysteme){
		gfx_FillRectangle(x - strlen(starSystem_NameGet(systemeStellaires[i])) * 4, y + 8, strlen(starSystem_NameGet(systemeStellaires[i])) * 8, 10);
		gfx_PrintStringXY(starSystem_NameGet(systemeStellaires[i]), x - (strlen(starSystem_NameGet(systemeStellaires[i])) * 4), y + 9);
	} else {
		if(starSystem_IntelLevelGet(systemeStellaires[i]) >= INTEL_LOW || settings_SeeAllGet(parametres)) {
			if(starSystem_IntelLevelGet(systemeStellaires[i]) >= INTEL_HIGH) {
				gfx_SetTextFGColor(1);
			} else {
				gfx_SetTextFGColor(11);
			}
			gfx_PrintStringXY(starSystem_NameGet(systemeStellaires[i]), x - (strlen(starSystem_NameGet(systemeStellaires[i])) * 4), y + 9);
		}
	}
	if(nombrePlanetesHabitablesSysteme > 0) {
		gfx_TransparentSprite(highHabitablePlanet, x - 10, y);
	}
}

/**
 *dessine les flottes sur la map
 */
static void DessinerFlottesMap(EmpireList* empireListe, Empire* joueur, StarSystem **systemeStellaires, Camera* camera, Settings *parametres){
	Flotte* flotte;
	Empire *empire;
	int system, xFlotte, yFlotte;
	int index = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	int empireIndex = 1;
	int fleetIndex = 0;
	int size = 0;
	int empireSize = 0;
	//dessiner flottes joueur
	flotte = FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), 0);
	size = FleetArraySize(empire_FleetListGet(joueur));
	while(fleetIndex <= size) {
		system = GetFleetSystem(flotte);
		xFlotte = starSystem_GetX(systemeStellaires[system]) * GetCameraZoom(camera) - GetCameraX(camera) + 165;
		yFlotte = starSystem_GetY(systemeStellaires[system]) * GetCameraZoom(camera) - GetCameraY(camera) + 110;
		if(((xFlotte > 0) && (xFlotte < 320)) && ((0 < yFlotte) && (yFlotte < 240))){
			while(gfx_GetPixel(xFlotte + 4, yFlotte + 1) != 0){
				xFlotte += 11;
			}
			gfx_TransparentSprite(ourFleet, xFlotte, yFlotte);
			switch(GetFleetType(flotte)){
				case FLOTTE_MILITAIRE:
					gfx_TransparentSprite(force_our, xFlotte + 6, yFlotte - 1);
					if (GetFleetPower(flotte) > 500)
					{
						gfx_TransparentSprite(force_our, xFlotte + 10, yFlotte - 1);
					}
					if (GetFleetPower(flotte) > 1500)
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
		index = GetFleetPathProgress(flotte) - 1;
		gfx_SetColor(13);
		while((GetFleetPath(flotte, index) != 0) && (GetFleetPath(flotte, index + 1) != 0)){
			x1 = starSystem_GetX(systemeStellaires[GetFleetPath(flotte, index)]) * GetCameraZoom(camera) - GetCameraX(camera) + 160;
			y1 = starSystem_GetY(systemeStellaires[GetFleetPath(flotte, index)]) * GetCameraZoom(camera) - GetCameraY(camera) + 120;
			x2 = starSystem_GetX(systemeStellaires[GetFleetPath(flotte, index + 1)]) * GetCameraZoom(camera) - GetCameraX(camera) + 160;
			y2 = starSystem_GetY(systemeStellaires[GetFleetPath(flotte, index + 1)]) * GetCameraZoom(camera) - GetCameraY(camera) + 120;
			if((((x1 > 0) && (x1 < 320)) && ((0 < y1) && (y1 < 240))) && (((x2 > 0) && (x2 < 320)) && ((0 < y2) && (y2 < 240)))){
				gfx_Line_NoClip(x1, y1, x2, y2);
			}
			index++;
		}
		fleetIndex++;
		flotte = FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), fleetIndex);
	}
	empire = empire_Get(empireListe, 1);
	empireIndex = 1;
	empireSize = empire_ArraySize(empireListe);
	while(empireIndex <= empireSize){
		fleetIndex = 0;
		flotte = FlotteNumero(empire_FleetListGet(empire), 0);
		size = FleetArraySize(empire_FleetListGet(empire));
		while(fleetIndex <= size) {
			system = GetFleetSystem(flotte);
			if((starSystem_IntelLevelGet(systemeStellaires[system]) >= INTEL_MEDIUM) || settings_SeeAllGet(parametres)){
				xFlotte = starSystem_GetX(systemeStellaires[system]) * GetCameraZoom(camera) - GetCameraX(camera) + 165;
				yFlotte = starSystem_GetY(systemeStellaires[system]) * GetCameraZoom(camera) - GetCameraY(camera) + 110;
				if(((xFlotte > 0) && (xFlotte < 320)) && ((0 < yFlotte) && (yFlotte < 240))){
					while(gfx_GetPixel(xFlotte + 4, yFlotte + 1) != 0){
						xFlotte += 11;
					}
					gfx_TransparentSprite(neutralFleet, xFlotte, yFlotte);
					switch(GetFleetType(flotte)){
					case FLOTTE_MILITAIRE:
						gfx_TransparentSprite(force_neutral, xFlotte + 6, yFlotte - 1);
						if (GetFleetPower(flotte) > 500){
							gfx_TransparentSprite(force_neutral, xFlotte + 10, yFlotte - 1);
						}
						if (GetFleetPower(flotte) > 1500){
							gfx_TransparentSprite(force_neutral, xFlotte + 8, yFlotte - 4);
						}
						break;
					case FLOTTE_DE_CONSTRUCTION:
						gfx_TransparentSprite(construction_ship_neutral_icon, xFlotte + 6, yFlotte - 4);
						break;
					case FLOTTE_SCIENTIFIQUE:
						gfx_TransparentSprite(science_ship_neutral_icon, xFlotte + 6, yFlotte - 4);
						break;
					}
				}
			}
			fleetIndex++;
			flotte = FlotteNumero(empire_FleetListGet(empire), fleetIndex);
		}
		empireIndex++;
		empire = empire_Get(empireListe, empireIndex);
	}
}

/**
 *Dessine une hyperlane
 */
static void DessinerHyperlane(int8_t niveauDeConnaissance1, int8_t niveauDeConnaissance2, int16_t x, int16_t y, int16_t xLn, int16_t yLn, Camera* camera){
	if((niveauDeConnaissance1 == INTEL_UNKNOWN) || (niveauDeConnaissance2 == INTEL_UNKNOWN)) {
		gfx_SetColor(11);
	}
	else {
		gfx_SetColor(12);
	}
	
	if(GetCameraMapType(camera) == VUE_GALACTIC)
	{
		xLn = xLn * GetCameraZoom(camera) - GetCameraX(camera) + 160;
		yLn = yLn * GetCameraZoom(camera) - GetCameraY(camera) + 120;
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

/**
 *dessiner la map en vision normale et galactique
 */
static void DessinerVueMap(StarSystem **systemeStellaires, Camera *camera, EmpireList *empireListe, Settings *parametres){
	int i = 0, x = 0, y = 0, xLn, yLn, hyperLane1 = 0, hyperLane2 = 0, hyperLane3 = 0, hyperLane4 = 0, systeme = 0;
	//dessiner carte
	gfx_SetTextConfig(gfx_text_clip);
	SetCameraViewedSystem(camera, -1);
	while (i < (GALAXY_WIDTH * GALAXY_WIDTH) - 1) {
		hyperLane1 = 255;
		hyperLane2 = 255;
		hyperLane3 = 255;
		hyperLane4 = 255;
		
		xLn = 0;
		yLn = 0;
		if(GetCameraMapType(camera) == VUE_GALACTIC){
			x = starSystem_GetX(systemeStellaires[i]) * GetCameraZoom(camera) - GetCameraX(camera) + 160;
			y = starSystem_GetY(systemeStellaires[i]) * GetCameraZoom(camera) - GetCameraY(camera) + 120;
		} else if(GetCameraMapType(camera) == VUE_MAP) {
			x = starSystem_GetX(systemeStellaires[i]) / 2.5 + 10;
			y = starSystem_GetY(systemeStellaires[i]) / 2.5 - 30;
		}
		if ( (((0 <= x) && (x <= 320)) && ((0 <= y)&& (y <= 240))) && ((starSystem_GetX(systemeStellaires[i]) != 0) && (starSystem_GetY(systemeStellaires[i]) != 0)) ) {
			if((starSystem_IntelLevelGet(systemeStellaires[i]) != INTEL_UNKNOWN) || settings_SeeAllGet(parametres)) {
				if(starSystem_EmpireGet(systemeStellaires[i]) != -1){
					gfx_SetColor(GetEmpireColor(empire_Get(empireListe, starSystem_EmpireGet(systemeStellaires[i]))));
					if(GetCameraMapType(camera) == VUE_GALACTIC){
						gfx_Circle(x, y, 20 * GetCameraZoom(camera));
						gfx_Circle(x, y, (19 * GetCameraZoom(camera)) + 1);
					} else {
						gfx_Circle(x, y, 10);
						gfx_Circle(x, y, 9);
					}
				}
				
				if(GetCameraMapType(camera) == VUE_GALACTIC){
					DessinerPlanetesHabitables(systemeStellaires, parametres, i, x, y);
				}

				//dessiner hyperLanes
				hyperLane1 = hyperlane_DestinationGet(systemeStellaires[i], 0);
				if (hyperLane1 != 255){
					xLn = starSystem_GetX(systemeStellaires[hyperLane1]);
					yLn = starSystem_GetY(systemeStellaires[hyperLane1]);
					DessinerHyperlane(starSystem_IntelLevelGet(systemeStellaires[hyperLane1]), starSystem_IntelLevelGet(systemeStellaires[i]), x, y, xLn, yLn, camera);
				}
				
				hyperLane2 = hyperlane_DestinationGet(systemeStellaires[i], 1);
				if (hyperLane2 != 255){
					xLn = starSystem_GetX(systemeStellaires[hyperLane2]);
					yLn = starSystem_GetY(systemeStellaires[hyperLane2]);
					DessinerHyperlane(starSystem_IntelLevelGet(systemeStellaires[hyperLane1]), starSystem_IntelLevelGet(systemeStellaires[i]), x, y, xLn, yLn, camera);
				}

				hyperLane3 = hyperlane_DestinationGet(systemeStellaires[i], 2);
				if (hyperLane3 != 255){
					xLn = starSystem_GetX(systemeStellaires[hyperLane3]);
					yLn = starSystem_GetY(systemeStellaires[hyperLane3]);
					DessinerHyperlane(starSystem_IntelLevelGet(systemeStellaires[hyperLane3]), starSystem_IntelLevelGet(systemeStellaires[i]), x, y, xLn, yLn, camera);
				}
				
				hyperLane4 = hyperlane_DestinationGet(systemeStellaires[i], 3);
				if (hyperLane4 != 255){
					xLn = starSystem_GetX(systemeStellaires[hyperLane4]);
					yLn = starSystem_GetY(systemeStellaires[hyperLane4]);
					DessinerHyperlane(starSystem_IntelLevelGet(systemeStellaires[hyperLane4]), starSystem_IntelLevelGet(systemeStellaires[i]), x, y, xLn, yLn, camera);
				}
			}
			
			if(GetCameraMapType(camera) == VUE_GALACTIC){
				CouleurEtoile(starSystem_StarTypeGet(systemeStellaires[i]));
				switch(starSystem_StarTypeGet(systemeStellaires[i])){
					case STAR_TYPE_B: //B
						gfx_FillCircle(x, y, 1 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_A: //A
						gfx_FillCircle(x, y, 2 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_F: //F
						gfx_FillCircle(x, y, 2 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_G: //G
						gfx_FillCircle(x, y, 2 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_K: //K
						gfx_FillCircle(x, y, 1 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_M: //M
						gfx_FillCircle(x, y, 2 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_BLACKHOLE: //trou noir
						gfx_Circle(x, y, 2 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_PULSAR: //pulsar
						gfx_FillCircle(x, y, 1 * GetCameraZoom(camera));
						break;
					case STAR_TYPE_NEUTRON: ///toile a neutrons
						gfx_FillCircle(x, y, 1 * GetCameraZoom(camera));
						gfx_Circle(x, y, 3 * GetCameraZoom(camera));
						break;
					default:
						gfx_FillCircle(x, y, 2 * GetCameraZoom(camera));
						break;
				}
			} else {
				CouleurEtoile(starSystem_StarTypeGet(systemeStellaires[i]));
				gfx_SetPixel(x, y);
			}
		}

		//Le if suivant teste si la systeme est celui selectionné
		if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y))) {
			gfx_SetColor(9);
			gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
			SetCameraViewedSystem(camera, i);
		}
		i++;
	}
	gfx_SetTextConfig(gfx_text_noclip);
}


/**
 * Dessine les fleches des hyperlane en vue systeme
 * */
static void DessinerHyperlanesSysteme(StarSystem **systemeStellaires, Camera *camera, Window *fenetre, char *key){
	StarSystem *systeme;
	int hyperlane;
	int x, y;
	char angleChar[10];
	int index;

	systeme = systemeStellaires[GetCameraSystem(camera)];
	
	gfx_SetColor(COLOR_HYPERLANES_UNKNOWN);
	index = 0;
	// verifie que les hyperlane existent
	while(index < 4){
		hyperlane = hyperlane_DestinationGet(systeme, index);
		if(hyperlane != 255){
			x = hyperlane_XGet(systeme, index) - GetCameraXSystem(camera);
			y = hyperlane_YGet(systeme, index) - GetCameraYSystem(camera);

			if(((0 < x) && (x < 310)) && ((0 < y) && (y < 230))){
				gfx_Rectangle_NoClip(x, y, 10, 10);
			}
		}
		index++;
	}
}

/**
 *set la couleur pour une étoile
 */
static void CouleurEtoile(int type){	
	//dessiner etoile
	switch(type) {
		case STAR_TYPE_B: // B
			gfx_SetColor(COLOR_STAR_B);
			break;
		case STAR_TYPE_A: // A
			gfx_SetColor(COLOR_STAR_A);
			break;
		case STAR_TYPE_F: // F
			gfx_SetColor(COLOR_STAR_F);
			break;
		case STAR_TYPE_G: // G
			gfx_SetColor(COLOR_STAR_G);
			break;
		case STAR_TYPE_K: // K
			gfx_SetColor(COLOR_STAR_K);
			break;
		case STAR_TYPE_M: // M
			gfx_SetColor(COLOR_STAR_M);
			break;
		case STAR_TYPE_BLACKHOLE: // trou noir
			gfx_SetColor(COLOR_STAR_BLACK_HOLE);
			break;
		case STAR_TYPE_PULSAR: // pulsar
			gfx_SetColor(COLOR_STAR_PULSAR);
			break;
		case STAR_TYPE_NEUTRON: // étoile a neutrons
			gfx_SetColor(COLOR_STAR_NEUTRINOS);
			break;
		default:
			gfx_SetColor(COLOR_STAR_DEFAULT);
			break;
	}
}

/**
 *Dessine l'étoile en vue systeme
 */
static void DessinerEtoile(StarSystem *systeme, Camera* camera, Window *fenetre, char* key){
	int xEtoile = SYSTEM_MIDDLE_X - GetCameraXSystem(camera), yEtoile = SYSTEM_MIDDLE_Y - GetCameraYSystem(camera), rayon = 0;
	switch(starSystem_StarTypeGet(systeme)){
		case STAR_TYPE_B:
			rayon = 6;
			break;
		case STAR_TYPE_A:
			rayon = 5;
			break;
		case STAR_TYPE_F:
			rayon = 4;
			break;
		case STAR_TYPE_G:
			rayon = 3;
			break;
		case STAR_TYPE_K:
			rayon = 2;
			break;
		case STAR_TYPE_M:
			rayon = 2;
			break;
		case STAR_TYPE_BLACKHOLE:
			rayon = 2;
			break;
		case STAR_TYPE_PULSAR:
			rayon = 3;
			break;
		case STAR_TYPE_NEUTRON:
			rayon = 4;
			break;
	}
	if(starSystem_StarTypeGet(systeme) < 6){
		CouleurEtoile(starSystem_StarTypeGet(systeme));
		gfx_FillCircle(xEtoile, yEtoile, rayon);
	}
	else{
		CouleurEtoile(starSystem_StarTypeGet(systeme));
		switch(starSystem_StarTypeGet(systeme)){
			case STAR_TYPE_BLACKHOLE: //trou noir
				gfx_Circle(xEtoile, yEtoile, 4);
				break;
			case STAR_TYPE_PULSAR: //pulsar
				gfx_FillCircle(xEtoile, yEtoile, 1);
				break;
			case STAR_TYPE_NEUTRON: ///toile a neutrons
				gfx_FillCircle(xEtoile, yEtoile, 1);
				gfx_Circle(xEtoile, yEtoile, 8);
				break;
			default:
				gfx_Rectangle(xEtoile, yEtoile, 4, 4);
		}
	}
	if(((150 <= xEtoile) && (170 >= xEtoile)) && ((110 <= yEtoile) && (130 >= yEtoile)))
	{
		gfx_SetColor(9);
		gfx_Rectangle_NoClip(xEtoile - 8, yEtoile - 8, 16, 16);
		if((*key == sk_Enter) && (GetCameraLockStatus(camera) == false)){
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_ETOILE);
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
 *Dessine une planète pour le menu systeme de la fonction StellarisHUD
 */
static void DessinerPlanete(StarSystem* systeme, Planet* planete, Camera* camera, Window *fenetre, int numero, char* key){
	int x, y;
	char nomNumero[10], decalage = 0;
	x = planet_GetX(planete) - GetCameraXSystem(camera);
	y = planet_GetY(planete) - GetCameraYSystem(camera);
	gfx_SetColor(11);
	gfx_Circle(SYSTEM_MIDDLE_X - GetCameraXSystem(camera), SYSTEM_MIDDLE_Y - GetCameraYSystem(camera), planet_OrbitRadiusGet(planete));
	CouleurPlanete(GetPlanetType(planete));
	gfx_FillCircle(x, y, planet_SizeGet(planete));
	gfx_SetTextFGColor(1);
	if(((50 < x) && (x < 310)) && ((10 < y) && (y < 220))){
		if(planet_HabitabilityGet(planete)){
			if(GetPlanetCityPopulation(planete) > 0){
				gfx_SetColor(6);
				gfx_FillRectangle(x - strlen(planet_NameGet(planete)) * 4 - 2, y + 8, strlen(planet_NameGet(planete)) * 8 + 4, 10);
				gfx_SetColor(7);
				gfx_Rectangle(x - strlen(planet_NameGet(planete)) * 4 - 2, y + 8, strlen(planet_NameGet(planete)) * 8 + 4, 10);
			} else {
				gfx_TransparentSprite(highHabitablePlanet, x - (strlen(planet_NameGet(planete)) * 4) - 9, y + 9);
			}
			gfx_PrintStringXY(planet_NameGet(planete), x - (strlen(planet_NameGet(planete)) * 4), y + 9);
		} else {
			switch(numero){
				case 0:
					decalage = 8;
					strcpy(nomNumero, " I");
					break;
				case 1:
					decalage = 12;
					strcpy(nomNumero, " II");
					break;
				case 2:
					decalage = 16;
					strcpy(nomNumero, " III");
					break;
				case 3:
					decalage = 12;
					strcpy(nomNumero, " IV");
					break;
				case 4:
					decalage = 8;
					strcpy(nomNumero, " V");
					break;
			}
			// gfx_SetColor(0);
			// gfx_FillRectangle(x - strlen(systeme->nom) * 4 - 2 - decalage, y + 8, strlen(systeme->nom) * 8 + 4 + (decalage * 2), 10);
			
			gfx_SetTextXY(x - (strlen(starSystem_NameGet(systeme)) * 4) - decalage, y + 9);
			gfx_PrintString(starSystem_NameGet(systeme));
			gfx_PrintString(nomNumero);
		}
	}

	if(((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y)))
	{
		gfx_SetColor(9);
		gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
		SetCameraSelection(camera, numero);
		if((*key == sk_Enter) && (GetCameraLockStatus(camera) == false)){
			OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_PLANETE_RESUME);
			SetWindowPlanet(fenetre, numero);
			*key = 0;
		}
	}
}

/**
 *Dessine la base du systeme
 */
void DessinerBase(StarSystem *systeme, Camera* camera, Window* fenetre, char* key){
	int x, y;
	if(starSystem_StationLevelGet(systeme) != AUCUNE_STATION){
		gfx_SetColor(1);
		x = 465 - GetCameraXSystem(camera);
		y = 345 - GetCameraYSystem(camera);
		
		//verifie que la base soit dans l'écran
		if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
			switch(starSystem_StationLevelGet(systeme)){ //dessine la station
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
				default:
					break;
			}

			if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (GetCameraLockStatus(camera) == false)))) {
				gfx_SetColor(9);
				gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);
				if((*key == sk_Enter) && (GetCameraLockStatus(camera) == false)){
					OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_STATION_RESUME);
					SetWindowSelection(fenetre, 1);
					*key = 0;
				}
			}
		}
	}
}

/**
 *Dessine les flottes du systeme
 */
static void DessinerFlottesSysteme(EmpireList *empireListe, Camera *camera, Window *fenetre, char* key){
	Empire* empire = NULL;
	Flotte* flotte = NULL;
	int empireIndex = 0, fleetIndex = 0;
	int empireSize = 0;
	int sizeFleet = 0;
	int x, y;
	gfx_SetColor(11);
	empire = empire_Get(empireListe, 0);
	fleetIndex = 0;
	flotte = FlotteNumero(empire_FleetListGet(empire), 0);
	sizeFleet = FleetArraySize(empire_FleetListGet(empire));
	while(fleetIndex <= sizeFleet){
		if(GetFleetSystem(flotte) == GetCameraSystem(camera)){
			x = GetFleetX(flotte) - GetCameraXSystem(camera) - 3;
			y = GetFleetY(flotte) - GetCameraYSystem(camera) - 3;
			while(gfx_GetPixel(x + 4, y + 1) != 0){
				y += 15;
			}
			if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
				if(GetFleetProgress(flotte) == 0){
					gfx_TransparentSprite(ourFleet, x, y);
					switch(GetFleetType(flotte)){
					case FLOTTE_MILITAIRE:
						gfx_TransparentSprite(force_our, x + 6, y - 1);
						if (GetFleetPower(flotte) > 500) {
							gfx_TransparentSprite(force_our, x + 10, y - 1);
						}
						if (GetFleetPower(flotte) > 1500) {
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
				} else if(GetFleetProgress(flotte) == 1) {//dessinne l'hypervitesse
					gfx_SetColor(1);
					gfx_FillCircle_NoClip(x + 3, y, 5);
					SetFleetProgress(flotte, 2);
				}
				x += 3;
				y += 3;
				if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (GetCameraLockStatus(camera) == false)))){
					gfx_SetColor(9);
					gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);			
					if((*key == sk_Enter) && (GetCameraLockStatus(camera) == false)){
						OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_FLOTTE_DETAILS);
						SetWindowSelection(fenetre, 1);
						SetWindowSelectedFleet(fenetre, fleetIndex);
						SetWindowPrevious(fenetre, 1);
						*key = 0;
					}
				}
			}
		}
		fleetIndex++;
		flotte = FlotteNumero(empire_FleetListGet(empire_Get(empireListe, 0)), fleetIndex);
	}
	empire = empire_Get(empireListe, 1);
	empireIndex = 1;
	empireSize = empire_ArraySize(empireListe);
	while(empireIndex <= empireSize){
		fleetIndex = 0;
		flotte = FlotteNumero(empire_FleetListGet(empire), 0);
		sizeFleet = FleetArraySize(empire_FleetListGet(empire));
		while(fleetIndex <= sizeFleet){
			if(GetFleetSystem(flotte) == GetCameraSystem(camera)){
				x = GetFleetX(flotte) - GetCameraXSystem(camera) - 3;
				y = GetFleetY(flotte) - GetCameraYSystem(camera) - 3;
				while(gfx_GetPixel(x + 4, y + 1) != 0){
					y += 15;
				}
				if(((0 < x) && (x < 315)) && ((0 < y) && (y < 235))){
					if(GetFleetProgress(flotte) == 0){
						gfx_TransparentSprite(neutralFleet, x, y);
						switch(GetFleetType(flotte)){
						case FLOTTE_MILITAIRE:
							gfx_TransparentSprite(force_neutral, x + 6, y - 1);
							if (GetFleetPower(flotte) > 500) {
								gfx_TransparentSprite(force_neutral, x + 10, y - 1);
							}
							if (GetFleetPower(flotte) > 1500) {
								gfx_TransparentSprite(force_neutral, x + 8, y - 4);
							}
							break;
						case FLOTTE_DE_CONSTRUCTION:
							gfx_TransparentSprite(construction_ship_neutral_icon, x + 6, y - 4);
							break;
						case FLOTTE_SCIENTIFIQUE:
							gfx_TransparentSprite(science_ship_neutral_icon, x + 6, y - 4);
							break;
						}
					} else if(GetFleetProgress(flotte) == 1) {//dessinne l'hypervitesse
						gfx_SetColor(1);
						gfx_FillCircle_NoClip(x + 3, y, 5);
						SetFleetProgress(flotte, 2);
					}
					x += 3;
					y += 3;
					if((((150 <= x) && (170 >= x)) && ((110 <= y) && (130 >= y) && (GetCameraLockStatus(camera) == false)))){
						gfx_SetColor(9);
						gfx_Rectangle_NoClip(x - 8, y - 8, 16, 16);			
						if((*key == sk_Enter) && (GetCameraLockStatus(camera) == false)){
							OpenMenu(fenetre, camera, MENU_SYSTEME, MENU_SYSTEME_FLOTTE_DETAILS);
							SetWindowSelection(fenetre, 1);
							SetWindowSelectedFleet(fenetre, fleetIndex);
							SetWindowPrevious(fenetre, 1);
							*key = 0;
						}
					}
				}
			}
			fleetIndex++;
			flotte = FlotteNumero(empire_FleetListGet(empire), fleetIndex);
		}
		empireIndex++;
		empire = empire_Get(empireListe, empireIndex);
	}
}

/**
 *dessine le systeme en vue systeme
 */
static void DessinerVueSysteme(StarSystem **systemeStellaires, Camera* camera, Window *fenetre, EmpireList* empireListe, char *key){
	StarSystem* systeme;
	int j = 0;

	systeme = systemeStellaires[GetCameraSystem(camera)];
	DessinerEtoile(systeme, camera, fenetre, key);
	DessinerBase(systeme, camera, fenetre, key);
	DessinerFlottesSysteme(empireListe, camera, fenetre, key);
	DessinerHyperlanesSysteme(systemeStellaires, camera, fenetre, key);
	while(j < GetSystemPlanetNumber(systeme)) {
		DessinerPlanete(systeme, GetPlanet(systeme, j), camera, fenetre, j, key);
		j++;
	}
}

/* entry points ======================================================== */

/**
 *fonction principale pour dessiner la map
 *
 *contient aussi la gestion des évenements
 */
void DrawMap(EmpireList *empireListe, StarSystem **systemeStellaires, Camera *camera, char *key, Time *date, Window *fenetre, Settings *parametres){
	Empire *joueur = empire_Get(empireListe, 0);
	gfx_SetColor(1);
	if(GetCameraMapType(camera) == VUE_GALACTIC) {
		//gere les événements des touches
		if(GetCommandPromptStatus(fenetre) == false){
			// KeyActionNormalMap(empireListe, systemeStellaires, camera, key, date, fenetre, joueur);
		}

		if (GetCameraZoom(camera) < 1) {
			SetCameraZoom(camera, 1);
			SetCameraMapType(camera, VUE_MAP);
		} else if (GetCameraZoom(camera) > 2) {
			SetCameraZoom(camera, 2);
		}
		
		if(GetCameraXVector(camera) != 0) {
			AddCameraX(camera, GetCameraXVector(camera));
			if(GetCameraXVector(camera) > 0) {
				AddCameraXVector(camera, -1);
			} else if(GetCameraXVector(camera) < 0) {
				AddCameraXVector(camera, +1);
			}
		}
		if(GetCameraYVector(camera) != 0) {
			AddCameraY(camera, GetCameraYVector(camera));
			if(GetCameraYVector(camera) > 0) {
				AddCameraYVector(camera, -1);
			} else if(GetCameraYVector(camera) < 0) {
				AddCameraYVector(camera, +1);
			}
		}

		//fait en sorte que la camera sorte pas des limites		
		if (GetCameraX(camera) < 160) {
			SetCameraX(camera, 160);
			SetCameraXVector(camera, 0);
		} else if ((GALAXY_WIDTH + 1) * 50  * GetCameraZoom(camera) < GetCameraX(camera)) {
			SetCameraX(camera, (GALAXY_WIDTH + 1) * 50 * GetCameraZoom(camera));
			SetCameraXVector(camera, 0);
		}
		if (GetCameraY(camera) < 120) {
			SetCameraY(camera, 120);
			SetCameraYVector(camera, 0);
		} else if((GALAXY_WIDTH + 1) * 50  * GetCameraZoom(camera) < GetCameraY(camera)) {
			SetCameraY(camera, (GALAXY_WIDTH + 1) * 50  * GetCameraZoom(camera));
			SetCameraYVector(camera, 0);
		}
		DessinerVueMap(systemeStellaires, camera, empireListe, parametres);
		DessinerFlottesMap(empireListe, joueur, systemeStellaires, camera, parametres);
	}
	else if(GetCameraMapType(camera) == VUE_MAP){
		switch(*key){
			case sk_Del:
				SetCameraMapType(camera, VUE_GALACTIC); //dezoomer
				*key = 0;
				break;
		}
		DessinerVueMap(systemeStellaires, camera, empireListe, parametres);
	}
	else if(GetCameraMapType(camera) == VUE_SYSTEM){
		if(GetCameraLockStatus(camera) == false){
			switch(*key){
				case sk_Mode:
					SetCameraMapType(camera, VUE_GALACTIC); //dezoomer
					SetCameraXVector(camera, 0);
					SetCameraYVector(camera, 0);
					break;
				case sk_Up:
					AddCameraYVector(camera, -4); 
					break;
				case sk_Down:
					AddCameraYVector(camera, +4);
					break;
				case sk_Left:
					AddCameraXVector(camera, -4);
					break;
				case sk_Right:
					AddCameraXVector(camera, +4);
					break;
			}
		}

		if(GetCameraXVector(camera) != 0) {
			AddCameraXSystem(camera, GetCameraXVector(camera));
			if(GetCameraXVector(camera) > 0) {
				AddCameraXVector(camera, -1);
			} else if(GetCameraXVector(camera) < 0) {
				AddCameraXVector(camera, +1);
			}
		}
		if(GetCameraYVector(camera) != 0) {
			AddCameraYSystem(camera, GetCameraYVector(camera));
			if(GetCameraYVector(camera) > 0) {
				AddCameraYVector(camera, -1);
			} else if(GetCameraYVector(camera) < 0) {
				AddCameraYVector(camera, +1);
			}
		}
		if(GetCommandPromptStatus(fenetre) == false) {
			// KeyActionNormal(empireListe, systemeStellaires, camera, key, date, fenetre, joueur);
		}


		//fait en sorte que la camera sorte pas des limites
		while(pow((double)(GetCameraXSystem(camera) - 320), 2.0) + pow((double)(GetCameraYSystem(camera) - 240), 2.0) > pow((double)SYSTEM_VIEW_RADIUS, 2.0)){
			SetCameraXVector(camera, 0);
			SetCameraYVector(camera, 0);
			if(GetCameraXSystem(camera) - 320 > 0){
				AddCameraXSystem(camera, -1);
			}
			if(GetCameraXSystem(camera) - 320 < 0){
				AddCameraXSystem(camera, +1);
			}
			if(GetCameraYSystem(camera) - 240 > 0){
				AddCameraYSystem(camera, -1);
			}
			if(GetCameraYSystem(camera) - 240 < 0){
				AddCameraYSystem(camera, +1);
			}
		}
		DessinerVueSysteme(systemeStellaires, camera, fenetre, empireListe, key);
	}
}
