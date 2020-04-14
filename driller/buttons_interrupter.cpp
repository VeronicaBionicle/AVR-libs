#include "buttons_interrupter.h"

extern uint8_t button_state;

ISR (INT0_vect) 	//interrupt for limit switch
{
  StepperStop(); //stop stepper movement
  /* Detach INT0 interrupt */
  EIMSK = 0; 
  EICRA = 0;
  EIFR = 0;
}

void ReadButtons() {  //attach to Timer0 to read buttons state
  button_state = (PINC & BUTTON_MASK) | (PIND & ADD_BUTTON_MASK);
};
