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

#include "camera.h"
#include "main.h"
#include "planetes.h"

struct CameraStruct{
	int x;
	int y;
	int xSysteme;
	int ySysteme;
	int xVector;
	int yVector;

	char zoom;
	VueType mapType;
	bool lock;

	int systemeSelectione; // systeme dans lequel on est
	int systeme; // systeme que la camera pointe

	int selection;
	int ouverte;

	bool bougerFlotte;
	int flotte;
	int empire;
};

struct FenetreStruct{
	ClassMenu menu;
	MenuSysteme menuSysteme;
	int selection;

	int planete;
	int flotteSelectionee;

	char precedente;
	int scroll;

	Error error;
	int errorCountDown;
	bool commandPrompt;
	
	int nombreDeVaisseaux; // utilisé dans la création de flottes
	Villes *villes; // utilisé dans le chanement de batiment
};

/* entry points ======================================================== */
void SetCameraX(Camera *camera, int x){
	camera->x = x;
}
int getCameraX(Camera *camera){
	return camera->x;
}
void SetCameraY(Camera *camera, int y){
	camera->y = y;
}
int getCameraY(Camera *camera){
	return camera->y;
}

void SetCameraXSystem(Camera *camera, int x){
	camera->xSysteme = x;
}
int getCameraXSystem(Camera *camera){
	return camera->xSysteme;
}
void SetCameraYSystem(Camera *camera, int y){
	camera->ySysteme = y;
}
int getCameraYSystem(Camera *camera){
	return camera->ySysteme;
}

void SetCameraXVector(Camera *camera, int x){
	camera->xVector = x;
}
int getCameraXVector(Camera *camera){
	return camera->xVector;
}
void SetCameraYVector(Camera *camera, int y){
	camera->yVector = y;
}
int getCameraYVector(Camera *camera){
	return camera->yVector;
}

void SetCameraZoom(Camera *camera, int zoom){
	camera->zoom = zoom;
}
int GetCameraZoom(Camera *camera){
	return camera->zoom;
}

void SetCameraMapType(Camera *camera, VueType mapType){
	camera->mapType = mapType;
}
VueType GetCameraMapType(Camera *camera){
	return camera->mapType;
}

void SetCameraLock(Camera *camera, bool lock){
	camera->lock = lock;
}
bool GetCameraLock(Camera *camera){
	return camera->lock;
}

void OpenMenu(Fenetre *fenetre, Camera *camera, ClassMenu classMenu, MenuSysteme menuSysteme){
	fenetre->menu = classMenu;
	fenetre->menuSysteme = menuSysteme;
	camera->lock = true;
}
void CloseMenu(Fenetre *fenetre, Camera *camera){
	fenetre->menu = MENU_AUCUN;
	fenetre->menuSysteme = MENU_AUCUN;
	camera->lock = false;
}

void OpenCommandPrompt(Fenetre *fenetre){
	fenetre->commandPrompt = true;
}
void CloseCommandPrompt(Fenetre *fenetre){
	fenetre->commandPrompt = false;
}
bool GetCommandPromptStatus(Fenetre *fenetre){
	return fenetre->commandPrompt;
}