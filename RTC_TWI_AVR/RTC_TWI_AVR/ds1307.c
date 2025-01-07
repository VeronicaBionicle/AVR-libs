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
	twi_init(F_CPU, F_I2C);
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
	if (on == START_CLOCK)
	{
		seconds_register = seconds_register & ~(1<<CH); // установить бит равным 0 - запустить часы
	} else {
		seconds_register = seconds_register | (1<<CH); // установить бит равным 1 - остановить часы
	}
	
	RTC_set_value(RTC_SEC_ADR, seconds_register);
}

/* Установить дату */
void RTC_set_date(Date date)
{
	/* Перевести в BCD */
	date.year = to_BCD(date.year);
	date.month = to_BCD(date.month);
	date.day = to_BCD(date.day);

	/* Сформировать состояние СТАРТ */
	twi(TWI_START);

	/* Выдать SLA-W - ведомый в режиме приемника */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* Передать адрес регистра, с которого начинаем запись */
	twi_transmit(RTC_DAY_WEEK_ADR);
	twi_transmit(date.day_week);
	twi_transmit(date.day);
	twi_transmit(date.month);
	twi_transmit(date.year);
	
	/* Сформировать состояние СТОП */
	twi(TWI_STOP);
}

/* Установить время */
void RTC_set_time(Time time)
{
	/* Перевести в BCD */
	time.seconds = to_BCD(time.seconds);
	time.minutes = to_BCD(time.minutes);
	if (time.time_format == 12)
	{
		time.hours = (time.hours%12 == 0 ? 12U : time.hours%12); // на всякий случай подрежем
		time.hours = to_BCD(time.hours) | (1<<TIME_FORMAT) | (time.am_pm << AM_PM); // Добавим биты для 12-часового формата
	} else {
		time.hours = to_BCD(time.hours); // тут настроечные биты оставляем нулевыми
	}
	
	/* Сформировать состояние СТАРТ */
	twi(TWI_START);

	/* Выдать SLA-W - ведомый в режиме приемника */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* Передать адрес регистра, с которого начинаем запись */
	twi_transmit(RTC_SEC_ADR);
	twi_transmit(time.seconds);
	twi_transmit(time.minutes);
	twi_transmit(time.hours);
	
	/* Сформировать состояние СТОП */
	twi(TWI_STOP);
}

/* Получить дату */
void RTC_get_date(Date * date)
{
	/* Установить указатель на регистр дня недели */
	RTC_set_value(RTC_DAY_WEEK_ADR, RTC_WRITE_POINTER);
	
	/* Сформировать состояние РЕСТАРТ */
	twi(TWI_RESTART);
	
	/* Выдать пакет SLA-R - ведомый в режиме передатчика */
	twi_transmit((DS1307_ADR<<1)|1);
	
	/* Считать данные с подтверждением, кроме последнего байта */
	twi_receive(&date->day_week, 1);
	twi_receive(&date->day, 1);
	twi_receive(&date->month, 1);
	twi_receive(&date->year, 0);	// завершающий прием без подтверждения
	
	/*Сформировать состояние СТОП*/
	twi(TWI_STOP);
	
	/* Преобразовать из BCD в десятичное число */
	date->day	  = from_BCD(date->day);
	date->month	  = from_BCD(date->month);
	date->year	  = from_BCD(date->year);
}

/* Получить время */
void RTC_get_time(Time * time)
{
	/* Сбрасываем на область памяти */
	RTC_set_value(RTC_SEC_ADR, RTC_WRITE_POINTER);
	
	/* Сформировать состояние РЕСТАРТ */
	twi(TWI_RESTART);
	
	/* Выдать пакет SLA-R - ведомый в режиме передатчика */
	twi_transmit((DS1307_ADR<<1)|1);
	
	/* Считать данные с подтверждением, кроме последнего байта */
	twi_receive(&time->seconds, 1);
	twi_receive(&time->minutes, 1);
	twi_receive(&time->hours, 0); // завершающий прием без подтверждения
	
	/* Сформировать состояние СТОП */
	twi(TWI_STOP);
	
	/* Преобразовать из BCD в десятичное число */
	time->seconds  = from_BCD(time->seconds & SECONDS_MASK);
	time->minutes  = from_BCD(time->minutes);
	time->time_format = (time->hours & (1<<TIME_FORMAT) ? 12 : 24);
	if (time->time_format == 24)
	{
		time->am_pm = (from_BCD(time->hours) > 12U ? PM: AM);
	} else {
		time->am_pm = (time->hours & (1<<AM_PM) ? PM : AM); // В режиме 12 часов бит 5 дает 1 для PM и 0 для AM
		time->hours &= HOUR_12_MASK;						// уберем лишние биты
	}
	
	time->hours	  = from_BCD(time->hours);
}

/* Запись в оперативную память часов */
uint8_t RTC_write_RAM(uint8_t address, uint8_t data)
{
	if (address < RTC_RAM_ADR || address > RTC_RAM_END)
		return 0;	// ничего не записали
				
	RTC_set_value(address, data);
	return 1;
}

/* Чтение из оперативной памяти часов */
uint8_t RTC_get_RAM(uint8_t address, uint8_t * data)
{
	if (address < RTC_RAM_ADR || address > RTC_RAM_END)
		return 0;	// ничего не считали

	// Устанавливаем указатель на нужный адрес и считываем данные
	RTC_set_value(address, RTC_WRITE_POINTER);
	RTC_get_value(data);
	return 1;
}

