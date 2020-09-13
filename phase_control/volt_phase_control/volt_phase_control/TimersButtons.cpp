#include "PhaseControl.h"
#include "TimersButtons.h"

extern uint8_t button_state;

ISR(PHASE_CONTROL_TIMER_INT) { if (((1<<ZEROCROSS_PIN) & PIND) == 0) On(); }
	
ISR(BUTTON_TIMER_INT) { button_state = (PIND & BUTTON_MASK); }

void StartPhaseControlTimer(uint16_t Period) {
	//cli();
	PHASE_CONTROL_OCR = ((uint32_t)Period * F_CPU_SH) / TIMERS_PRESCALER - 1;
	TCNT2 = 0;
	PHASE_CONTROL_TIMER_TIMSK = (1<<TIMERS_OCIE_A);
	//sei();
}

void StopPhaseControlTimer() { PHASE_CONTROL_TIMER_TIMSK &= ~(1<<TIMERS_OCIE_A); }

void InitTimersButtons(void) {
	//cli();
	DDRD &= ~(BUTTON_MASK);
	
	PHASE_CONTROL_TIMER_TCCR_A = TCCR_A_FOR_TIMERS;
	PHASE_CONTROL_TIMER_TCCR_B = PHASE_CONTROL_TIMER_CS;
	TCNT2 = 0;
	
	BUTTON_TIMER_TCCR_A = TCCR_A_FOR_TIMERS;
	BUTTON_TIMER_TCCR_B = BUTTON_TIMER_CS;
	BUTTON_TIMER_OCR = ((uint32_t)READ_BUTTONS_TIME * F_CPU_SH) / TIMERS_PRESCALER - 1;
	BUTTON_TIMER_TIMSK = (1 << TIMERS_OCIE_A);
	TCNT0 = 0;
	//sei();
}