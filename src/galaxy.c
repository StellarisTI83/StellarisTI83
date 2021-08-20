#include <tice.h>

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

char * nameStars[] = {
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

/**
 * @brief Function to generate a planet
 * 
 * @param starSystem 
 * @param planetIndex 
 * @param habitable 
 */
static void galaxy_PlanetInitialize(StarSystem *starSystem, int planetIndex){
    int orbitRadius = 0, size = 0, type = 0;
    int x = 0;
    int y = 0;
    bool habitable = 0;
    Planet *planet;

    planet = starSystem_PlanetAlloc(starSystem);
    
    // Choice of the type of the planet
    type = randInt(1, 100);
    switch(planetIndex){
        case 4:
            if(type <= 60) {
                type = GAS_GIANT;
                habitable = false;
            } else if (type <= 90){
                type = COLD_ARID;
                habitable = false;
            } else {
                type = BROKE;
                habitable = false;
            }
            break;
        case 3:
            if(type <= 30) {
                type = GAS_GIANT;
                habitable = false;
            } else if (type <= 70){
                type = COLD_ARID;
                habitable = false;
            } else if (type <= 75){
                type = HABITABLE_ALPINE;
                habitable = true;
            } else if (type <= 80){
                type = HABITABLE_ARCTIC;
                habitable = true;
            } else if (type <= 85){
                type = HABITABLE_TUNDRA;
                habitable = true;
            } else if (type <= 90){
                type = HABITABLE_CONTINENTAL;
                habitable = true;
            } else {
                type = BROKE;
                habitable = false;
            }	
            break;
        case 2:
            if(type <= 5) {
                type = GAS_GIANT;
                habitable = false;
            } else if (type <= 20){
                type = TOXIC;
                habitable = false;
            } else if (type <= 60){
                type = COLD_ARID;
                habitable = false;
            } else if (type <= 70){
                type = HABITABLE_ALPINE;
                habitable = true;
            } else if (type <= 74){
                type = HABITABLE_ARCTIC;
                habitable = true;
            } else if (type <= 78){
                type = HABITABLE_TUNDRA;
                habitable = true;
            } else if (type <= 82){
                type = HABITABLE_CONTINENTAL;
                habitable = true;
            } else if (type <= 86){
                type = HABITABLE_OCEAN;
                habitable = true;
            } else {
                type = BROKE;
                habitable = false;
            }
            break;
        case 1:
            if(type <= 2) {
                type = GAS_GIANT; 
                habitable = false;
            } else if (type <= 30){
                type = TOXIC; 
                habitable = false;
            } else if (type <= 80){
                type = ARID; 
                habitable = false;
            } else if (type <= 83){
                type = HABITABLE_CONTINENTAL; 
                habitable = true;
            } else if (type <= 86){
                type = HABITABLE_OCEAN; 
                habitable = true;
            } else if (type <= 89){
                type = HABITABLE_TROPICAL; 
                habitable = true;
            } else {
                type = BROKE; 
                habitable = false;
            }
            break;
        case 0:
            if(type <= 3) {
                type = HABITABLE_ARID; 
                habitable = true;
            } else if (type <= 6){
                type = HABITABLE_DESERT; 
                habitable = true;
            } else if (type <= 9){
                type = HABITABLE_SAVANNA; 
                habitable = true;
            } else if (type <= 40){
                type = ARID;
                habitable = false;
            } else if (type <= 50){
                type = BROKE;
                habitable = false;
            } else{
                type = MOLTEN;
                habitable = false;
            }
            break;
    }
    planet_TypeSet(planet, type);

    // Choice of the size of the planet
    size = randInt(1, 100);
    switch(planetIndex){
        case 0:
            orbitRadius = randInt(RADIUS_PLANET1, RADIUS_PLANET1 + 10);
            if(size <= 10) {
                size = 2;
            } else if(size <= 70) {
                size = 3;
            } else if(size <= 90) {
                size = 4;
            } else {
                size = 5;
            }
            break;
        case 1:
            orbitRadius = randInt(RADIUS_PLANET2, RADIUS_PLANET2 + 10);
            if(size <= 10) {
                size = 2;
            } else if(size <= 70) {
                size = 3;
            } else if(size <= 90) {
                size = 4;
            } else {
                size = 5;
            }
            break;
        case 2:
            orbitRadius = randInt(RADIUS_PLANET3, RADIUS_PLANET3 + 10);
            if(size <= 10) {
                size = 2;
            } else if(size <= 70) {
                size = 3;
            } else if(size <= 90) {
                size = 4;
            } else {
                size = 5;
            }
            break;
        case 3:
            orbitRadius = randInt(RADIUS_PLANET4, RADIUS_PLANET4 + 10);
            if(size <= 10) {
                size = 2;
            } else if(size <= 70) {
                size = 3;
            } else if(size <= 90) {
                size = 4;
            } else {
                size = 5;
            }
            break;
        case 4:
            orbitRadius = randInt(RADIUS_PLANET5, RADIUS_PLANET5 + 10);
            if(type == GAS_GIANT) {
                size = 5;
            } else if(size <= 10) {
                size = 2;
            } else if(size <= 70) {
                size = 3;
            } else if(size <= 90) {
                size = 4;
            } else {
                size = 5;
            }
            break;
    }
    planet_OrbitRadiusSet(planet, orbitRadius);
    planet_SizeSet(planet, size);

    // Set the x position
    x = randInt(SYSTEM_MIDDLE_X - orbitRadius, 
                SYSTEM_MIDDLE_X + orbitRadius);
    
    // Set the y position
    y = sqrt(pow((double)orbitRadius, 2.0) - pow((double)(x - SYSTEM_MIDDLE_X), 2.0)) + SYSTEM_MIDDLE_Y;
    if(randInt(0, 1) == 1) {
        y = SYSTEM_MIDDLE_Y - (y - SYSTEM_MIDDLE_Y);
    }
    planet_PositionSet( planet,
                        x, 
                        y);

    //Set the name of the planet
    if(habitable)
        planet_NameGenerate(planet);
}

/**
 * @brief Function to generate a system
 * 
 * @param xPosition 
 * @param yPosition 
 * @param starIndex 
 * @return StarSystem* 
 */
static StarSystem *galaxy_SystemGenerate(int xPosition, int yPosition, int starIndex){
    StarSystem *starSystem = NULL;
    StarType star = STAR_NONE;
    int planetNumber, habitablePlanetNumber;
    int planetIndex;
    // Create the structure of the star system
    starSystem = starSystem_Create();

    // Set the star system position
    starSystem_SetXY(starSystem, xPosition, yPosition);

    // Create the station for the system
    starSystem_StationCreate(starSystem);

    // Set the empire
    starSystem_EmpireSet(starSystem, NO_EMPIRE);
    
    // Set the intel level
    starSystem_IntelLevelSet(starSystem, INTEL_UNKNOWN);

    // Randomize the star
    star = randInt(1, STAR_TOTAL_PROBABILITY);

    if(star <= STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_B;
    } else if(star <= STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_A;
    } else if(star <= STAR_TYPE_F_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_F;
    } else if(star <= STAR_TYPE_G_PROBABILITY + STAR_TYPE_F_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_G;
    } else if(star <= STAR_TYPE_K_PROBABILITY + STAR_TYPE_G_PROBABILITY + STAR_TYPE_F_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_K;
    } else if(star <= STAR_TYPE_M_PROBABILITY + STAR_TYPE_K_PROBABILITY + STAR_TYPE_G_PROBABILITY + STAR_TYPE_F_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_M;
    } else if(star <= STAR_TYPE_BLACKHOLE_PROBABILITY + STAR_TYPE_M_PROBABILITY + STAR_TYPE_K_PROBABILITY + STAR_TYPE_G_PROBABILITY + STAR_TYPE_F_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_BLACKHOLE;
    } else if(star <= STAR_TYPE_PULSAR_PROBABILITY + STAR_TYPE_BLACKHOLE_PROBABILITY + STAR_TYPE_M_PROBABILITY + STAR_TYPE_K_PROBABILITY + STAR_TYPE_G_PROBABILITY + STAR_TYPE_F_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_B_PROBABILITY) {
        star = STAR_TYPE_PULSAR;
    } else {
        star = STAR_TYPE_NEUTRON;
    }
    star = STAR_TYPE_F;
    starSystem_StarTypeSet(starSystem, star);


    // Randomize the number of planet
    planetNumber = randInt(1, 5);
    habitablePlanetNumber = 0;

    // Randomize the number of habitable planet
    if((starSystem_StarTypeGet(starSystem) == STAR_TYPE_M) ||
    ((starSystem_StarTypeGet(starSystem) == STAR_TYPE_G) || 
    (starSystem_StarTypeGet(starSystem) == STAR_TYPE_K))){
        habitablePlanetNumber = randInt(0, planetNumber);
    }

    // Randomize the number of habitable planet
    planetIndex = 0;
    while(planetIndex < planetNumber){
        galaxy_PlanetInitialize(starSystem, planetIndex);
        planetIndex++;
    }	

    if(starIndex > 110)
        starIndex -= randInt(0, 100);
    starSystem_NameSet(starSystem, nameStars[starIndex]);

    hyperlane_DestinationSet(starSystem, 0, 255);
    hyperlane_DestinationSet(starSystem, 1, 255);
    hyperlane_DestinationSet(starSystem, 2, 255);
    hyperlane_DestinationSet(starSystem, 3, 255);

    return starSystem;
}

/**
 * @brief The function to generate every star system
 * 
 * @param galaxyMatrix 
 * @param starSystem 
 * @param loading 
 * @return int 
 */
static int galaxy_AllSystemGenerate(int *galaxyMatrix, 
                                    StarSystem **starSystem,
                                    int *loading){
    int xIndex = 0, yIndex = 0, starIndex = 0;
    int xPosition = 0, yPosition = 0;
    
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Generate system structure\n");
    #endif

    while(yIndex < GALAXY_WIDTH) {
        while(xIndex < GALAXY_WIDTH * 2) {
            
            // Get the positions
            xPosition   =   galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex];
            yPosition   =   galaxyMatrix[yIndex * GALAXY_WIDTH * 2 + xIndex + 1];

            starSystem[starIndex] = galaxy_SystemGenerate(xPosition, yPosition, starIndex);

            starIndex++;
            xIndex += 2;
            gfx_FillRectangle_NoClip(50, 160, *loading += 1, 5);
        }
        xIndex = 0;
        yIndex++;
    }
    return starIndex;
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
        if(hyperlane_DestinationGet(systeme, 0) != 255) {
            hyperlane_DestinationSet(systemeStellaires[hyperlane_DestinationGet(systeme, 0)], 2, i);
        }
        if(hyperlane_DestinationGet(systeme, 1) != 255) {
            hyperlane_DestinationSet(systemeStellaires[hyperlane_DestinationGet(systeme, 1)], 3, i);
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
            if(hyperlane_DestinationGet(systeme, j) != 255){
                if((starSystem_GetX(systemeStellaires[hyperlane_DestinationGet(systeme, j)]) != 0) && (starSystem_GetY(systemeStellaires[hyperlane_DestinationGet(systeme, j)]) != 0)){
                    double angle = 0;

                    angle = atan2(starSystem_GetY(systemeStellaires[hyperlane_DestinationGet(systeme, j)]) - starSystem_GetY(systeme), starSystem_GetX(systemeStellaires[hyperlane_DestinationGet(systeme, j)]) - starSystem_GetX(systeme));
                    
                    x = SYSTEM_MIDDLE_X + ((SYSTEM_VIEW_RADIUS + 5) * cos(angle));

                    y = SYSTEM_MIDDLE_Y + ((SYSTEM_VIEW_RADIUS + 5) * sin(angle));
                    hyperlane_XYSet(systeme, j, x, y);
                }
            }
            j++;
        }
        i++;
    }
}

