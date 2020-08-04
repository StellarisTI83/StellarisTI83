/********************************************
*											*
*  Fichier pour la gestion des sauvegardes  *
*											*
********************************************/

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

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include "gfx/gfx.h"

#include "main.h"
#include "boucle.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"
#include "flottes.h"
#include "console.h"
#include "ai.h"

#include "locale/locale.h"

/******************chargement ancienne partie******************/
int ChargementAnciennePartie(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre, Marche *marche)
{
	ti_var_t sauvegarde;
	int i = 0, j = 0, compteur = 0, compteurFlottes = 0;
	Flotte* flotte = NULL, *flottePrecedente = NULL;
	Empire* empire = NULL, *empirePrecedent = NULL;
	gfx_FillScreen(255);
	PrintCentered("Chargement de la partie..." ,60 ,1 , 0, 0);
	/*ouvrir sauvegarde*/
	ti_CloseAll();
	sauvegarde = ti_Open("sauv", "r+");
	if (sauvegarde == 0)
	{
		return 1;
	}
	ti_Read(parametres, sizeof(Parametres), 1, sauvegarde);
	ti_Read(date, sizeof(Date), 1, sauvegarde);
	ti_Read(camera, sizeof(Camera), 1, sauvegarde);
	ti_Read(marche, sizeof(Marche), 1, sauvegarde);
	
	i = 0;
	
	//ourvir
	/*while(i <= LARGEUR_GALAXIE * LARGEUR_GALAXIE)
	{
		ti_Read(&systemeStellaires[i], sizeof(SystemeStellaire), 1, sauvegarde);
		
		if(systemeStellaires[i].nombrePlanetes >= 1)
		{
			systemeStellaires[i].planete1 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete1, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 2)
		{
			systemeStellaires[i].planete2 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete2, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 3)
		{
			systemeStellaires[i].planete3 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete3, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 4)
		{
			systemeStellaires[i].planete4 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete4, sizeof(Planete), 1, sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes == 5)
		{
			systemeStellaires[i].planete5 = (Planete*)malloc(sizeof(Planete));
			ti_Read(systemeStellaires[i].planete5, sizeof(Planete), 1, sauvegarde);
		}
		i++;
	}*/
	ti_Read(&compteur, sizeof(int), 1, sauvegarde);

	i = 0;
	while(i < compteur){
		empire = EmpireAjouter(empireListe);
		ti_Read(empire, sizeof(Empire), 1, sauvegarde);
		empire->flotte = FlotteListeCreer();
		j = 0;
		compteurFlottes = 0;
		ti_Read(&compteurFlottes, sizeof(int), 1, sauvegarde);
		while(j < compteurFlottes){
			flotte = FlotteAjouter(empire->flotte);
			ti_Read(flotte, sizeof(flotte), 1, sauvegarde);
			j++;
		}
		i++;
	}
	joueur = EmpireNumero(empireListe, 1);

	

	PrintCentered(joueur->nom ,90 ,1 , 0, 0);
	camera->fenetre = MENU_AUCUN;
	camera->bloque = FALSE;
	StellarisBoucle(&sauvegarde, empireListe, joueur, parametres, date, systemeStellaires, camera, fenetre, marche);
	return 0;
}

/**
 *Crée les sauvegardes du jeu
 */
int StellarisSauvegarde(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, Marche *marche)
{
	int i = 0;
	Flotte* flotte = NULL, *flottePrecedente = NULL;
	Empire* empire = NULL, *empirePrecedent = NULL;
	int compteur = 0;
	ti_CloseAll();
	*sauvegarde = ti_Open("sauv", "w");
	ti_Write(parametres, sizeof(Parametres), 1, *sauvegarde);
	ti_Write(date, sizeof(Date), 1, *sauvegarde);
	ti_Write(camera, sizeof(Camera), 1, *sauvegarde);
	ti_Write(marche, sizeof(Marche), 1, *sauvegarde);
	i = 0;
	
	//enregistrer
	/*while(i <= LARGEUR_GALAXIE * LARGEUR_GALAXIE)
	{
		ti_Write(&systemeStellaires[i], sizeof(SystemeStellaire), 1, *sauvegarde);
		
		if(systemeStellaires[i].nombrePlanetes >= 1)
		{
			ti_Write(systemeStellaires[i].planete1, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 2)
		{
			ti_Write(systemeStellaires[i].planete2, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 3)
		{
			ti_Write(systemeStellaires[i].planete3, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes >= 4)
		{
			ti_Write(systemeStellaires[i].planete4, sizeof(Planete), 1, *sauvegarde);
		}
		if(systemeStellaires[i].nombrePlanetes == 5)
		{
			ti_Write(systemeStellaires[i].planete5, sizeof(Planete), 1, *sauvegarde);
		}
		i++;
	}*/
	
	compteur = 0;

	empire = empireListe->premier;
	while(empire != NULL) {
		compteur++;
		empire = empire->suivant;
	}

	ti_Write(&compteur, sizeof(int), 1, *sauvegarde);

	empire = empireListe->premier;
	while(empire != NULL) {
		ti_Write(empire, sizeof(Empire), 1, *sauvegarde);
		
		//enregistrer flottes
		compteur = 0;
		flotte = empire->flotte->premier;
		while(flotte != NULL) {
			compteur++;
			flotte = flotte->suivant;
		}
		ti_Write(&compteur, sizeof(int), 1, *sauvegarde);
		flotte = empire->flotte->premier;
		while(flotte != NULL) {
			ti_Write(flotte, sizeof(Flotte), 1, *sauvegarde);
			flotte = flotte->suivant;
		}

		empire = empire->suivant;
	}
	
	return 1;
}

