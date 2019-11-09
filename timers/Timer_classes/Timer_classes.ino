#include "timer_class.h"

Timer counter1, timer1;
uint32_t last;

void setup() {
/*  part for testing interrupt attaching  */
/* DDRB = 0xFF;  //for testing via oscilloscope
  timer1.attachTimerInterrupt(Plus, 10000);*/
  
/*  part for testing counters  */
  Serial.begin(9600);
  Serial.println("Start");
  counter1.startCounter(10, SECONDS);
}

void Plus() {
  PORTD ^= _BV(PD7);  //exclusive or
};

void Minus() {
  PORTD ^= _BV(PD7);  //exclusive or
};

void loop() {
  /*  part for testing interrupt attaching  */ 
  /*delay(1000);
  timer1.detachTimerInterrupt();
  delay(1000);
  timer1.attachTimerInterrupt(Minus, 30000);*/
 /* part for testing counter*/
     if (last != counter1.getCounterExp()) {
     last = counter1.getCounterExp();
     Serial.print("exp: ");
     Serial.println(counter1.getCounterExp());
     Serial.print("periods: ");
     Serial.println(counter1.getPeriods());
    }
  if (counter1.getCounterExp() >= 5) {
    counter1.stopTimerCounter();
    Serial.println("Stop!");
    delay(1000);
    Serial.println("Start again!");
    counter1.startCounter(10, SECONDS);
    };
}
