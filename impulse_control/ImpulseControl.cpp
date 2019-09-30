#include "ImpulseControl.h"

uint8_t zerocross, outpin;

void OutputInit (void){
	OUT_DDR |= (1<<OUT_PIN); //DDRB, 2 for example
}

void ZerocrossInit (void) {
	if (INT_PIN == PORTD2) {
		DDRD &= ~(1<<PORTD2);  // PD2- вход переход через ноль INT0
		MCUCR=(1<<ISC01) | (1<<ISC00);
		GIMSK=(1<<INT0);
		GIFR=(1<<INTF0);
		} else {	//if PORTD3
		DDRD &= ~(1<<PORTD3);  // PD2- вход переход через ноль INT0
		MCUCR=(1<<ISC11) | (1<<ISC10);
		GIMSK=(1<<INT1);
		GIFR=(1<<INTF1);
	}
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

void PhaseControl (uint8_t num_of_periods) {	
if (zerocross < num_of_periods) {
	On();
	} else if (zerocross < MAX_PERIODS) {Off();} else zerocross = 0; 
}