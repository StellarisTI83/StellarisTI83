#ifndef H_CAMERA
#define H_CAMERA

/* macros ============================================================== */

#define SYSTEM_SPECIAL_X 480
#define SYSTEM_SPECIAL_Y 360

#define SYSTEM_MIDDLE_X 320
#define SYSTEM_MIDDLE_Y 240

#define SYSTEM_VIEW_RADIUS 160

/* types =============================================================== */

/**
 * @brief Differents view types
 * 
 */
typedef enum {  VUE_MAP, 
                VUE_GALACTIC, 
                VUE_SYSTEM} VueType;

/**
 * @brief Differents zoom levels
 * 
 */
typedef enum {  ZOOM_MIN = 0, 
                ZOOM_NORMAL, 
                ZOOM_MAX} ZoomLevel;



/* structures ========================================================== */

typedef struct CameraStruct Camera;

typedef struct FenetreStruct Window;

#include "main.h"
#include "planet.h"
#include "menus.h"
#include "time.h"

/* entry points ======================================================== */

/**
 * @brief Create a new camera
 * 
 * @return Camera* 
 */
Camera *camera_Create();

void camera_XSet(Camera *camera, int x);
void camera_XAdd(Camera *camera, int x);
int camera_XGet(Camera *camera);
void camera_YSet(Camera *camera, int y);
void camera_YAdd(Camera *camera, int y);
int camera_YGet(Camera *camera);

void camera_XSystemSet(Camera *camera, int x);
void camera_XSystemAdd(Camera *camera, int x);
int camera_XSystemGet(Camera *camera);
void camera_YsystemSet(Camera *camera, int y);
void camera_YSystemAdd(Camera *camera, int y);
int camera_YSystemGet(Camera *camera);

void camera_XVectorSet(Camera *camera, int x);
void camera_XVectorAdd(Camera *camera, int x);
int camera_XVectorGet(Camera *camera);
void camera_YVectorSet(Camera *camera, int y);
void camera_YVectorAdd(Camera *camera, int y);
int camera_YVectorGet(Camera *camera);

void camera_ZoomSet(Camera *camera, int zoom);
int camera_ZoomGet(Camera *camera);

void camera_MapTypeSet(Camera *camera, VueType MapType);
VueType camera_MapTypeGet(Camera *camera);

void SetCameraLock(Camera *camera, int lock);
int GetCameraLockStatus(Camera *camera);

void SetCameraViewedSystem(Camera *camera, int system);
void SetCameraSystem(Camera *camera, int system);
void SetCameraSystemViewStatus(Camera *camera, int status);
int camera_SystemActualGet(Camera *camera);
int camera_SystemAimedGet(Camera *camera);

int IsCameraMoveFleet(Camera *camera);
void SetCameraMoveFleet(Camera *camera, int status);

void SetCameraSelection(Camera *camera, int selection);
int GetCameraSelection(Camera *camera);

void SetCameraFleet(Camera *camera, int fleet);
int GetCameraFleet(Camera *camera);

void SetCameraEmpire(Camera *camera, int empire);
int GetCameraEmpire(Camera *camera);

//fenetre
Window *AllocFenetre();

void OpenMenu(Window *fenetre, Camera *camera, ClassMenu classMenu, MenuSystem menuSysteme);
void CloseMenu(Window *fenetre, Camera *camera);
ClassMenu GetOpenedMenuClass(Window *fenetre);
MenuSystem GetOpenedMenuDetails(Window *fenetre);
void SetWindowMenuSystem(Window *fenetre, MenuSystem menu);

void OpenCommandPrompt(Window *fenetre, Camera *camera, Time *date);
void CloseCommandPrompt(Window *fenetre, Camera *camera, Time *date);
int GetCommandPromptStatus(Window *fenetre);

void SetWindowPlanet(Window *fenetre, int planete);
int GetWindowPlanet(Window *fenetre);

void SetWindowSelection(Window *fenetre, int selection);
int GetWindowSelection(Window *fenetre);
void IncrementWindowSelection(Window *fenetre);
void UnincrementWindowSelection(Window *fenetre);
void AddWindowSelection(Window *fenetre, int number);

void SetWindowSelectedFleet(Window *fenetre, int fleet);
int GetWindowSelectedFleet(Window *fenetre);

void SetWindowPrevious(Window *fenetre, int previous);
int GetWindowPrevious(Window *fenetre);

Error GetWindowError(Window *fenetre);
void SetWindowError(Window *fenetre, Error error);
int GetWindowErrorCountDown(Window *fenetre);
void UnincrementWindowErrorCountdown(Window *fenetre);
void SetWindowErrorCountdown(Window *fenetre, int countdown);

void SetWindowCity(Window *fenetre, City *city);
City *GetWindowCity(Window *fenetre);
void SetWindowEmpire(Window *fenetre, int empire);
int GetWindowEmpire(Window *fenetre);

void SetWindowScroll(Window *fenetre, int scroll);
void AddWindowScroll(Window *fenetre, int scroll);
int GetWindowScroll(Window *fenetre);

void SetWindowFleetSize(Window *fenetre, int size);
void AddWindowFleetSize(Window *fenetre, int size);
int GetWindowFleetSize(Window *fenetre);
#endif