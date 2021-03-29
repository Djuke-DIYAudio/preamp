#include "PreampController.h"
#include "cs3318.h"
#include "time_delay.h"

#define MAX_CS3318  2

static unsigned char _cs3318_addr[MAX_CS3318];	// 7-bit base address of the CS3318 chip

void cs3318_reset_defaults()
{
	unsigned char i;
	for(i=0;i<MAX_CS3318;i++) {
		_cs3318_addr[i] = 0b1000000;
	}
}

void cs3318_set_device_id(unsigned char device, unsigned char value)
{
	if (device >= MAX_CS3318) return;

	// Set chip address and set ENOut to enable next device
	cs3318_write_register(device, CS3318_REG_INDIVIDUAL_CHIP_ADDRESS, (value<<1)+1);
	_cs3318_addr[device] = value;
}

void cs3318_write_register(unsigned char device, unsigned char reg, unsigned char value)
{
	unsigned char temp;

	if (device >= MAX_CS3318) return;

	VOLUME_CS = 0;		// Enable SPI
	delay_us(1);
	temp = spi_rw(_cs3318_addr[device]<<1); // Write operation (shift address to left and clear R/W bit)
	temp = spi_rw(reg);	// Select register
	temp = spi_rw(value);	// Write value
	delay_us(1);
	VOLUME_CS = 1;		// Disable SPI
}

