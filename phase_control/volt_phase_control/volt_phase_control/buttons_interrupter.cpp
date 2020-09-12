#include "buttons_interrupter.h"

extern uint8_t button_state;

void ReadButtons() {  //attach to Timer0 to read buttons state
  button_state = (PIND & BUTTON_MASK);
};
