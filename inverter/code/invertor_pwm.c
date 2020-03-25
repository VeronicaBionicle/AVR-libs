#include <mega328p.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

#define low(x)   ((x) & 0xFF)
#define high(x)   (((x)>>8) & 0xFF)

#define FGEN 11900000UL
#define TIMER0_PWM (1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
#define TIMER2_PWM (1<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (1<<WGM20)
#define PWM_DIV (0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00)
#define TIMER1_DIV (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10)
#define ADC_VREF_TYPE (0<<REFS1) | (1<<REFS0) | (1<<ADLAR)
#define A 0
#define B 1
#define C 2

#define N 181 //points
#define PHASE_B_DELAY N/3
#define PHASE_C_DELAY 2*N/3
#define PHASE_A_PIN PORTD6
#define PHASE_B_PIN PORTD5
#define PHASE_C_PIN PORTB3
#define MAX_FREQUENCY 100

uint8_t adc_data[2];
uint8_t ADC_input=0;

uint8_t step[3];

uint8_t sinus[N+1];

uint8_t phase_b_step = 0;
uint8_t phase_c_step = 0;

uint8_t amplitude = 255;
uint8_t frequency = 100;

const float sinus_table[N+1] ={
    0.0, 0.0175, 0.035, 0.052, 0.0698, 0.087, 0.105, 0.122, 0.139, 0.156, 0.174, 0.191, 0.208,
    0.225, 0.242, 0.259, 0.276, 0.292, 0.309, 0.326, 0.342, 0.358, 0.375, 0.391, 0.407,
    0.423, 0.438, 0.454, 0.469, 0.485, 0.5, 0.515, 0.53, 0.545, 0.559, 0.574, 0.588,
    0.602, 0.616, 0.629, 0.643, 0.656, 0.669, 0.682, 0.695, 0.707, 0.719, 0.731, 0.743,
    0.755, 0.766, 0.777, 0.788, 0.799, 0.809, 0.819, 0.829, 0.839, 0.848, 0.857, 0.866,
    0.875, 0.883, 0.891, 0.899, 0.906, 0.914, 0.921, 0.927, 0.934, 0.94, 0.946, 0.951,
    0.956, 0.961, 0.966, 0.97, 0.974, 0.978, 0.982, 0.985, 0.988, 0.99, 0.993, 0.995,
    0.996, 0.998, 0.999, 0.999, 1.0, 1.0, 1.0, 0.999, 0.999, 0.998, 0.996,
    0.995, 0.993, 0.99, 0.988, 0.985, 0.982, 0.978, 0.974, 0.97, 0.966, 0.961, 0.956,
    0.951, 0.946, 0.94, 0.934, 0.927, 0.921, 0.914, 0.906, 0.899, 0.891, 0.883, 0.875,
    0.866, 0.857, 0.848, 0.839, 0.829, 0.819, 0.809, 0.799, 0.788, 0.777, 0.766, 0.755,
    0.743, 0.731, 0.719, 0.707, 0.695, 0.682, 0.669, 0.656, 0.643, 0.629, 0.616, 0.602,
    0.588, 0.574, 0.559, 0.545, 0.53, 0.515, 0.5, 0.485, 0.469, 0.454, 0.438, 0.423,
    0.407, 0.391, 0.375, 0.358, 0.342, 0.326, 0.309, 0.292, 0.276, 0.259, 0.242, 0.225,
    0.208, 0.191, 0.174, 0.156, 0.139, 0.122, 0.105, 0.087, 0.0698, 0.052, 0.035, 0.0175, 0.0};

void stop_timers() {
    TCCR0A=0; TCCR0B=0;
    OCR0A=0; OCR0B=0;
    TCCR1A=0; TCCR1B=0;
    OCR1AH=0; OCR1AL=0;
    TIMSK1= (1<<OCIE1B);
    TCCR2A=0; TCCR2B=0;
    OCR2A=0; OCR2B=0;
}

