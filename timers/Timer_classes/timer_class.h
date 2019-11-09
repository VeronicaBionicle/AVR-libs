#ifndef timer_class_h_
#define timer_class_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define F_CPU_SH 16 //F_CPU/1000000 i had some problems with overflow
/*Units for startCounter function*/
#define MILLIS 1
#define SECONDS 975    //must be 1000
#define MINUTES 58500   //must be 60000, but my arduinos "minute" is 1 min + 1.5 sec
#define HOURS 3510000
              
/*Choose your timer here
Timer0 and Timer2 have 8-bit resolution (period 1 us-16384 us)
Timer1 is 16-bit (period 1 us-4.19 s) */
//Timer0 is used for Delay and etc in Arduino, be careful with it

//#define TIMER TIMER_0
#define TIMER TIMER_1   
//#define TIMER TIMER_2

class Timer {
  private:
    uint32_t period, milliseconds, expirations;
    void setupTimer(uint32_t Period);  //service function for calculating CS and OCR for timers
  public:
    void startTimerForInterrupt(uint32_t Period);  //start timer for attaching function to ISR, period`s unit measure is microseconds
    void startCounter(uint32_t Period, uint32_t Unit);  //start counter, ISR period is 1ms, period of counting is 1 ms - 1223 hours
    void stopTimerCounter();  //stop counting and interrupts from Timer
    void clearCounterExp(); //clear expirations of counter
    uint32_t getCounterExp(); //get counter expired periods
    uint32_t getPeriods();  //get epired periods of ISR
    static void (*isrCallback)();   //
    static void isrDefaultUnused(); //blank function, if function isn`t attached to ISR
    void attachTimerInterrupt(void (*isr)(), uint32_t Period) __attribute__((always_inline)) {  //attach function to ISR (function must be void without arguments!!!)
      startTimerForInterrupt(Period); //Period in microseconds
      isrCallback = isr;
    };
    void detachTimerInterrupt() __attribute__((always_inline)) {  //detach function from ISR and stop interrupts
      stopTimerCounter();
      isrCallback = isrDefaultUnused;
    };
};
/*  Some definitions for our timers */
#if TIMER == TIMER_0
  #define TIMER_INT TIMER0_COMPA_vect
  #define TIMSK_ TIMSK0
  #define TCCR_A TCCR0A
  #define TCCR_B TCCR0B
  #define OCR_A OCR0A
  #define CS_0 CS00
  #define CS_1 CS01
  #define CS_2 CS02
  #define TCCR_A_FOR_TIMER (1 << WGM01)
  #define OCIE_A OCIE0A
  #define OCR_MAX 256
  #define CS_STOP ~(1<<CS_2)|(1<<CS_1)|(0<<CS_0)
  #define CS_FOR_NO_PRESC (1 << CS_0)
  #define PRESCALER_1 8
  #define CS_FOR_1_PRESC (1 << CS_1)
  #define PRESCALER_2 64
  #define CS_FOR_2_PRESC (1 << CS_1)|(1 << CS_0)
  #define PRESCALER_3 256
  #define CS_FOR_3_PRESC (1 << CS_2)
  #define PRESCALER_4 1024
  #define CS_FOR_4_PRESC (1 << CS_2)|(1 << CS_0)
  #define CS_FOR_COUNTER CS_FOR_2_PRESC
  #define OCR_FOR_COUNTER 0xF9 //max period 0.26214 sec = (64)/(16 MHz) * 65535 / OCR_A = 250 -> 0.001 sec
#elif TIMER == TIMER_1
  #define TIMER_INT TIMER1_COMPA_vect
  #define TIMSK_ TIMSK1
  #define TCCR_A TCCR1A
  #define TCCR_B TCCR1B
  #define OCR_A OCR1A
  #define CS_0 CS10
  #define CS_1 CS11
  #define CS_2 CS12
  #define TCCR_A_FOR_TIMER 0
  #define OCIE_A OCIE1A
  #define OCR_MAX 65536 
  #define CS_FOR_COUNTER (1 << WGM12)|(1 << CS_0)
  #define OCR_FOR_COUNTER 0x3E7F  
  #define CS_STOP ~(1<<CS_2)|(1<<CS_1)|(0<<CS_0)
  #define CS_FOR_NO_PRESC (1 << WGM12) |(1 << CS_0)
  #define PRESCALER_1 8
  #define CS_FOR_1_PRESC (1 << WGM12) |(1 << CS_1)
  #define PRESCALER_2 64
  #define CS_FOR_2_PRESC (1 << WGM12) |(1 << CS_1)|(1 << CS_0)
  #define PRESCALER_3 256
  #define CS_FOR_3_PRESC (1 << WGM12) |(1 << CS_2)
  #define PRESCALER_4 1024
  #define CS_FOR_4_PRESC (1 << WGM12) |(1 << CS_2)|(1 << CS_0)
  #define CS_FOR_COUNTER CS_FOR_2_PRESC
#elif TIMER == TIMER_2
  #define TIMER_INT TIMER2_COMPA_vect
  #define TIMSK_ TIMSK2
  #define TCCR_A TCCR2A
  #define TCCR_B TCCR2B
  #define OCR_A OCR2A
  #define CS_0 CS20
  #define CS_1 CS21
  #define CS_2 CS22
  #define TCCR_A_FOR_TIMER (1 << WGM21)
  #define OCIE_A OCIE2A
  #define OCR_MAX 256
  #define CS_STOP ~(1<<CS_2)|(1<<CS_1)|(0<<CS_0)
  #define CS_FOR_NO_PRESC (1 << CS_0)
  #define PRESCALER_1 8
  #define CS_FOR_1_PRESC (1<<CS_1)|(0<<CS_0)
  #define PRESCALER_2 64
  #define CS_FOR_2_PRESC (1<<CS_2)
  #define PRESCALER_3 256
  #define CS_FOR_3_PRESC (1<<CS_2)|(1<<CS_1)
  #define PRESCALER_4 1024
  #define CS_FOR_4_PRESC (1<<CS_2)|(1<<CS_1)|(1<<CS_0)
  #define CS_FOR_COUNTER CS_FOR_2_PRESC
  #define OCR_FOR_COUNTER 0xF9 //max period 0.26214 sec = (64)/(16 MHz) * 65535 / OCR_A = 250 -> 0.001 sec
#endif

extern Timer Timer1;
#endif /*timer_class_h_ */
