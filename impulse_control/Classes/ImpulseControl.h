#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATtiny13__)
	#define INTERRUPT_INIT MCUCR
	#define GENERAL_INTERRUPT GIMSK
	#define INTERRUPT_FLAG GIFR
	#define EXTERNAL_INTERRUPT_PORT DDRB
	#define EXTERNAL_INTERRUPT_PIN_0 1
	#define EXT_INT INT0_vect
#elif defined (__AVR_ATmega8__)
	#define INTERRUPT_INIT MCUCR
	#define GENERAL_INTERRUPT GIMSK
	#define INTERRUPT_FLAG GIFR
	#define EXTERNAL_INTERRUPT_PORT DDRD
	#define EXTERNAL_INTERRUPT_PIN_0 2
	#define EXT_INT INT0_vect
#elif defined (__AVR_ATmega64__)
	#define INTERRUPT_INIT EICRA
	#define GENERAL_INTERRUPT EIMSK
	#define INTERRUPT_FLAG EIFR
	#define EXTERNAL_INTERRUPT_PORT DDRD
	#define EXTERNAL_INTERRUPT_PIN_0 0
	#define EXT_INT INT0_vect
#elif defined (__AVR_ATmega328P__)
	#define INTERRUPT_INIT EICRA
	#define GENERAL_INTERRUPT EIMSK
	#define INTERRUPT_FLAG EIFR
	#define EXTERNAL_INTERRUPT_PORT DDRD
	#define EXTERNAL_INTERRUPT_PIN_0 2
	#define EXT_INT INT0_vect
#endif

/*	Zerocross pin selection	*/
#define ZEROCROSS_PIN EXTERNAL_INTERRUPT_PIN_0

/*	Output pin selection	*/
#define OUT_DDR DDRB
#define OUT_PORT PORTB

class ImpulseControl {
	private:
	uint8_t out_pin;
	uint8_t max_periods;
	uint8_t num_of_periods;
	public:
/*	Functions definitions	*/
ImpulseControl(uint8_t output_pin, uint8_t maximum_periods);
void OutputInit (void); //Initialize output pin
void ZerocrossInit (void); //Initialize zerocross pin
void On (void); //High level on output pin
void Off (void); //Low level on output pin
void PhaseControl (uint8_t number_of_periods); //What number of periods are passed from maximum period	
};