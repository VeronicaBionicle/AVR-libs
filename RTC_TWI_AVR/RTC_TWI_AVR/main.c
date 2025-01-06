#define F_CPU 16000000UL  // частота работы МК
#define F_I2C 100000UL

#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "ds1307.h"

char mes[20];

const char * weekdays[7] =
	{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int main(void)
{
	// Предделитель частоты 1
	CLKPR=(1<<CLKPCE);
	CLKPR=0;
	
	init_UART();
	
	RTC_init();
	
	// Запись во второй байт памяти числа 136
	uint8_t ram_status = RTC_write_RAM(RTC_RAM_ADR+1, 136);
	
	if (ram_status)
	{
		// Чтение из RAM
		uint8_t ram_data;
		ram_status = RTC_get_RAM(RTC_RAM_ADR+1, &ram_data);
		
		if (ram_status)
		{
			sprintf(mes, "read from RAM: %u", ram_data);
			send_buffer(mes, sizeof(mes) / sizeof(char));
			send_byte('\r');
		}
	}
	
	RTC_set_out(1); // На выходе OUT=1
	_delay_ms(200);
	
	RTC_set_out(0); // На выходе OUT=0
	_delay_ms(200);
	
	RTC_set_sqwe(F_1HZ); // Включить сигнал частотой 1 Гц
	
	Date date = {28, 12, 24, 7}; // установить дату 28.12.2024 Суббота
	RTC_set_date(date);
	
	Time time = { 13, 45, 37, 24, PM }; // установить время 13:45:37 в формате 24 часа PM
	RTC_set_time(time);	 
	
	RTC_start_stop_watch(STOP_CLOCK);	// выключить часы

	int ticks = 0;
	
	/* Пишем в порт дату и время */
	while (1)
	{
		// Через 2 секунды включить часы
		if (ticks < 2)
		{
			++ticks;
			} else if (ticks == 2) {
			RTC_start_stop_watch(START_CLOCK);
			++ticks;
			
			Time new_time = { 11, 59, 58, 12, AM }; // установить время 11:59:58 в формате 12 часов AM
			RTC_set_time(new_time);
			
			Date new_date = { 6, 1, 25, 2 }; // установить дату 06.01.2025 Понедельник
			RTC_set_date(new_date);
		}
		
		RTC_get_time(&time);
		sprintf(mes, "%02u:%02u:%02u %u %s\r", time.hours, time.minutes, time.seconds, time.time_format, time.am_pm == AM ? "AM" : "PM");
		send_buffer(mes, strlen(mes));
		
		RTC_get_date(&date);
		sprintf(mes, "%02u.%02u.%02u %s\r", date.day, date.month, date.year, weekdays[date.day_week-1]);
		send_buffer(mes, strlen(mes));
		
		_delay_ms(1000);
	}
}