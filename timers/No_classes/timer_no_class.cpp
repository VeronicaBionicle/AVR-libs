#include "timer_no_class.h"  

volatile uint64_t isr_milliseconds;

ISR(TIMER_INT)
{
isr_milliseconds++;
}

    //Start function
   void startTimer(Timer *t, uint32_t Period, uint32_t Unit){
  cli();
  t->period = Period*Unit;
  isr_milliseconds = 0;
  t->milliseconds = 0;
  t->expirations = 0;
  switch(TIMER){
    case TIMER_0:
    TIMSK0 = (1<<OCIE0A);
    TCCR0A=(1<<WGM01);
    TCCR0B=(1<<CS01) | (1<<CS00);
    OCR0A=0xF9;
    break;
    case TIMER_1:
    TIMSK1 = (1<<OCIE1A);
    TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11); //CTC, 64 div
    OCR1A = 250;  //max period 0.26214 sec = (64)/(16 MHz) * 65535 / OCR1A = 250 -> 0.001 sec
    break;
    case TIMER_2:
    TIMSK2=(1<<OCIE2A);
    TCCR2A=(1<<WGM21);
    TCCR2B=(1<<CS22);
    TCNT2=0x00;
    OCR2A=0xF9;
    break;
  }
  sei();
}

void stopTimer(Timer *t){
  cli();

  t->period = 0;
  t->milliseconds = 0;
  t->expirations = 0;

  switch(TIMER){
    case TIMER_0:
      TIMSK0 = 0;
      TCCR0A = TCCR0B = 0;
      OCR0A=0;
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

uint32_t getExp(Timer *t) {
  if (t->period != 0) {
    if (( isr_milliseconds-(t->milliseconds))/(t->period) >= 1){
    t->expirations = t->expirations + (isr_milliseconds-(t->milliseconds))/(t->period);
    t->milliseconds = isr_milliseconds;
    }
    return t->expirations; 
  } else return 0;
}

uint32_t getMillis() {
  return isr_milliseconds;
}

uint32_t getTime(Timer *t, uint32_t Unit) {
  return getExp(t)*(t->period)/Unit;
}
