#include "smooth_on_off.h"
#include "buttons_interrupter.h"
#include "step_motor_control.h"
/*Changeble*/
#define TURNS_TO_POINT 35
#define TURNS_TO_DRILL_PCB 2
#define BOOST 0.5 // BOOST * SPEED -> SPEED x 1/BOOST
#define SLOW 3  //SLOW * SPEED -> SPEED/SLOW
#define ROTATE_TIME 7000 //10 ms
/*Drilling stages*/
uint8_t drilling_stage;
#define NO_DRILLING 0 //drilling motor is stopped
#define DRILLING_DOWN 1 //motor is started and moves down
#define DRILLING_UP 2  //motor is started and moves up
#define POINTING 3
#define MANUAL_DRILLING 4

Timer0 button_timer;
uint8_t button_state;
uint16_t do_steps;    //HALF_STEPS_PER_ROTATION int(2*360/7.5) = 96 half steps per 1 turn

void GoHome() { //Go up, turn off drill motor, stop, when limit switch is
  DrillOff();
  EICRA = (1 << ISC01) | (0 << ISC00); // falling edge on PD2 (INT0) interrupt
  EIMSK = (1 << INT0);
  EIFR = (1 << INTF0);
  StepperMode(REVERSE_HALFSTEP, BOOST * ROTATE_TIME);
  while (EIMSK) {}; //wait until it goes to home position
  MadeSteps(0);
};

void setup() {
  DDRC = 0; // port for buttons
  DDRD = (0<<PD2) | (1 << PD3); //PD3 (OC2B) - Driller motor, PD2 - interrupter
  PORTD |= (1<<PD2);
  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3); //PB0 (B), PB1 (A), PB2 (-B), PB3 (-A)
  //GoHome(); //goto home position 
  button_timer.attachTimerInterrupt(ReadButtons, READ_BUTTONS_TIME);     /*start reading buttons*/
}

void loop() {
  switch (button_state) { 
    case DOWN:   //move driller down
      drilling_stage = NO_DRILLING;
      StepperMode(FORWARD_HALFSTEP, ROTATE_TIME);
      break;
    case UP:  //move driller up
      drilling_stage = NO_DRILLING;
      StepperMode(REVERSE_HALFSTEP, ROTATE_TIME);
      break;
    case GO_HOME: //go home
      GoHome();
      break;
    case DRILL: //while button is hold, it goes down and drill, when released, goes up to start position
      if (!drilling_stage) DrillSmoothOn();
      StepperMode(FORWARD_HALFSTEP, SLOW * ROTATE_TIME);
      drilling_stage = MANUAL_DRILLING;
      break;
   case PCB_POINT: //ponting for pcb drill on defined position
      DrillOff();
      GoHome();
      drilling_stage = POINTING;
      StepperMode(FORWARD_HALFSTEP, BOOST * ROTATE_TIME);
      do_steps = HALF_STEPS_PER_ROTATION * TURNS_TO_POINT; //make turns down
      break;
    case PCB_DRILL: //turn on drill, make some turns up and down
      if (!drilling_stage) DrillSmoothOn();
      drilling_stage = DRILLING_DOWN; //first stage
      StepperMode(FORWARD_HALFSTEP, SLOW * ROTATE_TIME);
      do_steps = HALF_STEPS_PER_ROTATION * TURNS_TO_DRILL_PCB; //make some turns down
      break;
    case BUTTON_MASK: 
      if (!drilling_stage) {  //when DOWN or UP are released, stepper stops
        StepperStop();
        MadeSteps(0);
      } else if (drilling_stage == MANUAL_DRILLING) { //when DRILL button is released, starts going up
        drilling_stage = DRILLING_UP; 
        StepperMode(REVERSE_HALFSTEP, ROTATE_TIME);
        do_steps = MadeSteps();
        MadeSteps(0);
        }
      break;
    default: break;
  }

  if (MadeSteps() == do_steps) {  //when turns are made
    switch (drilling_stage) {
      case DRILLING_DOWN: //after the end of first stage (turns down are made)
        MadeSteps(0);
        drilling_stage = DRILLING_UP; //the second stage starts
        StepperMode(REVERSE_HALFSTEP, ROTATE_TIME);
        do_steps = HALF_STEPS_PER_ROTATION * TURNS_TO_DRILL_PCB; //make some turns up
        break;
      case DRILLING_UP: //after the end of srcond stage (4 turns up are made)
        DrillOff();
        MadeSteps(0);
        drilling_stage = NO_DRILLING; //driller stops
        break;
      case POINTING: //turns up are made
        MadeSteps(0);
        drilling_stage = NO_DRILLING; //driller stops
        break;      
      default: break;
    }
  }
}
