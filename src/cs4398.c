#include "PreampController.h"
#include "cs4398.h"
#include "time_delay.h"

static uint8_t _cs4398_addr = 0b10011000;	// MSB: 7-bit address of the CS4398 chip + LSB: R/W bit

void cs4398_write_register(uint8_t reg, uint8_t value)
{
	uint8_t temp;

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs4398_addr); // Write operation (clear R/W bit)
	temp = spi_rw(reg);	// Select register
	temp = spi_rw(value);	// Write value
	DAC_CS = 1;		// Disable SPI

}

uint8_t cs4398_read_register(uint8_t reg)
{
	uint8_t temp;

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs4398_addr); // Write operation (clear R/W bit)
	temp = spi_rw(reg);	// Select register
	DAC_CS = 1;		// Disable SPI

	delay_us(10);

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs4398_addr | 0b1); // Read operation (set R/W bit)
	temp = spi_rw(0x00);	// Empty output
	DAC_CS = 1;		// Disable SPI
	return temp;
}

bool cs4398_is_connected(void)
{
	uint8_t data;

	data = cs4398_read_register(CS4398_REG_CHIP_ID);

	if ((data >> 3) == 0b01110) return true;
	else return false;

}

