#ifndef buttons_interrupter_h_
#define buttons_interrupter_h_
#include "step_motor_control.h"
#include "timer_0_class.h"  //timer for reading buttons

#define LIMIT_SWITCH 0b1000000

#define DOWN 0b11011111
#define UP 0b11101111
#define DRILL 0b11110111
#define PCB_DRILL 0b11111011
#define PCB_POINT 0b11111101
#define GO_HOME 0b11111110
#define BUTTON_MASK 0b00111111

#define SPEED_1 0b10111111
#define SPEED_2 0b01111111
#define ADD_BUTTON_MASK 0b11000000

#define NOTHING_PRESSED 0b11111111

#define READ_BUTTONS_TIME 15000 //15 ms

void ReadButtons();

#endif /*buttons_interrupter_h_ */
