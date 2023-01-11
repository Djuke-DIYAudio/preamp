#include "PreampController.h"
#include "mcp23s08.h"

static uint8_t _mcp23s08_addr = 0b01000110;	// MSB: 7-bit default address of the MCP23S08 chip + LSB: R/W bit

void mcp23s08_write_register(uint8_t reg, uint8_t value)
{
	INPUT_CS = 0;		// Enable SPI
	(void)spi_rw(_mcp23s08_addr); // Write operation (clear R/W bit)
	(void)spi_rw(reg);	// Write register value
	(void)spi_rw(value);	// Write value
	INPUT_CS = 1;		// Disable SPI

}

unsigned char mcp23s08_read_register(uint8_t reg)
{
	INPUT_CS = 0;		// Enable SPI
	(void)spi_rw(_mcp23s08_addr | 0b1); // Read operation (set R/W bit)
	(void)spi_rw(reg);	// Register
	uint8_t value = spi_rw(0x00);	// Empty output
	INPUT_CS = 1;		// Disable SPI
	return value;
}

void mcp23s08_set_gpio(uint8_t value) { mcp23s08_write_register(MCP23S08_REG_GPIO, value); }
//unsigned char mcp23s08_get_gpio() { return mcp23s08_read_register(MCP23S08_REG_GPIO); }

void mcp23s08_set_direction(uint8_t value) { mcp23s08_write_register(MCP23S08_REG_IODIR, value); }
unsigned char mcp23s08_get_direction() { return mcp23s08_read_register(MCP23S08_REG_IODIR); }

//void mcp23s08_set_pullup(uint8_t value) { mcp23s08_write_register(MCP23S08_REG_GPPU, value); }
//unsigned char mcp23s08_get_pullup() { return mcp23s08_read_register(MCP23S08_REG_GPPU); }
