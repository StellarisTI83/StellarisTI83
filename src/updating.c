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

#include "main.h"

#include "boucle.h"
#include "flottes.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"
#include "time.h"
#include "updating.h"

#include "locale/locale.h"

/* private functions =================================================== */

/**
 * calcule le niveau de connaissance (intel level) du systeme
 */
static void CalculerNiveauDeConnaissance(SystemeStellaire **systemeStellaires, EmpireListe *empireListe){
	int sizeFleet = 0;
	int indexFleet = 0;
	int numeroSysteme = 0;

	while(numeroSysteme < LARGEUR_GALAXIE * LARGEUR_GALAXIE) {
		if(GetSystemIntelLevel(systemeStellaires[numeroSysteme])  >= MOYEN) {
			SetSystemIntelLevel(systemeStellaires[numeroSysteme], MOYEN);
		}
		else {
			SetSystemIntelLevel(systemeStellaires[numeroSysteme], INCONNU);
		}
		if((GetSystemStationLevel(systemeStellaires[numeroSysteme]) > INCONNU) && (GetSystemEmpire(systemeStellaires[numeroSysteme]) == -1)) {
			SetSystemIntelLevel(systemeStellaires[numeroSysteme], TOTAL);
		}
		numeroSysteme++;
	}
	sizeFleet = FleetArraySize(EmpireFleetGetArray(EmpireNumero(empireListe, 0)));
	while(indexFleet <= sizeFleet){
        SetSystemIntelLevel(systemeStellaires[GetFleetSystem(FlotteNumero(EmpireFleetGetArray(EmpireNumero(empireListe, 0)), indexFleet))], ELEVEE);
		indexFleet++;
	}
}

/**
 * Effectue les actions de station
 */
void EffectuerActionsStations(SystemeStellaire **systemeStellaires, EmpireListe* empireListe){
	int numero = 0, nombreDeVaisseaux = 0;
	int nombreDeCorvette = 0, nombreDeDestroyer = 0, nombreDeCroiseur = 0, nombreDeCuirasse = 0;
	OrdreStation ordre = AUCUN_ORDRE_STATION;
    int info1 = 0;
    int info2 = 0;
	int numeroEmpire = 0;
	// Empire *joueur = EmpireNumero(empireListe, 0);
	while(numero < (LARGEUR_GALAXIE * LARGEUR_GALAXIE) - 1){
		ordre = GetSystemStationOrder(systemeStellaires[numero]);
		if(ordre != AUCUN_ORDRE_STATION){
			if(GetSystemStationOrderProgress(systemeStellaires[numero]) > 1){
				UnincrementSystemStationOrderProgress(systemeStellaires[numero]);
			}
			else{
				info1 = GetSystemStationInfo1(systemeStellaires[numero]);
				info2 = GetSystemStationInfo2(systemeStellaires[numero]);
				numeroEmpire = GetSystemEmpire(systemeStellaires[numero]);
				switch(ordre){
				case AMELIORER_STATION:
					SetSystemStationLevel(systemeStellaires[numero], (Stationlevel)(GetSystemStationLevel(systemeStellaires[numero]) + 1));
					break;
				case CONSTRUIRE_MODULE:
					SetSystemStationModule(systemeStellaires[numero], info1 - 1, (Module)info2);
					break;
				case CONSTRUIRE_VAISSEAU:
					nombreDeVaisseaux = info2;
					switch(info1){
						case 1:
							info1 = FLOTTE_SCIENTIFIQUE;
							nombreDeVaisseaux = 1;
							break;
						case 2:
							info1 = FLOTTE_DE_CONSTRUCTION;
							nombreDeVaisseaux = 1;
							break;
						case 3:
							info1 = FLOTTE_MILITAIRE;
							nombreDeCorvette = nombreDeVaisseaux;
							break;
						case 4:
							info1 = FLOTTE_MILITAIRE;
							nombreDeDestroyer = nombreDeVaisseaux;
							break;
						case 5:
							info1 = FLOTTE_MILITAIRE;
							nombreDeCroiseur = nombreDeVaisseaux;
							break;
						case 6:
							info1 = FLOTTE_MILITAIRE;
							nombreDeCuirasse = nombreDeVaisseaux;
							break;
					}
					NouvelleFlotte(EmpireFleetGetArray(EmpireNumero(empireListe, numeroEmpire)), numero, (FlotteType)info1, nombreDeCorvette, nombreDeDestroyer, nombreDeCroiseur, nombreDeCuirasse);
					break;
				default:
					break;
				}
				EndSystemStationOrder(systemeStellaires[numero]);
			}
		}
		numero++;
	}
}


