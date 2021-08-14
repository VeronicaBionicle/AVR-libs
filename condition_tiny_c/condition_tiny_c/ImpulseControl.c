#include "ImpulseControl.h"

volatile uint8_t zerocross;

ISR (EXT_INT){
	zerocross++;
}
/*
ImpulseControl::ImpulseControl (uint8_t output_pin, uint8_t maximum_periods){
	out_pin = output_pin;	
	max_periods = maximum_periods;
}*/

void OutputInit (ImpulseControl_t vent){
	OUT_DDR |= (1<< vent.out_pin);
}

void ZerocrossInit (void) {
	cli();
	EXTERNAL_INTERRUPT_PORT &= ~(1<<ZEROCROSS_PIN);
	INTERRUPT_INIT =(1<<ISC01) | (1<<ISC00);
	GENERAL_INTERRUPT = (1<<INT0);
	INTERRUPT_FLAG = (1<<INTF0);
	sei();
}


void On (ImpulseControl_t vent){	
	OUT_PORT |= (1 << vent.out_pin);
}

void Off (ImpulseControl_t vent){	
	OUT_PORT &= ~(1 << vent.out_pin);
}

void PhaseControl (ImpulseControl_t vent, uint8_t number_of_periods) {
vent.num_of_periods = number_of_periods;	
if (zerocross < vent.num_of_periods) {
	On(vent);
	} else if (zerocross < vent.max_periods) {Off(vent);} else zerocross = 0; 
}