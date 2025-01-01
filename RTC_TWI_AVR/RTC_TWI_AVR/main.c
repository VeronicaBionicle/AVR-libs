#define F_CPU 16000000UL  // частота работы МК
#define F_I2C 100000UL

#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "ds1307.h"

char mes[20];

uint8_t hours, minutes, seconds, day_week, day, month, year;

const char * weekdays[7] =
{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int main(void)
{
	// Предделитель частоты 1
	CLKPR=(1<<CLKPCE);
	CLKPR=0;
	
	init_UART();
	
	RTC_init();
	
	RTC_set_out(1); // На выходе OUT=1
	_delay_ms(2000);
	
	RTC_set_out(0); // На выходе OUT=0
	_delay_ms(2000);
	
	RTC_set_sqwe(F_1HZ); // Включить сигнал частотой 1 Гц
	
	RTC_set_date(28, 12, 24, 7); // установить дату 28.12.2024 Суббота
	RTC_set_time(13, 45, 32);	 // установить время 13:45:32
	
	RTC_start_stop_watch(0);	// выключить часы
	
	int ticks = 0;
	
	/* Пишем в порт разными способами */
	while (1)
	{
		RTC_get_time(&hours, &minutes, &seconds);
		sprintf(mes, "%02u:%02u:%02u", hours, minutes, seconds);
		send_buffer(mes, 8);
		send_byte('\r');
		
		RTC_get_date(&year, &month, &day, &day_week);
		sprintf(mes, "%02u.%02u.%u %s ", day, month, year, weekdays[day_week-1]);
		send_buffer(mes, sizeof(mes) / sizeof(char));
		send_byte('\r');
		
		// Через 5 секунд включить часы
		if (ticks < 10)
		{
			++ticks;
		} else if (ticks == 10) {
			RTC_start_stop_watch(1);
			++ticks;
		}
		
		_delay_ms(500);
	}
}