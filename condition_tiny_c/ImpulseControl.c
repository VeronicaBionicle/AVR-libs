#include "ImpulseControl.h"

volatile uint8_t zerocross;

ISR (EXT_INT){
	zerocross++;
}

ImpulseControl::ImpulseControl (uint8_t output_pin, uint8_t maximum_periods){
	out_pin = output_pin;	
	max_periods = maximum_periods;
}

void ImpulseControl::OutputInit (void){
	OUT_DDR |= (1<<out_pin);
}

void ImpulseControl::ZerocrossInit (void) {
	cli();
	EXTERNAL_INTERRUPT_PORT &= ~(1<<ZEROCROSS_PIN);
	INTERRUPT_INIT =(1<<ISC01) | (1<<ISC00);
	GENERAL_INTERRUPT = (1<<INT0);
	INTERRUPT_FLAG=(1<<INTF0);
	sei();
}



void ImpulseControl::On (void){	
	OUT_PORT |= (1<<out_pin);
}

void ImpulseControl::Off (void){	
	OUT_PORT &= ~(1<<out_pin);
}

void ImpulseControl::PhaseControl (uint8_t number_of_periods) {
num_of_periods = number_of_periods;	
if (zerocross < num_of_periods) {
	On();
	} else if (zerocross < max_periods) {Off();} else zerocross = 0; 
}