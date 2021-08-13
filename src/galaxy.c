#include <debug.h>
#include <fileioc.h>
#include <math.h>

#include <graphx.h>

#include "main.h"

#include "galaxy.h"
#include "flottes.h"
#include "parametres.h"
#include "systemes.h"

#include "locale/locale.h"

/* private variables =================================================== */

char * nomGalaxies[] = {
	"Yekt",
	"Veax",
	"Uthau",
	"Hep",
	"Xons",
	"Zigdioks",
	"Pluekreft",
	"Qauduks",
	"Sriucros",
	"Wuelsurs",
	"Ni",
	"Gree",
	"Etrooc",
	"Ro",
	"Ozraay",
	"Ekruacsus",
	"Ephaezde",
	"Yuazlen",
	"Zrusre",
	"Sroiruts",
	"Toik",
	"Efe",
	"Qiacs",
	"Oreak",
	"Xo",
	"Vlotaes",
	"Vreuzrab",
	"Praothal",
	"Iguezla",
	"Aflaovikt",
	"Kuad",
	"Zut",
	"Shias",
	"Traakt",
	"Ref",
	"Thumdeu",
	"Ebreocod",
	"Taaki",
	"Inertar",
	"Choaco",
	"Acu",
	"Vruth",
	"Afleen",
	"Su",
	"Ivriab",
	"Afablut",
	"Sleoclu",
	"Qeorit",
	"Vleastruc",
	"Struntar",
	"Clax",
	"Phab",
	"Zeop",
	"Imoig",
	"Vraash",
	"Qentael",
	"Hepheob",
	"Baokrap",
	"Uphiartal",
	"Sropsau",
	"Ulers",
	"Israef",
	"Vruey",
	"Fli",
	"Thio",
	"Zliokluh",
	"Fofeir",
	"Ufeepsuns",
	"Eslauprek",
	"Slebli",
	"Athent",
	"Iveaph",
	"Azleem",
	"Puets",
	"Avors",
	"Troaholt",
	"Strobrims",
	"Pluebsups",
	"Xucdiult",
	"Urimdin",
	"Thi",
	"Eclair",
	"Flio",
	"Tiud",
	"Poals",
	"Vleuzlars",
	"Gramaob",
	"Driwex",
	"Utrinsaa",
	"Flezlauft",
	"Xoab",
	"Chur",
	"Ijoor",
	"Oplaap",
	"Uzluak",
	"Esreeclu",
	"Ukraammiy",
	"Loissekt",
	"Klaezzash",
	"Strivo",
	"Zuel",
	"Lo",
	"Ekruem",
	"Osi",
	"Teo",
	"Yoizolt",
	"Cheddeel",
	"Iyiubsud",
	"Braddaub",
	"Koiploc"
};


char * nomPlanetes[] = {
	"Egnepra",
	"Aceonerth",
	"Tochade",
	"Pemapus",
	"Ruliv",
	"Chuturn",
	"Gidetera",
	"Gneputani",
	"Nora",
	"Gao",
	"Vunubos",
	"Zolveiril",
	"Ostriea",
	"Hibbillon",
	"Cheruta",
	"Puthea",
	"Cuinia",
	"Gachuruta",
	"Gone",
	"Thade",
	"Bevotov",
	"Chavivis",
	"Yuvore",
	"Biseron",
	"Zauter",
	"Sony",
	"Michethea",
	"Gravutis",
	"Lille",
	"Strade",
	"Kidririli",
	"Bonvanus",
	"Icharvis",
	"Zilminda",
	"Yutune",
	"Thaelia",
	"Luchazuno",
	"Truutis",
	"Lleshan",
	"Cyke",
	"Themeanov",
	"Miccoiria",
	"Gichurn",
	"Nibronoe",
	"Nieliv",
	"Roirus",
	"Bamipra",
	"Cregocaro",
	"Brypso",
	"Deron",
	"Kindounus",
	"Danniula",
	"Ondosie",
	"Yangomia",
	"Hephus",
	"Haiturn",
	"Crumuturn",
	"Loxistea",
	"Brade",
	"Tryria",
	"Yovoanide",
	"Ellaclite",
	"Naccone",
	"Relnoth",
	"Beuliv",
	"Thauyama",
	"Chayolara",
	"Phugemia",
	"Larth",
	"Griri"
};

