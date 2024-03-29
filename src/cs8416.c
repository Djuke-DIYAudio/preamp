#include "PreampController.h"
#include "cs8416.h"
#include "time_delay.h"

uint8_t _cs8416_addr = 0b00100000;	// MSB: 7-bit address of the CS8416 chip + LSB: R/W bit

void cs8416_write_register(uint8_t reg, uint8_t value)
{
	DAC_CS = 0;		// Enable SPI
	(void)spi_rw(_cs8416_addr); // Write operation (clear R/W bit)
	(void)spi_rw(reg);	// Select register
	(void)spi_rw(value);	// Write value
	DAC_CS = 1;		// Disable SPI

}

uint8_t cs8416_read_register(uint8_t reg)
{
	unsigned char temp;

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs8416_addr); // Write operation (clear R/W bit)
	temp = spi_rw(reg);	// Select register
	DAC_CS = 1;		// Disable SPI

	delay_us(1);

	DAC_CS = 0;		// Enable SPI
	temp = spi_rw(_cs8416_addr | 0b1); // Read operation (set R/W bit)
	temp = spi_rw(0x00);	// Empty output
	DAC_CS = 1;		// Disable SPI
	return temp;
}

bool cs8416_is_connected(void)
{
	if ((cs8416_read_register(CS8416_REG_ID_VERSION) >> 4) == 0b0010) return true;
	else return false;
}

void cs8416_set_input(uint8_t value)
{
	unsigned char data;

	data = 0b10000000;	// Set RUN mode

	data |= (value << 3);	// Set RX bits

	cs8416_write_register(CS8416_REG_CONTROL4, data);

//	delay_ms(50);	// Allow the PLL to settle
//	cs8416_read_register(CS8416_REG_RECEIVER_ERROR); // Read error register to clear errors
//	data = cs8416_read_register(CS8416_REG_RECEIVER_ERROR); // Read error register to clear errors
}

/*
unsigned char cs8416_is_format_pcm()
{
	unsigned char data;

	data = cs8416_read_register(CS8416_REG_AUDIO_FORMAT_DETECT);

	return (data & (1<<6) > 0 ? 1 : 0);
}

unsigned char cs8416_is_format_96khz()
{
	unsigned char data;

	data = cs8416_read_register(CS8416_REG_AUDIO_FORMAT_DETECT);

	return (data & (1<<0) > 0 ? 1 : 0);

}

unsigned char cs8416_pll_locked()
{
	unsigned char data;

	data = cs8416_read_register(CS8416_REG_RECEIVER_ERROR);

	return (data & (1<<4) > 0 ? 0 : 1);

}

unsigned char cs8416_data_valid()
{
	unsigned char data;

	data = cs8416_read_register(CS8416_REG_RECEIVER_ERROR);

	return (data & (1<<3) > 0 ? 0 : 1);

}*/

uint8_t cs8416_sample_rate(void)
{
	uint8_t data, rate;

	data = cs8416_read_register(CS8416_REG_CHA_STAT3);
	rate = data & 0b00001111;
	if (rate == 0b0100) return 22;
	else if (rate == 0b0000) return 44;
	else if (rate == 0b1000) return 88;
	else if (rate == 0b1100) return 176;
	else if (rate == 0b0110) return 24;
	else if (rate == 0b0010) return 48;
	else if (rate == 0b1010) return 96;
	else if (rate == 0b1110) return 192;
	else if (rate == 0b0011) return 32;
	else return 0;
}

uint8_t cs8416_bit_rate(void)
{
	uint8_t data, rate;

	data = cs8416_read_register(CS8416_REG_CHA_STAT4);
	rate = data & 0b00001111;
	if (rate == 0b1011) return 24;
	else if (rate == 0b0010) return 16;
	else return 0;
}
