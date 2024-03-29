#include <mega328p.h>
#include <math.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

#define low(x)   ((x) & 0xFF)
#define high(x)   (((x)>>8) & 0xFF)

//#define FGEN 9216000UL
#define FGEN 3072000UL
#define TIMER0_PWM (1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
#define TIMER2_PWM (1<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (1<<WGM20)
#define PWM_DIV (0<<WGM02) | (0<<CS02) | (0<<CS01) | (1<<CS00)

#define TIMER1_DIV (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10)
#define ADC_VREF_TYPE (0<<REFS1) | (1<<REFS0) | (1<<ADLAR)
#define A 0
#define B 1
#define C 2

#define N 181 //points
#define PHASE_B_DELAY 2*N/3
#define PHASE_C_DELAY N/3
#define PHASE_A_PIN PORTD6
#define PHASE_B_PIN PORTD5
#define PHASE_C_PIN PORTB3
#define MAX_FREQUENCY 200
#define TR_REST 1.2

uint8_t adc_data[2];
uint8_t ADC_input=0;

uint8_t step[3];

uint8_t sinus[N+1];

uint8_t phase_b_step = 0;
uint8_t phase_c_step = 0;

uint8_t amplitude = 255;
uint8_t frequency = 200;

const float sinus_table[N+1]={0, 0.0174497483512505, 0.0348782368720627, 0.0522642316338267, 0.0695865504800327, 0.0868240888334652, 0.10395584540888, 0.120960947799834, 0.1378186779085, 0.154508497187474, 0.171010071662834, 0.187303296707956, 0.2033683215379, 0.219185573394539, 0.234735781392945, 0.25, 0.264959632116602, 0.279596451735373, 0.293892626146237, 0.307830737662829, 0.32139380484327, 0.334565303179429, 0.347329185229499, 0.359669900169326, 0.371572412738697, 0.383022221559489, 0.394005376803361, 0.404508497187474, 0.414518786277521, 0.424024048078213, 0.433012701892219, 0.441473796429463, 0.449397023149584, 0.4567727288213, 0.463591927283394, 0.469846310392954, 0.475528258147577, 0.480630847969159, 0.485147863137998, 0.489073800366903, 0.492403876506104, 0.495134034370785, 0.497260947684137, 0.498782025129912, 0.499695413509548, 0.5, 0.499695413509548, 0.498782025129912, 0.497260947684137, 0.495134034370785, 0.492403876506104, 0.489073800366903, 0.485147863137998, 0.480630847969159, 0.475528258147577, 0.469846310392954, 0.463591927283394, 0.4567727288213, 0.449397023149583, 0.441473796429464, 0.433012701892219, 0.424024048078213, 0.414518786277521, 0.404508497187474, 0.394005376803361, 0.383022221559489, 0.371572412738697, 0.359669900169326, 0.347329185229499, 0.334565303179429, 0.32139380484327, 0.307830737662829, 0.293892626146237, 0.279596451735373, 0.264959632116602, 0.25, 0.234735781392945, 0.219185573394539, 0.2033683215379, 0.187303296707956, 0.171010071662834, 0.154508497187474, 0.1378186779085, 0.120960947799834, 0.10395584540888, 0.0868240888334653, 0.0695865504800327, 0.0522642316338269, 0.0348782368720628, 0.0174497483512506, 0, -0.0174497483512505, -0.0348782368720624, -0.0522642316338267, -0.0695865504800328, -0.0868240888334652, -0.10395584540888, -0.120960947799834, -0.137818677908499, -0.154508497187474, -0.171010071662834, -0.187303296707956, -0.2033683215379, -0.219185573394539, -0.234735781392945, -0.25, -0.264959632116602, -0.279596451735373, -0.293892626146237, -0.307830737662829, -0.32139380484327, -0.334565303179429, -0.347329185229499, -0.359669900169325, -0.371572412738697, -0.383022221559489, -0.394005376803361, -0.404508497187474, -0.414518786277521, -0.424024048078213, -0.433012701892219, -0.441473796429463, -0.449397023149583, -0.4567727288213, -0.463591927283394, -0.469846310392954, -0.475528258147577, -0.48063084796916, -0.485147863137998, -0.489073800366903, -0.492403876506104, -0.495134034370785, -0.497260947684137, -0.498782025129912, -0.499695413509548, -0.5, -0.499695413509548, -0.498782025129912, -0.497260947684137, -0.495134034370785, -0.492403876506104, -0.489073800366903, -0.485147863137998, -0.48063084796916, -0.475528258147577, -0.469846310392954, -0.463591927283394, -0.456772728821301, -0.449397023149584, -0.441473796429464, -0.433012701892219, -0.424024048078213, -0.414518786277521, -0.404508497187474, -0.394005376803361, -0.383022221559489, -0.371572412738697, -0.359669900169326, -0.347329185229499, -0.334565303179429, -0.32139380484327, -0.307830737662829, -0.293892626146237, -0.279596451735374, -0.264959632116603, -0.25, -0.234735781392945, -0.219185573394539, -0.2033683215379, -0.187303296707956, -0.171010071662835, -0.154508497187474, -0.137818677908499, -0.120960947799834, -0.10395584540888, -0.0868240888334652, -0.0695865504800329,
};

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
    uint16_t period = FGEN/N/frequency;
    OCR1AH = high(period);
    OCR1AL = low(period);
}

void sinus_amplitude(uint8_t amplitude) {
    uint8_t i = 0;
    int current_amplitude;
    for (i = 0; i < N; i++){
    current_amplitude = (int)(127 + TR_REST*amplitude*sinus_table[i]);
        if (current_amplitude > 255) sinus[i] = 255; else {
            if (current_amplitude < 0) {sinus[i] = 0;} else sinus[i] = current_amplitude;
        }
    }
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
   if (step[A] > N-1) step[A] = 0;
   OCR0A = sinus[step[A]];
   /* For phase B */
   if (phase_b_step < PHASE_B_DELAY) {phase_b_step++;} else {
       step[B]++;
       if (step[B] > N-1) step[B] = 0;
       OCR2A = sinus[step[B]];
   }
   /* For phase C */
   if (phase_c_step < PHASE_C_DELAY) {phase_c_step++;} else {
       step[C]++;
       if (step[C] > N-1) step[C] = 0;
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