/**
 * Effectue les actions des planetes
 */
void EffectuerActionsPlanetes(SystemeStellaire **systemeStellaires){
	int i = 0, j = 0;
	Villes *villes = NULL;
	// Batiment *batiment = NULL;
	OrdreConstruction ordre;
	for(i = 0; i < LARGEUR_GALAXIE * LARGEUR_GALAXIE; i++){
		for(j = 0; j < GetSystemPlanetNumber(systemeStellaires[i]); j++){
			villes = GetSystemPlanetCity(systemeStellaires[i], j);
			if(villes != NULL){
				ordre = GetCityOrder(villes);
				if(ordre != 0){
					if(GetCityOrderProgress(villes) > 1){
						UnincrementCityOrderProgress(villes);
					}
					else if(GetCityOrderProgress(villes) == 1){
						switch(GetCityOrder(villes)){
							case CONSTRUIRE_DISTRICT_URBAIN:
								AddSystemPlanetCityUrbanDistrict(systemeStellaires[i], j, 1);
								break;
							case CONSTRUIRE_DISTRICT_GENERATEUR:
								AddSystemPlanetCityGeneratorDistrict(systemeStellaires[i], j, 1);
								break;
							case CONSTRUIRE_DISTRICT_MINIER:
								AddSystemPlanetCityMiningDistrict(systemeStellaires[i], j, 1); 
								break;
							case CONSTRUIRE_DISTRICT_AGRICOLE:
								AddSystemPlanetCityAgricultureDistrict(systemeStellaires[i], j, 1); 
								break;
							case CONSTRUIRE_BATIMENT:
								SetSystemPlanetCityBuilding(systemeStellaires[i], j, GetCityOrderInfo1(villes), (Batiment)GetCityOrderInfo2(villes), 1 );
								break;
							default:
								break;
						}
						EndCityOrder(villes);
					}
				}
			}
		}
	}
}

/**
 * Update time
 */
static void UpdateTime(Date *date, char *key, Fenetre *fenetre){
	//différentes actions des touches
	if(!GetCommandPromptStatus(fenetre)){
		switch(*key){
			case sk_0://pause / dépause
				if(GetTimeSpeed(date) == 0){
                    UnpauseGame(date); 
				} else {
                    PauseGame(date);
				}
				break;
			case sk_Add:
                IncrementTimeSpeed(date);
				break;
			case sk_Sub:
				UnincrementTimeSpeed(date);
				break;
		}
	}
	UpdateClock(date);
}

static void UpdateWorld(EmpireListe *empireListe, SystemeStellaire **systemeStellaires){
    EffectuerActionsFlottes(empireListe, systemeStellaires);
    EffectuerActionsStations(systemeStellaires, empireListe);
    EffectuerActionsPlanetes(systemeStellaires);
    CalculerNiveauDeConnaissance(systemeStellaires, empireListe);
}

