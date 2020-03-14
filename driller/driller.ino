#include "timer_1_class.h"  //timer for step motor control
#include "timer_0_class.h"  //timer for reading buttons
#include "step_motor_control.h"
#include "smooth_on_off.h"

#define LIMIT_SWITCH 0b1000000
#define DOWN 0b00011111
#define UP 0b00101111
#define DRILL 0b00110111
#define PCB_DRILL 0b00111011
#define PCB_POINT 0b00111101
#define GO_HOME 0b00111110
#define BUTTON_MASK 0b00111111
#define READ_BUTTONS_TIME 15000 //15 ms

#define TURNS_TO_POINT 2
#define SPEED_FACTOR 1
uint8_t button_state;
Timer0 button_timer;

void ReadButtons() {  //attach to Timer0 to read buttons state
  button_state = PINC & BUTTON_MASK;
};

/*Drilling stages*/
uint8_t drilling_stage;
#define POINTING 3
#define NO_DRILLING 0 //drilling motor is stopped
#define DRILLING_DOWN 1 //motor is started and moves down
#define DRILLING_UP 2  //motor is started and moves up

#define ROTATE_TIME 10000 //10 ms
uint16_t do_steps;    //HALF_STEPS_PER_ROTATION int(2*360/7.5) = 96 half steps per 1 turn

ISR (INT0_vect) 	//interrupt for limit switch
{
  StepperStop(); //stop stepper movement
  /* Detach INT0 interrupt */
  EIMSK = 0; 
  EICRA = 0;
  EIFR = 0;
}

void GoHome() { //Go up, turn off drill motor, stop, when limit switch is
  DrillOff();
  EICRA = (1 << ISC01) | (0 << ISC00); // falling edge on PD2 (INT0) interrupt
  EIMSK = (1 << INT0);
  EIFR = (1 << INTF0);
  StepperMode(REVERSE_HALFSTEP, ROTATE_TIME);
  while (EIMSK) {}; //wait until it goes to home position
  MadeSteps(0);
};

void setup() {
  DDRC = 0;
  DDRD = (0<<PD2) | (1 << PD3); //PD3 (OC2B) - Driller motor
  PORTD |= (1<<PD2);
  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3); //PB0 (B), PB1 (A), PB2 (-B), PB3 (-A)
  //GoHome(); //goto home position 
  button_timer.attachTimerInterrupt(ReadButtons, READ_BUTTONS_TIME);     /*start reading buttons*/
}

void loop() {
  switch (button_state) {
    //when DOWN or UP are pressed, drill moves
    case DOWN:   //move driller down
      DrillOff();
      drilling_stage = NO_DRILLING;
      StepperMode(FORWARD_HALFSTEP, SPEED_FACTOR * ROTATE_TIME);
      break;
    case UP:  //move driller up
      DrillOff();
      drilling_stage = NO_DRILLING;
      StepperMode(REVERSE_HALFSTEP, SPEED_FACTOR * ROTATE_TIME);
      break;
    case GO_HOME: //go home
      GoHome();
      break;
    case DRILL: //go home
      DrillSmoothOn();
      break;
   case PCB_POINT: //make some turns down
      //GoHome();
      drilling_stage = POINTING;
      StepperMode(FORWARD_HALFSTEP, 2 * ROTATE_TIME);
      do_steps = HALF_STEPS_PER_ROTATION * TURNS_TO_POINT; //make turns down
      break;
    case PCB_DRILL: //turn on drill, make some turns up and down
      DrillSmoothOn();
      drilling_stage = DRILLING_DOWN; //first stage
      StepperMode(FORWARD_HALFSTEP, 2 * ROTATE_TIME);
      do_steps = HALF_STEPS_PER_ROTATION * 4; //make 4 turns down
      break;
    case BUTTON_MASK: //when DOWN or UP are released, stepper stops
      if (!drilling_stage) {
        StepperStop();
        MadeSteps(0);
      }
      break;
    default: break;
  }

  if (MadeSteps() == do_steps) {  //when turns are made
    switch (drilling_stage) {
      case DRILLING_DOWN: //after the end of first stage (4 turns down are made)
        MadeSteps(0);
        drilling_stage = DRILLING_UP; //the second stage starts
        StepperMode(REVERSE_HALFSTEP, 2 * ROTATE_TIME);
        do_steps = HALF_STEPS_PER_ROTATION * 4; //make 4 turns up
        break;
      case DRILLING_UP: //after the end of srcond stage (4 turns up are made)
        DrillOff();
        MadeSteps(0);
        drilling_stage = NO_DRILLING; //driller stops
        break;
      case POINTING: //( turns up are made)
        MadeSteps(0);
        drilling_stage = NO_DRILLING; //driller stops
        break;      
      default: break;
    }
  }
}
