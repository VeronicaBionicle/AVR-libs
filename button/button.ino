#include "timer_0_class.h"

#define LIMIT_SWITCH PC0
#define FIRST 0b00001100
#define SECOND 0b00001010
#define THIRD 0b00000110
#define BUTTON_MASK 0b00001110
#define READ_BUTTONS_TIME 15000 //15 ms

int button_state, last_state;
Timer0 button_timer;

void setup() {
  Serial.begin(9600);
  button_timer.attachTimerInterrupt(ReadButtons, READ_BUTTONS_TIME);
  DDRC = 0;
}

void ReadButtons(){
  button_state = PINC & BUTTON_MASK;
  };

void loop() {
  switch(button_state){
    case FIRST: Serial.println("First!"); break;
    case SECOND: Serial.println("Second!"); break;
    case THIRD: Serial.println("Third!"); break;
    default: break;
    }
}
