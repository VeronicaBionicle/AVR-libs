#include "ds1307.h"

uint8_t to_BCD(uint8_t n)
{
	return ((n/10) << 4) + n%10;
}

uint8_t from_BCD(uint8_t n)
{
	return 10 * (n >> 4) + (n & 0b1111);
}

/* ������������� ���������� I2C ��� ����� */
void RTC_init(void)
{
	twi_init(0, F_CPU, F_I2C);
}

/* ������ �������� � ���� */
void RTC_set_value(uint8_t address, uint8_t data)
{
	/* ������������ ��������� ����� */
	twi(TWI_START);
	
	/* ������ SLA-W - ������� � ������ ��������� */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* �������� ����� �������� ����� */
	twi_transmit(address);
	
	/* ��� �������� ��������� ���������� ������ ����� ��� ���� ��������� */
	if (data != RTC_WRITE_POINTER)
	{
		/* �������� ������ */
		twi_transmit(data);
		
		/* ������������ ��������� ���� */
		twi(TWI_STOP);
	}
}

/* �������� �������� � ����� */
void RTC_get_value(uint8_t * data)
{
	/* ������������ ��������� ����� */
	twi(TWI_START);
	
	/* ������ ����� SLA-R - ������� � ������ ����������� */
	twi_transmit((DS1307_ADR<<1)|1);
	
	/* ��������� ������ ��� ������������� */
	twi_receive(data, 0);
	
	/* ������������ ��������� ���� */
	twi(TWI_STOP);
}

/* ���������� ���� */
void RTC_set_date(uint8_t day, uint8_t month, uint8_t year, uint8_t day_week)
{
	/* ��������� � BCD */
	year = to_BCD(year);
	month = to_BCD(month);
	day = to_BCD(day);

	/* ������������ ��������� ����� */
	twi(TWI_START);

	/* ������ SLA-W - ������� � ������ ��������� */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* �������� ����� ��������, � �������� �������� ������ */
	twi_transmit(RTC_DAY_WEEK_ADR);
	twi_transmit(day_week);
	twi_transmit(day);
	twi_transmit(month);
	twi_transmit(year);
	
	/* ������������ ��������� ���� */
	twi(TWI_STOP);
}

/* ���������� ����� */
void RTC_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	/* ��������� � BCD */
	seconds = to_BCD(seconds);
	minutes = to_BCD(minutes);
	hours = to_BCD(hours);
	
	/* ������������ ��������� ����� */
	twi(TWI_START);

	/* ������ SLA-W - ������� � ������ ��������� */
	twi_transmit((DS1307_ADR<<1)|0);
	
	/* �������� ����� ��������, � �������� �������� ������ */
	twi_transmit(RTC_SEC_ADR);
	twi_transmit(seconds);
	twi_transmit(minutes);
	twi_transmit(hours);
	
	/* ������������ ��������� ���� */
	twi(TWI_STOP);
}

/* ��������� ����� OUT */
void RTC_set_out(uint8_t out)
{
	uint8_t control_register = ((out&1)<<OUT)|(0<<SQWE);
	RTC_set_value(RTC_CNTR_ADR, control_register);
}

/* ��������� ����� SQWE */
void RTC_set_sqwe(uint8_t frequency)
{
	uint8_t control_register = (0<<OUT)|(1<<SQWE)|frequency;
	RTC_set_value(RTC_CNTR_ADR, control_register);
}

/* ��������� ��� ���������� ���� */
void RTC_start_stop_watch(uint8_t on)
{
	/* ��������� ��������� �������� ������ */
	uint8_t seconds_register;
	RTC_set_value(RTC_SEC_ADR, RTC_WRITE_POINTER);
	RTC_get_value(&seconds_register);
	
	/* ����������� ��� Clock Hold */
	if (on)
	{
		seconds_register = seconds_register & ~(1<<CH);
	} else {
		seconds_register = seconds_register | (1<<CH); // ���������� ��� ������ 1 - ���������� ����
	}
	
	RTC_set_value(RTC_SEC_ADR, seconds_register);
}

/* �������� ���� */
void RTC_get_date(uint8_t * year, uint8_t * month, uint8_t * day, uint8_t * day_week)
{
	RTC_set_value(RTC_DAY_WEEK_ADR, RTC_WRITE_POINTER);
	
	/*������������ ��������� �������*/
	twi(TWI_RESTART);
	
	/*������ �� ���� ����� SLA-R*/
	twi_transmit((DS1307_ADR<<1)|1);
	
	/*��������� ������ � ��������������, ����� ���������� ����� */
	twi_receive(day_week, 1);
	twi_receive(day, 1);
	twi_receive(month, 1);
	twi_receive(year, 0);
	
	/*������������ ��������� ����*/
	twi(TWI_STOP);
	
	/* ������������� �� BCD � ���������� ����� */
	*day	  = from_BCD(*day);
	*month	  = from_BCD(*month);
	*year	  = from_BCD(*year);
}

/* �������� ����� */
void RTC_get_time(uint8_t * hours, uint8_t * minutes, uint8_t * seconds)
{
	/* ���������� �� ������� ������ */
	RTC_set_value(RTC_SEC_ADR, RTC_WRITE_POINTER);
	
	/*������������ ��������� �������*/
	twi(TWI_RESTART);
	
	/*������ �� ���� ����� SLA-R*/
	twi_transmit((DS1307_ADR<<1)|1);
	
	/*��������� ������ � ��������������, ����� ���������� ����� */
	twi_receive(seconds, 1);
	twi_receive(minutes, 1);
	twi_receive(hours, 0);
	
	/*������������ ��������� ����*/
	twi(TWI_STOP);
	
	/* ������������� �� BCD � ���������� ����� */
	*seconds  = from_BCD(*seconds & 0x7F);
	*minutes  = from_BCD(*minutes);
	*hours	  = from_BCD(*hours);
}

