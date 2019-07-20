#include "timer.h"

uint16_t past[5];

void setup() {
Serial.begin(9600);
pinMode(13, OUTPUT);
startTimer(0, 10);
startTimer(1, 10);
startTimer(2, 100);
startTimer(3, 250);
}

void loop() {
  for (int i=0; i<5; i++) {
     if (past[i] < getTime(i)) {Serial.print(i); Serial.print("timer "); Serial.println(getTime(i)); past[i] = getTime(i);};
     if (past[i] < getTime(i)) {digitalWrite(8+i, !digitalRead(8+i)); past[i] = getTime(i);
     };
    };
    if (getTime(3) == 50) {stopAll();};
} 
