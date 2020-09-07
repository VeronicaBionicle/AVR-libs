#include "PhaseControl.h"
#include "timer_class.h"

#define OUT_PIN 0

int main(void){
	PhaseControlSetup(OUT_PIN);
	while (1) ;
}
