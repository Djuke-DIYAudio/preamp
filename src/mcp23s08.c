#include "PreampController.h"
#include "mcp23s08.h"

static unsigned char _mcp23s08_addr = 0b0100011;	// 7-bit default address of the MCP23S08 chip

void mcp23s08_write_register(unsigned char reg, unsigned char value)
{
	unsigned char temp;

	INPUT_CS = 0;		// Enable SPI
	temp = spi_rw(_mcp23s08_addr<<1); // Write operation (shift address to left and clear R/W bit)
	temp = spi_rw(reg);	// Write register value
	temp = spi_rw(value);	// Write value
	INPUT_CS = 1;		// Disable SPI

}

unsigned char mcp23s08_read_register(unsigned char reg)
{
	unsigned char temp;

	INPUT_CS = 0;		// Enable SPI
	temp = spi_rw((_mcp23s08_addr<<1) | 0x01); // Read operation (shift address to left and set R/W bit)
	temp = spi_rw(reg);	// Register
	temp = spi_rw(0x00);	// Empty output
	INPUT_CS = 1;		// Disable SPI
	return temp;
}

void mcp23s08_set_gpio(unsigned char value) { mcp23s08_write_register(MCP23S08_REG_GPIO, value); }
unsigned char mcp23s08_get_gpio() { return mcp23s08_read_register(MCP23S08_REG_GPIO); }

void mcp23s08_set_direction(unsigned char value) { mcp23s08_write_register(MCP23S08_REG_IODIR, value); }
unsigned char mcp23s08_get_direction() { return mcp23s08_read_register(MCP23S08_REG_IODIR); }

void mcp23s08_set_pullup(unsigned char value) { mcp23s08_write_register(MCP23S08_REG_GPPU, value); }
unsigned char mcp23s08_get_pullup() { return mcp23s08_read_register(MCP23S08_REG_GPPU); }
