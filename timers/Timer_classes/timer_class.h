#ifndef timer_class_h_
#define timer_class_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define F_CPU_SH 16 //F_CPU/1000000 i had some problems with overflow
#define OCR_MAX 256

#define PRESCALER_1 8
#define PRESCALER_2 64
#define PRESCALER_3 256
#define PRESCALER_4 1024

/*Choose your timer here*/
#define TIMER TIMER_0
//#define TIMER TIMER_1
//#define TIMER TIMER_2

#if TIMER == TIMER_0
#define TIMER_INT TIMER0_COMPA_vect
#elif TIMER == TIMER_1
#define TIMER_INT TIMER1_COMPA_vect
#elif TIMER == TIMER_2
#define TIMER_INT TIMER2_COMPA_vect
#endif

#define MILLIS 1
#define SECS 975    //must be 1000
#define MINUTES 58500   //must be 60000, but my arduinos "minute" is 1 min + 1.5 sec
#define HOURS 3600000

class Timer {
  private:
    uint64_t period, milliseconds, expirations;
  public:
    Timer();
    void startTimer(uint32_t);
    void stopTimer();
    void clearExp();
    uint32_t getExp();
    uint32_t getMillis();
    uint32_t getPeriods(uint32_t);

    void attachInterrupt(void (*isr)()) __attribute__((always_inline)) {
      isrCallback = isr;
    };

    static void (*isrCallback)();
    static void isrDefaultUnused();
};

extern Timer Timer1;
#endif /*timer_class_h_ */
