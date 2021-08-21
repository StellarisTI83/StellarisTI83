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

#include "main.h"

#include "ai.h"
#include "camera.h"

/* structures ========================================================== */
struct CameraStruct{
	int x;
	int y;
	int xSysteme;
	int ySysteme;
	int xVector;
	int yVector;

	char zoom;
	VueType mapType;
	int lock;

	int systemeSelectione; // systeme dans lequel on est
	int systeme; // systeme que la camera pointe

	int selection;
	int ouverte;

	int bougerFlotte;
	int flotte;
	int empire;
};

struct FenetreStruct{
	ClassMenu menu;
	MenuSystem menuDetails;
	int selection;

	int planete;
	int flotteSelectionee;

	char precedente;
	int scroll;

	Error error;
	int errorCountDown;
	int commandPrompt;
	
	int nombreDeVaisseaux; // utilisé dans la création de flottes
	City *villes; // utilisé dans le changement de batiment
	int empire; // utilisé dans les relations
};

/* entry points ======================================================== */
Camera *camera_Alloc(){
	return calloc(1, sizeof(Camera));
}

void camera_XSet(Camera *camera, int x){
	camera->x = x;
	
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Set camera x position: %d\n", x);
	// #endif
}
void camera_XAdd(Camera *camera, int x){
	camera->x += x;
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Add camera x position: %d\n", x);
	// #endif
}
int camera_XGet(Camera *camera){
	return camera->x;
}
void camera_YSet(Camera *camera, int y){
	camera->y = y;
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Set camera y position: %d\n", y);
	// #endif
}
void camera_YAdd(Camera *camera, int y){
	camera->y += y;
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Add camera y position: %d\n", y);
	// #endif
}
int camera_YGet(Camera *camera){
	return camera->y;
}

void SetCameraXSystem(Camera *camera, int x){
	camera->xSysteme = x;
}
void AddCameraXSystem(Camera *camera, int x){
	camera->xSysteme += x;
}
int GetCameraXSystem(Camera *camera){
	return camera->xSysteme;
}
void SetCameraYSystem(Camera *camera, int y){
	camera->ySysteme = y;
}
void AddCameraYSystem(Camera *camera, int y){
	camera->ySysteme += y;
}
int GetCameraYSystem(Camera *camera){
	return camera->ySysteme;
}

void camera_XVectorSet(Camera *camera, int x){
	camera->xVector = x;
}
void camera_XVectorAdd(Camera *camera, int x){
	camera->xVector += x;
}
int camera_XVectorGet(Camera *camera){
	return camera->xVector;
}
void camera_YVectorSet(Camera *camera, int y){
	camera->yVector = y;
}
void camera_YVectorAdd(Camera *camera, int y){
	camera->yVector += y;
}
int camera_YVectorGet(Camera *camera){
	return camera->yVector;
}

void camera_ZoomSet(Camera *camera, int zoom){
	if(zoom > 3)
		return;
	if(zoom < 0)
		return;

	if(zoom < camera->zoom){
		camera_XSet(camera, camera_XGet(camera) * 0.5);
		camera_YSet(camera, camera_YGet(camera) * 0.5);
	} else {
		camera_XSet(camera, camera_XGet(camera) * 2);
		camera_YSet(camera, camera_YGet(camera) * 2);
	}
	camera->zoom = zoom;
}
int camera_ZoomGet(Camera *camera){
	return camera->zoom;
}

void SetCameraLock(Camera *camera, int lock){
	camera->lock = lock;
}
int GetCameraLockStatus(Camera *camera){
	return camera->lock;
}

void SetCameraMapType(Camera *camera, VueType mapType){
	camera->mapType = mapType;
}
VueType GetCameraMapType(Camera *camera){
	return camera->mapType;
}

void SetCameraViewedSystem(Camera *camera, int system){
	camera->systemeSelectione = system;
}
void SetCameraSystem(Camera *camera, int system){
	camera->systeme = system;
}
void SetCameraSystemViewStatus(Camera *camera, int status){
	switch (status) {
	case true:
		camera->mapType = VUE_SYSTEM;
		break;
	case false:
		camera->mapType = VUE_GALACTIC;
		break;
	}
	camera->systemeSelectione = camera->systeme;
}
int camera_SystemActualGet(Camera *camera){
	return camera->systemeSelectione;
}
int camera_SystemAimedGet(Camera *camera){
	return camera->systeme;
}

