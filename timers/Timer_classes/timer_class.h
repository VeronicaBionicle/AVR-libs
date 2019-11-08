#ifndef timer_class_h_
#define timer_class_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define F_CPU_SH 16 //F_CPU/1000000 i had some problems with overflow

#define PRESCALER_1 8
#define PRESCALER_2 64
#define PRESCALER_3 256
#define PRESCALER_4 1024

#define OCR_FOR_COUNTER 0xF9 //max period 0.26214 sec = (64)/(16 MHz) * 65535 / OCR1A = 250 -> 0.001 sec
#define MICROS 0
#define MILLIS 1
#define SECS 1000
#define MINUTES 60000
#define HOURS 3600000

/*Choose your timer here*/
#define TIMER TIMER_0
//#define TIMER TIMER_1
//#define TIMER TIMER_2

#if TIMER == TIMER_0
  #define TIMER_INT TIMER0_COMPA_vect
  #define TIMSK_ TIMSK0
  #define TCCR_A TCCR0A
  #define TCCR_B TCCR0B
  #define OCR_A OCR0A
  #define CS_0 CS00
  #define CS_1 CS01
  #define CS_2 CS02
  #define OCIE_A OCIE0A
  #define OCR_MAX 256
#elif TIMER == TIMER_1
  #define TIMER_INT TIMER1_COMPA_vect
  #define TIMSK_ TIMSK1
  #define TCCR_A TCCR1A
  #define TCCR_B TCCR1B
  #define OCR_A OCR1A
  #define CS_0 CS10
  #define CS_1 CS11
  #define CS_2 CS12
  #define OCIE_A OCIE1A
  #define OCR_MAX 65536
#elif TIMER == TIMER_2
  #define TIMER_INT TIMER2_COMPA_vect
  #define TIMSK_ TIMSK2
  #define TCCR_A TCCR2A
  #define TCCR_B TCCR2B
  #define OCR_A OCR2A
  #define CS_0 CS20
  #define CS_1 CS21
  #define CS_2 CS22
  #define OCIE_A OCIE2A
  #define OCR_MAX 256
#endif


class Timer {
  private:
    uint64_t period, milliseconds, expirations;
  public:
    void startTimerForInterrupt(uint32_t);
    void startCounter(uint32_t, uint32_t);
    void stopTimerCounter();
    void clearCounterExp();
    uint32_t getCounterExp();
    uint32_t getPeriods();

    void attachInterrupt(void (*isr)()) __attribute__((always_inline)) {
      isrCallback = isr;
    };

    static void (*isrCallback)();
    static void isrDefaultUnused();
};

extern Timer Timer1;
#endif /*timer_class_h_ */
