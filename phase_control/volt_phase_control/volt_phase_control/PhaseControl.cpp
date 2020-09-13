#include "PhaseControl.h"
#include "TimersButtons.h"

uint8_t zerocross;
uint8_t out_pin;
extern uint8_t triac_state;

#define MAX_STEPS 10

uint16_t deltas[MAX_STEPS] = {7000, 6500, 6000, 5500, 5000, 4500, 4000, 3500, 3000, 2000};

void OutputInit (uint8_t output_pin){
	out_pin = output_pin;
	OUT_DDR |= (1<<out_pin);
}

void On (void){ OUT_PORT |= (1<<out_pin); }

void Off (void){ OUT_PORT &= ~(1<<out_pin); }

void ZerocrossInit (void) {
	EXTERNAL_INTERRUPT_PORT &= ~(1<<ZEROCROSS_PIN);
	INTERRUPT_INIT |= (1<<ISC01) | (1<<ISC00);
	INTERRUPT_FLAG |= (1<<INTF0);
}

void ZerocrossStart (void) {
	zerocross = 0;
	GENERAL_INTERRUPT |= (1<<INT0);
}

void ZerocrossStop (void) { GENERAL_INTERRUPT &= ~(1<<INT0); }

ISR (EXT_INT){ PhaseControl(); }

void PhaseControl (void) {
	if (zerocross < MAX_STEPS) {
		Off();
		StartPhaseControlTimer(deltas[triac_state == TURNING_ON ? zerocross : MAX_STEPS-zerocross-1]);
		zerocross++;
		} else {
			ZerocrossStop();
			StopPhaseControlTimer();
			if (triac_state == TURNING_ON) { On(); triac_state = ON; } else { Off(); triac_state = OFF; }
	}
}

void PhaseSmooth (uint8_t final_state) {
	if (STEADY_ST(triac_state) && triac_state != final_state) {
		TOGGLE(triac_state);
		//cli();
		ZerocrossStart();
		//sei();
	}
}