void closed_mode() {
    stop_timers();
    PORTD=(1<<PHASE_A_PIN) | (1<<PHASE_B_PIN);
    PORTB=(1<<PHASE_C_PIN);
}

void sinus_period(uint8_t frequency) {
    uint16_t period = FGEN/2/N/frequency;
    OCR1AH = high(period);
    OCR1AL = low(period);
}

void sinus_amplitude(uint8_t amplitude) {
    uint8_t i = 0;
    for (i = 0; i < N; i++)
        sinus[i] = amplitude*sinus_table[i];
}

void start_PWM(uint8_t frequency, uint8_t amplitude) {
    sinus_period(frequency);
    sinus_amplitude(amplitude);
    step[A] = 0;
    step[B] = 0;
    step[C] = 0;
    // Timer/Counter 0 initialization
    TCCR0A=TIMER0_PWM;
    TCCR0B=PWM_DIV;
    TCNT0=0;
    // Timer/Counter 2 initialization
    TCCR2A=TIMER2_PWM;
    TCCR2B=PWM_DIV;
    TCNT2=0;
    // Timer/Counter 1 initialization
    TCCR1A=0;
    TCCR1B= TIMER1_DIV;
    TCNT1H=0; TCNT1L=0; ICR1H=0; ICR1L=0;
    TIMSK1 |= (1<<OCIE1A);
    #asm("sei")
}

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
    /* For phase A */
   step[A]++;
   if (step[A] > N) step[A] = 0;
   OCR0A = sinus[step[A]];
   /* For phase B */
   if (phase_b_step < PHASE_B_DELAY) {phase_b_step++;} else {
       step[B]++;
       if (step[B] > N) step[B] = 0;
       OCR2A = sinus[step[B]];
   }
   /* For phase C */
   if (phase_c_step < PHASE_C_DELAY) {phase_c_step++;} else {
       step[C]++;
       if (step[C] > N) step[C] = 0;
       OCR0B = sinus[step[C]];
   }
}

interrupt [ADC_INT] void adc_isr(void)
{
    adc_data[ADC_input]=ADCH;
    if (ADC_input == 1) {ADC_input=0;} else {ADC_input=1;}
    ADMUX= ADC_input + ADC_VREF_TYPE;
    ADCSRA|=(1<<ADSC);
}

void invertor_setup() {
    // Crystal Oscillator division factor: 1
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif
    //ADC init (read each ~5 ms - CTCB Timer1 interrupt)
    ACSR=(1<<ACD);
    DIDR0=(1<<ADC5D) | (1<<ADC4D) | (1<<ADC3D) | (1<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);  //ADC0, ADC1 are used
    ADMUX = ADC_VREF_TYPE;  // Voltage Reference: AVCC pin
    ADCSRA=(1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (0<<ADIF) | (1<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    ADCSRB=(1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0);
    TIMSK1 |= (1<<OCIE1B);
    OCR1BH=0xFF;
    OCR1BL=0xFF;
    // Ports init
    DDRD=(1<<PHASE_A_PIN) | (1<<PHASE_B_PIN);
    DDRB=(1<<PHASE_C_PIN);
    start_PWM(frequency, amplitude);
}

void main(void)
{
    invertor_setup();
    while (1)
          {
          /* if frequency/amplitude is 0, stop invertor */
          if (adc_data[0] == 0 || adc_data[1] == 0) {
            while (adc_data[0] == 0 || adc_data[1] == 0) {closed_mode();};
            start_PWM(MAX_FREQUENCY*(uint16_t)adc_data[1]/255, adc_data[0]);
          }
          /* if ADC data is updated, change frequency/amplitude*/
          if (amplitude != adc_data[0]) {amplitude = adc_data[0]; sinus_amplitude(amplitude);}
          if (frequency != MAX_FREQUENCY*(uint16_t)adc_data[1]/255) {frequency = MAX_FREQUENCY*(uint16_t)adc_data[1]/255; sinus_period(frequency);}
          }
}