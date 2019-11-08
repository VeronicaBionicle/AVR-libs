#include "timer_class.h"

Timer timer1;
uint32_t last;
void setup() {
  timer1.startTimer(10, MINUTES);
Serial.begin(9600);
  DDRB = 0xFF;  //for testing via oscilloscope
 // timer1.attachInterrupt(Plus);
}

void Plus() {
  PORTD ^= _BV(PD7);  //exclusive or
};

void loop() {
 if (last != timer1.getExp()) {
    last = timer1.getExp();
    Serial.print("exp: ");
    Serial.println(timer1.getExp());
    Serial.print("periods: ");
    Serial.println(timer1.getPeriods());
  }
}
