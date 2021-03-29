#ifndef CS8416_H
#define CS8416_H

// R/W registers
#define CS8416_REG_CONTROL0			0x00	// Control 0
#define CS8416_REG_CONTROL1			0x01	// Control 1
#define CS8416_REG_CONTROL2			0x02	// Control 2
#define CS8416_REG_CONTROL3			0x03	// Control 3
#define CS8416_REG_CONTROL4			0x04	// Control 4
#define CS8416_REG_SERIAL_AUDIO_DATA_FORMAT	0x05	// 
#define CS8416_REG_RECEIVER_ERROR_MASK		0x06	// 
#define CS8416_REG_INTERRUPT_MASK		0x07	// 
#define CS8416_REG_INTERRUPT_MODE_MSB		0x08	// 
#define CS8416_REG_INTERRUPT_MODE_LSB		0x09	// 
// Read-only registers
#define CS8416_REG_RECEIVER_CHANNEL_STATUS	0x0A	// 
#define CS8416_REG_AUDIO_FORMAT_DETECT		0x0B	// 
#define CS8416_REG_RECEIVER_ERROR		0x0C	// 
#define CS8416_REG_OMCK_RMCK_RATIO		0x18	// OMCK/RMCK ratio
#define CS8416_REG_CHA_STAT0			0x19	// Channel A status 0
#define CS8416_REG_CHA_STAT1			0x1A	// Channel A status 1
#define CS8416_REG_CHA_STAT2			0x1B	// Channel A status 2
#define CS8416_REG_CHA_STAT3			0x1C	// Channel A status 3
#define CS8416_REG_CHA_STAT4			0x1D	// Channel A status 4


#define CS8416_REG_ID_VERSION			0x7F	// ID & Version


// Function definitions
void cs8416_write_register(unsigned char reg, unsigned char value);
unsigned char cs8416_read_register(unsigned char reg);

unsigned char cs8416_is_connected();
void cs8416_set_input(unsigned char value);
unsigned char cs8416_is_format_pcm();
unsigned char cs8416_is_format_96khz();
unsigned char cs8416_pll_locked();
unsigned char cs8416_data_valid();

unsigned char cs8416_sample_rate();
unsigned char cs8416_bit_rate();

void cs8416_set_gpio(unsigned char value);
unsigned char cs8416_get_gpio();
void cs8416_set_direction(unsigned char value);
unsigned char cs8416_get_direction();
void cs8416_set_pullup(unsigned char value);
unsigned char cs8416_get_pullup();

#endif