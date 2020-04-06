#ifndef buttons_interrupter_h_
#define buttons_interrupter_h_
#include "step_motor_control.h"
#include "timer_0_class.h"  //timer for reading buttons

#define LIMIT_SWITCH 0b1000000
#define DOWN 0b00011111
#define UP 0b00101111
#define DRILL 0b00110111
#define PCB_DRILL 0b00111011
#define PCB_POINT 0b00111101
#define GO_HOME 0b00111110
#define BUTTON_MASK 0b00111111
#define READ_BUTTONS_TIME 15000 //15 ms

void ReadButtons();

#endif /*buttons_interrupter_h_ */
