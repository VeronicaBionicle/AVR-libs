#include "timer.h"  

Time timers[NUM_TIMERS];
Time milliseconds[NUM_TIMERS];
uint32_t periods[NUM_TIMERS];

ISR(TIMER1_COMPA_vect)
{
  for (int i=0; i < NUM_TIMERS; i++) {
    if (periods[i] != 0) {
      milliseconds[i]++;
    if (milliseconds[i] == periods[i]) {
      timers[i]++;
      milliseconds[i] = 0;
    }     
  }
 }
}

void startTimer(uint8_t timer_num, uint16_t Period,  uint16_t Unit){
  cli();
  TCCR1A = 0; TCCR1B = 0;
  TIMSK1 = 1<<OCIE1A;
  TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11); //CTC, 64 div
  OCR1A = 250;  //max period 0.26214 sec = (64)/(16 MHz) * 65535 / OCR1A = 250 -> 0.001 sec
  timers[timer_num] = 0;
  periods[timer_num] = Period*Unit;
  sei();

/* //Timer/Counter 0 initialization
TCCR0A=(1<<WGM01);
TCCR0B=(1<<CS01) | (1<<CS00);
OCR0A=0xF9;
TIMSK0=(1<<OCIE0A)
*/

/* //Timer/Counter 2 initialization
TCCR2A=(1<<WGM21);
TCCR2B=(1<<CS22);
TCNT2=0x00;
OCR2A=0xF9;
OCR2B=0x00;
TIMSK2=(1<<OCIE2A);
*/
};

void stopTimer(uint8_t timer_num){
  periods[timer_num]=0;
  };

uint32_t getTime(uint8_t timer_num, uint16_t Unit) {
  return timers[timer_num]/Unit;
};
