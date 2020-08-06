#ifndef H_SYSTEMES
#define H_SYSTEMES

#include "planetes.h"
#include "stations.h"

/* types =============================================================== */
typedef enum {INCONNU, FAIBLE, MOYEN, ELEVEE, TOTAL} NiveauDeConnaissance;

typedef enum {ETOILE_TYPE_B = 1, ETOILE_TYPE_A, ETOILE_TYPE_F, ETOILE_TYPE_G, ETOILE_TYPE_K, ETOILE_TYPE_M, ETOILE_TYPE_TROU_NOIR, ETOILE_TYPE_PULSAR, ETOILE_TYPE_ETOILE_A_NEUTRONS} EtoileType;

/* structures ========================================================== */
typedef struct HyperlaneStruct Hyperlane;

typedef struct SystemeStellaireStruct SystemeStellaire;


/* internal public functions =========================================== */
void SetSystemXY(SystemeStellaire *systemeStellaire, int x, int y);
int GetSystemX(SystemeStellaire *systemeStellaire);
int GetSystemY(SystemeStellaire *systemeStellaire);

void CreateSystemStation(SystemeStellaire *systemeStellaire);

void SetSystemIntelLevel(SystemeStellaire *systemeStellaire, NiveauDeConnaissance niveau);
NiveauDeConnaissance GetSystemIntelLevel(SystemeStellaire *systemeStellaire);

void SetSystemStationLevel(SystemeStellaire *systemeStellaire, Stationlevel level);
Stationlevel GetSystemStationLevel(SystemeStellaire *systemeStellaire);

void SetSystemStarType(SystemeStellaire *systemeStellaire, EtoileType type);
EtoileType GetSystemStarType(SystemeStellaire *systemeStellaire);

void SetSystemPlanetNumber(SystemeStellaire *systemeStellaire, int number);
int GetSystemPlanetNumber(SystemeStellaire *systemeStellaire);

void SetSystemPlanetHabitableNumber(SystemeStellaire *systemeStellaire, int number);
int GetSystemPlanetHabitableNumber(SystemeStellaire *systemeStellaire);

void SetSystemName(SystemeStellaire *systemeStellaire, char* string);
char* GetSystemName(SystemeStellaire *systemeStellaire);

void SetHyperlaneDestination(SystemeStellaire *systemeStellaire, int numeroHyperlane, int destination);
int GetHyperlaneDestination(SystemeStellaire *systemeStellaire, int numeroHyperlane);

void SetHyperlaneXY(SystemeStellaire *systemeStellaire, int numeroHyperlane, int x, int y);
int GetHyperlaneX(SystemeStellaire *systemeStellaire, int numeroHyperlane);
int GetHyperlaneY(SystemeStellaire *systemeStellaire, int numeroHyperlane);

void CreateSystemPlanet(SystemeStellaire *systemeStellaire, int position);

void SetSystemPlanetHabitability(SystemeStellaire *systemeStellaire, int number, bool habitability);
bool GetSystemPlanetHabitability(SystemeStellaire *systemeStellaire, int number);

void SetSystemPlanetOrbitRadius(SystemeStellaire *systemeStellaire, int number, int);
int GetSystemPlanetOrbitRadius(SystemeStellaire *systemeStellaire, int number);

void SetSystemPlanetXY(SystemeStellaire *systemeStellaire, int number, int x, int y);
int GetSystemPlanetX(SystemeStellaire *systemeStellaire, int number);
int GetSystemPlanetY(SystemeStellaire *systemeStellaire, int number);

void SetSystemPlanetType(SystemeStellaire *systemeStellaire, int number, PlanetType type);
PlanetType GetSystemPlanetType(SystemeStellaire *systemeStellaire, int number);

void SetSystemPlanetRadius(SystemeStellaire *systemeStellaire, int number, int radius);
int GetSystemPlanetRadius(SystemeStellaire *systemeStellaire, int number);

void SetSystemPlanetName(SystemeStellaire *systemeStellaire, int number, char* string);
char* GetSystemPlanetName(SystemeStellaire *systemeStellaire, int number);
#endif