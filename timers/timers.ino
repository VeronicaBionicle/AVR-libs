#include <timer_class.h>

Timer timer1(2, HOURS);
uint32_t last = 0;
void setup() {
Serial.begin(9600);
timer1.startTimer(TIMER_1);
Serial.println("Start");
}

void loop() {
if (last != timer1.getExp()){
Serial.print("exp: ");
Serial.println(timer1.getExp());
Serial.print("mills: ");
Serial.println(timer1.getMillis());
last = timer1.getExp();
}

} 
