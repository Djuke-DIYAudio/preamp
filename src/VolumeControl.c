#include "PreampController.h"
#include "VolumeControl.h"
#include "time_delay.h"
#include "cs3318.h"
#include "terminal.h"
#include <stdio.h>

#define VC_MIN_VOLUME	-182	// -96 dB
#define VC_MAX_VOLUME	44	// +22 dB
#define VC_MIN_CHANNEL_OFFSET	-40	// -20 dB
#define VC_MAX_CHANNEL_OFFSET	40	// +20 dB
#define VC_MAX_DEVICES		2
#define VC_MAX_CHANNELS		16

// Global variables
static unsigned char _volumecontrol_mute_register[VC_MAX_DEVICES] = {0, 0};	// Mute control register (per device)
static unsigned char _volumecontrol_output_mode = CH5_SUB;
static unsigned char _volumecontrol_input_mode = CH5_SUB;
static unsigned char _volumecontrol_nr_channels = 8;
static unsigned char _volumecontrol_nr_devices = 2;
static unsigned char _volumecontrol_type = VC_NORMAL;
static short _volumecontrol_channel_offset[VC_MAX_CHANNELS];

short volumecontrol_limit(short input, short min, short max) {
	if (input < min) return min;
	if (input > max) return max;
	return input;
}

const char *volumecontrol_channel_name_string(unsigned char channel) { 
	const char string[10];

	if (_volumecontrol_type == VC_NORMAL) {
		switch (channel) {
			case FRONT_LEFT: return "Front Left";
			case FRONT_RIGHT: return "Front Right";
			case REAR_LEFT: return "Rear Left";
			case REAR_RIGHT: return "Rear Right";
			case CENTER: return "Center";
			case SUBWOOFER: return "Subwoofer";
			case HEADPHONE_LEFT: return "HP Left";
			case HEADPHONE_RIGHT: return "HP Right";
			default: return "?";
		}
	} else {
		sprintf(string, "Number %d", channel+1);
		return string;
	}
}

const char *volumecontrol_channel_string(unsigned char channels) {
	const char string[4];

	unsigned char nr_channels = channels & 0x7F;
	if (channels > 0x7F) nr_channels++;

	if (_volumecontrol_type == VC_NORMAL) {
		switch(channels)
		{
			case CH2:	return "2.0";
			case CH3:	return "3.0";
			case CH4:	return "4.0";
			case CH5:	return "5.0";
			case CH2_SUB:	return "2.1";
			case CH3_SUB:	return "3.1";
			case CH4_SUB:	return "4.1";
			case CH5_SUB:	return "5.1";
			default:	return "?.?";
	    }
	} else {
		sprintf(string, "%d", nr_channels);
		return string;
	}
}

void volumecontrol_set_input_mode(unsigned char channels) {
	_volumecontrol_input_mode = channels;
	volumecontrol_set_hafler(_volumecontrol_input_mode == CH2);
}

unsigned char volumecontrol_get_input_mode() { return _volumecontrol_input_mode; }
void volumecontrol_set_output_mode(unsigned char channels) { 
	unsigned char i;

	_volumecontrol_output_mode = channels;
	for (i=0;i<_volumecontrol_nr_channels;i++) {
//		volumecontrol_set_channel_offset(i, volumecontrol_get_channel_offset(i));
		if (volumecontrol_channel_in_use(i)) volumecontrol_unmute_channel(i);
		else volumecontrol_mute_channel(i);
	}
}
unsigned char volumecontrol_get_output_mode() { return _volumecontrol_output_mode; }
unsigned char volumecontrol_next_output_mode() {
	if (_volumecontrol_type == VC_NORMAL) {
		_volumecontrol_output_mode++;
		if (_volumecontrol_output_mode > CH5 && _volumecontrol_output_mode < CH2_SUB) _volumecontrol_output_mode = CH2_SUB;
		else if (_volumecontrol_output_mode > CH5_SUB) _volumecontrol_output_mode = CH2;
	}
	return _volumecontrol_output_mode;
}

