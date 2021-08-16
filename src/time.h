#ifndef H_TIME
#define H_TIME

typedef enum {  TIME_SPEED_VERY_SLOW = -2,
                TIME_SPEED_SLOW,
                TIME_SPEED_PAUSE,
                TIME_SPEED_NORMAL,
                TIME_SPEED_FAST,
                TIME_SPEED_VERY_FAST} gameSpeed;

/* structures ========================================================== */

typedef struct DateStruct Time;

/* entry points ======================================================== */

Time *AllocDate();

void SetTime(Time *date, char d, char m, int y);
void SetTimeSpeed(Time *date, gameSpeed speed, gameSpeed savedSpeed);
void SetTimeSpeedOnly(Time *date, gameSpeed speed);
int GetTimeSpeed(Time *date);
void PauseGame(Time *date);
void UnpauseGame(Time *date);

void IncrementTimeSpeed(Time *date);
void UnincrementTimeSpeed(Time *date);

void AddTimeClock(Time *date);
int GetTimeClock(Time *date);
int GetTimeDay(Time *date);
int GetTimeMonth(Time *date);
int GetTimeYear(Time *date);

void IncrementTime(Time *date);

void IncrementTimeSpeed(Time *date);
void UnincrementTimeSpeed(Time *date);

void UpdateClock(Time *date);



#endif