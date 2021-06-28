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
	Villes *villes; // utilisé dans le chanement de batiment
};

/* entry points ======================================================== */
Camera *AllocCamera(){
	return calloc(1, sizeof(Camera));
}

void SetCameraX(Camera *camera, int x){
	camera->x = x;
	
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Set camera x position: %d\n", x);
	// #endif
}
void AddCameraX(Camera *camera, int x){
	camera->x += x;
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Add camera x position: %d\n", x);
	// #endif
}
int GetCameraX(Camera *camera){
	return camera->x;
}
void SetCameraY(Camera *camera, int y){
	camera->y = y;
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Set camera y position: %d\n", y);
	// #endif
}
void AddCameraY(Camera *camera, int y){
	camera->y += y;
	// #ifdef DEBUG_VERSION
	// 	dbg_sprintf(dbgout, "Add camera y position: %d\n", y);
	// #endif
}
int GetCameraY(Camera *camera){
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

void SetCameraXVector(Camera *camera, int x){
	camera->xVector = x;
}
void AddCameraXVector(Camera *camera, int x){
	camera->xVector += x;
}
int GetCameraXVector(Camera *camera){
	return camera->xVector;
}
void SetCameraYVector(Camera *camera, int y){
	camera->yVector = y;
}
void AddCameraYVector(Camera *camera, int y){
	camera->yVector += y;
}
int GetCameraYVector(Camera *camera){
	return camera->yVector;
}

void SetCameraZoom(Camera *camera, int zoom){
	camera->zoom = zoom;
}
int GetCameraZoom(Camera *camera){
	return camera->zoom;
}

void SetCameraLock(Camera *camera, int lock){
	camera->lock = lock;
}
int GetCameraLock(Camera *camera){
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
		camera->mapType = SYSTEME;
		break;
	case false:
		camera->mapType = NORMAL;
	}
	camera->systemeSelectione = camera->systeme;
}
int GetCameraViewedSystem(Camera *camera){
	return camera->systemeSelectione;
}
int GetCameraSystem(Camera *camera){
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
Fenetre *AllocFenetre(){
	return calloc(1, sizeof(Fenetre));
}

void OpenMenu(Fenetre *fenetre, Camera *camera, ClassMenu classMenu, MenuSystem menuSysteme){
	fenetre->menu = classMenu;
	fenetre->menuDetails = menuSysteme;
	fenetre->selection = 1;
	camera->lock = true;
}
void CloseMenu(Fenetre *fenetre, Camera *camera){
	fenetre->menu = MENU_AUCUN;
	fenetre->menuDetails = MENU_AUCUN;
	camera->lock = false;
}
ClassMenu GetOpenedMenuClass(Fenetre *fenetre){
	return fenetre->menu;
}
MenuSystem GetOpenedMenuDetails(Fenetre *fenetre){
	return fenetre->menuDetails;
}

void OpenCommandPrompt(Fenetre *fenetre, Camera *camera, Date *date){
	fenetre->commandPrompt = true;
	camera->lock = true;
	PauseGame(date);
}
void CloseCommandPrompt(Fenetre *fenetre, Camera *camera, Date *date){
	fenetre->commandPrompt = false;
	camera->lock = false;
	UnpauseGame(date);
}
int GetCommandPromptStatus(Fenetre *fenetre){
	return fenetre->commandPrompt;
}

void SetWindowPlanet(Fenetre *fenetre, int planete){
	fenetre->planete = planete;
}
int GetWindowPlanet(Fenetre *fenetre){
	return fenetre->planete;
}

void SetWindowSelection(Fenetre *fenetre, int selection){
	fenetre->selection = selection;
}
int GetWindowSelection(Fenetre *fenetre){
	return fenetre->selection;
}
void IncrementWindowSelection(Fenetre *fenetre){
	fenetre->selection++;
}
void UnincrementWindowSelection(Fenetre *fenetre){
	fenetre->selection--;
}
void AddWindowSelection(Fenetre *fenetre, int number){
	fenetre->selection += number;
}

void SetWindowSelectedFleet(Fenetre *fenetre, int fleet){
	fenetre->flotteSelectionee = fleet;
}
int GetWindowSelectedFleet(Fenetre *fenetre){
	return fenetre->flotteSelectionee;
}

void SetWindowPrevious(Fenetre *fenetre, int previous){
	fenetre->precedente = previous;
}
int GetWindowPrevious(Fenetre *fenetre){
	return fenetre->precedente;
}

Error GetWindowError(Fenetre *fenetre){
	return fenetre->error;
}
void SetWindowError(Fenetre *fenetre, Error error){
	fenetre->error = error;
}
int GetWindowErrorCountDown(Fenetre *fenetre){
	return fenetre->errorCountDown;
}
void UnincrementWindowErrorCountdown(Fenetre *fenetre){
	fenetre->errorCountDown--;
}
void SetWindowErrorCountdown(Fenetre *fenetre, int countdown){
	fenetre->errorCountDown = countdown;
}

void SetWindowCity(Fenetre *fenetre, Villes *city){
	fenetre->villes = city;
}
Villes *GetWindowCity(Fenetre *fenetre){
	return fenetre->villes;
}

void SetWindowScroll(Fenetre *fenetre, int scroll){
	fenetre->scroll = scroll;
}
void AddWindowScroll(Fenetre *fenetre, int scroll){
	fenetre->scroll += scroll;
}
int GetWindowScroll(Fenetre *fenetre){
	return fenetre->scroll;
}

void SetWindowFleetSize(Fenetre *fenetre, int size){
	fenetre->nombreDeVaisseaux = size;
}
void AddWindowFleetSize(Fenetre *fenetre, int size){
	fenetre->nombreDeVaisseaux += size;
}
int GetWindowFleetSize(Fenetre *fenetre){
	return fenetre->nombreDeVaisseaux;
}

void SetWindowMenuSystem(Fenetre *fenetre, MenuSystem menu){
	fenetre->menuDetails = menu;
}