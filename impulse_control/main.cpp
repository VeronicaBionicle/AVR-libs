#include "ImpulseControl.h"

int main(void){
	cli();
	OutputInit();
	ZerocrossInit();
	sei();
	while(1){
		PhaseControl(30);
	}
}
