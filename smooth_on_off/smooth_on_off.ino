#include "smooth_on_off.h"

void setup() {}

void loop() {
  SmoothSwitch(ON);
  delay(1000);
  SmoothSwitch(OFF);
  delay(1000);
}
