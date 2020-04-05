#include "smooth_on_off.h"

uint8_t duty;
uint8_t max_duty = 255;

ISR(TIMER2_COMPB_vect) {
  OCR2B = duty;
    if (duty < max_duty) duty += PWM_INCREMENT; else if (duty == ON) DrillOn();
}

void DrillOn() {
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;
  PORTD = (1 << PD3);
}

void DrillOff() {
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;
  PORTD = (0 << PD3);
}

void DrillSmoothOn() {
  duty = 0;
  //start pwm
  TCCR2A = (1 << COM2B1) | (1 << WGM20);
  TCCR2B = (0 << WGM22) | (0 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 = (1 << OCIE2B);
  OCR2B = 0; //255 - 100%
  sei();
};

void DrillPWM(uint8_t duty_cycle) {
  max_duty = duty_cycle;
  //start pwm
  TCCR2A = (1 << COM2B1) | (1 << WGM20);
  TCCR2B = (0 << WGM22) | (0 << CS22) | (1 << CS21) | (1 << CS20);
  OCR2B = duty_cycle; //255 - 100%
  sei();
};

void DrillSetMaxDuty(uint8_t new_max_duty) {
  max_duty = new_max_duty;
};