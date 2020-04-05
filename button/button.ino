#include "timer_0_class.h"
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

int button_state, last_state;
Timer0 button_timer;
int current_duty = 255;
ISR (INT0_vect)
{
button_state = LIMIT_SWITCH;

}


void setup() {
EICRA=(1<<ISC01) | (0<<ISC00); // falling edge on PD2 (INT0) interrupt
EIMSK=(0<<INT1) | (1<<INT0);
EIFR=(0<<INTF1) | (1<<INTF0);
sei();
  DDRC = 0;
  DDRD = (0<<PD2) | (1 << PD3); //PD3 (OC2B) - Driller motor
  PORTD |= (1<<PD2);
  button_timer.attachTimerInterrupt(ReadButtons, READ_BUTTONS_TIME);

}

void ReadButtons(){
 button_state = PINC & BUTTON_MASK;
  };

void loop() {
  if (button_state != last_state){
  switch(button_state){
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
    default: break;
    }
    last_state = button_state;
    }
}
