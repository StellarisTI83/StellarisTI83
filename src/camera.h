#ifndef H_CAMERA
#define H_CAMERA

#include "main.h"
#include "planetes.h"
#include "time.h"

/* types =============================================================== */
enum VueTypeEnum {CARTE, NORMAL, SYSTEME};

typedef enum VueTypeEnum VueType;

enum ClassMenuEnum {MENU_QUITTER = -1, MENU_AUCUN, MENU_SYSTEME, MENU_MARCHE, MENU_FLOTTE, MENU_RECHERCHE, MENU_CONTACTS, MENU_CONTACTS_DETAILS};

typedef enum ClassMenuEnum ClassMenu;

enum MenuSystemeEnum {MENU_SYSTEME_AUCUN, MENU_SYSTEME_FLOTTES = 1, MENU_SYSTEME_ETOILE, 
MENU_SYSTEME_PLANETE_RESUME, MENU_SYSTEME_PLANETE_DISTRICT, MENU_SYSTEME_PLANETE_BATIMENT, MENU_SYSTEME_PLANETE_BATIMENT_CHOIX, 
MENU_SYSTEME_FLOTTE_DETAILS, 
MENU_SYSTEME_STATION_RESUME, MENU_SYSTEME_STATION_MODULES, MENU_SYSTEME_STATION_MODULES_CHOIX, MENU_SYSTEME_STATION_CHANTIER,
MENU_SYSTEME_STATION_CHANTIER_CHOIX};

typedef enum MenuSystemeEnum MenuSystem;

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

void SetCameraLock(Camera *camera, int lock);
int GetCameraLock(Camera *camera);

void SetCameraViewedSystem(Camera *camera, int system);
void SetCameraSystem(Camera *camera, int system);
void SetCameraSystemViewStatus(Camera *camera, int status);
int GetCameraViewedSystem(Camera *camera);
int GetCameraSystem(Camera *camera);

int IsCameraMoveFleet(Camera *camera);
void SetCameraMoveFleet(Camera *camera, int status);

void SetCameraSelection(Camera *camera, int selection);
int GetCameraSelection(Camera *camera);

void SetCameraFleet(Camera *camera, int fleet);
int GetCameraFleet(Camera *camera);

void SetCameraEmpire(Camera *camera, int empire);
int GetCameraEmpire(Camera *camera);

//fenetre
Fenetre *AllocFenetre();

void OpenMenu(Fenetre *fenetre, Camera *camera, ClassMenu classMenu, MenuSystem menuSysteme);
void CloseMenu(Fenetre *fenetre, Camera *camera);
ClassMenu GetOpenedMenuClass(Fenetre *fenetre);
MenuSystem GetOpenedMenuDetails(Fenetre *fenetre);
void SetWindowMenuSystem(Fenetre *fenetre, MenuSystem menu);

void OpenCommandPrompt(Fenetre *fenetre, Camera *camera, Date *date);
void CloseCommandPrompt(Fenetre *fenetre, Camera *camera, Date *date);
int GetCommandPromptStatus(Fenetre *fenetre);

void SetWindowPlanet(Fenetre *fenetre, int planete);
int GetWindowPlanet(Fenetre *fenetre);

void SetWindowSelection(Fenetre *fenetre, int selection);
int GetWindowSelection(Fenetre *fenetre);
void IncrementWindowSelection(Fenetre *fenetre);
void UnincrementWindowSelection(Fenetre *fenetre);
void AddWindowSelection(Fenetre *fenetre, int number);

void SetWindowSelectedFleet(Fenetre *fenetre, int fleet);
int GetWindowSelectedFleet(Fenetre *fenetre);

void SetWindowPrevious(Fenetre *fenetre, int previous);
int GetWindowPrevious(Fenetre *fenetre);

Error GetWindowError(Fenetre *fenetre);
void SetWindowError(Fenetre *fenetre, Error error);
int GetWindowErrorCountDown(Fenetre *fenetre);
void UnincrementWindowErrorCountdown(Fenetre *fenetre);
void SetWindowErrorCountdown(Fenetre *fenetre, int countdown);

void SetWindowCity(Fenetre *fenetre, Villes *city);
Villes *GetWindowCity(Fenetre *fenetre);

void SetWindowScroll(Fenetre *fenetre, int scroll);
void AddWindowScroll(Fenetre *fenetre, int scroll);
int GetWindowScroll(Fenetre *fenetre);

void SetWindowFleetSize(Fenetre *fenetre, int size);
void AddWindowFleetSize(Fenetre *fenetre, int size);
int GetWindowFleetSize(Fenetre *fenetre);
#endif