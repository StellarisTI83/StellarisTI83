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
	int index = 0;
	int numeroSysteme = 0;

	while(numeroSysteme < LARGEUR_GALAXIE * LARGEUR_GALAXIE) {
		if(GetSystemIntelLevel(systemeStellaires[numeroSysteme])  >= MOYEN) {
			SetSystemIntelLevel(systemeStellaires[numeroSysteme], MOYEN);
		}
		if((GetSystemStationLevel(systemeStellaires[numeroSysteme]) > AUCUN) && (GetSystemEmpire(systemeStellaires[numeroSysteme]) == 1)) {
			SetSystemIntelLevel(systemeStellaires[numeroSysteme], TOTAL);
		}
		numeroSysteme++;
	}
	while(index < FleetArraySize(GetFleetArray(EmpireNumero(empireListe, 1)))){
        SetSystemIntelLevel(systemeStellaires[GetFleetSystem(GetFleetArray(EmpireNumero(empireListe, 1)), index)], TOTAL);
		index++;
	}
}

/**
 * Effectue les actions de station
 */
void EffectuerActionsStations(SystemeStellaire **systemeStellaires, EmpireListe* empireListe){
	int numero = 0, nombreDeVaisseaux = 0;
	int nombreDeCorvette = 0, nombreDeDestroyer = 0, nombreDeCroiseur = 0, nombreDeCuirasse = 0;
	OrdreStation ordre = 0;
    int info1 = 0;
    int info2 = 0;
	Empire *joueur = EmpireNumero(empireListe, 1);
	while(numero < (LARGEUR_GALAXIE * LARGEUR_GALAXIE) - 1){
		ordre = GetSystemStationOrder(systemeStellaires[numero]);
		if(ordre != AUCUN){
			if(GetSystemStationOrderProgress(systemeStellaires[numero]) > 1){
				UnincrementSystemStationOrderProgress(systemeStellaires[numero]);
			}
			else{
				info1 = GetSystemStationInfo1(systemeStellaires[numero]);
				info2 = GetSystemStationInfo2(systemeStellaires[numero]);
				switch(ordre){
				case AMELIORER_STATION:
					SetSystemStationLevel(systemeStellaires[numero], GetSystemStationLevel(systemeStellaires[numero]) + 1);
					break;
				case CONSTRUIRE_MODULE:
					SetSystemStationModule(systemeStellaires[numero], info1 - 1, info2);
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
					NouvelleFlotte(GetFleetArray(joueur), numero, info1, nombreDeCorvette, nombreDeDestroyer, nombreDeCroiseur, nombreDeCuirasse);
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
void EffectuerActionsPlanetes(SystemeStellaire **systemeStellaires, Empire *joueur){
	int i = 0, j = 0;
	Villes *villes = NULL;
	Batiment *batiment = NULL;
	OrdreConstruction ordre;
	while(i < LARGEUR_GALAXIE * LARGEUR_GALAXIE){
		j = 0;
		while(j < 5){
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
								SetSystemPlanetCityBuilding(systemeStellaires[i], j, GetCityOrderInfo1(villes), GetCityOrderInfo2(villes), 1 );
								break;
						}
						EndCityOrder(villes);
					}
				}
			}
			j++;
		}
		i++;
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
}

static void UpdateWorld(EmpireListe *empireListe, SystemeStellaire *systemeStellaires){
    EffectuerActionsFlottes(empireListe, systemeStellaires);
    EffectuerActionsStations(systemeStellaires, EmpireNumero(empireListe, 1));
    EffectuerActionsPlanetes(systemeStellaires, EmpireNumero(empireListe, 1));
    CalculerNiveauDeConnaissance(systemeStellaires, empireListe);
}

static void TestKey(char *key, EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre){

}

/* entry points ======================================================== */
int UpdateGame(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre){
    TestKey(key, empireListe, systemeStellaires, date, camera, fenetre);

    UpdateTime(date, &key, fenetre);
    
	if((((GetTimeDay(date) == 30) || (GetTimeDay(date) == 10)) || (GetTimeDay(date) == 20)) && (GetTimeClock(date) == 0)){
        // UpdatePlayersData();

        UpdateWorld(empireListe, systemeStellaires);

    }
}
