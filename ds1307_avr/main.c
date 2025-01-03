#include <mega328p.h>
#include <i2c.h>    // ������� I2C
#include <ds1307.h> // ������� DS1307
#include <stdio.h>  // ������� ������������ �����-������
#include <delay.h>  // ������� ��������

#define RS0 0
#define RS1 1 

/* ��������� ��� UART */
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)

#define F_CPU 16000000UL  // ������� ��
#define BAUDRATE 9600L  // �������� �������� UART
#define BAUDRATE_REG (F_CPU/(16*BAUDRATE)-1) 

// ������������� UART
void init_UART()
{
	UCSR0A = 0;						                    // ����������� �����, 1 �������� ���, ��� �������� ��������
	UCSR0B = (1<<TXEN0);					            // �������� ����������
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);	// 8 ��� ������
	UBRR0H = HI(BAUDRATE_REG);				            // ������ �������� ��������
	UBRR0L = LO(BAUDRATE_REG);
}

// ��� ������
const char * weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void main(void)
{
    // ��������� �����
    unsigned char rs = (1 << RS1)|(1 << RS0); 
    unsigned char sqwe = 1;
    unsigned char out = 0;
    
    // ���� � �����
    unsigned char week_day, day, month, year; 
    unsigned char hour, minute, second;         

    // �������� �������� �������: 1
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif
    
    // ��������� ���������� ����������
    ACSR=(1<<ACD);
    // ��������� ���
    ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
    
    // ��������� UART
    init_UART();
    
    // ��������� I2C
    i2c_init();

    // ��������� �����
    rtc_init(rs, sqwe, out); 
                
    // ��������� ����: ���� ������, ����, �����, ���  
    rtc_set_date(6, 20, 12, 24);      
    
    // ��������� �������: ����, ������, �������
    rtc_set_time(16, 1, 30);
    
    while (1) // ������� ����� ������ �������
    {
        rtc_get_date(&week_day,&day,&month,&year);    
        printf("%02u\\%02u\\%02u %p\r", day, month, year, weekdays[week_day-1]); 
        
        rtc_get_time(&hour,&minute,&second);
        printf("%02u:%02u:%02u\r", hour, minute, second);         
        
        delay_ms(1000);
    }
}