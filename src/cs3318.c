#include "PreampController.h"
#include "cs3318.h"
#include "time_delay.h"

#define MAX_CS3318  2

static uint8_t _cs3318_addr[MAX_CS3318];	// MSB: 7-bit base address of the CS3318 chip + LSB: R/W bit

void cs3318_reset_defaults(void)
{
	for(uint8_t i=0;i<MAX_CS3318;i++) {
		_cs3318_addr[i] = 0b10000000;
	}
}

void cs3318_set_device_id(uint8_t device, uint8_t address)
{
	if (device >= MAX_CS3318) return;

	// Set chip address and set ENOut to enable next device
	cs3318_write_register(device, CS3318_REG_INDIVIDUAL_CHIP_ADDRESS, address | 0b1);
	_cs3318_addr[device] = address;
}

void cs3318_write_register(uint8_t device, uint8_t reg, uint8_t value)
{
	uint8_t temp;

	if (device >= MAX_CS3318) return;

	VOLUME_CS = 0;		// Enable SPI
	delay_us(1);
	temp = spi_rw(_cs3318_addr[device]); // Write operation (clear R/W bit)
	temp = spi_rw(reg);	// Select register
	temp = spi_rw(value);	// Write value
	delay_us(1);
	VOLUME_CS = 1;		// Disable SPI
}

