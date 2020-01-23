#include "smooth_on_off.h"

float duty, end_duty;

ISR(TIMER2_COMPB_vect) {
  OCR2B = duty;
  if (end_duty == ON) {
    if (duty < end_duty) duty += PWM_INCREMENT; else {
      TCCR2A = 0;  //smooth on
      TCCR2B = 0;
      TIMSK2 = 0;
      PORTD = (1 << PD3);
    }
  }
  else {
    if (duty > end_duty) duty -= PWM_INCREMENT;  //smooth off
    else {
      TCCR2A = 0;
      TCCR2B = 0;
      TIMSK2 = 0;
      PORTD = (0 << PD3);
    }
  }
}

void On() {
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;
  DDRD = (1 << DDD3);
  PORTD = (1 << PD3);
}

void Off() {
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;
  DDRD = (1 << DDD3);
  PORTD = (0 << PD3);
}

void SmoothSwitch(uint8_t switching_direction) {
  if (switching_direction == ON) {
    duty = OFF;
    end_duty = ON;
  } else {
    duty = ON;
    end_duty = OFF;
  }
  //start pwm
  DDRD = (1 << DDD3);
  TCCR2A = (1 << COM2B1) | (1 << WGM20);
  TCCR2B = (0 << WGM22) | (0 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 = (1 << OCIE2B);
  OCR2B = 0x0; //255 - 100%
  sei();
};
