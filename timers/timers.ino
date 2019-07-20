#include "timer.h"

Time past[5];

void setup() {
Serial.begin(9600);
pinMode(13, OUTPUT);
startTimer(0, 10, MILLIS);
startTimer(1, 10, MILLIS);
startTimer(2, 100, MILLIS);
startTimer(3, 250, MILLIS);
}

void loop() {
  for (int i=0; i<5; i++) {
    // if (past[i] < getTime(i, MILLIS)) {Serial.print(i); Serial.print("timer "); Serial.println(getTime(i, MILLIS)); past[i] = getTime(i, MILLIS);};
     if (past[i] < getTime(i, MILLIS)) {
       digitalWrite(8+i, !digitalRead(8+i));
       past[i] = getTime(i, MILLIS);
     }
    }
    if (getTime(3, MILLIS) == 50) {stopTimer(2);}
} 
