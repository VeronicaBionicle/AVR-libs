#include "PhaseControl.h"
#include "PhaseControlTimer.h"

# define F_CPU 16000000UL
#include <util/delay.h>

#define OUT_PIN 0

void setup () {
	InitPhaseControlTimer();
	OutputInit(OUT_PIN);
	ZerocrossInit();
}

int main(void){
	setup();
	while (1) {
		PhaseSmooth(ON);
		_delay_ms(500);
		PhaseSmooth(OFF);
		_delay_ms(500);
	}
}
