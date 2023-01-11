#ifndef MCP23S08_H
#define MCP23S08_H

#include <xc.h>

// Registers
#define MCP23S08_REG_IODIR		0x00	// IO direction
#define MCP23S08_REG_IPOL		0x01	// Input polarity
#define MCP23S08_REG_GPINTEN	0x02	// Interrupt on-change
#define MCP23S08_REG_DEFVAL		0x03	// Default compare
#define MCP23S08_REG_INTCON		0x04	// Interrupt control
#define MCP23S08_REG_IOCON		0x05	// Configuration
#define MCP23S08_REG_GPPU		0x06	// Pull-up resistor
#define MCP23S08_REG_INTF		0x07	// Interrupt flag
#define MCP23S08_REG_INTCAP		0x08	// Interrupt capture
#define MCP23S08_REG_GPIO		0x09	// Outputs

// Function definitions
void mcp23s08_write_register(uint8_t reg, uint8_t value);
unsigned char mcp23s08_read_register(uint8_t  reg);
void mcp23s08_set_gpio(uint8_t  value);
unsigned char mcp23s08_get_gpio(void);
void mcp23s08_set_direction(uint8_t value);
unsigned char mcp23s08_get_direction(void);
void mcp23s08_set_pullup(uint8_t value);
unsigned char mcp23s08_get_pullup(void);

#endif