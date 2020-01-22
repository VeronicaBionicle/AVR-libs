#include "timer_class.h"
#include "step_motor_control.h"
#include "smooth_on_off.h"

#define ROTATE_TIME 10000

//FULL_STEPS_PER_ROTATION int(360/7.5) //48
//HALF_STEPS_PER_ROTATION int(2*360/7.5) //96

void setup() {
  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3); //PB0 (B), PB1 (A), PB2 (-B), PB3 (-A), PB4 - Reverse button
  StepperMode(FORWARD_HALFSTEP, ROTATE_TIME);
  //SmoothSwitch(ON); //OFF to off! Drill motor on PD3 (OC2B)
 // StepperMode(FORWARD_FULLSTEP, ROTATE_TIME);
}

void loop() {
  if (MadeSteps() == HALF_STEPS_PER_ROTATION) {
    Stop();
    MadeSteps(0);
    delay(1000);
    if (PINB & (1 << PB4)) {
      StepperMode(REVERSE_HALFSTEP, ROTATE_TIME);
       SmoothSwitch(OFF);
    } else {
      StepperMode(FORWARD_HALFSTEP, ROTATE_TIME);
      SmoothSwitch(ON);
    };
  }
}