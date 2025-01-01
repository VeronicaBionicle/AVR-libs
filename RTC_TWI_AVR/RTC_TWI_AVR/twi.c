#include "twi.h"

/* Инициализация скорости и делителя I2C */
uint8_t twi_init(uint8_t prescaler, uint32_t cpu_frequency, uint32_t frequency)
{
	uint32_t twbr = (cpu_frequency/frequency-16)/2;
	if ((twbr > 0xFF) || (twbr == 0))
	{
		return 0;
	}
	TWBR = twbr;
	TWSR = prescaler;
	return 1;
}

/* Команды I2C */
uint8_t twi(uint8_t action)
{
	switch(action){
		case TWI_START:
		case TWI_RESTART:
			TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
			break;
		case TWI_STOP:
			TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
			break;
		case TWI_TRANSMIT:
		case TWI_RECEIVE_NACK:
			TWCR = (1<<TWINT)|(1<<TWEN);
			break;
		case TWI_RECEIVE_ACK:
			TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
			break;
		default:
			break;
	}
	
	if (action != TWI_STOP)
	{
		while (!(TWCR & (1<<TWINT)));
	}
	
	uint8_t status = TWSR & 0xF8; // Статус выполнения (0xF8 = 11111000 - без двух настроечных и одного зарезервивованного бита)
	return status;
}

/* Передать данные */
uint8_t twi_transmit(uint8_t data)
{
	TWDR = data;
	return twi(TWI_TRANSMIT);
}

/* Получить данные */
uint8_t twi_receive(uint8_t * data, uint8_t with_ack)
{
	uint8_t status = twi(with_ack > 0 ? TWI_RECEIVE_ACK : TWI_RECEIVE_NACK);
	*data = TWDR;
	return status;
}