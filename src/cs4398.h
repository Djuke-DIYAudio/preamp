#ifndef CS4398_H
#define CS4398_H

#include <stdbool.h>
#include <stdint.h>

// Read-only register
#define CS4398_REG_CHIP_ID              0x01	// Read chip-id
// R/W registers
#define CS4398_REG_MODE_CONTROL			0x02	// Control 1
#define CS4398_REG_VOL_CONTROL			0x03	// Control 2
#define CS4398_REG_MUTE_CONTROL			0x04	// Control 3
#define CS4398_REG_CHA_VOL_CONTROL		0x05	// Control 4
#define CS4398_REG_CHB_VOL_CONTROL		0x06	// 
#define CS4398_REG_RAMP_FILTER_CONTROL	0x07	// 
#define CS4398_REG_MISC_CONTROL			0x08	// 
#define CS4398_REG_MISC_CONTROL2		0x09	// 

// Function definitions
void cs4398_write_register(uint8_t reg, uint8_t value);
uint8_t cs4398_read_register(uint8_t reg);
bool cs4398_is_connected(void);

#endif