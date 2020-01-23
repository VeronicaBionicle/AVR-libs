#include "timer_1_class.h"

volatile uint32_t isr_1_periods;

ISR(TIMER1_COMPA_vect)  //ISR for couners and Timer1
{
  isr_1_periods++;
  timer1.isrCallback();
}

/*  Group of functions for interrupts*/
void Timer1::isrDefaultUnused() {} 
void (*Timer1::isrCallback)() = Timer1::isrDefaultUnused;

void Timer1::startTimerForInterrupt(uint32_t Period) {
  cli();
  period = Period;
  isr_1_periods = 0;
  milliseconds = 0;
  expirations = 0;
  TIMSK1 = (1 << OCIE1A);
  TCCR1A = TCCR_A_FOR_TIMER_1;
  Timer1::setupTimer(Period);
  sei();
}

/*Group of functions for counters*/
void Timer1::startCounter(uint32_t Period, uint32_t Unit) {  //start counter with pediod from 1ms to 
  cli();
  period = Period * Unit;
  isr_1_periods = 0;
  milliseconds = 0;
  expirations = 0;
  TIMSK1 = (1<<OCIE1A);
  OCR1A = OCR_FOR_COUNTER_1;
  TCCR1B = CS_FOR_COUNTER_1;
  sei();
}

void Timer1::clearCounterExp() {
  expirations = 0;
}

uint32_t Timer1::getCounterExp() {
  if (period != 0) {
    if ((isr_1_periods - milliseconds) / period >= 1) {
      expirations = expirations + (isr_1_periods - milliseconds) / period;
      milliseconds = isr_1_periods;
    }
    return expirations;
  } else return 0;
}

void Timer1::stopTimerCounter() {
  Timer1::clearCounterExp();
  TCCR1B &= CS_STOP;
  TIMSK1 = 0;
  TCCR1A = 0;
  OCR1A = 0;
  period = 0;
  milliseconds = 0;
  expirations = 0;
}

uint32_t Timer1::getPeriods() {
  return isr_1_periods;
}

/*Service function for calculating Timer1 registers*/
void Timer1::setupTimer(uint32_t Period){
   if (Period <= OCR_1_MAX / F_CPU_SH) {
    TCCR1B = CS_FOR_NO_PRESC_1;
    OCR1A = (Period * F_CPU_SH) - 1;
  } else {
    if (Period <= PRESCALER_1 * OCR_1_MAX / F_CPU_SH) {
      TCCR1B = CS_FOR_1_PRESC_1;
      OCR1A = (Period * F_CPU_SH) / PRESCALER_1 - 1;
    } else {
      if (Period <= PRESCALER_2 * OCR_1_MAX / F_CPU_SH) {
        TCCR1B = CS_FOR_2_PRESC_1;
        OCR1A = (Period * F_CPU_SH) / PRESCALER_2 - 1;
      } else {
        if (Period <= PRESCALER_3 / F_CPU_SH * OCR_1_MAX) {
          TCCR1B = CS_FOR_3_PRESC_1;
          OCR1A = (Period * F_CPU_SH) / PRESCALER_3 - 1;
        } else {
          if (Period <= PRESCALER_4 / F_CPU_SH * OCR_1_MAX) {
            TCCR1B = CS_FOR_4_PRESC_1;
            OCR1A = (Period * F_CPU_SH) / PRESCALER_4 - 1;
          } else {
            TCCR1B = CS_FOR_4_PRESC_1;
            OCR1A = OCR_1_MAX - 1;
          };
        };
      };
    };
  };
};
