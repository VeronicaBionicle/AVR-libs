#include "timer_class.h"
#include "PhaseControl.h"

ISR(TIMER_INT) { if (PIND != (1<<ZEROCROSS_PIN)) On();  }

void initTimer () {
  TCCR_A = TCCR_A_FOR_TIMER;
  TCCR_B = CS_FOR_4_PRESC;
  TCNT2 = 0;	
}

void startTimer(uint16_t Period) {
  cli();
  OCR_A = ((uint32_t)Period * F_CPU_SH) / PRESCALER_4 - 1;
  TCNT2 = 0;
  TIMSK_ = (1<<OCIE_A);
  sei();
}

void stopTimer() { TIMSK_ &= ~(1<<OCIE_A); }