static void TestKey(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre, Parametres *parametres){
	if(GetCameraMapType(camera) == NORMAL){
		switch(*key){
		case sk_Up:
			if (GetCameraLockStatus(camera) != true)
				AddCameraYVector(camera, -5);
			break;
		case sk_Down:
			if (GetCameraLockStatus(camera) != true)
				AddCameraYVector(camera, +5);
			break;
		case sk_Left:
			if (GetCameraLockStatus(camera) != true)
				AddCameraXVector(camera, -5);
			break;
		case sk_Right:
			if (GetCameraLockStatus(camera) != true)
				AddCameraXVector(camera, +5);
			break;
		case sk_Mode:
			if (GetCameraLockStatus(camera) != true) {
				SetCameraZoom(camera, GetCameraZoom(camera) - 1);
				if(GetCameraZoom(camera) >= 1) {
					SetCameraX(camera, GetCameraX(camera) * 0.5);
					SetCameraY(camera, GetCameraY(camera) * 0.5);
				}
			}
			break;
		case sk_Del:
			if ((GetCameraZoom(camera) == 2) && (((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (GetCameraViewedSystem(camera) != -1)) && ((IsCameraMoveFleet(camera) == false) && ((GetSystemIntelLevel(systemeStellaires[GetCameraViewedSystem(camera)]) != INCONNU) || GetSeeAll(parametres))))) {
				SetCameraLock(camera, false);
				SetCameraMapType(camera, SYSTEME);
				if(GetCameraSystem(camera) != GetCameraViewedSystem(camera)) {
					SetCameraXSystem(camera, 320);
					SetCameraYSystem(camera, 240);
				}
				SetCameraSystem(camera, GetCameraViewedSystem(camera));
				#ifdef DEBUG_VERSION
					dbg_sprintf((char*)dbgout, "open system %d\n", GetCameraViewedSystem(camera));
				#endif
				*key = 0;
			}
			if (GetCameraLockStatus(camera) != true) {
				SetCameraZoom(camera, GetCameraZoom(camera) + 1);
				if (GetCameraZoom(camera) < 3 && GetCameraZoom(camera) >= 0) {
					SetCameraX(camera, GetCameraX(camera) * 2);
					SetCameraY(camera, GetCameraY(camera) * 2);
				}
			}
			break;
		case sk_Enter:
			if (((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (GetCameraViewedSystem(camera) != -1)) && ((IsCameraMoveFleet(camera) == false) && (GetSystemIntelLevel(systemeStellaires[GetCameraViewedSystem(camera)]) != INCONNU || GetSeeAll(parametres)))){
				SetCameraLock(camera, false);
				SetCameraMapType(camera, SYSTEME);
				if(GetCameraSystem(camera) != GetCameraViewedSystem(camera)) {
					SetCameraXSystem(camera, 320);
					SetCameraYSystem(camera, 240);
				}
				SetCameraSystem(camera, GetCameraViewedSystem(camera));
				#ifdef DEBUG_VERSION
					dbg_sprintf((char*)dbgout, "open system %d\n", GetCameraViewedSystem(camera));
				#endif
				*key = 0;
			} else if(((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (GetCameraViewedSystem(camera) != -1)) && (IsCameraMoveFleet(camera) == true)) {
				BougerFlotte(GetCameraFleet(camera), GetCameraEmpire(camera), GetCameraViewedSystem(camera), fenetre, camera, empireListe, systemeStellaires);
				*key = 0;
			}
			break;
		}
	}

	//touches generales

	switch(*key){
	case sk_Clear:
		if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) && (IsCameraMoveFleet(camera) == false)) {
			OpenMenu(fenetre, camera, MENU_QUITTER, MENU_SYSTEME_AUCUN);
			PauseGame(date);
			*key = 0;
		} else if(IsCameraMoveFleet(camera) == true) {
			SetCameraMoveFleet(camera, false);
			*key = 0;
		}
		break;
	case sk_Yequ :
		if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 1)) {
			OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 1);
		} else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 1)) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Window :
		if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 2)) {
			OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 2);
		} else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 2)) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Zoom :
		if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 3)) {
			OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 3);
		} else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 3)) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Trace :
		if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 4)) {
			OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 4);
		} else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 4)) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Graph :
		if ((GetOpenedMenuClass(fenetre) == MENU_AUCUN) || (GetWindowSelection(fenetre) != 5)) {
			OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 5);
		} else if ((GetOpenedMenuClass(fenetre) == MENU_MARCHE) && (GetWindowSelection(fenetre) != 5)) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Recip:
		if ((GetOpenedMenuClass(fenetre) != MENU_MARCHE) && (GetOpenedMenuClass(fenetre) != MENU_QUITTER)) {
			OpenMenu(fenetre, camera, MENU_MARCHE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 2);
		} else if (GetOpenedMenuClass(fenetre) == MENU_MARCHE) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Math:
		if ((GetOpenedMenuClass(fenetre) != MENU_CONTACTS) && (GetOpenedMenuClass(fenetre) != MENU_QUITTER)) {
			OpenMenu(fenetre, camera, MENU_CONTACTS, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 1);
		} else if (GetOpenedMenuClass(fenetre) == MENU_CONTACTS) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Square:
		if ((GetOpenedMenuClass(fenetre) != MENU_RECHERCHE) && (GetOpenedMenuClass(fenetre) != MENU_QUITTER)) {
			OpenMenu(fenetre, camera, MENU_RECHERCHE, MENU_SYSTEME_AUCUN);
		} else if (GetOpenedMenuClass(fenetre) == MENU_RECHERCHE) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_Log:
		if ((GetOpenedMenuClass(fenetre) != MENU_FLOTTE) && (GetOpenedMenuClass(fenetre) != MENU_QUITTER)) {
			OpenMenu(fenetre, camera, MENU_FLOTTE, MENU_SYSTEME_AUCUN);
			SetWindowSelection(fenetre, 0);
		} else if (GetOpenedMenuClass(fenetre) == MENU_FLOTTE) {
			CloseMenu(fenetre, camera);
		}
		break;
	case sk_2nd:
		if(!GetCommandPromptStatus(fenetre))
			OpenCommandPrompt(fenetre, camera, date);
		else
			CloseCommandPrompt(fenetre, camera, date);
		*key = 0;
		break;
	}
}



