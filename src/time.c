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
	gameSpeed speed;
	gameSpeed previousSpeed; //vitesse sauvegardé pour quand on passe de pause à action
	char day;
	char month;
	int year;
	int tick;
};

/* entry points ======================================================== */
Time *AllocDate(){
    return calloc(1, sizeof(Time));
}

void SetTime(Time *date, char d, char m, int y){
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

    date->day = d;
    date->month = m;
    date->year = y;
}
int GetTimeSpeed(Time *date){
    return date->speed;
}
void SetTimeSpeed(Time *date, gameSpeed speed, int savedSpeed){
    date->speed = speed;
    date->previousSpeed = savedSpeed;
}
void SetTimeSpeedOnly(Time *date, gameSpeed speed){
    date->speed = speed;
}

void PauseGame(Time *date){
    date->previousSpeed = date->speed;
    date->speed = 0;
}
void UnpauseGame(Time *date){
    date->speed = date->previousSpeed;
}

void AddTimeClock(Time *date){
    date->tick++;
}
int GetTimeClock(Time *date){
    return date->tick;
}
int GetTimeDay(Time *date){
    return date->day;
}
int GetTimeMonth(Time *date){
    return date->month;
}
int GetTimeYear(Time *date){
    return date->year;
}

void IncrementTime(Time *date){
    date->day++;
    if(date->day == 31){
        date->day = 1;
        date->month++;
    }
    if(date->month == 13){
        date->month = 1;
        date->year++;
    }
}

void IncrementTimeSpeed(Time *date) {
    date->speed++;
    if(date->speed > TIME_SPEED_VERY_FAST)
        date->speed = TIME_SPEED_VERY_FAST;
}
void UnincrementTimeSpeed(Time *date) {
    date->speed--;
    if(date->speed < TIME_SPEED_VERY_SLOW)
        date->speed = TIME_SPEED_VERY_SLOW;
}

void UpdateClock(Time *date) {
	switch (date->speed) {
		case TIME_SPEED_VERY_SLOW:
			date->tick ++;
			break;
		case TIME_SPEED_SLOW:
			date->tick += 2;
			break;
		case TIME_SPEED_NORMAL:
			date->tick += 3;
			break;
		case TIME_SPEED_FAST:
			date->tick += 4;
			break;
		case TIME_SPEED_VERY_FAST:
			date->tick += 6;
			break;
        default:
            break;
	}
    
    //protection contre le fait de faire plusieurs fois les actions calculées au 0 de l'horloge
	if (date->tick >= 24) {
		date->tick = 0;
	}
	if ((date->tick == 0) && (date->speed != TIME_SPEED_PAUSE)) {
		date->day++;
		if(date->day > 30) {
            date->day = 1;
            date->month++;
		}

		if(date->month > 12) {
            date->month = 1;
            date->year++;
		}
	}
}

