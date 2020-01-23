#ifndef smooth_on_off_h_
#define smooth_on_off_h_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define PWM_INCREMENT 1
#define ON 255
#define OFF 0

void SmoothSwitch(uint8_t switching_direction);
void On();
void Off();

#endif /*smooth_on_off_h_ */
