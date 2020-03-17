#include <mega328p.h>

#define low(x)   ((x) & 0xFF)
#define high(x)   (((x)>>8) & 0xFF)

//#define FGEN 7680000UL
#define FGEN 7946000UL
#define TIMER0_A_PWM (1<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
#define TIMER0_B_PWM (0<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
#define N 181 //points
#define DEAD_TIME 2

unsigned char polarity = 1;
unsigned char B_polarity = 0;
unsigned char step = 0;

unsigned char sinus[N+1];

const float sinus_table[N+1] ={
0.0, 0.0175, 0.035, 0.052, 0.0698, 0.087, 0.105,
0.122, 0.139, 0.156, 0.174, 0.191, 0.208,
0.225, 0.242, 0.259, 0.276, 0.292, 0.309,
0.326, 0.342, 0.358, 0.375, 0.391, 0.407,
0.423, 0.438, 0.454, 0.469, 0.485, 0.5,
0.515, 0.53, 0.545, 0.559, 0.574, 0.588,
0.602, 0.616, 0.629, 0.643, 0.656, 0.669,
0.682, 0.695, 0.707, 0.719, 0.731, 0.743,
0.755, 0.766, 0.777, 0.788, 0.799, 0.809,
0.819, 0.829, 0.839, 0.848, 0.857, 0.866,
0.875, 0.883, 0.891, 0.899, 0.906, 0.914,
0.921, 0.927, 0.934, 0.94, 0.946, 0.951,
0.956, 0.961, 0.966, 0.97, 0.974, 0.978,
0.982, 0.985, 0.988, 0.99, 0.993, 0.995,
0.996, 0.998, 0.999, 0.999, 1.0,  1.0,
1.0, 0.999, 0.999, 0.998, 0.996,
0.995, 0.993, 0.99, 0.988, 0.985, 0.982,
0.978, 0.974, 0.97, 0.966, 0.961, 0.956,
0.951, 0.946, 0.94, 0.934, 0.927, 0.921,
0.914, 0.906, 0.899, 0.891, 0.883, 0.875,
0.866, 0.857, 0.848, 0.839, 0.829, 0.819,
0.809, 0.799, 0.788, 0.777, 0.766, 0.755,
0.743, 0.731, 0.719, 0.707, 0.695, 0.682,
0.669, 0.656, 0.643, 0.629, 0.616, 0.602,
0.588, 0.574, 0.559, 0.545, 0.53, 0.515,
0.5, 0.485, 0.469, 0.454, 0.438, 0.423,
0.407, 0.391, 0.375, 0.358, 0.342, 0.326,
0.309, 0.292, 0.276, 0.259, 0.242, 0.225,
0.208, 0.191, 0.174, 0.156, 0.139, 0.122,
0.105, 0.087, 0.0698, 0.052, 0.035, 0.0175, 0.0};

void stop_timers() {
TCCR0A=0;
TCCR0B=0;
TCNT0=0;
OCR0A=0;
OCR0B=0;
TIMSK1=0;
}

void DC_mode() {
stop_timers();
PORTC=(1<<PORTC1);
PORTD=(1<<PORTD5);
}

void sinus_period(unsigned int frequency) {
    if (frequency != 0) {
    OCR1AH = high(FGEN/2/N/frequency);
    OCR1AL = low(FGEN/2/N/frequency);
    } else {
        DC_mode();
    }
}

void sinus_amplitude(unsigned char amplitude) {
unsigned char i = 0;
    for (i = 0; i < N; i++) {
     sinus[i] = 2.55*amplitude*sinus_table[i];
    }
}

void start_PWM(unsigned int frequency, unsigned int amplitude) {
// Timer/Counter 0 initialization
TCCR0A=TIMER0_A_PWM;
TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0;
// Timer/Counter 1 initialization
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
TCNT1H=0; TCNT1L=0; ICR1H=0; ICR1L=0;
// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);

sinus_period(frequency);
sinus_amplitude(amplitude);
}

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
   if (step > N) {polarity ^= 1; step = 0;};
   if (step == DEAD_TIME || step == N-DEAD_TIME) {B_polarity ^= 1;};
   step++;
   if (polarity == 1) {
    TCCR0A = TIMER0_A_PWM;
    OCR0A = sinus[step];
    PORTC = (B_polarity << PORTC0);
   } else {
    TCCR0A=TIMER0_B_PWM;
    OCR0B = sinus[step];
    PORTC = (B_polarity << PORTC1);
   }
}

void setup() {
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
ACSR=(1<<ACD);
// Ports init
DDRC=(1<<DDC1) | (1<<DDC0);
DDRD=(1<<DDD6) | (1<<DDD5);

start_PWM(100, 100);

#asm("sei")
}

void main(void)
{
 setup();
while (1)
      {
      }
}