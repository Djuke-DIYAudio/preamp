#include "PreampController.h"
#include "DAC.h"
#include "time_delay.h"
#include "cs8416.h"
#include "cs4398.h"
#include "terminal.h"

#include <stdio.h>

#define DAC_MIN_VOLUME	-182	// -96 dB
#define DAC_MAX_VOLUME	12	// +6 dB
#define DAC_OFFSET	12	// +6 dB electrical gain

unsigned char _dac_nr_channels = 2;
unsigned char _dac_nr_inputs = 4;
short _dac_volume = +12;
short _dac_offset[] = { 0, 0 };

short dac_limit(short input, short min, short max) {
	if (input < min) return min;
	if (input > max) return max;
	return input;
}

void dac_init()
{
	DAC_CS = HIGH;	// Enter SPI mode
	delay_ms(1);

	if (dac_is_connected()) info("DAC detected");
   else error("No DAC detected");

	cs4398_write_register(CS4398_REG_MISC_CONTROL, 0b11000000);	// PDN=1, CPEN, no freeze
	
	delay_ms(1);

	cs4398_write_register(CS4398_REG_MODE_CONTROL, 0b00010001);	// I2S, no de-emphasis, dual speed
	cs4398_write_register(CS4398_REG_VOL_CONTROL, 0b00001001);	// Normal polarity, default mixing
	cs4398_write_register(CS4398_REG_MUTE_CONTROL, 0b11000000);	// Auto-mute, not muted
	dac_update_channel_offset(0);
	dac_update_channel_offset(1);
	cs4398_write_register(CS4398_REG_RAMP_FILTER_CONTROL, 0b10110000);	// Default ramp/filter control (soft ramp)
//	cs4398_write_register(CS4398_REG_MISC_CONTROL2, 0b00000000);	// Only for DSD

	delay_ms(1);

	cs8416_write_register(CS8416_REG_CONTROL1, 0b10000000);	// RMCKF=0, SWCLK=1
//	cs8416_write_register(CS8416_REG_CONTROL2, 0b00000000);	// No de-emphasis, GPO0 to GND
//	cs8416_write_register(CS8416_REG_CONTROL3, 0b00000000);	// GPO1/2 to GND
//	cs8416_write_register(CS8416_REG_CONTROL4, 0b00000000);	// RUN=1, RXD=0, input RXP0, output RXP0
	cs8416_write_register(CS8416_REG_SERIAL_AUDIO_DATA_FORMAT, 0b10000101); // I2S, 64*Fs, 24-bit
	cs8416_write_register(CS8416_REG_RECEIVER_ERROR_MASK, 0b00011111);	// All but (Q)CRCM
	cs8416_write_register(CS8416_REG_INTERRUPT_MASK, 0b00011111);	// All but (Q)CRCM
	cs8416_write_register(CS8416_REG_CONTROL0, 0b00001000);	// PDUR=1, FSWCLK=0, TRUNC=0

	cs4398_write_register(CS4398_REG_MISC_CONTROL, 0b01000000);	// Power-up

}

unsigned char dac_is_connected()
{
	if (cs4398_is_connected() && cs8416_is_connected()) return 1;
	else return 0;
}

void dac_set_input(unsigned char value) {
	cs8416_set_input(value);
}

unsigned char dac_get_nr_channels() {
	return _dac_nr_channels;
}

const char *dac_rate_string() {
	const char string[8] = "";
	unsigned char sample_rate = cs8416_sample_rate();
	unsigned char bit_rate = cs8416_bit_rate();
	if (!dac_is_connected()) return "--";
	if (sample_rate && bit_rate) sprintf(string, "%d/%dk", bit_rate, sample_rate);
	else if (bit_rate) sprintf(string, "%d", bit_rate);
	else if (sample_rate) sprintf(string, "%dk", sample_rate);
	return string;
}

const char *dac_error_string() {
	const char string[8] = "";
	unsigned char error = dac_receiver_error();
	if (!dac_is_connected()) return "n/a";
	if (error & (1<<4)) sprintf(string, "no lock");
	else if(error & (1<<3)) sprintf(string, "no pcm", error);
	else sprintf(string, "err:%d", error);
	return string;
}

unsigned char dac_receiver_error() {
	cs8416_read_register(CS8416_REG_RECEIVER_ERROR);	// Read the register twice to clear latches
	return cs8416_read_register(CS8416_REG_RECEIVER_ERROR);
}

unsigned char dac_connection_changed() {
	static unsigned char is_connected = 0;
	unsigned char new_is_connected = dac_is_connected();
	if (new_is_connected != is_connected) {
		is_connected = new_is_connected;
		return 1;
	}
	return 0;
}

unsigned char dac_receiver_error_changed() {
	static unsigned char receiver_error = 0;
	unsigned char new_receiver_error = dac_receiver_error();
	if (new_receiver_error != receiver_error) {
		receiver_error = new_receiver_error;
		return 1;
	}
	return 0;
}

unsigned char dac_sample_rate_changed() {
	static unsigned char sample_rate = 0;
	unsigned char new_sample_rate = cs8416_sample_rate();
	if (new_sample_rate != sample_rate) {
		sample_rate = new_sample_rate;
		return 1;
	}
	return 0;
}

unsigned char dac_bit_rate_changed() {
	static unsigned char bit_rate = 0;
	unsigned char new_bit_rate = cs8416_bit_rate();
	if (new_bit_rate != bit_rate) {
		bit_rate = new_bit_rate;
		return 1;
	}
	return 0;
}

unsigned char dac_input_changed() {
	if (dac_connection_changed() || dac_receiver_error_changed() ||
	   dac_sample_rate_changed() || dac_bit_rate_changed()) return 1;
	return 0;
}

void dac_set_volume(short volume_half_db) {
	unsigned i;
	_dac_volume = dac_limit(volume_half_db, DAC_MIN_VOLUME, DAC_MAX_VOLUME);

	for (i=0;i<dac_get_nr_channels();i++) dac_update_channel_offset(i);
}

short dac_get_volume() {
	return _dac_volume;
}

void dac_update_channel_offset(unsigned char channel) {
	unsigned char regval;
	short volume;

	if (channel >= dac_get_nr_channels()) return;

	volume = dac_limit(_dac_volume + _dac_offset[channel], DAC_MIN_VOLUME, DAC_MAX_VOLUME);

	regval = DAC_OFFSET - volume;

	cs4398_write_register(CS4398_REG_CHA_VOL_CONTROL+channel, regval);

}

void dac_set_channel_offset(unsigned char channel, short offset_half_db) {
	unsigned char regval;

	if (channel >= dac_get_nr_channels()) return;

	_dac_offset[channel] = dac_limit(offset_half_db, DAC_MIN_VOLUME, DAC_MAX_VOLUME);

	dac_update_channel_offset(channel);
}

short dac_get_channel_offset(unsigned char channel) {
	return _dac_offset[channel];
}

void dac_mute() {
	info("DAC mute");

	cs8416_write_register(CS8416_REG_CONTROL1, 0b11000100);	// RMCKF=0, SWCLK=1, MUTESA=1
//	cs4398_write_register(CS4398_REG_MUTE_CONTROL, 0b00011000);	// No-auto-mute, muted
}

void dac_unmute() {
	info("DAC unmute");

	cs8416_write_register(CS8416_REG_CONTROL1, 0b10000000);	// RMCKF=0, SWCLK=1, MUTESA=0
//	cs4398_write_register(CS4398_REG_MUTE_CONTROL, 0b11000000);	// Auto-mute, not muted
}
