#ifndef H_TIME
#define H_TIME

#define TIMER_FREQ      32768 /* Frequency of timer in Hz */
#define ONE_SECOND      (TIMER_FREQ / 1)
#define HALF_SECOND     (TIMER_FREQ / 2)
#define QUARTER_SECOND  (TIMER_FREQ / 4)
#define CHANGE_TIME     (TIMER_FREQ / 1) * 1


#define DEBUG_FPS     

typedef enum {  TIME_SPEED_VERY_SLOW = -2,
                TIME_SPEED_SLOW,
                TIME_SPEED_PAUSE,
                TIME_SPEED_NORMAL,
                TIME_SPEED_FAST,
                TIME_SPEED_VERY_FAST} gameSpeed;

/* struct ============================================================== */

typedef struct DateStruct Time;

/* entry points ======================================================== */

Time *time_Alloc();

void time_DateSet(Time *date, char d, char m, int y);
void time_SpeedSet(Time *date, gameSpeed speed, gameSpeed savedSpeed);
void SetTimeSpeedOnly(Time *date, gameSpeed speed);
int speed_TimeGet(Time *date);
void time_Pause(Time *date);
void time_Unpause(Time *date);

void time_SpeedIncrement(Time *date);
void time_SpeedUnincrement(Time *date);

void time_TickIncrement(Time *date);
int time_TickGet(Time *date);
int time_DayGet(Time *date);
int time_MonthGet(Time *date);
int time_YearGet(Time *date);

void IncrementTime(Time *date);

void time_SpeedIncrement(Time *date);
void time_SpeedUnincrement(Time *date);

void time_Update(Time *date);

void time_FPSSet(Time *time, int fps, float updateTime, float screenTime);
int time_FPSGet(Time *time);
float time_UpdateTimeGet(Time *time);
float time_ScreenTimeGet(Time *time);


#endif