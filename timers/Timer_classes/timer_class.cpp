#include "timer_class.h"

volatile uint64_t isr_milliseconds;

void Timer::isrDefaultUnused() {}
void (*Timer::isrCallback)() = Timer::isrDefaultUnused;


ISR(TIMER_INT)
{
  isr_milliseconds++;
  Timer1.isrCallback();
}

//Constructor
Timer::Timer() { }


//Start function
void Timer::startTimer(uint32_t Period) {
  cli();
  isr_milliseconds = 0;
  milliseconds = 0;
  expirations = 0;
  switch (TIMER) {
    case TIMER_0:
      TIMSK0 = (1 << OCIE0A);
      TCCR0A = (1 << WGM01);

      if (Period <= OCR_MAX / F_CPU_SH) {
        TCCR0B = (1 << CS00);
        OCR0A = (Period * F_CPU_SH) - 1;
      } else {
        if (Period <= PRESCALER_1 * OCR_MAX / F_CPU_SH) {
          TCCR0B = (1 << CS01);
          OCR0A = (Period * F_CPU_SH) / PRESCALER_1 - 1;
        } else {
          if (Period <= PRESCALER_2 * OCR_MAX / F_CPU_SH) {
            TCCR0B = (1 << CS01) | (1 << CS00);
            OCR0A = (Period * F_CPU_SH) / PRESCALER_2 - 1;
          } else {
            if (Period <= PRESCALER_3 / F_CPU_SH * OCR_MAX) {
              TCCR0B = (1 << CS02);
              OCR0A = (Period * F_CPU_SH) / PRESCALER_3 - 1;
            } else {
              if (Period <= PRESCALER_4 / F_CPU_SH * OCR_MAX) {
                TCCR0B = (1 << CS02) | (1 << CS00);
                OCR0A = (Period * F_CPU_SH) / PRESCALER_4 - 1;
              } else {
                TCCR0B = (1 << CS02) | (1 << CS00);
                OCR0A = OCR_MAX - 1;
              };
            };
          };
        };
      };
      break;
    case TIMER_1:
      TIMSK1 = (1 << OCIE1A);
      TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS11); //CTC, 64 div
      OCR1A = 250;  //max period 0.26214 sec = (64)/(16 MHz) * 65535 / OCR1A = 250 -> 0.001 sec
      break;
    case TIMER_2:
      TIMSK2 = (1 << OCIE2A);
      TCCR2A = (1 << WGM21);
      TCCR2B = (1 << CS22);
      TCNT2 = 0x00;
      OCR2A = 0xF9;
      break;
  }
  sei();
}

void Timer::stopTimer() {
  cli();

  period = 0;
  milliseconds = 0;
  expirations = 0;

  switch (TIMER) {
    case TIMER_0:
      TIMSK0 = 0;
      TCCR0A = TCCR0B = 0;
      OCR0A = 0;
      break;
    case TIMER_1:
      TIMSK1 = 0;
      TCCR1A = TCCR1B = 0;
      OCR1A = 0;
      break;
    case TIMER_2:
      TIMSK2 = 0;
      TCCR2A = TCCR2B = 0;
      OCR2A = 0;
      break;
  }
}

void Timer::clearExp() {
  expirations = 0;
}

uint32_t Timer::getExp() {
  if (period != 0) {
    if ((isr_milliseconds - milliseconds) / period >= 1) {
      expirations = expirations + (isr_milliseconds - milliseconds) / period;
      milliseconds = isr_milliseconds;
    }
    return expirations;
  } else return 0;
}

uint32_t Timer::getMillis() {
  return isr_milliseconds;
}

uint32_t Timer::getPeriods(uint32_t Unit) {
  return getExp() * period / Unit;
}
