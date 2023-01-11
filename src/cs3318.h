#ifndef CS3318_H
#define CS3318_H

#include <stdint.h>

// R/W registers
#define CS3318_REG_CH1_VOLUME               0x01	// Channel 1 volume
#define CS3318_REG_CH2_VOLUME               0x02	// Channel 2 volume
#define CS3318_REG_CH3_VOLUME               0x03	// Channel 3 volume
#define CS3318_REG_CH4_VOLUME               0x04	// Channel 4 volume
#define CS3318_REG_CH5_VOLUME               0x05	// Channel 5 volume
#define CS3318_REG_CH6_VOLUME               0x06	// Channel 6 volume
#define CS3318_REG_CH7_VOLUME               0x07	// Channel 7 volume
#define CS3318_REG_CH8_VOLUME               0x08	// Channel 8 volume
#define CS3318_REG_QUARTER_DB_CONTROL       0x09	// Control 1/4db
#define CS3318_REG_MUTE_CONTROL             0x0A	// Mute control
#define CS3318_REG_DEVICE_CONFIG_1          0x0B	// Device config 1
#define CS3318_REG_DEVICE_CONFIG_2          0x0C	// Device config 2
#define CS3318_REG_CHANNEL_POWER            0x0D	// Channel power
#define CS3318_REG_MASTER_POWER             0x0E	// Master power
#define CS3318_REG_FREEZE_CONTROL           0x0F	// Freeze control
#define CS3318_REG_MASTER_1_MASK            0x10	// Master 1 mask
#define CS3318_REG_MASTER_1_VOLUME          0x11	// Master 1 volume
#define CS3318_REG_MASTER_1_CONTROL         0x12	// Master 1 control
#define CS3318_REG_MASTER_2_MASK            0x13	// Master 2 mask
#define CS3318_REG_MASTER_2_VOLUME          0x14	// Master 2 volume
#define CS3318_REG_MASTER_2_CONTROL         0x15	// Master 2 control
#define CS3318_REG_MASTER_3_MASK            0x16	// Master 3 mask
#define CS3318_REG_MASTER_3_VOLUME          0x17	// Master 3 volume
#define CS3318_REG_MASTER_3_CONTROL         0x18	// Master 3 control
#define CS3318_REG_GROUP_2_CHIP_ADDRESS		0x19	// Group 2 chip address
#define CS3318_REG_GROUP_1_CHIP_ADDRESS		0x1A	// Group 1 chip address
#define CS3318_REG_INDIVIDUAL_CHIP_ADDRESS	0x1B	// Individual chip address


// Function definitions
void cs3318_reset_defaults(void);
void cs3318_write_register(uint8_t device, uint8_t reg, uint8_t value);
void cs3318_set_device_id(uint8_t device, uint8_t address);
#endif
