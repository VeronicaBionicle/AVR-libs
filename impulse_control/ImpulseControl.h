#include <avr/io.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define MAX_PERIODS 100

//#define INT_PIN PORTD2
#define INT_PIN PORTD3

#define OUT_DDR DDRB
//#define OUT_DDR DDRC
//#define OUT_DDR DDRD

#define OUT_PORT PORTB
//#define OUT_PORT PORTC
//#define OUT_PORT PORTD

#define OUT_PIN 2

ISR (EXT_INT);

#if INT_PIN == PORTD2
	#define EXT_INT INT0_vect
#elif INT_PIN == PORTD3
	#define EXT_INT INT1_vect
#endif

void OutputInit (void);
void ZerocrossInit (void);
void On (void);
void Off (void);
void PhaseControl (uint8_t num_of_periods);