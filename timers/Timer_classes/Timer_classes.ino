#include "timer_class.h"

Timer timer1;

void setup() {
  timer1.startTimer(15000);

  DDRB = 0xFF;  //for testing via oscilloscope
  timer1.attachInterrupt(Plus);
}

void Plus() {
  PORTD ^= _BV(PD7);  //exclusive or
};

void loop() {
}
