#ifndef timer_class_h_
#define timer_class_h_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define MILLIS 1
#define SECS 975    //must be 1000
#define MINUTES 58500   //must be 60000, but my arduinos "minute" is 1 min + 1.5 sec
#define HOURS 3600000

class Timer{
private:
     uint64_t period, milliseconds, expirations;
public:
    Timer(uint32_t,  uint32_t);
    void startTimer(uint8_t);
    void stopTimer();
    void clearExp();
    uint32_t getExp();
    uint32_t getMillis();
    uint32_t getPeriods(uint32_t);
};
#endif /*timer_class_h_ */
