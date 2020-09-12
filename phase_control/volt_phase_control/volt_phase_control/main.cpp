#include "PhaseControl.h"
#include "PhaseControlTimer.h"
#include "timer_0_class.h"

uint8_t button_state;

/*# define F_CPU 16000000UL
#include <util/delay.h>*/

#define OUT_PIN 0

#define IS_ON (PINB & (1<<OUT_PIN))

void setup () {
	cli();
	InitPhaseControlTimer();
	OutputInit(OUT_PIN);
	ZerocrossInit();
	InitButtonTimer();
	sei();
}

uint8_t current_state = OFF;

int main(void){
	setup();
	while (1) {
		if (button_state == DOOR_BELL) {
			if (current_state != ON) { PhaseSmooth(ON); current_state = ON; }
			} else { if (current_state != OFF) { PhaseSmooth(OFF); current_state = OFF; } }
		/*
		PhaseSmooth(ON);
		_delay_ms(501);
		PhaseSmooth(OFF);
		_delay_ms(501);
		*/
	}
	
}
