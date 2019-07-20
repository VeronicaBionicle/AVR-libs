#ifndef timer_h_
#define timer_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define NUM_TIMERS 5
typedef volatile uint16_t Time;

void startTimer(uint8_t timer_num, uint16_t Period);
void stopTimer(uint8_t timer_num);
uint16_t getTime(uint8_t timer_num);

#endif /*timer_h_ */
