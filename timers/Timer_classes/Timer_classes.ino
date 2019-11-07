#include "timer_class.h"

Timer timer1(1, SECS);
uint64_t last = 0;
uint32_t some = 0;
void setup() {
Serial.begin(9600);
timer1.startTimer();
Serial.println("Start");
}

void Plus(){
  Serial.print("some: ");
  };

void loop() {
  timer1.attachInterrupt(Plus);
 
if (last != timer1.getExp()){
Serial.print("exp: ");
Serial.println(timer1.getExp());
last = timer1.getExp();
}


} 
