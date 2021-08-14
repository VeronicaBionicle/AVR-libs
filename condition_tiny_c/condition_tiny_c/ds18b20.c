/**
 * Copyright (c) 2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 *
 * This is tinyAVR (ATtiny13, ATtiny25, ATtiny45, ATtiny85) library for DS18B20 temperature sensor.
 */

#include "onewire.h"
#include "ds18b20.h"

int16_t
DS18B20_read(uint8_t pin)
{
	uint8_t msb, lsb;
	uint16_t t;

	onewire_reset(pin);
	onewire_write(pin, ONEWIRE_SKIP_ROM);
	onewire_write(pin, DS18B20_CONVERT_T);

	onewire_reset(pin);
	onewire_write(pin, ONEWIRE_SKIP_ROM);
	onewire_write(pin, DS18B20_READ);

	lsb = onewire_read(pin);
	msb = onewire_read(pin);
	onewire_reset(pin);
	
	t = (lsb >> 4) | (msb&0x7 << 4);
	
	return t;
}

