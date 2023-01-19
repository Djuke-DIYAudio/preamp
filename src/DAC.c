#include "PreampController.h"
#include "DAC.h"
#include "time_delay.h"
#include "cs8416.h"
#include "cs4398.h"
#include "terminal.h"

#include <stdio.h>

#define DAC_MIN_VOLUME	-182	// -96 dB
#define DAC_MAX_VOLUME	12	// +6 dB
#define DAC_MIN_OFFSET	-20	// -10 dB
#define DAC_MAX_OFFSET	20	// +10 dB
#define DAC_OFFSET	12	// +6 dB analog gain

uint8_t _dac_nr_channels = 2;
bool _dac_cs4398_detected = false;
bool _dac_cs8416_detected = false;
bool _dac_muted = true;
uint8_t _dac_receiver_error = 0xFF;
uint8_t _dac_format_detect = 0;
uint8_t _dac_bit_rate = 0;
uint8_t _dac_sample_rate = 0;
int16_t _dac_volume = +12;
int8_t _dac_offset[] = { 0, 0 };

int16_t dac_limit(int16_t input, int16_t min, int16_t max) {
	if (input < min) return min;
	if (input > max) return max;
	return input;
}

void dac_init(void)
{
	DAC_CS = HIGH;	// Enter SPI mode
    delay_ms(1);
    
    // Try to detect the CS4398 DAC chip
    (void)dac_update_cs4398_connected();
    if (_dac_cs4398_detected) {

        cs4398_write_register(CS4398_REG_MISC_CONTROL, 0b11000000);	// PDN=1, CPEN, no freeze
        cs4398_write_register(CS4398_REG_MODE_CONTROL, 0b00010001);	// I2S, no de-emphasis, dual speed
        cs4398_write_register(CS4398_REG_VOL_CONTROL, 0b00001001);	// Normal polarity, default mixing
        cs4398_write_register(CS4398_REG_MUTE_CONTROL, 0b11000000);	// Auto-mute, not muted
        dac_update_channel_offset(0);
        dac_update_channel_offset(1);
        cs4398_write_register(CS4398_REG_RAMP_FILTER_CONTROL, 0b10110000);	// Default ramp/filter control (soft ramp)
    //	cs4398_write_register(CS4398_REG_MISC_CONTROL2, 0b00000000);	// Only for DSD
    }
    
    // Try to detect the CS8416 DIR chip
    (void)dac_update_cs8416_connected();
	if (_dac_cs8416_detected) {

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
}

bool dac_update(void)
{
    bool changed = false;
    if (dac_update_cs4398_connected()) changed = true;
    if (dac_update_cs8416_connected()) changed = true;

    if (dac_is_connected())
    {
        if (dac_update_receiver_error()) changed = true;    
        if (dac_update_bit_rate()) changed = true;    
        if (dac_update_sample_rate()) changed = true;    
        if (dac_update_format_detect()) changed = true;    
    }

    return changed;
}

bool dac_update_cs4398_connected(void)
{
    bool changed = false;

    // Try to detect the CS4398 DAC chip
	bool detected = cs4398_is_connected();
    if (detected != _dac_cs4398_detected)
    {
        changed = true;
        if (detected) info("CS4398 detected");
        else error("CS4398 lost");
    }
    _dac_cs4398_detected = detected;

    return changed;
}

bool dac_update_cs8416_connected(void)
{
    bool changed = false;

    // Try to detect the CS4398 DAC chip
	bool detected = cs8416_is_connected();
    if (detected != _dac_cs8416_detected)
    {
        changed = true;
        if (detected) info("CS8416 detected");
        else error("CS8416 lost");
    }
    _dac_cs8416_detected = detected;

    return changed;
}

bool dac_update_receiver_error(void)
{
    bool changed = false;

	uint8_t error = cs8416_read_register(CS8416_REG_RECEIVER_ERROR);
	if (error != _dac_receiver_error) {
        changed = true;
        char str[25];
        sprintf(str, "DAC error: %X->%X", _dac_receiver_error, error);
        debug(3, str);
	}
    _dac_receiver_error = error;

	return changed;
}

bool dac_update_sample_rate(void) {
    bool changed = false;

	uint8_t sample_rate = cs8416_sample_rate();
	if (sample_rate != _dac_sample_rate) {
        changed = true;
        char str[25];
        sprintf(str, "DAC sample: %d->%d", _dac_sample_rate, sample_rate);
        debug(3, str);
	}
	_dac_sample_rate = sample_rate;

	return changed;
}

bool dac_update_bit_rate(void) {
    bool changed = false;

	uint8_t bit_rate = cs8416_bit_rate();
	if (bit_rate != _dac_bit_rate) {
        changed = true;
        char str[25];
        sprintf(str, "DAC bitrate: %d->%d", _dac_bit_rate, bit_rate);
        debug(3, str);
	}
	_dac_bit_rate = bit_rate;

	return changed;
}

bool dac_update_format_detect(void)
{
    bool changed = false;

	uint8_t format = cs8416_read_register(CS8416_REG_AUDIO_FORMAT_DETECT);
	if (format != _dac_format_detect) {
        changed = true;
        char str[25];
        sprintf(str, "DAC format: %X->%X", _dac_format_detect, format);
        debug(3, str);
	}
    _dac_format_detect = format;

	return changed;
}


bool dac_is_connected(void)
{
    return _dac_cs8416_detected && _dac_cs4398_detected;
}

void dac_set_input(uint8_t value) {
	cs8416_set_input(value);
}

uint8_t dac_get_nr_channels(void) {
	return _dac_nr_channels;
}

const char *dac_rate_string(void) {
    static char string[8] = "";
	if (!dac_is_connected()) return "--";
	if (_dac_sample_rate && _dac_bit_rate) sprintf(string, "%d/%dk", _dac_bit_rate, _dac_sample_rate);
	else if (_dac_bit_rate) sprintf(string, "%d", _dac_bit_rate);
	else if (_dac_sample_rate) sprintf(string, "%dk", _dac_sample_rate);
	return string;
}

const char *dac_error_string(void) {
	static char string[8] = "";
	if (!dac_is_connected()) return "n/a";
	if (_dac_receiver_error & (1<<4)) sprintf(string, "no lock");
	else if(_dac_receiver_error & (1<<3)) sprintf(string, "no pcm");
    else sprintf(string, "err:%d", _dac_receiver_error);
	return string;
}

uint8_t dac_receiver_error(void) {
	return _dac_receiver_error;
}

void dac_set_volume(int16_t volume_half_db) {
	_dac_volume = dac_limit(volume_half_db, DAC_MIN_VOLUME, DAC_MAX_VOLUME);

	for (uint8_t i=0;i<dac_get_nr_channels();i++) dac_update_channel_offset(i);
}

void dac_update_channel_offset(uint8_t channel) {
	if (channel >= dac_get_nr_channels()) return;

	int16_t volume = dac_limit(_dac_volume + _dac_offset[channel], DAC_MIN_VOLUME, DAC_MAX_VOLUME);

	uint8_t regval = (uint8_t) (DAC_OFFSET - volume);
    if (DAC_OFFSET - volume < 0) regval = 0;
    if (DAC_OFFSET - volume > 255) regval = 255;

	cs4398_write_register(CS4398_REG_CHA_VOL_CONTROL+channel, regval);

}

void dac_set_channel_offset(uint8_t channel, int8_t offset_half_db) {
	if (channel >= dac_get_nr_channels()) return;

	_dac_offset[channel] = (int8_t)dac_limit(offset_half_db, DAC_MIN_OFFSET, DAC_MAX_OFFSET);

	dac_update_channel_offset(channel);
}

int8_t dac_get_channel_offset(uint8_t channel) {
	return _dac_offset[channel];
}

void dac_mute(void) {
    if (!_dac_muted)
    {
        info("DAC mute");

        cs8416_write_register(CS8416_REG_CONTROL1, 0b11000100);	// RMCKF=0, SWCLK=1, MUTESA=1
    //	cs4398_write_register(CS4398_REG_MUTE_CONTROL, 0b00011000);	// No-auto-mute, muted
        _dac_muted = true;
    }
}

void dac_unmute(void) {
    if (_dac_muted)
    {
        info("DAC unmute");

        cs8416_write_register(CS8416_REG_CONTROL1, 0b10000000);	// RMCKF=0, SWCLK=1, MUTESA=0
    //	cs4398_write_register(CS4398_REG_MUTE_CONTROL, 0b11000000);	// Auto-mute, not muted
        _dac_muted = false;
    }
}