/* private functions =================================================== */

/**
 * Create matrix
 */
static void CreateGalacticMatrix(int *galaxie, int espaceEntreEtoiles, int *barreDeChargement){
	int hauteur = 0, largeur = 0;
	int x = LIMITE_GAUCHE;
	int y = LIMITE_HAUT;
	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Create galaxy matrix\n");
	#endif
	while(hauteur < LARGEUR_GALAXIE) {
		while(largeur < LARGEUR_GALAXIE*2) {
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur] = x;
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1] = y;
			x += espaceEntreEtoiles;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		x = LIMITE_GAUCHE;
		y += espaceEntreEtoiles;
		hauteur++;
	}
}

/**
 * Randomise matrix position
 */
static void RandomGalacticMatrix(int *galaxie, int coefficientDeplacementStellaire, int *barreDeChargement){
	int hauteur = 0, largeur = 0;
	int coefficientX = 0, coefficientY = 0;
	int x = 0, y = 0;
	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Randomize galaxy matrix\n");
	#endif
	while(hauteur < LARGEUR_GALAXIE) {
		while(largeur < LARGEUR_GALAXIE*2) {
			x = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur];
			y = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1];
			coefficientX = randInt (-coefficientDeplacementStellaire, coefficientDeplacementStellaire);
			coefficientY = randInt (-coefficientDeplacementStellaire, coefficientDeplacementStellaire);
			x = x + (coefficientX / 5);
			y = y + (coefficientY / 5);
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur] = x;
			galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1] = y;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		hauteur++;
	}
}

static void RoundGalacticMatrix(int *galaxie, int espaceEntreEtoiles, int rayonExterieur, int rayonInterieur, int *barreDeChargement){
	int hauteur = 0, largeur = 0;
	int x = 0, y = 0;
	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Round galaxy matrix\n");
	#endif
	while(hauteur < LARGEUR_GALAXIE)
	{
		while(largeur < LARGEUR_GALAXIE * 2)
		{
			x = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur];
			y = galaxie[hauteur * LARGEUR_GALAXIE*2 + largeur + 1];
			if(((pow(x - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2) + pow(y - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2)) > (pow(rayonExterieur,2))) || ((pow(x - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2) + pow(y - (((espaceEntreEtoiles * LARGEUR_GALAXIE) / 2) + (espaceEntreEtoiles * 2)) ,2)) < (pow(rayonInterieur = 50,2))))
			{
				x = 0;
				y = 0;
			}
			galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur] = x;
			galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur + 1] = y;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		hauteur++;
	}
}

