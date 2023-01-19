#ifndef DAC_H
#define DAC_H

#include <stdint.h>
#include <stdbool.h>

// Function definitions
void dac_init(void);
bool dac_update(void);
bool dac_update_cs4398_connected(void);
bool dac_update_cs8416_connected(void);
bool dac_update_receiver_error(void);
bool dac_update_sample_rate(void);
bool dac_update_bit_rate(void);
bool dac_update_format_detect(void);

bool dac_is_connected(void);
void dac_set_input(uint8_t value);
const char *dac_rate_string(void);
const char *dac_error_string(void);
uint8_t dac_receiver_error(void);
void dac_set_volume(int16_t volume_half_db);
void dac_set_channel_offset(uint8_t channel, int8_t offset_half_db);
int8_t dac_get_channel_offset(uint8_t channel);
void dac_update_channel_offset(uint8_t channel);
int16_t dac_limit(int16_t input, int16_t min, int16_t max);
uint8_t dac_get_nr_channels(void);
void dac_mute(void);
void dac_unmute(void);

#endif
