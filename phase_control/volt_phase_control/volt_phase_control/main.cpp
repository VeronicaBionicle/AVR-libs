#include "PhaseControl.h"
#include "TimersButtons.h"

uint8_t button_state;
uint8_t triac_state = OFF;

# define F_CPU 16000000UL

#define OUT_PIN 0

void setup () {
	cli();
	InitTimersButtons();
	OutputInit(OUT_PIN);
	ZerocrossInit();
	sei();
}

int main(void){
	setup();
	while (1) {
		if (button_state & (DOOR_BELL | BUTTON_1 | BUTTON_2)) PhaseSmooth(ON); else PhaseSmooth(OFF);
	}
	
}
