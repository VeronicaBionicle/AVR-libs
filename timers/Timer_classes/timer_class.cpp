#include "timer_class.h"  

volatile uint64_t isr_milliseconds;

ISR(TIMER1_COMPA_vect)
{
isr_milliseconds++;
}

 //Constructor
 Timer::Timer(uint32_t Period,  uint32_t Unit){
        period = Period*Unit;
        milliseconds = 0;
        expirations = 0;
    }


    //Start function
   void Timer::startTimer(uint8_t Timer_name){
  cli();
  isr_milliseconds = 0;
  milliseconds = 0;
  expirations = 0;
  switch(Timer_name){
    case TIMER_0:
    TIMSK0 = (1<<OCIE0A);
    TCCR0A=(1<<WGM01);
    TCCR0B=(1<<CS01) | (1<<CS00);
    OCR0A=0xF9;
    break;
    case TIMER_1:
 // TCCR1A = 0; TCCR1B = 0;
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

void Timer::stopTimer(){
  cli();

  period = 0;
  milliseconds = 0;
  expirations = 0;
  TIMSK0 = 0;
  TCCR0A = TCCR0B = 0;
  OCR0A=0;

  TIMSK1 = 0;
  TCCR1A = TCCR1B = 0;
  OCR1A = 0;

  TIMSK2 = 0;
  TCCR2A = TCCR2B = 0;
  OCR2A = 0;
  }

void Timer::clearExp() {
  expirations = 0;
}

uint32_t Timer::getExp() {
  if (period != 0) {
    if ((isr_milliseconds-milliseconds)/period >= 1){
    expirations = expirations + (isr_milliseconds-milliseconds)/period;
    milliseconds = isr_milliseconds;
    }
    return expirations; 
  } else return 0;
}

uint32_t Timer::getMillis() {
  return isr_milliseconds;
}

uint32_t Timer::getPeriods(uint32_t Unit) {
  return getExp()*period/Unit;
}



