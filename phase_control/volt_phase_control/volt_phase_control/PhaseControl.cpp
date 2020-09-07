#include "PhaseControl.h"
#include "timer_class.h"

uint8_t zerocross;
uint8_t out_pin;

#define MAX_DELTAS 10

uint16_t deltas[MAX_DELTAS] = {7000, 6500, 6000, 5500, 5000, 4500, 4000, 3500, 3000, 2000};

void OutputInit (uint8_t output_pin){
	out_pin = output_pin;
	OUT_DDR |= (1<<out_pin);
}

void On (void){
	OUT_PORT |= (1<<out_pin);
}

void Off (void){
	OUT_PORT &= ~(1<<out_pin);
}

void ZerocrossInit (void) {
	zerocross = 0;
	EXTERNAL_INTERRUPT_PORT &= ~(1<<ZEROCROSS_PIN);
	INTERRUPT_INIT |= (1<<ISC01) | (1<<ISC00);
	GENERAL_INTERRUPT |= (1<<INT0);
	INTERRUPT_FLAG |= (1<<INTF0);
}

void ZerocrossStop (void) {
	INTERRUPT_INIT &= ~(1<<ISC01) | (1<<ISC00);
	GENERAL_INTERRUPT &= ~(1<<INT0);
	INTERRUPT_FLAG &= ~(1<<INTF0);
}

ISR (EXT_INT){
	if (zerocross < MAX_DELTAS) {
		Off();
		startTimer(deltas[zerocross]);
		zerocross++;
	} else {  
		
		/*//smooth on
		ZerocrossStop();
		zerocross = 0;
		stopTimer();
		On();
		*/
		 //Debugging
		if (zerocross != 2*MAX_DELTAS) {
			stopTimer();
			Off();
			zerocross++;
		} else zerocross = 0;
		
	} 
}

void PhaseControlSetup (uint8_t output_pin) {
	cli();
	initTimer();
	OutputInit(output_pin);
	ZerocrossInit();
	sei();
}