unsigned char volumecontrol_get_nr_output_channels() {
	unsigned char output_channels = _volumecontrol_output_mode & 0x7F;
	if (volumecontrol_use_subwoofer()) output_channels++;
	return output_channels;
}

unsigned char volumecontrol_get_nr_channels() {
	return _volumecontrol_nr_channels;
}

void volumecontrol_init(unsigned char type) {
	float offset;
	unsigned char i;

	_volumecontrol_type = type;
	cs3318_reset_defaults();	// Reset the default chip-addresses

	offset = get_signal_offset();
	VOLUME_UNMUTE = LOW;

	HAFLER_RELAY = LOW;
	HAFLER_RELAY_DIRECTION = INPUT_BIT;
	delay_ms(1);
	if (HAFLER_RELAY) info("VolumeControl8ch detected");
	else if (!HAFLER_RELAY && offset > 0.2) info("VolumeControl detected");
	else error("VolumeControl not detected");
	HAFLER_RELAY_DIRECTION = OUTPUT_BIT;
	delay_ms(1);

	if (_volumecontrol_type == VC_NORMAL) {
		_volumecontrol_nr_channels = 8;
		_volumecontrol_nr_devices = 1;
		cs3318_write_register(0, CS3318_REG_MASTER_POWER, 0b00000001);	// PDN_ALL=1
		cs3318_write_register(0, CS3318_REG_CH7_VOLUME, 0b11100110);	// +10dB for headphones
		cs3318_write_register(0, CS3318_REG_CH8_VOLUME, 0b11100110);	// +10dB for headphones
	
		cs3318_write_register(0, CS3318_REG_MASTER_1_MASK, 0b00000011);	// Stereo channels
		cs3318_write_register(0, CS3318_REG_MASTER_2_MASK, 0b00111100);	// Surround channels
		cs3318_write_register(0, CS3318_REG_MASTER_3_MASK, 0b11000000);	// Headphone channels
	} else if (_volumecontrol_type == VC_8CH) {
		cs3318_write_register(0, CS3318_REG_MASTER_POWER, 0b00000001);	// PDN_ALL=1
		cs3318_write_register(0, CS3318_REG_MASTER_1_MASK, 0b11111111);	// Master channels
		cs3318_write_register(0, CS3318_REG_MASTER_2_MASK, 0b00000000);	// Do not use
		cs3318_write_register(0, CS3318_REG_MASTER_3_MASK, 0b00000000);	// Do not use
		cs3318_set_device_id(0, 0b1100000); // Set new device-id for first device with enabled ENout
		delay_ms(10);
		_volumecontrol_nr_channels = 8;
		_volumecontrol_nr_devices = 1;
	} else if (_volumecontrol_type == VC_8CHx2) {
		cs3318_write_register(0, CS3318_REG_MASTER_POWER, 0b00000001);	// PDN_ALL=1
		cs3318_write_register(0, CS3318_REG_DEVICE_CONFIG_1, 0b00100000);	// Enable MUTE input (active low)
		cs3318_write_register(0, CS3318_REG_DEVICE_CONFIG_2, 0b00001101);	// 18ms zero-crossing timeout
		cs3318_write_register(0, CS3318_REG_MASTER_1_MASK, 0b11111111);	// Master channels
		cs3318_write_register(0, CS3318_REG_MASTER_2_MASK, 0b00000000);	// Do not use
		cs3318_write_register(0, CS3318_REG_MASTER_3_MASK, 0b00000000);	// Do not use
		cs3318_set_device_id(0, 0b1100000); // Set new device-id for first device and enable next device
		delay_ms(10);
		cs3318_write_register(1, CS3318_REG_MASTER_POWER, 0b00000001);	// PDN_ALL=1
		cs3318_write_register(1, CS3318_REG_DEVICE_CONFIG_1, 0b00100000);	// Enable MUTE input (active low)
		cs3318_write_register(1, CS3318_REG_DEVICE_CONFIG_2, 0b00001101);	// 18ms zero-crossing timeout
		cs3318_write_register(1, CS3318_REG_MASTER_1_MASK, 0b11111111);	// Master channels
		cs3318_write_register(1, CS3318_REG_MASTER_2_MASK, 0b00000000);	// Do not use
		cs3318_write_register(1, CS3318_REG_MASTER_3_MASK, 0b00000000);	// Do not use
		cs3318_set_device_id(1, 0b1110000); // Set new device-id for second device with enabled ENout
		delay_ms(10);
		_volumecontrol_nr_channels = 16;
		_volumecontrol_nr_devices = 2;
	}

	for (i=0;i<_volumecontrol_nr_devices;i++) {

//		cs3318_write_register(i, CS3318_REG_CH1_VOLUME, 0b11010010);	// 0dB
//		cs3318_write_register(i, CS3318_REG_CH2_VOLUME, 0b11010010);	// 0dB
//		cs3318_write_register(i, CS3318_REG_CH3_VOLUME, 0b11010010);	// 0dB
//		cs3318_write_register(i, CS3318_REG_CH4_VOLUME, 0b11010010);	// 0dB
//		cs3318_write_register(i, CS3318_REG_CH5_VOLUME, 0b11010010);	// 0dB
//		cs3318_write_register(i, CS3318_REG_CH6_VOLUME, 0b11010010);	// 0dB

//		cs3318_write_register(i, CS3318_REG_MUTE_CONTROL, _volumecontrol_mute_register[i]);	// Unmute all channels
//		cs3318_write_register(i, CS3318_REG_DEVICE_CONFIG_1, 0b00100000);	// Enable MUTE input (active low)
//		cs3318_write_register(i, CS3318_REG_DEVICE_CONFIG_2, 0b00001101);	// 18ms zero-crossing timeout

//		cs3318_write_register(i, CS3318_REG_CHANNEL_POWER, 0b00000000);	// Enable power for all channels
//		cs3318_write_register(i, CS3318_REG_FREEZE_CONTROL, 0b00000000);	// No freeze control

		cs3318_write_register(i, CS3318_REG_MASTER_POWER, 0b00000000);	// PDN_ALL=0, enable operation
	}
	
	delay_ms(10);
}

