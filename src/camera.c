/**
 * @file camera.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief Camera management and movements of the camera and window management
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright GNU General Public License v3.0
 * 
 */

#include <debug.h>
#include <assert.h>

#include "main.h"

#include "ai.h"
#include "camera.h"

/* struct ============================================================== */
struct CameraStruct{
    int         x;
    int         y;
    int         xSysteme;
    int         ySysteme;
    int         xVector;
    int         yVector;

    char        zoom;
    VueType     mapType;
    int         lock;

    int         systemeSelectione; // systeme dans lequel on est
    int         systeme; // systeme que la camera pointe

    int         ouverte;

    int         bougerFlotte;
    int         flotte;
};

/* entry points ======================================================== */

Camera *camera_Create(){
    Camera *camera = calloc_count(1, sizeof(Camera));
    if(!camera){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "Malloc returned NULL when creating camera");
        #endif
        assert(camera);
    }
    return camera;
}

// Update the camera position

void camera_Update(Camera *camera) {
    assert(camera);
    if(camera->xVector){
        camera->x += camera->xVector;
        if(camera->xVector > 0) 
            camera->xVector--;
        else
            camera->xVector++;
    }
    if(camera->yVector){
        camera->y += camera->yVector;
        if(camera->yVector > 0) 
            camera->yVector--;
        else
            camera->yVector++;
    }
}

// Functions to set the x and y positions in map vue

void camera_XSet(Camera *camera, int x){
    assert(camera);
    camera->x = x;

}
void camera_XAdd(Camera *camera, int x){
    assert(camera);
    camera->x += x;
}
int camera_XGet(Camera *camera){
    assert(camera);
    return camera->x;
}
void camera_YSet(Camera *camera, int y){
    assert(camera);
    camera->y = y;
}
void camera_YAdd(Camera *camera, int y){
    assert(camera);
    camera->y += y;
}
int camera_YGet(Camera *camera){
    assert(camera);
    return camera->y;
}

// Functions to set the x and y positions in system vue

void camera_XSystemSet(Camera *camera, int x){
    assert(camera);
    camera->xSysteme = x;
}
void camera_XSystemAdd(Camera *camera, int x){
    assert(camera);
    camera->xSysteme += x;
}
int camera_XSystemGet(Camera *camera){
    assert(camera);
    return camera->xSysteme;
}
void camera_YsystemSet(Camera *camera, int y){
    assert(camera);
    camera->ySysteme = y;
}
void camera_YSystemAdd(Camera *camera, int y){
    assert(camera);
    camera->ySysteme += y;
}
int camera_YSystemGet(Camera *camera){
    assert(camera);
    return camera->ySysteme;
}

// Functions to set the x and y vectors
// they are the same for system and map vue, and are used to make the movements
// smooth

void camera_XVectorSet(Camera *camera, int x){
    assert(camera);
    camera->xVector = x;
}
void camera_XVectorAdd(Camera *camera, int x){
    assert(camera);
    camera->xVector += x;
}
int camera_XVectorGet(Camera *camera){
    assert(camera);
    return camera->xVector;
}
void camera_YVectorSet(Camera *camera, int y){
    assert(camera);
    camera->yVector = y;
}
void camera_YVectorAdd(Camera *camera, int y){
    assert(camera);
    camera->yVector += y;
}
int camera_YVectorGet(Camera *camera){
    assert(camera);
    return camera->yVector;
}

// Functions about the zoom

void camera_ZoomSet(Camera *camera, int zoom){
    assert(camera);
    if(zoom > ZOOM_MAX)
        return;
    if(zoom < ZOOM_MIN)
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
    assert(camera);
    return camera->zoom;
}

// Functions to change the vue

void camera_MapTypeSet(Camera *camera, VueType mapType){
    assert(camera);
    camera->mapType = mapType;
}
VueType camera_MapTypeGet(Camera *camera){
    assert(camera);
    return camera->mapType;
}

// Functions to lock or unlock moving the camera

void camera_LockSet(Camera *camera, int lock){
    assert(camera);
    camera->lock = lock;
}
int camera_LockGet(Camera *camera){
    assert(camera);
    return camera->lock;
}

// Functions to set the aimed system in map vue

void camera_SystemAimedSet(Camera *camera, int system){
    assert(camera);
    camera->systemeSelectione = system;
}
int camera_SystemActualGet(Camera *camera){
    assert(camera);
    return camera->systemeSelectione;
}

// Functions to set the viewed system in system vue

void camera_SystemActualSet(Camera *camera, int system){
    assert(camera);
    camera->systeme = system;
    if(camera_SystemAimedGet(camera) != camera_SystemActualGet(camera)){
        camera_XSystemSet(camera, SYSTEM_MIDDLE_X);
        camera_YsystemSet(camera, SYSTEM_MIDDLE_Y);
    }   
}
int camera_SystemAimedGet(Camera *camera){
    assert(camera);
    return camera->systeme;
}

// Function to set and get if the player is actually moving a fleet or not

int camera_FleetMoveGet(Camera *camera){
    assert(camera);
    return camera->bougerFlotte;
}
void camera_FleetMoveSet(Camera *camera, int status){
    assert(camera);
    camera->bougerFlotte = status;
}

// Functions to set and get the fleet the player is moving a fleet

void camera_FleetSet(Camera *camera, int fleet){
    assert(camera);
    camera->flotte = fleet;
}
int camera_FleetGet(Camera *camera){
    assert(camera);
    return camera->flotte;
}