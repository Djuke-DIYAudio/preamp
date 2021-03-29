#ifndef DAC_H
#define DAC_H

// Function definitions
void dac_init();

unsigned char dac_is_connected();
void dac_set_input(unsigned char value);

const char *dac_rate_string();
const char *dac_error_string();
unsigned char dac_receiver_error();
unsigned char dac_receiver_error_changed();
unsigned char dac_sample_rate_changed();
unsigned char dac_bit_rate_changed();
unsigned char dac_input_changed();
unsigned char dac_connection_changed();

void dac_set_volume(short volume_half_db);
short dac_get_volume();

void dac_set_channel_offset(unsigned char channel, short offset_half_db);
short dac_get_channel_offset(unsigned char channel);
void dac_update_channel_offset(unsigned char channel);

short dac_limit(short input, short min, short max);

unsigned char dac_get_nr_channels();
void dac_mute();
void dac_unmute();

#endif
