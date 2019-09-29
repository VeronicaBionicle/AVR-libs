#ifndef timer_no_class_h_
#define timer_no_class_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

/*Choose your timer here*/
#define TIMER TIMER_0
//#define TIMER TIMER_1
//#define TIMER TIMER_2

#if TIMER == TIMER_0
    #define TIMER_INT TIMER0_COMPA_vect
#elif TIMER == TIMER_1
    #define TIMER_INT TIMER1_COMPA_vect
#elif TIMER == TIMER_2
    #define TIMER_INT TIMER2_COMPA_vect
#endif

#define MILLIS 1
#define SECONDS 975    //must be 1000
#define MINUTES 58500   //must be 60000, but my arduinos "minute" is 1 min + 1.5 sec
#define HOURS 3600000

typedef struct timer_struct
{
    uint64_t period, milliseconds, expirations;
} Timer;

void startTimer(Timer *, uint32_t Period, uint32_t Unit); //start timer with period in MILLIS, SECONDS, MINUTES, HOURS
void stopTimer(Timer *);
uint32_t getExp(Timer *);
uint32_t getMillis();
uint32_t getTime(Timer *, uint32_t Unit); // get time in MILLIS, SECONDS, MINUTES, HOURS

#endif /*timer_no_class_h_ */
