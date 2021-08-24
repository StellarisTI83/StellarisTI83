#include <stddef.h>

#include <stdlib.h>

#include <debug.h>

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
    int fps;
};

/* entry points ======================================================== */
Time *time_Alloc(){
    return calloc(1, sizeof(Time));
}

void time_DateSet(Time *time, char d, char m, int y){
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

    time->day = d;
    time->month = m;
    time->year = y;
}
int speed_TimeGet(Time *time){
    return time->speed;
}
void time_SpeedSet(Time *time, gameSpeed speed, int savedSpeed){
    time->speed = speed;
    time->previousSpeed = savedSpeed;
}
void SetTimeSpeedOnly(Time *time, gameSpeed speed){
    time->speed = speed;
}

void time_Pause(Time *time){
    time->previousSpeed = time->speed;
    time->speed = 0;
}
void time_Unpause(Time *time){
    time->speed = time->previousSpeed;
}

void AddTimeClock(Time *time){
    time->tick++;
}
int GetTimeClock(Time *time){
    return time->tick;
}
int GetTimeDay(Time *time){
    return time->day;
}
int GetTimeMonth(Time *time){
    return time->month;
}
int GetTimeYear(Time *time){
    return time->year;
}

void IncrementTime(Time *time){
    time->day++;
    if(time->day == 31){
        time->day = 1;
        time->month++;
    }
    if(time->month == 13){
        time->month = 1;
        time->year++;
    }
}

void time_SpeedIncrement(Time *time) {
    time->speed++;
    if(time->speed > TIME_SPEED_VERY_FAST)
        time->speed = TIME_SPEED_VERY_FAST;
}
void time_SpeedUnincrement(Time *time) {
    time->speed--;
    if(time->speed < TIME_SPEED_VERY_SLOW)
        time->speed = TIME_SPEED_VERY_SLOW;
}

void time_Update(Time *time) {
	switch (time->speed) {
		case TIME_SPEED_VERY_SLOW:
			time->tick ++;
			break;
		case TIME_SPEED_SLOW:
			time->tick += 2;
			break;
		case TIME_SPEED_NORMAL:
			time->tick += 3;
			break;
		case TIME_SPEED_FAST:
			time->tick += 4;
			break;
		case TIME_SPEED_VERY_FAST:
			time->tick += 6;
			break;
        default:
            break;
	}
    
    //protection contre le fait de faire plusieurs fois les actions calculées au 0 de l'horloge
	if (time->tick >= 24) {
		time->tick = 0;
	}
	if ((time->tick == 0) && (time->speed != TIME_SPEED_PAUSE)) {
		time->day++;
		if(time->day > 30) {
            time->day = 1;
            time->month++;
		}

		if(time->month > 12) {
            time->month = 1;
            time->year++;
		}
	}
}

void time_FPSSet(Time *time, long fps) {
    time->fps = fps;
}

long time_FPSGet(Time *time) {
    return time->fps;
}

