#include "timer_class.h"

#define MAX_STAGES 3
#define ROTATE_TIME 10000
Timer timer1;
uint8_t current_stage;
uint8_t reverse = 0;

uint8_t forward_stage[4] = {   //-A, -B, A, B active high
  0b00000011,
  0b00001001,
  0b00001100,
  0b00000110
};

uint8_t forward_stage_low[4] = {   //-A, -B, A, B active low
  0b00001100,
  0b00000110,
  0b00000011,
  0b00001001
};

uint8_t reverse_stage[4] = {   //-A, -B, A, B
  0b00000011,
  0b00000110,
  0b00001100,
  0b00001001
};

void setup() {
  DDRB = 0x0F;  //PB0 (B), PB1 (A), PB2 (-B), PB3 (-A), PB4 - Reverse button
}

void Forward() {
  PORTB = forward_stage[current_stage];
  if (current_stage < MAX_STAGES) {current_stage = current_stage + 1;} else {current_stage = 0;}
};

void Reverse() {
  PORTB = reverse_stage[current_stage];
  if (current_stage < MAX_STAGES) {current_stage++;} else {current_stage = 0;}
};

void loop() {
      if (PINB & (1<<PB4)) { // если нажата
          if (!reverse) {
            timer1.stopTimerCounter();
            PORTB = 0;
            delay(500);
            timer1.attachTimerInterrupt(Reverse, ROTATE_TIME);
            reverse = 1;
            current_stage = 0;
            } 
        } else {
        if (reverse) {
          
          timer1.stopTimerCounter();
          PORTB = 0;
          delay(500);
          timer1.attachTimerInterrupt(Forward, ROTATE_TIME);
          reverse = 0;
          current_stage = 0;
          }
        };  
}
