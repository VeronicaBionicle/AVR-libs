#include "timer_class.h"

volatile uint32_t isr_periods;

ISR(TIMER_INT)  //ISR for couners and timer
{
  isr_periods++;
  Timer1.isrCallback();
}

/*  Group of functions for interrupts*/
void Timer::isrDefaultUnused() {} 
void (*Timer::isrCallback)() = Timer::isrDefaultUnused;

void Timer::startTimerForInterrupt(uint32_t Period) {
  cli();
  period = Period;
  isr_periods = 0;
  milliseconds = 0;
  expirations = 0;
  TIMSK_ = (1 << OCIE_A);
  TCCR_A = TCCR_A_FOR_TIMER;
  Timer::setupTimer(Period);
  sei();
}

/*Group of functions for counters*/
void Timer::startCounter(uint32_t Period, uint32_t Unit) {  //start counter with pediod from 1ms to 
  cli();
  period = Period * Unit;
  isr_periods = 0;
  milliseconds = 0;
  expirations = 0;
  TIMSK_ = (1<<OCIE_A);
  OCR_A = OCR_FOR_COUNTER;
  TCCR_B = CS_FOR_COUNTER;
  sei();
}

void Timer::clearCounterExp() {
  expirations = 0;
}

uint32_t Timer::getCounterExp() {
  if (period != 0) {
    if ((isr_periods - milliseconds) / period >= 1) {
      expirations = expirations + (isr_periods - milliseconds) / period;
      milliseconds = isr_periods;
    }
    return expirations;
  } else return 0;
}

void Timer::stopTimerCounter() {
  Timer::clearCounterExp();
  TCCR_B &= CS_STOP;
  TIMSK_ = 0;
  TCCR_A = 0;
  OCR_A = 0;
  period = 0;
  milliseconds = 0;
  expirations = 0;
}

uint32_t Timer::getPeriods() {
  return isr_periods;
}

/*Service function for calculating timer registers*/
void Timer::setupTimer(uint32_t Period){
   if (Period <= OCR_MAX / F_CPU_SH) {
    TCCR_B = CS_FOR_NO_PRESC;
    OCR_A = (Period * F_CPU_SH) - 1;
  } else {
    if (Period <= PRESCALER_1 * OCR_MAX / F_CPU_SH) {
      TCCR_B = CS_FOR_1_PRESC;
      OCR_A = (Period * F_CPU_SH) / PRESCALER_1 - 1;
    } else {
      if (Period <= PRESCALER_2 * OCR_MAX / F_CPU_SH) {
        TCCR_B = CS_FOR_2_PRESC;
        OCR_A = (Period * F_CPU_SH) / PRESCALER_2 - 1;
      } else {
        if (Period <= PRESCALER_3 / F_CPU_SH * OCR_MAX) {
          TCCR_B = CS_FOR_3_PRESC;
          OCR_A = (Period * F_CPU_SH) / PRESCALER_3 - 1;
        } else {
          if (Period <= PRESCALER_4 / F_CPU_SH * OCR_MAX) {
            TCCR_B = CS_FOR_4_PRESC;
            OCR_A = (Period * F_CPU_SH) / PRESCALER_4 - 1;
          } else {
            TCCR_B = CS_FOR_4_PRESC;
            OCR_A = OCR_MAX - 1;
          };
        };
      };
    };
  };
};
