#include "PhaseControl.h"
#include "timer_0_class.h"

uint8_t zerocross;
Timer0 phase_timer;

void OutputInit (uint8_t out_pin){
	OUT_DDR |= (1<<out_pin);
}

void ZerocrossInit (void) {
	EXTERNAL_INTERRUPT_PORT &= ~(1<<ZEROCROSS_PIN);
	INTERRUPT_INIT = (1<<ISC01) | (1<<ISC00);
	GENERAL_INTERRUPT = (1<<INT0);
	INTERRUPT_FLAG = (1<<INTF0);
}

void ZerocrossStop (void) {
	INTERRUPT_INIT ^= (1<<ISC01) | (1<<ISC00);
	GENERAL_INTERRUPT ^= (1<<INT0);
	INTERRUPT_FLAG ^= (1<<INTF0);
}

ISR (EXT_INT){
	zerocross++;
	if (zerocross < MAX_DELTAS) {
		Off();
		phase_timer.attachTimerInterrupt(On, DELTA_T*(MAX_DELTAS - zerocross));
	} else {
		zerocross = 0;
		ZerocrossStop(); //stop interrupt
		On();
	}
}

void On (void){	
	OUT_PORT |= (1<<OUT_PIN);
	phase_timer.detachTimerInterrupt();
}

void Off (void){	
	OUT_PORT &= ~(1<<OUT_PIN);
}

void PhaseControlSetup (void) {
	cli();
	OutputInit(0);
	ZerocrossInit();
	sei();
}

void PhaseControl (uint8_t max_periods, uint8_t num_of_periods) {	
	if (zerocross < num_of_periods) {
		Off();
		phase_timer.attachTimerInterrupt(On, DELTA_T*(MAX_DELTAS - zerocross));  //set_timer(); 
	} else {
		zerocross = 0;
		ZerocrossStop();//stop interrupt
		On();
	} 
}