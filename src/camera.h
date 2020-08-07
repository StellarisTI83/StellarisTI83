#ifndef H_CAMERA
#define H_CAMERA

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
int getCameraX(Camera *camera);
void SetCameraY(Camera *camera, int y);
int getCameraY(Camera *camera);

void SetCameraXSystem(Camera *camera, int x);
int getCameraXSystem(Camera *camera);
void SetCameraYSystem(Camera *camera, int y);
int getCameraYSystem(Camera *camera);

void SetCameraXVector(Camera *camera, int x);
int getCameraXVector(Camera *camera);
void SetCameraYVector(Camera *camera, int y);
int getCameraYVector(Camera *camera);

void SetCameraZoom(Camera *camera, int zoom);
int GetCameraZoom(Camera *camera);

void SetCameraMapType(Camera *camera, VueType MapType);
VueType GetCameraMapType(Camera *camera);

void SetCameraLock(Camera *camera, bool lock);
bool GetCameraLock(Camera *camera);

void SetCameraSystem(Camera *camera, int system);
void SetCameraSystemViewStatus(Camera *camera, bool status);
bool GetCameraViewedSystem(Camera *camera);

Fenetre *AllocFenetre();
void OpenMenu(Fenetre *fenetre, Camera *camera, ClassMenu classMenu, MenuSysteme menuSysteme);
void CloseMenu(Fenetre *fenetre, Camera *camera);

void OpenCommandPrompt(Fenetre *fenetre);
void CloseCommandPrompt(Fenetre *fenetre);
bool GetCommandPromptStatus(Fenetre *fenetre);
#endif