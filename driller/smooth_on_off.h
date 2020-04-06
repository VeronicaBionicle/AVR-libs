#ifndef smooth_on_off_h_
#define smooth_on_off_h_

#include <avr/interrupt.h>

#define PWM_INCREMENT 1
#define ON 255

void DrillSmoothOn();
void DrillOn();
void DrillOff();
void DrillPWM(uint8_t duty_cycle);
void DrillSetMaxDuty(uint8_t new_max_duty);

#endif /*smooth_on_off_h_ */
