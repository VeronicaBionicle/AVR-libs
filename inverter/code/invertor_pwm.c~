#include <mega328p.h>

#define low(x)   ((x) & 0xFF)
#define high(x)   (((x)>>8) & 0xFF)

#define FGEN 7680000UL
#define TIMER0_A_PWM (1<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
#define TIMER0_B_PWM (0<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
#define N 180 //points

unsigned char polarity = 1;
unsigned char B_polarity = 0;
unsigned char step = 0;


void sinus_period(unsigned int frequency, unsigned char points) {
    OCR1AH = high(FGEN/2/points/frequency);
    OCR1AL = low(FGEN/2/points/frequency);
}

const unsigned char sinus[N+1] = {0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44, 49, 53, 57, 62, 66,
                                    70, 75, 79, 83, 87, 91, 96, 100, 104, 108, 112, 116, 120, 124, 127,
                                    131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 167, 171, 174, 177,
                                    180, 183, 186, 190, 192, 195, 198, 201, 204, 206, 209, 211, 214, 216,
                                    219, 221, 223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 241, 243,
                                    244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 253, 254, 254, 254,
                                    255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
                                    250, 249, 248, 247, 246, 245, 244, 243, 241, 240, 238, 236, 235, 233,
                                    231, 229, 227, 225, 223, 221, 219, 216, 214, 211, 209, 206, 204, 201,
                                    198, 195, 192, 190, 186, 183, 180, 177, 174, 171, 167, 164, 160, 157,
                                    153, 150, 146, 143, 139, 135, 131, 127, 124, 120, 116, 112, 108, 104,
                                    100, 96, 91, 87, 83, 79, 75, 70, 66, 62, 57, 53, 49, 44, 40, 35,
                                    31, 27, 22, 18, 13, 9, 4, 0};


interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
   if (step > N) {polarity ^= 1; step = 0;};
   if (step == 2 || step == N-2) {B_polarity ^= 1;};
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

// Ports init
DDRC=(1<<DDC1) | (1<<DDC0);
DDRD=(1<<DDD6) | (1<<DDD5);

// Timer/Counter 0 initialization
TCCR0A=TIMER0_A_PWM;
TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0;
OCR0A=0;
OCR0B=0;

// Timer/Counter 1 initialization
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
TCNT1H=0;
TCNT1L=0;
ICR1H=0;
ICR1L=0;
OCR1BH=0;
OCR1BL=0;
// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
ACSR=(1<<ACD);

sinus_period(50, N);

#asm("sei")
}

void main(void)
{
 setup();
while (1)
      {
      }
}