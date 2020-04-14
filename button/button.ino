#include "timer_0_class.h"
#include "smooth_on_off.h"

#define LIMIT_SWITCH 0b1000000

#define DOWN 0b11011111
#define UP 0b11101111
#define DRILL 0b11110111
#define PCB_DRILL 0b11111011
#define PCB_POINT 0b11111101
#define GO_HOME 0b11111110
#define BUTTON_MASK 0b00111111

#define SPEED_1 0b10111111
#define SPEED_2 0b01111111
#define ADD_BUTTON_MASK 0b11000000

#define READ_BUTTONS_TIME 15000 //15 ms

int button_state, add_button_state, last_state;
Timer0 button_timer;
int current_duty = 255;
ISR (INT0_vect)
{
button_state = LIMIT_SWITCH;

}


void setup() {
  Serial.begin(9600);
EICRA=(1<<ISC01) | (0<<ISC00); // falling edge on PD2 (INT0) interrupt
EIMSK=(0<<INT1) | (1<<INT0);
EIFR=(0<<INTF1) | (1<<INTF0);
sei();
  DDRC = 0;
  DDRD = (0<<PD2) | (1 << PD3); //PD3 (OC2B) - Driller motor
  PORTD |= (1<<PD2);
  button_timer.attachTimerInterrupt(ReadButtons, READ_BUTTONS_TIME);
Serial.println("Ready!");
}

void ReadButtons(){
 button_state = (PINC & BUTTON_MASK) | (PIND & ADD_BUTTON_MASK);
  };

void loop() {
  // Serial.println(button_state, BIN);
  if (button_state != last_state){
Serial.println(button_state, BIN);
  switch(button_state){
    case SPEED_1:
    Serial.println("Speed 1");
      break;
    case SPEED_2:
    Serial.println("Speed 2");
      break;
    case DRILL:
    Serial.println("drill");
      break;
    case UP:
    Serial.println("up");
      break;
    case DOWN:
    Serial.println("down");
      break;
    case GO_HOME:
    Serial.println("go home");
      break;
    /*
    case UP:
      if (current_duty < 255) current_duty += 10; else current_duty = 255;
      DrillPWM(current_duty);
      DrillSetMaxDuty(current_duty);
      break;
    case DOWN: 
      if (current_duty > 10) current_duty -= 10; else current_duty = 10;
      DrillPWM(current_duty);
      DrillSetMaxDuty(current_duty);
      break;
    case DRILL:
      DrillSmoothOn();
      break;
    case GO_HOME:
    DrillOff();
      break;
      */
    default: break;
    }
    
    last_state = button_state;
    }
}
