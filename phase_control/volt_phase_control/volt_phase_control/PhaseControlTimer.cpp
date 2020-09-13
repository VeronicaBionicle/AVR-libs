#include "PhaseControlTimer.h"
#include "PhaseControl.h"

ISR(TIMER_INT) {
	if (((1<<ZEROCROSS_PIN) & PIND) == 0) On(); 
}

void InitPhaseControlTimer () {
  TCCR_A = TCCR_A_FOR_TIMER;
  TCCR_B = CS_FOR_4_PRESC;
  TCNT2 = 0;	
}

void StartPhaseControlTimer(uint16_t Period) {
  //cli();
  OCR_A = ((uint32_t)Period * F_CPU_SH) / PRESCALER_4 - 1;
  TCNT2 = 0;
  TIMSK_ = (1<<OCIE_A);
  //sei();
}

void StopPhaseControlTimer() {
	TIMSK_ &= ~(1<<OCIE_A);
}
