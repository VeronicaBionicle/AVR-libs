#ifndef smooth_on_off_h_
#define smooth_on_off_h_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define PWM_INCREMENT 1
#define ON 255

void DrillSmoothOn();
void DrillOn();
void DrillOff();
void DrillSetDuty(uint8_t duty_cycle);

#endif /*smooth_on_off_h_ */
