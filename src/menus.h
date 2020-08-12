#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <fileioc.h>
#include "main.h"

#include "ai.h"
#include "camera.h"
#include "flottes.h"
#include "time.h"
#include "parametres.h"
#include "systemes.h"

int DrawMenu(EmpireListe *empireListe, Date *date, char *key, Camera *camera, SystemeStellaire **systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche);

#endif