#include "PreampController.h"
#include "cs4398.h"
#include "time_delay.h"

static unsigned char _cs4398_addr = 0b1001100;	// 7-bit address of the CS4398 chip

void cs4398_write_register(unsigned char reg, unsigned char value)
{
	unsigned char temp;

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs4398_addr<<1); // Write operation (shift address to left and clear R/W bit)
	temp = spi_rw(reg);	// Select register
	temp = spi_rw(value);	// Write value
	DAC_CS = 1;		// Disable SPI

}

unsigned char cs4398_read_register(unsigned char reg)
{
	unsigned char temp;

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs4398_addr<<1); // Write operation (shift address to left and clear R/W bit)
	temp = spi_rw(reg);	// Select register
	DAC_CS = 1;		// Disable SPI

	delay_us(10);

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw((_cs4398_addr<<1) | 0x01); // Read operation (shift address to left and set R/W bit)
	temp = spi_rw(0x00);	// Empty output
	DAC_CS = 1;		// Disable SPI
	return temp;
}

unsigned char cs4398_is_connected()
{
	unsigned char data;

	data = cs4398_read_register(CS4398_REG_CHIP_ID);

	if ((data >> 3) == 0b01110) return 1;
	else return 0;

}

