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

#include "ordres.h"
#include "main.h"

/* structures ========================================================== */
struct OrdreElement{
	int ordre;
	int info1; //numero du module / type de flotte / numero du batiment
	int info2; //type du module / nombre de vaisseaux dans la flotte
	int prix;
	int tempsTotal;
	int tempsActuel;
	int empire;
	Ordre* ordreSuivant;
};

struct OrdreFileStruct{
	Ordre* premierOrdre;
};

/* entry points ======================================================== */
OrdreFile *CreerFileOrdres(){
	OrdreFile *ordreFile = NULL;
	ordreFile = calloc(1, sizeof(OrdreFile));
    if(!ordreFile){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating order queue");
		#endif
        exit(EXIT_FAILURE);
    }
	ordreFile->premierOrdre = NULL;
	return ordreFile;
}

void SupprimerFileOrdres(OrdreFile *ordreFile){
	Ordre *ordre = NULL, *ordreSuivant = NULL;
	ordre = ordreFile->premierOrdre;
	while(ordre != NULL){
		ordreSuivant = ordre->ordreSuivant;
		free(ordre);
		ordre = ordreSuivant;
	}
	free(ordreFile);
}

void NouvelOrdre(OrdreFile* ordreFile, int ordre, int empire, int tempsTotal, int info1, int info2, int prix){
	Ordre *ordreElement = NULL;

	if(ordreFile->premierOrdre == NULL){
		ordreFile->premierOrdre = calloc(1, sizeof(Ordre));
		if(!ordreFile->premierOrdre){
			#ifdef DEBUG_VERSION
			dbg_sprintf(dbgerr, "Malloc returned NULL when creating order");
			#endif
			exit(EXIT_FAILURE);
		}
		ordreElement = ordreFile->premierOrdre;
	}
	else{
		ordreElement = ordreFile->premierOrdre;
		while(ordreElement->ordreSuivant != NULL){
			ordreElement = ordreElement->ordreSuivant;
		}
		ordreElement->ordreSuivant = calloc(1, sizeof(Ordre));
			if(!ordreFile->premierOrdre){
				#ifdef DEBUG_VERSION
				dbg_sprintf(dbgerr, "Malloc returned NULL when creating order");
				#endif
				exit(EXIT_FAILURE);
			}
		ordreElement = ordreElement->ordreSuivant;
	}

	ordreElement->ordre = ordre;
	ordreElement->tempsTotal = tempsTotal;
	ordreElement->tempsActuel = tempsTotal;
	ordreElement->info1 = info1;
	ordreElement->info2 = info2;
	ordreElement->prix = prix;
	ordreElement->empire = empire;
	ordreElement->ordreSuivant = NULL;
}

void FinirOrdre(OrdreFile *ordreFile){
	Ordre *ordre;
	ordre = ordreFile->premierOrdre;
	if(ordre != NULL){
		ordreFile->premierOrdre = ordre->ordreSuivant;
		free(ordre);
	}
}

Ordre* RecupererOrdre(OrdreFile *ordreFile){
	Ordre *ordre = NULL;
	ordre = ordreFile->premierOrdre;
	return ordre;
}

int NombredOrdres(OrdreFile *ordreFile){
	int nombre = 0;
	Ordre *ordre = NULL;
	ordre = ordreFile->premierOrdre;
	while(ordre != NULL){
		ordre = ordre->ordreSuivant;
		nombre++;
	}
	return(nombre);
}

int GetOrder(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->ordre;
	else 
		return 0;
}

int GetOrderEmpire(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->empire;
	else 
		return 0;
}

int GetOrderProgress(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->tempsActuel;
	else 
		return 0;
}

int GetOrderTotalTime(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->tempsTotal;
	else 
		return 0;
}

void UnincrementOrderProgress(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		RecupererOrdre(ordreFile)->tempsActuel--;
}

int GetOrderInfo1(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->info1;
	else
		return 0;
}
int GetOrderInfo2(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->info2;
	else
		return 0;
}

int GetOrderPrice(OrdreFile *ordreFile){
	if(RecupererOrdre(ordreFile) != NULL)
		return RecupererOrdre(ordreFile)->prix;
	else
		return 0;
}

int GetOrderProgressPourcent(Ordre *ordre){
	if(ordre != NULL)
		return ((ordre->tempsTotal - ordre->tempsActuel) * 100) / ordre->tempsTotal;
	else
		return 0;
}