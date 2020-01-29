#include "timer_class.h"

ISR(TIMER_INT)  //ISR for couners and timer
{
  Timer1.isrCallback();
}

/*  Group of functions for interrupts*/
void Timer::isrDefaultUnused() {} 
void (*Timer::isrCallback)() = Timer::isrDefaultUnused;

void Timer::startTimerForInterrupt(uint16_t Period) {
  cli();
  period = Period;
  TIMSK_ = (1 << OCIE_A);
  TCCR_A = TCCR_A_FOR_TIMER;
  Timer::setupTimer(Period);
  sei();
}

void Timer::stopTimerCounter() {
  TCCR_B &= CS_STOP;
  TIMSK_ = 0;
  TCCR_A = 0;
  OCR_A = 0;
  period = 0;
}


/*Service function for calculating timer registers*/
void Timer::setupTimer(uint16_t Period){
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
