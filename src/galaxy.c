#include <debug.h>
#include <fileioc.h>
#include <math.h>

#include <graphx.h>

#include "colors.h"

#include "main.h"

#include "galaxy.h"
#include "fleet.h"
#include "settings.h"
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
 * @brief Put every star in a regular grid
 * 
 * @param galaxie 
 * @param loading 
 */
static void galaxy_MatrixCreate(int *galaxyMatrix,
                                int *loading){
    int xIndex = 0, yIndex = 0;
    int x = LIMIT_LEFT;
    int y = LIMIT_TOP;

    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Create galaxy matrix\n");
    #endif

    while(yIndex < GALAXY_WIDTH) {
        while(xIndex < GALAXY_WIDTH * 2) {
            galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex] = x;
            galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex + 1] = y;
            x += SPACE_BETWEEN_STARS;
            xIndex += 2;	// To jump the y position
        }

        gfx_FillRectangle_NoClip(50, 160, *loading += 1, 5);
        
        xIndex = 0;
        x = LIMIT_LEFT;
        y += SPACE_BETWEEN_STARS;
        yIndex++;
    }
}

/**
 * @brief Randomize the stars positions
 * 
 * @param galaxie 
 * @param loading 
 */
static void galaxy_MatrixRandomize(	int *galaxyMatrix,  
                                    int *loading){
    int xIndex = 0, yIndex = 0;
    int xCoefficient = 0, yCoefficient = 0;
    int xPosition = 0, yPosition = 0;

    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Randomize galaxy matrix\n");
    #endif

    while(yIndex < GALAXY_WIDTH) {
        while(xIndex < GALAXY_WIDTH * 2) {
            // We get the x and y positions of the star
            xPosition = galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex];
            yPosition = galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex + 1];

            // We get the x and y displacement coefficient
            xCoefficient = randInt (-RANDOMIZE_COEFFICIENT, RANDOMIZE_COEFFICIENT);
            yCoefficient = randInt (-RANDOMIZE_COEFFICIENT, RANDOMIZE_COEFFICIENT);

            // We add the 2
            xPosition = xPosition + (xCoefficient / 5);
            yPosition = yPosition + (yCoefficient / 5);

            // We put it back in the matrix
            galaxyMatrix[yIndex * GALAXY_WIDTH*2 + xIndex] = xPosition;
            galaxyMatrix[yIndex * GALAXY_WIDTH*2 + xIndex + 1] = yPosition;

            xIndex += 2;
        }
        gfx_FillRectangle_NoClip(50, 160, *loading += 1, 5);
        xIndex = 0;
        yIndex++;
    }
}
/**
 * @brief We crop the galaxy to a round shape
 * 
 * @param galaxyMatrix 
 * @param radiusExtern 
 * @param rayonInterieur 
 * @param loading 
 */
static void galaxy_MatrixCrop(  int *galaxyMatrix, 
                                int radiusExtern, 
                                int radiusIntern, 
                                int *loading){
    int xIndex = 0, yIndex = 0;
    int xPosition = 0, yPosition = 0;

    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Round galaxy matrix\n");
    #endif

    while(yIndex < GALAXY_WIDTH) {
        while(xIndex < GALAXY_WIDTH * 2) {
            xPosition = galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex];
            yPosition = galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex + 1];

            // If the star is outside of the galaxy, we dele the star
            if(((pow(xPosition - (((SPACE_BETWEEN_STARS * GALAXY_WIDTH) / 2) + (SPACE_BETWEEN_STARS * 2)) ,2) + pow(yPosition - (((SPACE_BETWEEN_STARS * GALAXY_WIDTH) / 2) + (SPACE_BETWEEN_STARS * 2)) ,2)) > (pow(radiusExtern,2))) 
            || ((pow(xPosition - (((SPACE_BETWEEN_STARS * GALAXY_WIDTH) / 2) + (SPACE_BETWEEN_STARS * 2)) ,2) + pow(yPosition - (((SPACE_BETWEEN_STARS * GALAXY_WIDTH) / 2) + (SPACE_BETWEEN_STARS * 2)) ,2)) < (pow(radiusIntern = 50,2)))){
                xPosition = 0;
                yPosition = 0;
            }

            galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex] = xPosition;
            galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex + 1] = yPosition;
            xIndex += 2;
        }
        gfx_FillRectangle_NoClip(50, 160, *loading += 1, 5);
        xIndex = 0;
        yIndex++;
    }
}

