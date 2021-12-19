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



/* struct ============================================================== */

typedef struct CameraStruct Camera;

#include "main.h"
#include "planet.h"
#include "time.h"
#include "menus.h"

/* entry points ======================================================== */

/**
 * @brief Create a new camera
 * 
 * @return Camera* 
 */
Camera *camera_Create();

// Update the camera position

void camera_Update(Camera *camera);

// Functions to set the x and y positions in map vue

void    camera_XSet             (Camera *camera, int x);
void    camera_XAdd             (Camera *camera, int x);
int     camera_XGet             (Camera *camera);
void    camera_YSet             (Camera *camera, int y);
void    camera_YAdd             (Camera *camera, int y);
int     camera_YGet             (Camera *camera);

// Functions to set the x and y positions in system vue

void    camera_XSystemSet       (Camera *camera, int x);
void    camera_XSystemAdd       (Camera *camera, int x);
int     camera_XSystemGet       (Camera *camera);
void    camera_YsystemSet       (Camera *camera, int y);
void    camera_YSystemAdd       (Camera *camera, int y);
int     camera_YSystemGet       (Camera *camera);

// Functions to set the x and y vectors
// they are the same for system and map vue, and are used to make the movements
// smooth

void    camera_XVectorSet       (Camera *camera, int x);
void    camera_XVectorAdd       (Camera *camera, int x);
int     camera_XVectorGet       (Camera *camera);
void    camera_YVectorSet       (Camera *camera, int y);
void    camera_YVectorAdd       (Camera *camera, int y);
int     camera_YVectorGet       (Camera *camera);

// Functions about the zoom

void    camera_ZoomSet          (Camera *camera, int zoom);
int     camera_ZoomGet          (Camera *camera);

// Functions to change the vue

void    camera_MapTypeSet       (Camera *camera, VueType MapType);
VueType camera_MapTypeGet       (Camera *camera);

// Functions to lock or unlock moving the camera

void    camera_LockSet          (Camera *camera, int lock);
int     camera_LockGet          (Camera *camera);

// Functions to set the aimed system in map vue

void    camera_SystemActualSet  (Camera *camera, int system);
int     camera_SystemActualGet  (Camera *camera);

// Functions to set the viewed system in system vue

void    camera_SystemAimedSet   (Camera *camera, int system);
int     camera_SystemAimedGet   (Camera *camera);

// Function to set and get if the player is actually moving a fleet or not

void    camera_FleetMoveSet     (Camera *camera, int status);
int     camera_FleetMoveGet     (Camera *camera);

// Functions to set and get the fleet the player is moving a fleet

void    camera_FleetSet         (Camera *camera, int fleet);
int     camera_FleetGet         (Camera *camera);

#endif