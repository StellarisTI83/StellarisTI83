#ifndef H_PLANETES
#define H_PLANETES

#include "ordres.h"

/* types =============================================================== */
typedef enum {AUCUN = 1, CAPITALE, FONDERIE, LABORATOIRE, USINE_CIVILE, THEATRE} Batiment;

/* 1 = arid, 2 = Desert, 3 = Savanna, 4 = Alpine, 5 = Arctic, 6 = Tundra, 7 = continental, 8 = ocean, 9 = tropical, 10 = monde aride,
 * 11 = monde aride froid, 12 = brisé, 13 = monde gelé, 14 = géante gazeuse, 15 = monde en fusion, 16 = toxic world
 */
typedef enum {HABITABLE_ARID = 1, HABITABLE_DESERT, HABITABLE_SAVANNA, HABITABLE_ALPINE, HABITABLE_ARCTIC, HABITABLE_TUNDRA,
    HABITABLE_CONTINENTAL, HABITABLE_OCEAN, HABITABLE_TROPICAL, ARID, COLD_ARID, BROKE, FROZEN, GAS_GIANT, MOLTEN, TOXIC} PlanetType;

/* structures ========================================================== */
typedef struct VillesStruct Villes;

typedef struct PlaneteStruct Planete;

/* entry points ======================================================== */
Planete *AllocPlanet();

void SetPlanetHabitability(Planete *planete, bool habitability);
bool GetPlanetHabitability(Planete *planete);

void SetPlanetOrbitRadius(Planete *planete, int orbitRadius);
int GetPlanetOrbitRadius(Planete *planete);

void SetPlanetXY(Planete *planete, int x, int y);
int GetPlanetX(Planete *planete);
int GetPlanetY(Planete *planete);

void SetPlanetType(Planete *planete, PlanetType type);
PlanetType GetPlanetType(Planete *planete);

void SetPlanetRadius(Planete *planete, int radius);
int GetPlanetRadius(Planete *planete);

void SetPlanetName(Planete *planete, char* string);
char* GetPlanetName(Planete *planete);
#endif