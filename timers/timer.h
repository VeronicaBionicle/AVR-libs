#ifndef timer_h_
#define timer_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define NUM_TIMERS 5

#define MILLIS 1
#define SECS 1000
#define MINUTES 60*SECS
#define HOURS 60*MINUTES

typedef volatile uint64_t Time;

void startTimer(uint8_t timer_num, uint16_t Period, uint32_t Unit);
void stopTimer(uint8_t timer_num);
uint64_t getTime(uint8_t timer_num, uint32_t Unit);

#endif /*timer_h_ */