void volumecontrol_set_hafler(unsigned char mode) {
	if (mode) HAFLER_RELAY = HIGH;
	else HAFLER_RELAY = LOW;
}

void volumecontrol_mute() { 
	VOLUME_UNMUTE = 0;
}

void volumecontrol_unmute() { 
	VOLUME_UNMUTE = 1;
}

void volumecontrol_mute_master1() {
	unsigned char i;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_1_CONTROL, 0b00000010);	// Muted

}

void volumecontrol_unmute_master1() {
	unsigned char i;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_1_CONTROL, 0b00000000);	// Not muted

}

void volumecontrol_mute_master2() {
	unsigned char i;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_2_CONTROL, 0b00000010);	// Muted

}

void volumecontrol_unmute_master2() {
	unsigned char i;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_2_CONTROL, 0b00000000);	// Not muted

}

void volumecontrol_mute_master3() {
	unsigned char i;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_3_CONTROL, 0b00000010);	// Muted

}

void volumecontrol_unmute_master3() {
	unsigned char i;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_3_CONTROL, 0b00000000);	// Not muted

}

void volumecontrol_set_volume(short volume_half_db) {
	if (_volumecontrol_type == VC_NORMAL) {
		if (headphones_connected()) {
			volumecontrol_mute_master1();				// Mute stereo channels
			volumecontrol_mute_master2();				// Mute surround channels
			volumecontrol_set_master3_volume(volume_half_db);	// Headphones channels
		} else {
			volumecontrol_unmute_master1();				// Unmute stereo channels
			volumecontrol_unmute_master2();				// Unmute surround channels
			volumecontrol_set_master3_volume(volume_half_db);	// Headphones channels
			volumecontrol_set_master2_volume(volume_half_db);	// Surround channels
			volumecontrol_set_master1_volume(volume_half_db);	// Stereo channels
		}
	} else {
			volumecontrol_set_master1_volume(volume_half_db);	// All channels
	}
}