static void CreatePlanetSystem(StarSystem *systemeStellaire, int numeroPlanete, int habitable){
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


// TODO Refactor the function
/**
 * generate the struct of the system
 */
static int GenerateSystemeStruct(int *galaxie, StarSystem **systemeStellaires, int *barreDeChargement){//enregistrer matrice et generer hyperlane
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

    StarType etoile = 0;
    int trouNoir = 0;

    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Generate system structure\n");
    #endif

    while(hauteur < GALAXY_WIDTH) {
        while(largeur < GALAXY_WIDTH * 2) {
            systemeStellaires[k] = starSystem_Create();

            x = galaxie[hauteur * GALAXY_WIDTH * 2 + largeur];
            y = galaxie[hauteur * GALAXY_WIDTH * 2 + largeur + 1];

            starSystem_SetXY(systemeStellaires[k], x, y);
            
            CreateSystemStation(systemeStellaires[k]);
            SetSystemEmpire(systemeStellaires[k], -1);
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
                trouNoir = true;
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
            
            if((starSystem_GetX(systemeStellaires[k - GALAXY_WIDTH]) != 0) && (hyperLane1)) {
                if((k - GALAXY_WIDTH > 0) && (k - GALAXY_WIDTH < 255))
                    SetHyperlaneDestination(systemeStellaires[k], 0, k - GALAXY_WIDTH);
            }
            
            if((starSystem_GetX(systemeStellaires[k - 1]) != 0) && (hyperLane2)) {
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

// TODO Refactor the function
/**
 * Function to recreate the hyperlanes
 */
static void RecreateHyperlanes(StarSystem **systemeStellaires, int k){
    int i = 0;
    int j = 0;
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Recreate hyperlanes\n");
    #endif
    //recreation des hyperlanes
    while(i < k) {
        StarSystem *systeme;
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
        StarSystem *systeme;
        int x = 0, y = 0;
        systeme = systemeStellaires[i];
        //calcul des positions de sortie
        j = 0;
        while(j < 4){
            if(GetHyperlaneDestination(systeme, j) != 255){
                if((starSystem_GetX(systemeStellaires[GetHyperlaneDestination(systeme, j)]) != 0) && (starSystem_GetY(systemeStellaires[GetHyperlaneDestination(systeme, j)]) != 0)){
                    double angle = 0;

                    angle = atan2(starSystem_GetY(systemeStellaires[GetHyperlaneDestination(systeme, j)]) - starSystem_GetY(systeme), starSystem_GetX(systemeStellaires[GetHyperlaneDestination(systeme, j)]) - starSystem_GetX(systeme));
                    
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

// TODO Refactor the function
/**
 * Creer Empires
 */
void CreerEmpires(Settings *parametres, EmpireListe *empireListe, StarSystem **systemeStellaires, Camera *camera){
    int i = 0, fin = 1, j = 0;
    int k = GALAXY_WIDTH * GALAXY_WIDTH;
    int planete = 0;
    Empire *joueur = EmpireNumero(empireListe, 0);
    //creation joueur
    while(fin == 1) { // choix du systeme
        i = randInt(0, k - 1);
        gfx_SetTextXY(50, 70);
        if(((starSystem_GetX(systemeStellaires[i]) >= 160) && (starSystem_GetY(systemeStellaires[i]) >= 120)) && (GetSystemStarType(systemeStellaires[i]) != ETOILE_TYPE_TROU_NOIR))
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
    SetSystemEmpire(systemeStellaires[i], 0);
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
    
    SetCameraX(camera, starSystem_GetX(systemeStellaires[i])*2); // centre la vue sur le systeme
    SetCameraY(camera, starSystem_GetY(systemeStellaires[i])*2);
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
        dbg_sprintf(dbgout, "Empire: %d (%p)\n -System: %d (%d, %d)\n -Color: %d\n -Planet: %d\n -Fleet: %p\nCamera: %d %d\n", 1, joueur, i, starSystem_GetX(systemeStellaires[i]), starSystem_GetY(systemeStellaires[i]), GetEmpireColor(joueur), planete, EmpireFleetGetArray(joueur), GetCameraX(camera), GetCameraY(camera));
    #endif
    
    for(j = 1; j < settings_EmpireNumberGet(parametres); j++){
        Empire *empire = NULL;
        fin = 1;
        i = 0;
        while(fin == 1) { // choix du systeme
            i = randInt(0, k - 1);
            if(((starSystem_GetX(systemeStellaires[i]) >= 160) && (starSystem_GetY(systemeStellaires[i]) >= 120)) && ((GetSystemStarType(systemeStellaires[i]) != ETOILE_TYPE_TROU_NOIR) && (GetSystemEmpire(systemeStellaires[i]) == -1)))
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

        planete = randInt(0, GetSystemPlanetNumber(systemeStellaires[i]) - 1);

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


void galaxy_CreateNew(StarSystem **starSystem){
    int loading = 1;	// The advancement of the loading bar
    int radiusExtern = ((SPACE_BETWEEN_STARS * GALAXY_WIDTH) - SPACE_BETWEEN_STARS) / 2 - 25; 
    int radiusIntern = 50;
    int *galaxyMatrix = NULL;
    int k;
    galaxyMatrix = malloc(GALAXY_WIDTH * GALAXY_WIDTH * 2 * sizeof(int));
    if(!galaxyMatrix) {
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating galaxy matrix");
		#endif
        exit(EXIT_FAILURE);
    }
    
    gfx_SetColor(COLOR_WHITE);
    gfx_Rectangle_NoClip(49, 159, 222, 7);
    gfx_SetColor(COLOR_BLUE_HOVER);
    gfx_SetDrawScreen();
    
    galaxy_MatrixCreate(galaxyMatrix, &loading);

    galaxy_MatrixRandomize(galaxyMatrix, &loading);
    
    galaxy_MatrixCrop(galaxyMatrix, radiusExtern, radiusIntern, &loading);

    k = GenerateSystemeStruct(galaxyMatrix, starSystem, &loading);

    free(galaxyMatrix);
    RecreateHyperlanes(starSystem, k);
}