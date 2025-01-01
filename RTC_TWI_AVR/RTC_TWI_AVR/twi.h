#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

// ������� I2C
enum twi_action
{
	TWI_START,
	TWI_RESTART,
	TWI_STOP,
	TWI_TRANSMIT,
	TWI_RECEIVE_ACK,
	TWI_RECEIVE_NACK
};

// �������
enum twi_status
{
	START_OK				= 0x08,	// ��������� ����� ������� ��������
	RESTART_OK				= 0x10,	// ��������� ���������� ������ ������� ��������
	
	TRANSMIT_ADDRESS_ACK_OK = 0x18, // ������� �������� ����� SLA-W � �������� ������������� �� ��������
	TRANSMIT_DATA_ACK_OK	= 0x28,	// ������� ���� ������ � �������� ������������� �� ��������
	TRANSMIT_ADDRESS_NO_ACK = 0x20, // ������� �������� ����� SLA-W, ������� �� ��� �������������
	TRANSMIT_DATA_NO_ACK	= 0x30,	// ������� ���� ������, ������� �� ��� �������������
	
	RECEIVE_ADDRESS_ACK_OK	= 0x40,	// ������� �������� ����� SLA+R � �������� ������������� �� ��������
	RECEIVE_DATA_ACK_OK		= 0x50,	// ������ ���� ������ � ���������� ������������� ��������
	RECEIVE_ADDRESS_NO_ACK	= 0x48,	// ������� �������� ����� SLA+R, ������� �� ��� �������������
	RECEIVE_DATA_NO_ACK		= 0x58,	// ������ ���� ������, �������� �� ���������� ������������� 
	
	BUS_BUSY				= 0x38,	// ������ ���������� (���-�� ��� ����� ����)
	STATE_NOT_READY			= 0xF8,	// ���������� � ��������� �� ��������, ��� TWINT = 0
	BUS_ERROR				= 0x00	// ������ ���� ��-�� ������������ ��������� ����� ��� ����
};

uint8_t twi_init(uint8_t prescaler, uint32_t cpu_frequency, uint32_t frequency);
uint8_t twi(uint8_t action);
uint8_t twi_transmit(uint8_t data);
uint8_t twi_receive(uint8_t * data, uint8_t with_ack);
uint8_t simple_check_status(uint8_t status);

#endif /* TWI_H_ */