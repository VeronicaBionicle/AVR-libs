#include "timer_1_class.h"
#include "step_motor_control.h"
#include "smooth_on_off.h"

#define ROTATE_TIME 10000

uint8_t driller_on = 0;

//FULL_STEPS_PER_ROTATION int(360/7.5) //48
//HALF_STEPS_PER_ROTATION int(2*360/7.5) //96

void setup() {
  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3); //PB0 (B), PB1 (A), PB2 (-B), PB3 (-A), PB4 - Reverse button
  StepperMode(FORWARD_HALFSTEP, ROTATE_TIME);
  //SmoothSwitch(ON); //OFF to off! Drill motor on PD3 (OC2B) lol
}

void loop() {
  if (MadeSteps() == HALF_STEPS_PER_ROTATION) {
    Stop();
    MadeSteps(0);
    delay(1000);
    if (PINB & (1 << PB4)) {
      StepperMode(REVERSE_HALFSTEP, ROTATE_TIME);
     if (driller_on){
        Off();
        driller_on = 0;}
    } else {
      StepperMode(FORWARD_HALFSTEP, ROTATE_TIME);
      if (!driller_on) {
        SmoothSwitch(ON);
        driller_on = 1;
        }
    };
  }
}
