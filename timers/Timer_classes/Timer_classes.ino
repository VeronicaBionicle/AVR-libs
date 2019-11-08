#include "timer_class.h"

Timer counter1, timer1;
uint32_t last;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  //counter1.startCounter(1, SECS);
  timer1.startTimerForInterrupt(50);
  DDRB = 0xFF;  //for testing via oscilloscope
  timer1.attachInterrupt(Plus);
}

void Plus() {
  PORTD ^= _BV(PD7);  //exclusive or
};

void loop() {

  /*if (last != counter1.getCounterExp()) {
     last = counter1.getCounterExp();
     Serial.print("exp: ");
     Serial.println(counter1.getCounterExp());
     Serial.print("periods: ");
     Serial.println(counter1.getPeriods());
    }*/
  /*if (counter1.getCounterExp() >= 5) {
    counter1.stopTimerCounter();
    Serial.println("Stop!");
    };
  */
}
