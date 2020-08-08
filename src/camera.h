#ifndef H_CAMERA
#define H_CAMERA

#include "planetes.h"
#include "time.h"

/* types =============================================================== */
typedef enum {CARTE, NORMAL, SYSTEME} VueType;

typedef enum {MENU_QUITTER = -1, MENU_AUCUN, MENU_SYSTEME, MENU_MARCHE, MENU_FLOTTE, MENU_RECHERCHE, MENU_CONTACTS} ClassMenu;

typedef enum {MENU_SYSTEME_FLOTTES = 1, MENU_SYSTEME_ETOILE, 
MENU_SYSTEME_PLANETE_RESUME, MENU_SYSTEME_PLANETE_DISTRICT, MENU_SYSTEME_PLANETE_BATIMENT, MENU_SYSTEME_PLANETE_BATIMENT_CHOIX, 
MENU_SYSTEME_FLOTTE_DETAILS, 
MENU_SYSTEME_STATION_RESUME, MENU_SYSTEME_STATION_MODULES, MENU_SYSTEME_STATION_MODULES_CHOIX, MENU_SYSTEME_STATION_CHANTIER,
MENU_SYSTEME_STATION_CHANTIER_CHOIX} MenuSysteme;

/* structures ========================================================== */
typedef struct CameraStruct Camera;

typedef struct FenetreStruct Fenetre;

/* entry points ======================================================== */
Camera *AllocCamera();

void SetCameraX(Camera *camera, int x);
void AddCameraX(Camera *camera, int x);
int GetCameraX(Camera *camera);
void SetCameraY(Camera *camera, int y);
void AddCameraY(Camera *camera, int y);
int GetCameraY(Camera *camera);

void SetCameraXSystem(Camera *camera, int x);
void AddCameraXSystem(Camera *camera, int x);
int GetCameraXSystem(Camera *camera);
void SetCameraYSystem(Camera *camera, int y);
void AddCameraYSystem(Camera *camera, int y);
int GetCameraYSystem(Camera *camera);

void SetCameraXVector(Camera *camera, int x);
void AddCameraXVector(Camera *camera, int x);
int GetCameraXVector(Camera *camera);
void SetCameraYVector(Camera *camera, int y);
void AddCameraYVector(Camera *camera, int y);
int GetCameraYVector(Camera *camera);

void SetCameraZoom(Camera *camera, int zoom);
int GetCameraZoom(Camera *camera);

void SetCameraMapType(Camera *camera, VueType MapType);
VueType GetCameraMapType(Camera *camera);

void SetCameraLock(Camera *camera, bool lock);
bool GetCameraLock(Camera *camera);

void SetCameraViewedSystem(Camera *camera, int system);
void SetCameraSystem(Camera *camera, int system);
void SetCameraSystemViewStatus(Camera *camera, bool status);
int GetCameraViewedSystem(Camera *camera);
int GetCameraSystem(Camera *camera);

bool IsCameraMoveFleet(Camera *camera);
void SetCameraMoveFleet(Camera *camera, bool status);

void SetCameraSelection(Camera *camera, int selection);
int GetCameraSelection(Camera *camera);

void SetCameraFleet(Camera *camera, int fleet);
int GetCameraFleet(Camera *camera);

void SetCameraEmpire(Camera *camera, int empire);
int GetCameraEmpire(Camera *camera);

//fenetre
Fenetre *AllocFenetre();

void OpenMenu(Fenetre *fenetre, Camera *camera, ClassMenu classMenu, MenuSysteme menuSysteme);
void CloseMenu(Fenetre *fenetre, Camera *camera);
ClassMenu GetOpenedMenuClass(Fenetre *fenetre);
MenuSysteme GetOpenedMenuSystem(Fenetre *fenetre);

void OpenCommandPrompt(Fenetre *fenetre, Camera *camera, Date *date);
void CloseCommandPrompt(Fenetre *fenetre, Camera *camera, Date *date);
bool GetCommandPromptStatus(Fenetre *fenetre);

void SetWindowPlanet(Fenetre *fenetre, int planete);
int GetWindowPlanet(Fenetre *fenetre);

void SetWindowSelection(Fenetre *fenetre, int selection);
int GetWindowSelection(Fenetre *fenetre);
void IncrementWindowSelection(Fenetre *fenetre);
void UnincrementWindowSelection(Fenetre *fenetre);
void AddWindowSelection(Fenetre *fenetre, int number);

void SetWindowSelectedFleet(Fenetre *fenetre, int fleet);
void GetWindowSelectedFleet(Fenetre *fenetre);

void SetWindowPrevious(Fenetre *fenetre, int previous);
void GetWindowPrevious(Fenetre *fenetre);
#endif