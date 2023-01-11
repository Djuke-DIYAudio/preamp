#ifndef CS8416_H
#define CS8416_H

#include <stdbool.h>
#include <stdint.h>

// R/W registers
#define CS8416_REG_CONTROL0                 0x00	// Control 0
#define CS8416_REG_CONTROL1                 0x01	// Control 1
#define CS8416_REG_CONTROL2                 0x02	// Control 2
#define CS8416_REG_CONTROL3                 0x03	// Control 3
#define CS8416_REG_CONTROL4                 0x04	// Control 4
#define CS8416_REG_SERIAL_AUDIO_DATA_FORMAT	0x05	// 
#define CS8416_REG_RECEIVER_ERROR_MASK		0x06	// 
#define CS8416_REG_INTERRUPT_MASK           0x07	// 
#define CS8416_REG_INTERRUPT_MODE_MSB		0x08	// 
#define CS8416_REG_INTERRUPT_MODE_LSB		0x09	// 

// Read-only registers
#define CS8416_REG_RECEIVER_CHANNEL_STATUS	0x0A	// 
#define CS8416_REG_AUDIO_FORMAT_DETECT		0x0B	// 
#define CS8416_REG_RECEIVER_ERROR           0x0C	// 
#define CS8416_REG_OMCK_RMCK_RATIO          0x18	// OMCK/RMCK ratio
#define CS8416_REG_CHA_STAT0                0x19	// Channel A status 0
#define CS8416_REG_CHA_STAT1                0x1A	// Channel A status 1
#define CS8416_REG_CHA_STAT2                0x1B	// Channel A status 2
#define CS8416_REG_CHA_STAT3                0x1C	// Channel A status 3
#define CS8416_REG_CHA_STAT4                0x1D	// Channel A status 4

#define CS8416_REG_ID_VERSION               0x7F	// ID & Version


// Function definitions
void cs8416_write_register(uint8_t reg, uint8_t value);
uint8_t cs8416_read_register(uint8_t reg);
bool cs8416_is_connected(void);
void cs8416_set_input(unsigned char value);
bool cs8416_is_format_pcm(void);
bool cs8416_is_format_96khz(void);
bool cs8416_pll_locked(void);
bool cs8416_data_valid(void);
uint8_t cs8416_sample_rate(void);
uint8_t cs8416_bit_rate(void);
void cs8416_set_gpio(uint8_t value);
uint8_t cs8416_get_gpio(void);
void cs8416_set_direction(uint8_t value);
uint8_t cs8416_get_direction(void);
void cs8416_set_pullup(uint8_t value);
uint8_t cs8416_get_pullup(void);

#endif