static void volumecontrol_set_master1_volume(short volume_half_db) {
	unsigned char regval, i;
	char string[22];
	sprintf(string, "set master1 vol: %d", volume_half_db);
	debug(3, string);

	regval = volumecontrol_limit(volume_half_db, VC_MIN_VOLUME, VC_MAX_VOLUME) + 210;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_1_VOLUME, regval);
}

static void volumecontrol_set_master2_volume(short volume_half_db) {
	unsigned char regval, i;

	regval = volumecontrol_limit(volume_half_db, VC_MIN_VOLUME, VC_MAX_VOLUME) + 210;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_2_VOLUME, regval);
}

static void volumecontrol_set_master3_volume(short volume_half_db) {
	unsigned char regval, i;

	regval = volumecontrol_limit(volume_half_db, VC_MIN_VOLUME, VC_MAX_VOLUME) + 210;

	for(i=0;i<_volumecontrol_nr_devices;i++)
		cs3318_write_register(i, CS3318_REG_MASTER_3_VOLUME, regval);
}

void volumecontrol_set_channel_offset(unsigned char channel, short offset_half_db) {
	unsigned char regval;
	unsigned char device = channel / 8;

	if (channel >= _volumecontrol_nr_channels) return;

	_volumecontrol_channel_offset[channel] = volumecontrol_limit(offset_half_db, VC_MIN_CHANNEL_OFFSET, VC_MAX_CHANNEL_OFFSET);
	regval = _volumecontrol_channel_offset[channel] + 210;

	channel = channel % 8;
	cs3318_write_register(device, CS3318_REG_CH1_VOLUME+channel, regval);
}

short volumecontrol_get_channel_offset(unsigned char channel) {
	if (channel >= _volumecontrol_nr_channels) return 0;
	return _volumecontrol_channel_offset[channel];
}

void volumecontrol_mute_channel(unsigned char channel) {
	unsigned char device = channel / 8;
	channel = channel % 8;

	_volumecontrol_mute_register[device] |= (1 << channel);	// Set the channel mute bit

	cs3318_write_register(device, CS3318_REG_MUTE_CONTROL, _volumecontrol_mute_register[device]);

}

void volumecontrol_unmute_channel(unsigned char channel) {
	unsigned char device = channel / 8;
	channel = channel % 8;

	_volumecontrol_mute_register[device] &= ~(1 << channel);	// Clear the channel mute bit

	cs3318_write_register(device, CS3318_REG_MUTE_CONTROL, _volumecontrol_mute_register[device]);
}

unsigned char volumecontrol_has_signal_level() {
	if (_volumecontrol_type == VC_NORMAL) return 1;
	else return 0;
}

unsigned char volumecontrol_has_headphones() {
	if (_volumecontrol_type == VC_NORMAL) return 1;
	else return 0;
}

static unsigned char volumecontrol_use_subwoofer() {
	if (_volumecontrol_output_mode >= CH2_SUB) return 1;
	else return 0;
}

static unsigned char volumecontrol_use_center() {
	if ((_volumecontrol_output_mode & 0x7F) == CH3) return 1;
	else if ((_volumecontrol_output_mode & 0x7F) == CH5) return 1;
	else return 0;
}

static unsigned char volumecontrol_use_rear() {
	if ((_volumecontrol_output_mode & 0x7F) > CH3) return 1;
	return 0;
}

unsigned char volumecontrol_is_hafler_mode() {
	if (_volumecontrol_type != VC_NORMAL) return 0;
	if (_volumecontrol_input_mode == CH2) return 1;
	else return 0;
}

unsigned char volumecontrol_channel_in_use(unsigned char channel) {
    if (_volumecontrol_type == VC_NORMAL) {
	    if (channel==FRONT_LEFT || channel==FRONT_RIGHT) return 1;
	    if (channel==HEADPHONE_LEFT || channel==HEADPHONE_RIGHT) return 1;
	    if ((channel==REAR_LEFT || channel==REAR_RIGHT) && volumecontrol_use_rear()) return 1;
	    if (channel == CENTER && volumecontrol_use_center()) return 1;
	    if (channel==SUBWOOFER && volumecontrol_use_subwoofer()) return 1;
    	return 0;
    } else return 1;
}
