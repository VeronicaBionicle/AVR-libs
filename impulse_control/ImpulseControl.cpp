#include "ImpulseControl.h"

uint8_t zerocross;

void OutputInit (void){
	OUT_DDR |= (1<<OUT_PIN);
}

void ZerocrossInit (void) {
		EXTERNAL_INTERRUPT_PORT &= ~(1<<ZEROCROSS_PIN);
		INTERRUPT_INIT =(1<<ISC01) | (1<<ISC00);
		GENERAL_INTERRUPT = (1<<INT0);
		INTERRUPT_FLAG=(1<<INTF0);
}

ISR (EXT_INT){
	zerocross++;
}

void On (void){	
	OUT_PORT |= (1<<OUT_PIN);
}

void Off (void){	
	OUT_PORT &= ~(1<<OUT_PIN);
}

void PhaseControl (uint8_t max_periods, uint8_t num_of_periods) {	
if (zerocross < num_of_periods) {
	On();
	} else if (zerocross < max_periods) {Off();} else zerocross = 0; 
}