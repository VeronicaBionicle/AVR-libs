#include "timer_no_class.h"

Timer timer1, timer2;
uint64_t last1 = 0;
uint64_t last2 = 0;
void setup() {
  Serial.begin(9600);
  startTimer(&timer1, 1, MINUTES);
  startTimer(&timer2, 10, SECONDS);
  Serial.println("Start");
}

void loop() {
    if (last2 != getExp(&timer2)) {
    last2 = getExp(&timer2);
    Serial.print("2 exp: ");
    Serial.println(getExp(&timer2));
    Serial.print("2 mills: ");
    Serial.println(getMillis());
    Serial.print("2 time: ");
    Serial.println(getTime(&timer2, SECONDS));
  }
  if (last1 != getExp(&timer1)) {
    last1 = getExp(&timer1);
    Serial.print("1 exp: ");
    Serial.println(getExp(&timer1));
    Serial.print("1 mills: ");
    Serial.println(getMillis());
    Serial.print("1 time: ");
    Serial.println(getTime(&timer1, SECONDS));
  }
}
