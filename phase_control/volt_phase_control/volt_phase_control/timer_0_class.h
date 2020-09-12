#ifndef timer_0_class_h_
#define timer_0_class_h_
#include <avr/interrupt.h>
#include <inttypes.h>

#define DOOR_BELL (1<<PORTC0)
#define DOOR_1 (1<<PORTC1)
#define DOOR_2 (1<<PORTC2)
#define BUTTON_1 (1<<PORTC3)
#define BUTTON_2 (1<<PORTC4)

#define BUTTON_MASK (DOOR_BELL | DOOR_1 | DOOR_2 | BUTTON_1 | BUTTON_2)

#define READ_BUTTONS_TIME 15000UL //15 ms
          
/*Timer0 have 8-bit resolution (period 1 us-16384 us) */
//Timer0 is used for Delay and etc in Arduino, be careful with it

  #define BUTTON_TIMER_INT TIMER0_COMPA_vect
  #define BUTTON_TIMER_TIMSK TIMSK0
  #define BUTTON_TIMER_TCCR_A TCCR0A
  #define BUTTON_TIMER_TCCR_B TCCR0B
  #define BUTTON_TIMER_OCR_A OCR0A
  #define BUTTON_TIMER_TCCR_A_FOR_TIMER (1 << WGM01)
  #define BUTTON_TIMER_CS_0 CS00
  #define BUTTON_TIMER_CS_1 CS01
  #define BUTTON_TIMER_CS_2 CS02
  #define BUTTON_TIMER_OCIE_A OCIE0A
  #define BUTTON_TIMER_CS_FOR_4_PRESC_0 (1 << CS_2)|(1 << CS_0)
  
  void InitButtonTimer(void);
  
#endif /*timer_class_h_ */
