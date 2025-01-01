#include "uart.h"

char * data;			// ����� ��� �������� ������
uint8_t data_i = 0;		// ������� ��������� � ������
uint8_t data_to_send = 0;	// ������� ����� ��������� ����

/* ������������� UART */
void init_UART()
{
	UCSR0A = 0;						// ����������� �����, 1 �������� ���, ��� �������� ��������
	UCSR0B = (1<<TXEN0);					// �������� ����������
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);	// 8 ��� ������
	UBRR0H = HI(BAUDRATE_REG);				// ������ �������� ��������
	UBRR0L = LO(BAUDRATE_REG);
}

/* ���������� �� ����������� ������ (��������� ��������) */
ISR (USART_UDRE_vect)
{
	UDR0 = data[data_i];	// ���������� ��������� ������
	++data_i;		// ����������� �������
	
	if (data_i == data_to_send)	// ���� ������� ���������
	{
		UCSR0B &=~(1<<UDRIE0);	// ��������� ���������� �� ����������� - �������� ���������
		cli();			// ��������� ����������
	}
}

/* ������� �������� ����� �� ���������� */
void send_buffer_by_interrupt(char * buffer, uint8_t sz)
{
	data = buffer;		// ��������� �����
	data_to_send = sz;	// ��������� ������
	data_i = 0;		// �������������� �������
	sei();			// ��������� ����������
	UCSR0B |= (1<<UDRIE0);	// ��������� ���������� �� "�������" UDRE
}

/* ������� ��� �������� ����� ��� ���������� */
void send_byte(uint8_t byte)
{
	while(!( UCSR0A & (1 << UDRE0))) ;	// �������, ���� UDR0 �� ����� ������� (�� ���������� ��������)
	UDR0 = byte;				// ���������� ���� � ����
}

/* ������� ��� �������� ������ ��� ���������� */
void send_buffer(char * buffer, uint8_t sz)
{
	for (uint8_t i = 0; i < sz; ++i)
	send_byte(buffer[i]);
}