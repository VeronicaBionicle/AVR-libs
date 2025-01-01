#include "ds1307.h"

uint8_t to_BCD(uint8_t n)
{
	return ((n/10) << 4) + n%10;
}

uint8_t from_BCD(uint8_t n)
{
	return 10 * (n >> 4) + (n & 0b1111);
}

/* Инициализация интерфейса I2C для часов */
void RTC_init(void)
{
	twi_init(0, F_CPU, F_I2C);
}

/* Запись значения в часы */
void RTC_set_value(uint8_t address, uint8_t data)
{
	/* Сформировать состояние СТАРТ */
	twi(TWI_START);
	
	/* Выдать SLA-W - ведомый в режиме приемника */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* Передать адрес регистра часов */
	twi_transmit(address);
	
	/* При передаче указателя передается только адрес без стоп состояния */
	if (data != RTC_WRITE_POINTER)
	{
		/* Передать данные */
		twi_transmit(data);
		
		/* Сформировать состояние СТОП */
		twi(TWI_STOP);
	}
}

/* Получить значение с часов */
void RTC_get_value(uint8_t * data)
{
	/* Сформировать состояние СТАРТ */
	twi(TWI_START);
	
	/* Выдать пакет SLA-R - ведомый в режиме передатчика */
	twi_transmit((DS1307_ADR<<1)|1);
	
	/* считываем данные без подтверждения */
	twi_receive(data, 0);
	
	/* Сформировать состояние СТОП */
	twi(TWI_STOP);
}

/* Установить дату */
void RTC_set_date(uint8_t day, uint8_t month, uint8_t year, uint8_t day_week)
{
	/* Перевести в BCD */
	year = to_BCD(year);
	month = to_BCD(month);
	day = to_BCD(day);

	/* Сформировать состояние СТАРТ */
	twi(TWI_START);

	/* Выдать SLA-W - ведомый в режиме приемника */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* Передать адрес регистра, с которого начинаем запись */
	twi_transmit(RTC_DAY_WEEK_ADR);
	twi_transmit(day_week);
	twi_transmit(day);
	twi_transmit(month);
	twi_transmit(year);
	
	/* Сформировать состояние СТОП */
	twi(TWI_STOP);
}

/* Установить время */
void RTC_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	/* Перевести в BCD */
	seconds = to_BCD(seconds);
	minutes = to_BCD(minutes);
	hours = to_BCD(hours);
	
	/* Сформировать состояние СТАРТ */
	twi(TWI_START);

	/* Выдать SLA-W - ведомый в режиме приемника */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* Передать адрес регистра, с которого начинаем запись */
	twi_transmit(RTC_SEC_ADR);
	twi_transmit(seconds);
	twi_transmit(minutes);
	twi_transmit(hours);
	
	/* Сформировать состояние СТОП */
	twi(TWI_STOP);
}

/* Настроить выход OUT */
void RTC_set_out(uint8_t out)
{
	uint8_t control_register = ((out&1)<<OUT)|(0<<SQWE);
	RTC_set_value(RTC_CNTR_ADR, control_register);
}

/* Настроить выход SQWE */
void RTC_set_sqwe(uint8_t frequency)
{
	uint8_t control_register = (0<<OUT)|(1<<SQWE)|frequency;
	RTC_set_value(RTC_CNTR_ADR, control_register);
}

/* Запустить или остановить часы */
void RTC_start_stop_watch(uint8_t on)
{
	/* Сохранить последнее значение секунд */
	uint8_t seconds_register;
	RTC_set_value(RTC_SEC_ADR, RTC_WRITE_POINTER);
	RTC_get_value(&seconds_register);
	
	/* Перевернуть бит Clock Hold */
	if (on)
	{
		seconds_register = seconds_register & ~(1<<CH);
	} else {
		seconds_register = seconds_register | (1<<CH); // установить бит равным 1 - остановить часы
	}
	
	RTC_set_value(RTC_SEC_ADR, seconds_register);
}

/* Получить дату */
void RTC_get_date(uint8_t * year, uint8_t * month, uint8_t * day, uint8_t * day_week)
{
	RTC_set_value(RTC_DAY_WEEK_ADR, RTC_WRITE_POINTER);
	
	/*Сформировать состояние РЕСТАРТ*/
	twi(TWI_RESTART);
	
	/*Выдать на шину пакет SLA-R*/
	twi_transmit((DS1307_ADR<<1)|1);
	
	/*считываем данные с подтверждением, кроме последнего байта */
	twi_receive(day_week, 1);
	twi_receive(day, 1);
	twi_receive(month, 1);
	twi_receive(year, 0);
	
	/*Сформировать состояние СТОП*/
	twi(TWI_STOP);
	
	/* Преобразовать из BCD в десятичное число */
	*day	  = from_BCD(*day);
	*month	  = from_BCD(*month);
	*year	  = from_BCD(*year);
}

/* Получить время */
void RTC_get_time(uint8_t * hours, uint8_t * minutes, uint8_t * seconds)
{
	/* Сбрасываем на область памяти */
	RTC_set_value(RTC_SEC_ADR, RTC_WRITE_POINTER);
	
	/*Сформировать состояние РЕСТАРТ*/
	twi(TWI_RESTART);
	
	/*Выдать на шину пакет SLA-R*/
	twi_transmit((DS1307_ADR<<1)|1);
	
	/*считываем данные с подтверждением, кроме последнего байта */
	twi_receive(seconds, 1);
	twi_receive(minutes, 1);
	twi_receive(hours, 0);
	
	/*Сформировать состояние СТОП*/
	twi(TWI_STOP);
	
	/* Преобразовать из BCD в десятичное число */
	*seconds  = from_BCD(*seconds & 0x7F);
	*minutes  = from_BCD(*minutes);
	*hours	  = from_BCD(*hours);
}

