#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <fileioc.h>
#include "main.h"

#include "ai.h"
#include "camera.h"
#include "fleet.h"
#include "time.h"
#include "settings.h"
#include "systemes.h"

int DrawMenu(EmpireListe *empireListe, Time *date, char *key, Camera *camera, StarSystem **systemeStellaires, Window *fenetre, Settings *parametres, Market *marche);

#endif