static void UpdateEmpirePower(EmpireListe *empireListe) {
	Empire *empire;
	int empireNumero = 0;
		empire = EmpireNumero(empireListe, empireNumero);
	while (empire != NULL) {
		CalculateEmpirePower(empire);
		empireNumero++;
		empire = EmpireNumero(empireListe, empireNumero);
	}
}

/* entry points ======================================================== */
void UpdatePlayersData(char appliquer, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, NotificationList *notificationList){
	Empire *empire = NULL;
	Flotte *flotte = NULL;
	FlotteListe *flotteListe = NULL;
	Planete *planete = NULL;
	int empireIndex = 0;
	int empireArraySize = 0;
	int flotteIndex = 0;
	int flotteArraySize = 0;
	int systemIndex = 0;
	int planetaryIndex = 0;
	int planetaryArraySize = 0;
	int planetaryBuildingIndex = 0;
	
	//retirer argent flottes
	empireArraySize = EmpireArraySize(empireListe);
	for(empireIndex = 0; empireIndex < empireArraySize; empireIndex++){
		empire = EmpireNumero(empireListe, empireIndex);

		//reinitialise tout les changements
		SetEmpireCreditChange(empire, 0);
		SetEmpireFoodChange(empire, 0);
		SetEmpireAlloysChange(empire, 0);
		SetEmpireMineralsChange(empire, 0);
		SetEmpireConsumerGoodsChange(empire, 0);

		flotteListe = EmpireFleetGetArray(empire);
		flotteArraySize = FleetArraySize(flotteListe);
		for(flotteIndex = 0; flotteIndex < flotteArraySize; flotteIndex++){
			flotte = FlotteNumero(flotteListe, flotteIndex);
			AddEmpireCreditChange(empire, -1);
		}
	}

	//retirer et ajouter argent systemes et planetes
	for(systemIndex = 0; systemIndex < LARGEUR_GALAXIE * LARGEUR_GALAXIE; systemIndex++){
		if(GetSystemEmpire(systemeStellaires[systemIndex]) != -1){
			empire = EmpireNumero(empireListe, GetSystemEmpire(systemeStellaires[systemIndex]));
			planetaryArraySize = GetSystemPlanetNumber(systemeStellaires[systemIndex]);

			for(planetaryIndex = 0; planetaryIndex < planetaryArraySize; planetaryIndex++){
				planete = GetPlanet(systemeStellaires[systemIndex], planetaryIndex);
				
				//si la planete est habitée
				if(GetPlanetCityStatus(planete)){

					AddEmpireCreditChange(empire, -GetPlanetCityAgricultureDistrictNumber(planete));
					AddEmpireFoodChange(empire, GetPlanetCityAgricultureDistrictNumber(planete) * 3);

					AddEmpireCreditChange(empire, -GetPlanetCityMiningDistrictNumber(planete));
					AddEmpireMineralsChange(empire, 6 * GetPlanetCityMiningDistrictNumber(planete));
					
					AddEmpireCreditChange(empire, -2 * GetPlanetCityUrbanDistrictNumber(planete));
					
					AddEmpireCreditChange(empire, 8 * GetPlanetCityGeneratorDistrictNumber(planete));
					
					for(planetaryBuildingIndex = 1; planetaryBuildingIndex <= 6; planetaryBuildingIndex++){
						switch(GetPlanetCityBuildingNumber(planete, planetaryBuildingIndex)){
							case CAPITALE:
								AddEmpireCreditChange(empire, -2);
								break;
							case FONDERIE:
								AddEmpireCreditChange(empire, -2);
								AddEmpireMineralsChange(empire, -6);
								AddEmpireAlloysChange(empire, 6);
								break;
							case LABORATOIRE:
								AddEmpireCreditChange(empire, -2);
								break;
							case USINE_CIVILE:
								AddEmpireCreditChange(empire, -2);
								AddEmpireMineralsChange(empire, -6);
								AddEmpireAlloysChange(empire, 6);
								break;
							case THEATRE:
								AddEmpireCreditChange(empire, -2);
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}
	if(appliquer){
		for(empireIndex = 0; empireIndex < empireArraySize; empireIndex++){
			empire = EmpireNumero(empireListe, empireIndex);

			if(GetEmpireAlloys(empire) + GetEmpireAlloysChange(empire) <= 0){
				SetEmpireAlloys(empire, 0);
			}
			else
				AddEmpireAlloys(empire, GetEmpireAlloysChange(empire));
				
			if(GetEmpireCredit(empire) + GetEmpireCreditChange(empire) <= 0){
				SetEmpireCredit(empire, 0);
			}
			else
				AddEmpireCredit(empire, GetEmpireCreditChange(empire));
			
			if(GetEmpireFood(empire) + GetEmpireFoodChange(empire) <= 0){
				SetEmpireFood(empire, 0);
			}
			else
				AddEmpireFood(empire, GetEmpireFoodChange(empire));
			
			if(GetEmpireMinerals(empire) + GetEmpireMineralsChange(empire) <= 0){
				SetEmpireMinerals(empire, 0);
			}
			else
				AddEmpireMinerals(empire, GetEmpireMineralsChange(empire));
		}

		//teste si il n'y a plus beaucoup de ressources
		empire = EmpireNumero(empireListe, 0);

		if((GetEmpireAlloys(empire) + (GetEmpireAlloysChange(empire) / 12)) <= 0) {
			NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
		}
		if((GetEmpireCredit(empire) + (GetEmpireCreditChange(empire) / 12)) <= 0) {
			NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
		}
		if((GetEmpireFood(empire) + (GetEmpireFoodChange(empire) / 12)) <= 0) {
			NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
		}
		if((GetEmpireMinerals(empire) + (GetEmpireMineralsChange(empire) * 12)) <= 0) {
			NewNotification(notificationList, MED_PRIORITY, LOW_RESSOURCES, 31);
		}
	}
}

int UpdateGame(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre, NotificationList *notificationList, Parametres *parametres){
    if(!GetCommandPromptStatus(fenetre))
		TestKey(key, empireListe, systemeStellaires, date, camera, fenetre, parametres);

    UpdateTime(date, key, fenetre);
    if(GetTimeClock(date) == 0){
		if(((GetTimeDay(date) == 10) || (GetTimeDay(date) == 20)) || (GetTimeDay(date) == 30)){
			UpdateWorld(empireListe, systemeStellaires);
			UpdateEmpirePower(empireListe);
		}
		if(GetTimeDay(date) == 1){
			UpdatePlayersData(true, empireListe, systemeStellaires, notificationList);
		}
		
		EmpireAI(empireListe, systemeStellaires, date);
		
	}
	return 1;
}