static void CreatePlanetSystem(SystemeStellaire *systemeStellaire, int numeroPlanete, int habitable){
	int rayonOrbite = 0, taille = 0, type = 0;
    int x = 0;
    int y = 0;

	type = randInt(1, 100);
	switch(numeroPlanete + 1){
	case 5:
		if(type <= 60) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 90){
			type = COLD_ARID; //monde aride froid
			habitable = 0;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}
		break;
	case 4:
		if(type <= 30) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 70){
			type = COLD_ARID; //monde aride froid
			habitable = 0;
		} else if (type <= 75){
			type = HABITABLE_ALPINE; //monde Alpine habitable
			habitable = 1;
		} else if (type <= 80){
			type = HABITABLE_ARCTIC; //monde Arctic froid
			habitable = 1;
		} else if (type <= 85){
			type = HABITABLE_TUNDRA; //monde Tundra habitable
			habitable = 1;
		} else if (type <= 90){
			type = HABITABLE_CONTINENTAL; //monde continental froid
			habitable = 1;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}	
		break;
	case 3:
		if(type <= 5) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 20){
			type = TOXIC; //monde toxique
			habitable = 0;
		} else if (type <= 60){
			type = COLD_ARID; //monde aride froid
			habitable = 0;
		} else if (type <= 70){
			type = HABITABLE_ALPINE; //monde Alpine habitable
			habitable = 1;
		} else if (type <= 74){
			type = HABITABLE_ARCTIC; //monde Arctic froid
			habitable = 1;
		} else if (type <= 78){
			type = HABITABLE_TUNDRA; //monde Tundra habitable
			habitable = 1;
		} else if (type <= 82){
			type = HABITABLE_CONTINENTAL; //monde continental froid
			habitable = 1;
		} else if (type <= 86){
			type = HABITABLE_OCEAN; //monde ocean
			habitable = 1;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}
		break;
	case 2:
		if(type <= 2) {
			type = GAS_GIANT; //geante gazeuse
			habitable = 0;
		} else if (type <= 30){
			type = TOXIC; //monde toxique
			habitable = 0;
		} else if (type <= 80){
			type = ARID; //monde arid 
			habitable = 0;
		} else if (type <= 83){
			type = HABITABLE_CONTINENTAL; //monde continental habitable
			habitable = 1;
		} else if (type <= 86){
			type = HABITABLE_OCEAN; //monde ocean habitable
			habitable = 1;
		} else if (type <= 89){
			type = HABITABLE_TROPICAL; //monde tropical habitable
			habitable = 1;
		} else {
			type = BROKE; //monde brisé
			habitable = 0;
		}
		break;
	case 1:
		if(type <= 3) {
			type = HABITABLE_ARID; //monde arid
			habitable = 1;
		} else if (type <= 6){
			type = HABITABLE_DESERT; //monde Desert
			habitable = 1;
		} else if (type <= 9){
			type = HABITABLE_SAVANNA; //monde Savanna
			habitable = 1;
		} else if (type <= 40){
			type = ARID; //monde aride
			habitable = 0;
		} else if (type <= 50){
			type = BROKE; //monde brisé
			habitable = 0;
		} else{
			type = MOLTEN; //monde en fusion
			habitable = 0;
		}
		break;
	}

	taille = randInt(1, 100);
	switch(numeroPlanete + 1){
	case 1:
		rayonOrbite = randInt(RAYON_PLANETE1, RAYON_PLANETE1 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 2:
		rayonOrbite = randInt(RAYON_PLANETE2, RAYON_PLANETE2 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 3:
		rayonOrbite = randInt(RAYON_PLANETE3, RAYON_PLANETE3 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 4:
		rayonOrbite = randInt(RAYON_PLANETE4, RAYON_PLANETE4 + 10);
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	case 5:
		rayonOrbite = randInt(RAYON_PLANETE5, RAYON_PLANETE5 + 10);
		if(type == 14) {
			taille = 5;
		}
		if(taille <= 10) {
			taille = 2;
		} else if(taille <= 70) {
			taille = 3;
		} else if(taille <= 90) {
			taille = 4;
		} else {
			taille = 5;
		}
		break;
	}

    SetSystemPlanetHabitability(systemeStellaire, numeroPlanete, habitable);
    SetSystemPlanetOrbitRadius(systemeStellaire, numeroPlanete, rayonOrbite);

    x = randInt(X_CENTRE_SYSTEME - rayonOrbite, X_CENTRE_SYSTEME + rayonOrbite); //aleatoire de x
    y = sqrt(pow((double)rayonOrbite, 2.0) - pow((double)(x - X_CENTRE_SYSTEME), 2.0)) + Y_CENTRE_SYSTEME; //calcule de y pour ce x
    if(randInt(0, 1) == 1) {
    y = Y_CENTRE_SYSTEME - (y - Y_CENTRE_SYSTEME);
}
    SetSystemPlanetXY(systemeStellaire, numeroPlanete, x, y);

    SetSystemPlanetType(systemeStellaire, numeroPlanete, type);
    SetSystemPlanetRadius(systemeStellaire, numeroPlanete, taille);

	if(habitable == 1) {
        SetSystemPlanetName(systemeStellaire, numeroPlanete, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
	}
}

/**
 * generate the struct of the system
 */
static int GenerateSystemeStruct(int *galaxie, SystemeStellaire **systemeStellaires, int *barreDeChargement){//enregistrer matrice et generer hyperlane
	int largeur = 0;
	int hauteur = 0;
	int x = 0;
	int y = 0;
    int k = 0;

    int nombrePlanetes = 0;
    int nombrePlanetesHabitables = 0;
    int planeteHabitable[5] = {0};
    int nombreHyperlanes = 0;
    int nomInt = 0;

    int planetIndex = 0;

    int hyperLane1 = 0;
    int hyperLane2 = 0;
    int hyperLane3 = 0;
    int hyperLaneSup1 = 0;
    int hyperLaneSup2 = 0;

    EtoileType etoile = 0;
    int trouNoir = 0;

	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Generate system structure\n");
	#endif

	while(hauteur < LARGEUR_GALAXIE) {
		while(largeur < LARGEUR_GALAXIE * 2) {
			systemeStellaires[k] = AllocSystem();

			x = galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur];
			y = galaxie[hauteur * LARGEUR_GALAXIE * 2 + largeur + 1];

            SetSystemXY(systemeStellaires[k], x, y);
            
            CreateSystemStation(systemeStellaires[k]);
            
			etoile = randInt(1, 100);
			trouNoir = 0;

            SetSystemIntelLevel(systemeStellaires[k], INCONNU);
			if(etoile <= 10) {
				etoile = ETOILE_TYPE_B;
			} else if(etoile <= 30) {
				etoile = ETOILE_TYPE_A;
			} else if(etoile <= 44) {
				etoile = ETOILE_TYPE_F;
			} else if(etoile <= 57) {
				etoile = ETOILE_TYPE_G;
			} else if(etoile <= 70) {
				etoile = ETOILE_TYPE_K;
			} else if(etoile <= 85) {
				etoile = ETOILE_TYPE_M;
			} else if(etoile <= 90) {
				etoile = ETOILE_TYPE_TROU_NOIR;
				trouNoir = TRUE;
			} else if(etoile <= 95) {
				etoile = ETOILE_TYPE_PULSAR;
			} else {
				etoile = ETOILE_TYPE_ETOILE_A_NEUTRONS;
			}
            SetSystemStationLevel(systemeStellaires[k], AUCUNE_STATION);
			nombrePlanetes = 0;
			nombrePlanetes = randInt(1, 100);
			if(nombrePlanetes <= 8) {
				nombrePlanetes = 1;
			} else if(nombrePlanetes <= 20) {
				nombrePlanetes = 2;
			} else if(nombrePlanetes <= 50) {
				nombrePlanetes = 3;
			} else if(nombrePlanetes <= 80) {
				nombrePlanetes = 4;
			} else {
				nombrePlanetes = 5;
			}
			if(trouNoir == 1) {
				nombrePlanetes = 0;
			}
			SetSystemStarType(systemeStellaires[k], etoile);
            
            SetSystemPlanetNumber(systemeStellaires[k], 0);
			
            if((GetSystemStarType(systemeStellaires[k]) == ETOILE_TYPE_M) ||
            ((GetSystemStarType(systemeStellaires[k]) == ETOILE_TYPE_G) || 
            (GetSystemStarType(systemeStellaires[k]) == ETOILE_TYPE_K))){
				nombrePlanetesHabitables = randInt(0, nombrePlanetes);
                SetSystemPlanetHabitableNumber(systemeStellaires[k], nombrePlanetesHabitables);
			}
			nombreHyperlanes = randInt(1, 15);
			
			hyperLane1 = 1;
			hyperLane2 = 1;
			hyperLane3 = 1;
			
			if(nombreHyperlanes == 1)
			{
				do
				{
					hyperLaneSup1 = randInt(1, 2);
					hyperLaneSup2 = randInt(1, 2);
				} while(hyperLaneSup1 == hyperLaneSup2);
				switch(hyperLaneSup1)
					{
					case 1:
						hyperLane1 = 0;
						break;
					case 2:
						hyperLane2 = 0;
						break;
					/*case 3:
						hyperLane3 = 0;
						break;
				}
				switch(hyperlaneSup2)
				{
					case 1:
						hyperLane1 = 0;
						break;
					case 2:
						hyperLane2 = 0;
						break;
					case 3:
						hyperLane3 = 0;
						break;*/
				}
			}
			else if(nombreHyperlanes <= 5)
			{
				hyperLaneSup1 = randInt(1, 3);
				switch(hyperLaneSup1)
				{
					case 1:
						hyperLane1 = 0;
						break;
					case 2:
						hyperLane2 = 0;
						break;
					/*case 3:
						hyperLane3 = 0;
						break;*/
				}
			}	
			else
			{
				hyperLane1 = 1;
				hyperLane2 = 1;
				//hyperLane3 = 1;
			}
			
            SetHyperlaneDestination(systemeStellaires[k], 0, 255);
            SetHyperlaneDestination(systemeStellaires[k], 1, 255);
            SetHyperlaneDestination(systemeStellaires[k], 2, 255);
            SetHyperlaneDestination(systemeStellaires[k], 3, 255);
            
			if((GetSystemX(systemeStellaires[k - LARGEUR_GALAXIE]) != 0) && (hyperLane1)) {
				if((k - LARGEUR_GALAXIE > 0) && (k - LARGEUR_GALAXIE < 255))
            		SetHyperlaneDestination(systemeStellaires[k], 0, k - LARGEUR_GALAXIE);
			}
			
			if((GetSystemX(systemeStellaires[k - 1]) != 0) && (hyperLane2)) {
				if((k - 1 > 0) && (k - 1 < 255))
                	SetHyperlaneDestination(systemeStellaires[k], 1, k - 1);
			}
			
			nomInt = randInt(0, (sizeof(nomGalaxies)/sizeof(nomGalaxies[0])) - 1);
            SetSystemName(systemeStellaires[k], nomGalaxies[nomInt]);
			
			//gestion des planetes
			switch(nombrePlanetes){
			case 5:
				planeteHabitable[4] = randInt(1, 10);
			case 4:
				planeteHabitable[3] = randInt(1, 10);
			case 3:
				planeteHabitable[2] = randInt(1, 10);
			case 2:
				planeteHabitable[1] = randInt(1, 10);
			case 1:
				planeteHabitable[0] = randInt(1, 10);
			}

			planetIndex = 0;
			while(planetIndex < nombrePlanetes){
                CreateSystemPlanet(systemeStellaires[k], planetIndex);
				planetIndex++;
			}	

			planetIndex = 0;	
			while(planetIndex < nombrePlanetes){
				CreatePlanetSystem(systemeStellaires[k], planetIndex, randInt(1, 10));
				planetIndex++;
			}
			SetSystemPlanetNumber(systemeStellaires[k], nombrePlanetes);

			k++;
			largeur += 2;
		}
		gfx_FillRectangle_NoClip(50, 160, *barreDeChargement += 3, 5);
		largeur = 0;
		hauteur++;
	}
	return k;
}

/**
 * Function to recreate the hyperlanes
 */
static void RecreateHyperlanes(SystemeStellaire **systemeStellaires, int k){
	int i = 0;
	int j = 0;
	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Recreate hyperlanes\n");
	#endif
	//recreation des hyperlanes
	while(i < k) {
		SystemeStellaire *systeme;
		systeme = systemeStellaires[i];
		if(GetHyperlaneDestination(systeme, 0) != 255) {
			SetHyperlaneDestination(systemeStellaires[GetHyperlaneDestination(systeme, 0)], 2, i);
		}
		if(GetHyperlaneDestination(systeme, 1) != 255) {
			SetHyperlaneDestination(systemeStellaires[GetHyperlaneDestination(systeme, 1)], 3, i);
		}
		i++;
	}

	i = 0;
	j = 0;
	while(i < k){
		SystemeStellaire *systeme;
        int x = 0, y = 0;
		systeme = systemeStellaires[i];
		//calcul des positions de sortie
		j = 0;
		while(j < 4){
			if(GetHyperlaneDestination(systeme, j) != 255){
				if((GetSystemX(systemeStellaires[GetHyperlaneDestination(systeme, j)]) != 0) && (GetSystemY(systemeStellaires[GetHyperlaneDestination(systeme, j)]) != 0)){
					double angle = 0;

					angle = atan2(GetSystemY(systemeStellaires[GetHyperlaneDestination(systeme, j)]) - GetSystemY(systeme), GetSystemX(systemeStellaires[GetHyperlaneDestination(systeme, j)]) - GetSystemX(systeme));
					
					x = X_CENTRE_SYSTEME + ((RAYON_DE_VUE_SYSTEME + 5) * cos(angle));

					y = Y_CENTRE_SYSTEME + ((RAYON_DE_VUE_SYSTEME + 5) * sin(angle));
					SetHyperlaneXY(systeme, j, x, y);
				}
			}
			j++;
		}
		i++;
	}
}

/* entry points ======================================================== */

/**
 * Creer Empires
 */
void CreerEmpires(Parametres *parametres, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Camera *camera){
	int i = 0, fin = 1, j = 0;
	int k = LARGEUR_GALAXIE * LARGEUR_GALAXIE;
	int planete = 0;
	Empire *joueur = EmpireNumero(empireListe, 1);
	//creation joueur
	while(fin == 1) { // choix du systeme
		i = randInt(0, k - 1);
		gfx_SetTextXY(50, 70);
		if(((GetSystemX(systemeStellaires[i]) >= 160) && (GetSystemY(systemeStellaires[i]) >= 120)) && (GetSystemStarType(systemeStellaires[i]) != ETOILE_TYPE_TROU_NOIR))
			fin = 0;
	}
	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "\nCreate empires\n");
	#endif
	SetEmpireColor(joueur, 9);
	SetEmpireSystemCapital(joueur, i);

	SetSystemStarType(systemeStellaires[i], ETOILE_TYPE_K);
	SetSystemPlanetHabitableNumber(systemeStellaires[i], 1);
	SetSystemPlanetInhabitedNumber(systemeStellaires[i], 1);
	SetSystemEmpire(systemeStellaires[i], 1);
	SetSystemIntelLevel(systemeStellaires[i], TOTAL);

	SetEmpireCredit(joueur, 100);
	SetEmpireMinerals(joueur, 100);
	SetEmpireFood(joueur, 200);
	SetEmpireAlloys(joueur, 100);
	SetEmpireConsumerGoods(joueur, 100);

	SetSystemStationLevel(systemeStellaires[i], PORT_STELLAIRE);
	SetSystemStationModule(systemeStellaires[i], 0, CHANTIER_SPATIAL);
	SetSystemStationModule(systemeStellaires[i], 1, CARREFOUR_COMMERCIAL);
	

	EmpireFlotteNouvelle(joueur, i, FLOTTE_MILITAIRE, 3, 0, 0, 0);

	EmpireFlotteNouvelle(joueur, i, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);

	EmpireFlotteNouvelle(joueur, i, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);
	
	SetCameraX(camera, GetSystemX(systemeStellaires[i])*2); // centre la vue sur le systeme
	SetCameraY(camera, GetSystemY(systemeStellaires[i])*2);
	SetCameraSystem(camera, i);

	planete = randInt(0, GetSystemPlanetNumber(systemeStellaires[i]) - 1);

	SetSystemPlanetHabitability(systemeStellaires[i], planete, true);
	SetSystemPlanetType(systemeStellaires[i], planete, HABITABLE_CONTINENTAL);

	SetCameraXSystem(camera, GetSystemPlanetX(systemeStellaires[i], planete) - 160);
	SetCameraYSystem(camera, GetSystemPlanetY(systemeStellaires[i], planete) - 120);
	SetSystemPlanetName(systemeStellaires[i], planete, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);

	CreateSystemPlanetCity(systemeStellaires[i], planete);
	SetSystemPlanetCityPopulation(systemeStellaires[i], planete, 27);
	SetSystemPlanetCityDistrict(systemeStellaires[i], planete, 4, 3, 3, 3);

	CalculateSystemPlanetCityJob(systemeStellaires[i], planete);

	SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 1, CAPITALE, 3);
	SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 2, USINE_CIVILE, 1);
	SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 3, FONDERIE, 1);

	CalculateEmpirePower(joueur);

	#ifdef DEBUG_VERSION
		dbg_sprintf(dbgout, "Empire: %d (%p)\n -System: %d (%d, %d)\n -Color: %d\n -Planet: %d\n -Fleet: %p\nCamera: %d %d\n", 1, joueur, i, GetSystemX(systemeStellaires[i]), GetSystemY(systemeStellaires[i]), GetEmpireColor(joueur), planete, EmpireFleetGetArray(joueur), GetCameraX(camera), GetCameraY(camera));
	#endif
	
	for(j = 2; j <= GetEmpireNumber(parametres); j++){
		Empire *empire = NULL;
		fin = 1;
		i = 0;
		while(fin == 1) { // choix du systeme
			i = randInt(0, k - 1);
			if(((GetSystemX(systemeStellaires[i]) >= 160) && (GetSystemY(systemeStellaires[i]) >= 120)) && ((GetSystemStarType(systemeStellaires[i]) != ETOILE_TYPE_TROU_NOIR) && (GetSystemEmpire(systemeStellaires[i]) == 0)))
				fin = 0;
		}
		empire = EmpireAjouter(empireListe);
		EmpireFlotteCreer(empire);
		EmpireGenerateRandomName(empire);
		SetEmpireSpecies(empire, randInt(0, 3));
		SetEmpireClothes(empire, randInt(0, 2));

		SetEmpireColor(empire, randInt(20, 29));
		SetEmpireSystemCapital(empire, i);
		SetSystemStarType(systemeStellaires[i], ETOILE_TYPE_K);
		SetSystemPlanetHabitableNumber(systemeStellaires[i], 1);
		SetSystemPlanetInhabitedNumber(systemeStellaires[i], 1);
		SetSystemEmpire(systemeStellaires[i], j);
		SetSystemIntelLevel(systemeStellaires[i], INCONNU);

		SetEmpireCredit(empire, 100);
		SetEmpireMinerals(empire, 100);
		SetEmpireFood(empire, 200);
		SetEmpireAlloys(empire, 100);
		SetEmpireConsumerGoods(empire, 100);
		
		SetSystemStationLevel(systemeStellaires[i], PORT_STELLAIRE);
		SetSystemStationModule(systemeStellaires[i], 0, CHANTIER_SPATIAL);
		SetSystemStationModule(systemeStellaires[i], 1, CARREFOUR_COMMERCIAL);

		EmpireFlotteNouvelle(empire, i, FLOTTE_MILITAIRE, 3, 0, 0, 0);
		EmpireFlotteNouvelle(empire, i, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);
		EmpireFlotteNouvelle(empire, i, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);

		planete = randInt(0, GetSystemPlanetNumber(systemeStellaires[i])- 1);

		SetSystemPlanetHabitability(systemeStellaires[i], planete, true);
		SetSystemPlanetType(systemeStellaires[i], planete, HABITABLE_CONTINENTAL);
		SetSystemPlanetName(systemeStellaires[i], planete, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);

		CreateSystemPlanetCity(systemeStellaires[i], planete);
		SetSystemPlanetCityPopulation(systemeStellaires[i], planete, 27);
		SetSystemPlanetCityDistrict(systemeStellaires[i], planete, 4, 3, 3, 3);

		CalculateSystemPlanetCityJob(systemeStellaires[i], planete);

		SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 1, CAPITALE, 3);
		SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 2, USINE_CIVILE, 1);
		SetSystemPlanetCityBuilding(systemeStellaires[i], planete, 3, FONDERIE, 1);
		
		CalculateEmpirePower(empire);

		#ifdef DEBUG_VERSION
			dbg_sprintf(dbgout, "Empire: %d (%p)\n -System: %d\n -Color: %d\n -Planet: %d\n", j, empire,i, GetEmpireColor(empire), planete);
		#endif
	}
	RelationAllListeUpdate(empireListe);
}


