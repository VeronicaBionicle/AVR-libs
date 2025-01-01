#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

// Команды I2C
enum twi_action
{
	TWI_START,
	TWI_RESTART,
	TWI_STOP,
	TWI_TRANSMIT,
	TWI_RECEIVE_ACK,
	TWI_RECEIVE_NACK
};

// Статусы
enum twi_status
{
	START_OK				= 0x08,	// состояние СТАРТ успешно передано
	RESTART_OK				= 0x10,	// состояние повторного СТАРТа успешно передано
	
	TRANSMIT_ADDRESS_ACK_OK = 0x18, // передан адресный пакет SLA-W и получено подтверждение от ведомого
	TRANSMIT_DATA_ACK_OK	= 0x28,	// передан байт данных и получено подтверждение от ведомого
	TRANSMIT_ADDRESS_NO_ACK = 0x20, // передан адресный пакет SLA-W, ведомый не дал подтверждения
	TRANSMIT_DATA_NO_ACK	= 0x30,	// передан байт данных, ведомый не дал подтверждения
	
	RECEIVE_ADDRESS_ACK_OK	= 0x40,	// передан адресный пакет SLA+R и получено подтверждение от ведомого
	RECEIVE_DATA_ACK_OK		= 0x50,	// принят байт данных и отправлено подтверждение ведомому
	RECEIVE_ADDRESS_NO_ACK	= 0x48,	// передан адресный пакет SLA+R, ведомый не дал подтверждения
	RECEIVE_DATA_NO_ACK		= 0x58,	// принят байт данных, ведомому не отправлено подтверждение 
	
	BUS_BUSY				= 0x38,	// потеря приоритета (кто-то уже занял шину)
	STATE_NOT_READY			= 0xF8,	// информация о состоянии не доступна, бит TWINT = 0
	BUS_ERROR				= 0x00	// ошибка шины из-за некорректных состояний СТАРТ или СТОП
};

uint8_t twi_init(uint8_t prescaler, uint32_t cpu_frequency, uint32_t frequency);
uint8_t twi(uint8_t action);
uint8_t twi_transmit(uint8_t data);
uint8_t twi_receive(uint8_t * data, uint8_t with_ack);
uint8_t simple_check_status(uint8_t status);

#endif /* TWI_H_ */