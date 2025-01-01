#include "uart.h"

char * data;			// буфер под передачу данных
uint8_t data_i = 0;		// текущее положение в буфере
uint8_t data_to_send = 0;	// сколько нужно отправить байт

/* Инициализация UART */
void init_UART()
{
	UCSR0A = 0;						// Асинхронный режим, 1 стоповый бит, без проверки четности
	UCSR0B = (1<<TXEN0);					// Включаем передатчик
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);	// 8 бит данных
	UBRR0H = HI(BAUDRATE_REG);				// Задаем скорость передачи
	UBRR0L = LO(BAUDRATE_REG);
}

/* Прерывание по опустошению буфера (окончанию передачи) */
ISR (USART_UDRE_vect)
{
	UDR0 = data[data_i];	// запихиваем следующий символ
	++data_i;		// увеличиваем счетчик
	
	if (data_i == data_to_send)	// если символы кончились
	{
		UCSR0B &=~(1<<UDRIE0);	// Запрещаем прерывание по опустошению - передача закончена
		cli();			// Отключаем прерывания
	}
}

/* Функция передачи байта по прерыванию */
void send_buffer_by_interrupt(char * buffer, uint8_t sz)
{
	data = buffer;		// Переносим буфер
	data_to_send = sz;	// Переносим размер
	data_i = 0;		// Инициализируем счетчик
	sei();			// Разрешаем прерывания
	UCSR0B |= (1<<UDRIE0);	// Разрешаем прерывание по "пустому" UDRE
}

/* Функция для передачи байта без прерываний */
void send_byte(uint8_t byte)
{
	while(!( UCSR0A & (1 << UDRE0))) ;	// ожидаем, пока UDR0 не будет нулевым (не закончится передача)
	UDR0 = byte;				// записываем байт в порт
}

/* Функция для передачи строки без прерываний */
void send_buffer(char * buffer, uint8_t sz)
{
	for (uint8_t i = 0; i < sz; ++i)
	send_byte(buffer[i]);
}