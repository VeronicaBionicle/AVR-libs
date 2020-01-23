#include "smooth_on_off.h"
int on = 0;
void setup() { DDRD = (1 << DDD3);}

void loop() {
    if (PINB & (1 << PB4)) {
      if (on){
        Off();
        on = 0;}
       
    } else {
      if (!on) {
        //On();
        SmoothSwitch(ON);
        on = 1;}
      
    };
}
