#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL  // ������� ������ ��

#include <avr/interrupt.h>

#define BAUDRATE 9600L  // �������� �������� �� UART
#define BAUDRATE_REG (F_CPU/(16*BAUDRATE)-1)

/* ���������� ��� ��������� �������� � �������� ������ */
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)

void init_UART();

void send_buffer_by_interrupt(char * buffer, uint8_t sz);

void send_byte(uint8_t byte);
void send_buffer(char * buffer, uint8_t sz);

#endif /* UART_H_ */