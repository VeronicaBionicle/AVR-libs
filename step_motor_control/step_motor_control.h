#ifndef step_motor_control_h_
#define step_motor_control_h_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "timer_class.h"

#define MAX_FULLSTEP_STAGES 3
#define MAX_HALFSTEP_STAGES 7
#define FULL_STEPS_PER_ROTATION int(360/7.5) //7.5 degrees per step
#define HALF_STEPS_PER_ROTATION int(2*360/7.5) //3.75 degrees per step
/*Modes of driver*/
#define FORWARD_FULLSTEP 1
#define REVERSE_FULLSTEP 2
#define FORWARD_HALFSTEP 3
#define REVERSE_HALFSTEP 4
#define STOP 0

void StepperMode(uint8_t mode, uint16_t rotate_time);   //function to choose mode of work
void ForwardFullStep();
void ReverseFullStep();
void ForwardHalfStep();
void ReverseHalfStep();
void Stop();
uint16_t MadeSteps();   //returns the number of steps 
void MadeSteps(uint16_t new_number);    //defines new number of steps

/*State tables for steps*/
const uint8_t forward_stage_fullstep[4] = {   //-A, -B, A, B
  0b00000011,
  0b00001001,
  0b00001100,
  0b00000110
};
const uint8_t reverse_stage_fullstep[4] = {   //-A, -B, A, B
  0b00000011,
  0b00000110,
  0b00001100,
  0b00001001
};
const uint8_t forward_stage_halfstep[8] = {   //-A, -B, A, B
  0b00000010,
  0b00000011,
  0b00000001,
  0b00001001,
  0b00001000,
  0b00001100,
  0b00000100,
  0b00000110
};
const uint8_t reverse_stage_halfstep[8] = {   //-A, -B, A, B
  0b00000001,
  0b00000011,
  0b00000010,
  0b00000110,
  0b00000100,
  0b00001100,
  0b00001000,  
  0b00001001
};
#endif /*dstep_motor_control_h_ */
