#ifndef buttons_interrupter_h_
#define buttons_interrupter_h_

#include "timer_0_class.h"  //timer for reading buttons

#define DOOR_BELL (1<<PORTD3)
#define DOOR_1 (1<<PORTD4)
#define DOOR_2 (1<<PORTD5)
#define BUTTON_1 (1<<PORTD6)
#define BUTTON_2 (1<<PORTD7)

#define BUTTON_MASK (DOOR_BELL | DOOR_1 | DOOR_2 | BUTTON_1 | BUTTON_2)

#define READ_BUTTONS_TIME 15000 //15 ms

#endif /*buttons_interrupter_h_ */
