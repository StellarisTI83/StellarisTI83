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

#include "time.h"

/* structures ========================================================== */
struct DateStruct{
	char vitesse;
	char vitesseSauvegardee; //vitesse sauvegardé pour quand on passe de pause à action
	char jour;
	char mois;
	int annee;
	int horloge;
};

/* entry points ======================================================== */
Date *AllocDate(){
    return calloc(1, sizeof(Date));
}

void SetTime(Date *date, int d, int m, int y){
    if(d > 30)
        d = 30;
    if(d < 1)
        d = 1;

    if(m > 12)
        m = 12;
    if(m < 1)
        m = 1;
    
    if(y > 3000)
        y = 3000;
    if(y < 2200)
        y = 2200;

    date->jour = d;
    date->mois = m;
    date->annee = y;
}
int GetTimeSpeed(Date *date){
    return date->vitesse;
}

void PauseGame(Date *date){
    date->vitesseSauvegardee = date->vitesse;
    date->vitesse = 0;
}
void UnpauseGame(Date *date){
    date->vitesse = date->vitesseSauvegardee;
}

int GetTimeClock(Date *date){
    return date->horloge;
}
int GetTimeDay(Date *date){
    return date->jour;
}
int GetTimeMonth(Date *date){
    return date->mois;
}
int GetTimeYear(Date *date){
    return date->annee;
}

void AddTime(Date *date, int d){
    date->jour++;
    if(date->jour == 31){
        date->jour = 1;
        date->mois++;
    }
    if(date->mois == 13){
        date->mois = 1;
        date->annee++;
    }
}

void IncrementTimeSpeed(Date *date) {
    date->vitesse++;
    if(date->vitesse > 3)
        date->vitesse = 3;
}
void UnincrementTimeSpeed(Date *date) {
    date->vitesse--;
    if(date->vitesse < -2)
        date->vitesse = -2;
}

void UpdateClock(Date *date) {
	switch (date->vitesse) {
		case -2:
			date->horloge ++;
			break;
		case -1:
			date->horloge += 2;
			break;
		case 1:
			date->horloge += 3;
			break;
		case 2:
			date->horloge += 4;
			break;
		case 3:
			date->horloge += 6;
			break;
	}
	if (date->horloge >= 24) {
		date->horloge = 0;
	}
	if ((date->horloge == 0) && (date->vitesse !=0)) {
		date->jour++;
		if(date->jour > 30) {
            date->jour = 1;
            date->mois++;
		}

		if(date->mois > 12) {
            date->mois = 1;
            date->annee++;
		}
	}
}


void AddClock(Date *date){
    date->horloge++;
}
int GetClock(Date *date){
    return date->horloge;
}