/**
 * Function to create the galaxy
 */
int ChargementNouvellePartieGalaxie(SystemeStellaire **systemeStellaires){
	int espaceEntreEtoiles = 50, barreDeChargement = 1, fin = 0;
	int coefficientDeplacementStellaire = 100, rayon = ((espaceEntreEtoiles * LARGEUR_GALAXIE) - espaceEntreEtoiles) / 2 - 25, rayonInterieur = 50;
    int k = LARGEUR_GALAXIE * LARGEUR_GALAXIE;

	int *galaxy = NULL;
	galaxy = malloc(LARGEUR_GALAXIE * LARGEUR_GALAXIE * 2 * sizeof(int));

	if(!galaxy) {    
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating galaxy");
		#endif
		exit(EXIT_FAILURE);
	}

	gfx_SetDrawScreen();
	gfx_FillScreen(1);
	PrintCentered(_(lc_create_galaxie), 120, 1, 0, 0);
	gfx_SetColor(7);
	gfx_Rectangle_NoClip(49, 159, 222, 7);
	gfx_SetColor(4);
	gfx_SetTextXY(50, 50);
	CreateGalacticMatrix(galaxy, espaceEntreEtoiles, &barreDeChargement);

	RandomGalacticMatrix(galaxy, coefficientDeplacementStellaire, &barreDeChargement);
	
	RoundGalacticMatrix(galaxy, espaceEntreEtoiles, rayon, rayonInterieur, &barreDeChargement);

	k = GenerateSystemeStruct(galaxy, systemeStellaires, &barreDeChargement);
	free(galaxy);
	
	RecreateHyperlanes(systemeStellaires, k);

	
	fin = 1;
	return 1;
}
