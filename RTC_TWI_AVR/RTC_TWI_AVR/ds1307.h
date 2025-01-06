#ifndef DS1307_H_
#define DS1307_H_

#include "twi.h"

#define F_I2C 100000UL
#define F_CPU 16000000UL  // частота работы МК

// Структура хранения времени
typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t time_format; // 24/12
	uint8_t am_pm;		 // AM/PM
} Time;
// Первая половина дня / вторая половина дня
#define AM 0
#define PM 1

// Структура хранения даты
typedef struct {
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t day_week;
} Date;

// Значение - только устанавливаем адрес, не передавая данные
#define RTC_WRITE_POINTER   0xFF

// Адрес часов
#define DS1307_ADR			0b1101000

// Адреса регистров времени
#define RTC_SEC_ADR			0x00
#define RTC_MIN_ADR			0x01
#define RTC_HOUR_ADR		0x02

// Адреса регистров даты
#define RTC_DAY_WEEK_ADR    0x03
#define RTC_DAY_ADR			0x04
#define RTC_MONTH_ADR		0x05
#define RTC_YEAR_ADR		0x06

// Адрес регистра управления
#define RTC_CNTR_ADR		0x07

// Адрес оперативной памяти
#define RTC_RAM_ADR			0x08
#define RTC_RAM_END			0x3F

// Биты контрольного регистра
#define OUT	 7
#define SQWE 4
#define RS0  0
#define RS1	 1
// Бит Clock Hold
#define CH 7
// Биты регистра часов
#define TIME_FORMAT 6
#define AM_PM 5

// Маски для битов часов и секунд
#define HOUR_24_MASK 0b111111
#define HOUR_12_MASK 0b11111
#define SECONDS_MASK 0b1111111

// Установки частоты SQWE
#define F_1HZ   (0<<RS1)|(0<<RS0)
#define F_4KHZ  (0<<RS1)|(1<<RS0)
#define F_8HZ   (1<<RS1)|(0<<RS0)
#define F_32KHZ (1<<RS1)|(1<<RS0)

// Останов и запуск часов
#define STOP_CLOCK 0
#define START_CLOCK 1

void RTC_init(void);

void RTC_set_value(uint8_t address, uint8_t data);
void RTC_get_value(uint8_t * data);

void RTC_set_out(uint8_t out);
void RTC_set_sqwe(uint8_t frequency);

void RTC_start_stop_watch(uint8_t on);

void RTC_set_date(Date date);
void RTC_set_time(Time time);

void RTC_get_date(Date * date);
void RTC_get_time(Time * time);

uint8_t RTC_write_RAM(uint8_t address, uint8_t data);
uint8_t RTC_get_RAM(uint8_t address, uint8_t * data);

#endif /* DS1307_H_ */