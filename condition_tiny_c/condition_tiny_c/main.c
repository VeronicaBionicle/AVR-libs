#define F_CPU 128000UL

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "ImpulseControl.h"
#include "ds18b20.h"

#define	DS18B20_PIN	PB2

#define OUT_PIN PB0
#define MAX_PERIODS 10

ImpulseControl_t ventilation =  {OUT_PIN, MAX_PERIODS, MAX_PERIODS};

#define WAIT_TIME 35000U
#define VALVE_PIN PB4

void init() {
	OutputInit(ventilation);
	ZerocrossInit();
	DDRB &= ~(1 << VALVE_PIN); /* Input for valve detection */
	PORTB &= ~(1 << VALVE_PIN);
	On(ventilation);	/* Enable ventilation on 100% */
	_delay_ms(WAIT_TIME); /* Wait for 35 s*/
};

int16_t temp = 25;

int main(void){
	init();
	while(1){
		if (PINB&(1 << VALVE_PIN)) {
				On(ventilation);	/* If valve is enabled, speed = 100% */
			} else {
			temp = DS18B20_read(DS18B20_PIN);
			/* Choose speed */
			if		(temp < 15) PhaseControl(ventilation, 2);
			else if (temp < 20) PhaseControl(ventilation, 3);
			else if (temp < 25) PhaseControl(ventilation, 4);
			else if (temp < 30) PhaseControl(ventilation, 6);
			else if (temp < 35) PhaseControl(ventilation, 8);
			else				On(ventilation);
		};
	}
}