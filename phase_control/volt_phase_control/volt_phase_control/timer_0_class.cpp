#include "timer_0_class.h"
#include "PhaseControlTimer.h"

extern uint8_t button_state;

ISR(BUTTON_TIMER_INT) {
  button_state = (PINC & BUTTON_MASK);
}

void InitButtonTimer(void){
	//cli();
	DDRC = 0;
	BUTTON_TIMER_TIMSK = (1 << BUTTON_TIMER_OCIE_A);
	BUTTON_TIMER_TCCR_A = BUTTON_TIMER_TCCR_A_FOR_TIMER;
	BUTTON_TIMER_TCCR_B = (1 << BUTTON_TIMER_CS_2)|(1 << BUTTON_TIMER_CS_0);
	BUTTON_TIMER_OCR_A = ((uint32_t)READ_BUTTONS_TIME * F_CPU_SH) / PRESCALER_4 - 1;
	//sei();
};
