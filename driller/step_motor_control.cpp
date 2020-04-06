#include "step_motor_control.h"

Timer1 stepper;
uint8_t current_stage;
uint16_t steps;
/*
void ForwardFullStep() {
  PORTB = forward_stage_fullstep[current_stage];
  if (current_stage < MAX_FULLSTEP_STAGES) {current_stage++;} else {current_stage = 0;}
  steps++;
};

void ReverseFullStep() {
  PORTB = reverse_stage_fullstep[current_stage];
  if (current_stage < MAX_FULLSTEP_STAGES) {current_stage++;} else {current_stage = 0;}
  steps++;
};
*/
void ForwardHalfStep() {
  PORTB = forward_stage_halfstep[current_stage];
  if (current_stage < MAX_HALFSTEP_STAGES) {current_stage++;} else {current_stage = 0;}
  steps++;
};

void ReverseHalfStep() {
  PORTB = reverse_stage_halfstep[current_stage];
  if (current_stage < MAX_HALFSTEP_STAGES) {current_stage++;} else {current_stage = 0;}
  steps++;
};

void StepperStop() {
  stepper.stopTimerCounter();
  PORTB = 0;
  current_stage = 0;
};

void MadeSteps(uint16_t new_number){
     steps = new_number;
};

uint16_t MadeSteps(){
    return steps;
};

void StepperMode(uint8_t mode, uint16_t rotate_time){
  switch (mode) {
    //case FORWARD_FULLSTEP: stepper.attachTimerInterrupt(ForwardFullStep, rotate_time); break;
    //case REVERSE_FULLSTEP: stepper.attachTimerInterrupt(ReverseFullStep, rotate_time); break;
    case FORWARD_HALFSTEP: stepper.attachTimerInterrupt(ForwardHalfStep, rotate_time/2); break;
    case REVERSE_HALFSTEP: stepper.attachTimerInterrupt(ReverseHalfStep, rotate_time/2); break;
    //case STOP: StepperStop(); break;
    default: break;
    };
  };
