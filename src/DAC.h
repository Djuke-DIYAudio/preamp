#ifndef DAC_H
#define DAC_H

#include <stdint.h>
#include <stdbool.h>

// Function definitions
void dac_init(void);
bool dac_is_connected(void);
void dac_set_input(uint8_t value);
const char *dac_rate_string(void);
const char *dac_error_string(void);
uint8_t dac_receiver_error(void);
bool dac_receiver_error_changed(void);
bool dac_sample_rate_changed(void);
bool dac_bit_rate_changed(void);
bool dac_input_changed(void);
bool dac_connection_changed(void);
void dac_set_volume(int16_t volume_half_db);
int16_t dac_get_volume(void);
void dac_set_channel_offset(uint8_t channel, int8_t offset_half_db);
int8_t dac_get_channel_offset(uint8_t channel);
void dac_update_channel_offset(uint8_t channel);
int16_t dac_limit(int16_t input, int16_t min, int16_t max);
uint8_t dac_get_nr_channels(void);
void dac_mute(void);
void dac_unmute(void);

#endif
