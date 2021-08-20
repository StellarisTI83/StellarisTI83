#ifndef _GALAXY_INCLUDE
#define _GALAXY_INCLUDE

#include "ai.h"
#include "camera.h"
#include "settings.h"
#include "systemes.h"

#define GALAXY_WIDTH    11

#define SPACE_BETWEEN_STARS     50
#define RANDOMIZE_COEFFICIENT   100

#define LIMIT_LEFT  180
#define LIMIT_TOP   160


#define STAR_TYPE_B_PROBABILITY         12
#define STAR_TYPE_A_PROBABILITY         15
#define STAR_TYPE_F_PROBABILITY         20
#define STAR_TYPE_G_PROBABILITY         20
#define STAR_TYPE_K_PROBABILITY         12
#define STAR_TYPE_M_PROBABILITY         8
#define STAR_TYPE_BLACKHOLE_PROBABILITY 5
#define STAR_TYPE_PULSAR_PROBABILITY    3
#define STAR_TYPE_NEUTRON_PROBABILITY   5

#define STAR_TOTAL_PROBABILITY          STAR_TYPE_B_PROBABILITY + STAR_TYPE_A_PROBABILITY + STAR_TYPE_F_PROBABILITY + STAR_TYPE_G_PROBABILITY + STAR_TYPE_K_PROBABILITY + STAR_TYPE_M_PROBABILITY + STAR_TYPE_BLACKHOLE_PROBABILITY + STAR_TYPE_PULSAR_PROBABILITY + STAR_TYPE_NEUTRON_PROBABILITY 

/* constants =========================================================== */
#define RADIUS_PLANET1 30
#define RADIUS_PLANET2 60
#define RADIUS_PLANET3 90
#define RADIUS_PLANET4 110
#define RADIUS_PLANET5 130

/**
 * @brief Function to create all empires
 * 
 * @param parametres 
 * @param empireListe 
 * @param starSystem 
 * @param camera 
 */
void galaxy_StartEmpiresInitialize( Settings *parametres, 
                                    EmpireList *empireListe, 
                                    StarSystem **starSystem, 
                                    Camera *camera);

/**
 * @brief Create a new galaxy in the sent aray
 * 
 * @param starSystem 
 */
void galaxy_CreateNew(StarSystem **starSystem);

#endif