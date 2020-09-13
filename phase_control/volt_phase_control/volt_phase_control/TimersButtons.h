#ifndef TIMERS_BUTTONS_H_
#define TIMERS_BUTTONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define F_CPU_SH 16 //F_CPU/1000000 i had some problems with overflow
/* Buttons */
#define DOOR_BELL (1<<PORTD3)
#define DOOR_1 (1<<PORTD4)
#define DOOR_2 (1<<PORTD5)
#define BUTTON_1 (1<<PORTD6)
#define BUTTON_2 (1<<PORTD7)
#define BUTTON_MASK (DOOR_BELL | DOOR_1 | DOOR_2 | BUTTON_1 | BUTTON_2)
#define READ_BUTTONS_TIME 15000UL //15 ms
/*  Some definitions for our timers */
#define CS_0 CS20
#define CS_1 CS21
#define CS_2 CS22
#define TIMERS_PRESCALER 1024
#define TCCR_A_FOR_TIMERS (1 << WGM21)
#define TIMERS_OCIE_A OCIE2A
#define CS_STOP ~(1<<CS_2)|(1<<CS_1)|(0<<CS_0)
/* Timer2 - for buttons */
#define PHASE_CONTROL_TIMER_INT TIMER2_COMPA_vect
#define PHASE_CONTROL_TIMER_TIMSK TIMSK2
#define PHASE_CONTROL_TIMER_TCCR_A TCCR2A
#define PHASE_CONTROL_TIMER_TCCR_B TCCR2B
#define PHASE_CONTROL_OCR OCR2A
#define PHASE_CONTROL_TIMER_CS (1<<CS_2)|(1<<CS_1)|(1<<CS_0)
/* Timer0 - for phase control */
#define BUTTON_TIMER_INT TIMER0_COMPA_vect
#define BUTTON_TIMER_TIMSK TIMSK0
#define BUTTON_TIMER_TCCR_A TCCR0A
#define BUTTON_TIMER_TCCR_B TCCR0B
#define BUTTON_TIMER_OCR OCR0A
#define BUTTON_TIMER_CS (1 << CS_2)|(1 << CS_0)

void InitTimersButtons(void);
void StartPhaseControlTimer(uint16_t Period);  //start timer for attaching function to ISR, period`s unit measure is microseconds
void StopPhaseControlTimer();  //stop counting and interrupts from Timer

#endif /* TIMERS_BUTTONS_H_ */