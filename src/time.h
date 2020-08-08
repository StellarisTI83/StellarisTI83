#ifndef H_TIME
#define H_TIME

/* structures ========================================================== */
typedef struct DateStruct Date;

/* entry points ======================================================== */
Date *AllocDate();

void SetTime(Date *date, int d, int m, int y);
int GetTimeSpeed(Date *date);
void PauseGame(Date *date);
void UnpauseGame(Date *date);

void IncrementTimeSpeed(Date *date);
void UnincrementTimeSpeed(Date *date);

int GetTimeClock(Date *date);
int GetTimeDay(Date *date);
int GetTimeMonth(Date *date);
int GetTimeYear(Date *date);

void AddTime(Date *date, int d);

void SetTimeSpeed(Date *date, int speed, int savedSpeed);
#endif