/* entry points ======================================================== */

void galaxy_StartEmpiresInitialize(Settings *parametres, EmpireList *empireListe, StarSystem **starSystem, Camera *camera){
    int systemIndex = 0, fin = 1, empireIndex = 0;
    int k = GALAXY_WIDTH * GALAXY_WIDTH;
    int planete = 0;
    Empire *joueur = empire_Get(empireListe, 0);
    //creation joueur
    while(fin == 1) { // choix du systeme
        systemIndex = randInt(0, k - 1);
        gfx_SetTextXY(50, 70);
        #ifdef DEBUG_VERSION
            dbg_sprintf(dbgout, "%d %d %d\n", starSystem_GetX(starSystem[systemIndex]), starSystem_GetY(starSystem[systemIndex]), starSystem_StarTypeGet(starSystem[systemIndex]));
        #endif
        if(((starSystem_GetX(starSystem[systemIndex]) >= 160) && (starSystem_GetY(starSystem[systemIndex]) >= 120)) && (starSystem_StarTypeGet(starSystem[systemIndex]) != STAR_TYPE_BLACKHOLE))
            fin = 0;
    }
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "\nCreate empires\n");
    #endif
    empire_ColorSet(joueur, 9);
    empire_CapitalSystemSet(joueur, systemIndex);

    starSystem_StarTypeSet(starSystem[systemIndex], STAR_TYPE_K);
    SetSystemPlanetHabitableNumber(starSystem[systemIndex], 1);
    SetSystemPlanetInhabitedNumber(starSystem[systemIndex], 1);
    starSystem_EmpireSet(starSystem[systemIndex], 0);
    starSystem_IntelLevelSet(starSystem[systemIndex], INTEL_FULL);

    SetEmpireCredit(joueur, 100);
    SetEmpireMinerals(joueur, 100);
    SetEmpireFood(joueur, 200);
    SetEmpireAlloys(joueur, 100);
    SetEmpireConsumerGoods(joueur, 100);

    starSystem_StationLevelSet(starSystem[systemIndex], PORT_STELLAIRE);
    starSystem_StationModuleSet(starSystem[systemIndex], 0, CHANTIER_SPATIAL);
    starSystem_StationModuleSet(starSystem[systemIndex], 1, CARREFOUR_COMMERCIAL);
    

    empire_FleetAdd(joueur, systemIndex, FLOTTE_MILITAIRE, 3, 0, 0, 0);

    empire_FleetAdd(joueur, systemIndex, FLOTTE_DE_CONSTRUCTION, 0, 0, 0, 0);

    empire_FleetAdd(joueur, systemIndex, FLOTTE_SCIENTIFIQUE, 0, 0, 0, 0);
    
    SetCameraX(camera, starSystem_GetX(starSystem[systemIndex])*2); // centre la vue sur le systeme
    SetCameraY(camera, starSystem_GetY(starSystem[systemIndex])*2);
    SetCameraSystem(camera, systemIndex);

    planete = randInt(0, GetSystemPlanetNumber(starSystem[systemIndex]) - 1);

    starSystem_PlanetHabitabilitySet(starSystem[systemIndex], planete, true);
    starSystem_PlanetTypeSet(starSystem[systemIndex], planete, HABITABLE_CONTINENTAL);

    SetCameraXSystem(camera, starSystem_PlanetXGet(starSystem[systemIndex], planete) - 160);
    SetCameraYSystem(camera, starSystem_PlanetYGet(starSystem[systemIndex], planete) - 120);
    // starSystem_PlanetNameSet(systemeStellaires[i], planete, planetesName[randInt(0, (sizeof(planetesName)/sizeof(planetesName[0])) - 1 )]);

    starSystem_PlanetCityCreate(starSystem[systemIndex], planete);
    starSystem_PlanetCityPopulationSet(starSystem[systemIndex], planete, 27);
    starSystem_PlanetCityDistrictSet(starSystem[systemIndex], planete, 4, 3, 3, 3);

    CalculateSystemPlanetCityJob(starSystem[systemIndex], planete);

    SetSystemPlanetCityBuilding(starSystem[systemIndex], planete, 1, BUILDING_CAPITAL, 3);
    SetSystemPlanetCityBuilding(starSystem[systemIndex], planete, 2, BUILDING_CIVILIAN_INDUSTRIES, 1);
    SetSystemPlanetCityBuilding(starSystem[systemIndex], planete, 3, BUILDING_FOUNDRIES, 1);

    CalculateEmpirePower(joueur);

    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Empire: %d (%p)\n -System: %d (%d, %d)\n -Color: %d\n -Planet: %d\n -Fleet: %p\nCamera: %d %d\n", 1, joueur, systemIndex, starSystem_GetX(starSystem[systemIndex]), starSystem_GetY(starSystem[systemIndex]), GetEmpireColor(joueur), planete, empire_FleetListGet(joueur), GetCameraX(camera), GetCameraY(camera));
    #endif
    
    for(empireIndex = 1; empireIndex < settings_EmpireNumberGet(parametres); empireIndex++){
        Empire *empire = NULL;
        fin = 1;
        systemIndex = 0;
        while(fin == 1) { // choix du systeme
            systemIndex = randInt(0, k - 1);
            if(((starSystem_GetX(starSystem[systemIndex]) >= 160) && (starSystem_GetY(starSystem[systemIndex]) >= 120)) && ((starSystem_StarTypeGet(starSystem[systemIndex]) != STAR_TYPE_BLACKHOLE) && (starSystem_EmpireGet(starSystem[systemIndex]) == -1)))
                fin = 0;
        }
        empire = empire_Add(empireListe);
        empire_Generate(empire, empireIndex, starSystem[systemIndex], systemIndex, randInt(20, 29));
    }
    ai_RelationsUpdate(empireListe);
}


void galaxy_CreateNew(StarSystem **starSystem){
    int loading = 1;	// The advancement of the loading bar
    int radiusExtern = ((SPACE_BETWEEN_STARS * GALAXY_WIDTH) - SPACE_BETWEEN_STARS) / 2 - 25; 
    int radiusIntern = 50;
    int *galaxyMatrix = NULL;
    int starNumber;
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

    starNumber = galaxy_AllSystemGenerate(galaxyMatrix, starSystem, &loading);

    free(galaxyMatrix);
    // RecreateHyperlanes(starSystem, starNumber);
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "End of galaxy\n");
    #endif
}