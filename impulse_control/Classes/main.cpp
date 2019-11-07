#include "ImpulseControl.h"

#define OUT_PIN 0
#define MAX_PERIODS 10

int main(void){
	ImpulseControl pin1(OUT_PIN, MAX_PERIODS);
	ImpulseControl pin2(2, MAX_PERIODS);
	cli();
	pin1.OutputInit();
	pin1.ZerocrossInit();
		pin2.OutputInit();
		pin2.ZerocrossInit();
	sei();
	while(1){
		pin1.PhaseControl(5);
		pin2.PhaseControl(3);
	}
}