int IsCameraMoveFleet(Camera *camera){
	return camera->bougerFlotte;
}
void SetCameraMoveFleet(Camera *camera, int status){
	camera->bougerFlotte = status;
}

void SetCameraSelection(Camera *camera, int selection){
	camera->selection = selection;
}
int GetCameraSelection(Camera *camera){
	return camera->selection;
}

void SetCameraFleet(Camera *camera, int fleet){
	camera->flotte = fleet;
}
int GetCameraFleet(Camera *camera){
	return camera->flotte;
}

void SetCameraEmpire(Camera *camera, int empire){
	camera->empire = empire;
}
int GetCameraEmpire(Camera *camera){
	return camera->empire;
}

//fenetre
Window *AllocFenetre(){
	return calloc(1, sizeof(Window));
}

void OpenMenu(Window *fenetre, Camera *camera, ClassMenu classMenu, MenuSystem menuSysteme){
	fenetre->menu = classMenu;
	fenetre->menuDetails = menuSysteme;
	fenetre->selection = 1;
	camera->lock = true;
}
void CloseMenu(Window *fenetre, Camera *camera){
	fenetre->menu = MENU_AUCUN;
	fenetre->menuDetails = MENU_SYSTEME_AUCUN;
	camera->lock = false;
}
ClassMenu GetOpenedMenuClass(Window *fenetre){
	return fenetre->menu;
}
MenuSystem GetOpenedMenuDetails(Window *fenetre){
	return fenetre->menuDetails;
}

void OpenCommandPrompt(Window *fenetre, Camera *camera, Time *date){
	fenetre->commandPrompt = true;
	camera->lock = true;
	PauseGame(date);
}
void CloseCommandPrompt(Window *fenetre, Camera *camera, Time *date){
	fenetre->commandPrompt = false;
	camera->lock = false;
	UnpauseGame(date);
}
int GetCommandPromptStatus(Window *fenetre){
	return fenetre->commandPrompt;
}

void SetWindowPlanet(Window *fenetre, int planete){
	fenetre->planete = planete;
}
int GetWindowPlanet(Window *fenetre){
	return fenetre->planete;
}

void SetWindowSelection(Window *fenetre, int selection){
	fenetre->selection = selection;
}
int GetWindowSelection(Window *fenetre){
	return fenetre->selection;
}
void IncrementWindowSelection(Window *fenetre){
	fenetre->selection++;
}
void UnincrementWindowSelection(Window *fenetre){
	fenetre->selection--;
}
void AddWindowSelection(Window *fenetre, int number){
	fenetre->selection += number;
}

void SetWindowSelectedFleet(Window *fenetre, int fleet){
	fenetre->flotteSelectionee = fleet;
}
int GetWindowSelectedFleet(Window *fenetre){
	return fenetre->flotteSelectionee;
}

void SetWindowPrevious(Window *fenetre, int previous){
	fenetre->precedente = previous;
}
int GetWindowPrevious(Window *fenetre){
	return fenetre->precedente;
}

Error GetWindowError(Window *fenetre){
	return fenetre->error;
}
void SetWindowError(Window *fenetre, Error error){
	fenetre->error = error;
}
int GetWindowErrorCountDown(Window *fenetre){
	return fenetre->errorCountDown;
}
void UnincrementWindowErrorCountdown(Window *fenetre){
	fenetre->errorCountDown--;
}
void SetWindowErrorCountdown(Window *fenetre, int countdown){
	fenetre->errorCountDown = countdown;
}

void SetWindowCity(Window *fenetre, City *city){
	fenetre->villes = city;
}
City *GetWindowCity(Window *fenetre){
	return fenetre->villes;
}

void SetWindowEmpire(Window *fenetre, int empire){
	fenetre->empire = empire;
}
int GetWindowEmpire(Window *fenetre){
	return fenetre->empire;
}

void SetWindowScroll(Window *fenetre, int scroll){
	fenetre->scroll = scroll;
}
void AddWindowScroll(Window *fenetre, int scroll){
	fenetre->scroll += scroll;
}
int GetWindowScroll(Window *fenetre){
	return fenetre->scroll;
}

void SetWindowFleetSize(Window *fenetre, int size){
	fenetre->nombreDeVaisseaux = size;
}
void AddWindowFleetSize(Window *fenetre, int size){
	fenetre->nombreDeVaisseaux += size;
}
int GetWindowFleetSize(Window *fenetre){
	return fenetre->nombreDeVaisseaux;
}

void SetWindowMenuSystem(Window *fenetre, MenuSystem menu){
	fenetre->menuDetails = menu;
}