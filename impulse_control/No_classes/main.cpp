#include "ImpulseControl.h"

#define MAX_PERIODS 10

int main(void){
	cli();
	OutputInit(0);
	ZerocrossInit();
	sei();
	while(1){
		PhaseControl(MAX_PERIODS, 7);
	}
}
