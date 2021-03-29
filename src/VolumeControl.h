#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

// Type definitions

enum VC_TYPE {
    VC_NORMAL = 0,  // normal version of VolumeControl pcb
    VC_8CH = 1,     // 8-channel version of VolumeControl pcb
    VC_8CHx2 = 2    // 2x8-channel version of VolumeControl pcb
};

enum CHANNEL {
	FRONT_LEFT = 0,
	FRONT_RIGHT,
	REAR_LEFT,
	REAR_RIGHT,
	CENTER,
	SUBWOOFER,
	HEADPHONE_LEFT,
	HEADPHONE_RIGHT
};

enum CHANNEL_MODE {
	CH2 = 2,
	CH3,
	CH4,
	CH5,
	CH8 = 8,
	CH16 = 16,
	CH2_SUB = 0x82,
	CH3_SUB,
	CH4_SUB,
	CH5_SUB
};

// Function definitions
void volumecontrol_init(unsigned char type);
short volumecontrol_limit(short input, short min, short max);

void volumecontrol_set_hafler(unsigned char mode);

// Channel related functions
const char *volumecontrol_channel_name_string(unsigned char channel);
const char *volumecontrol_channel_string(unsigned char channels);
unsigned char volumecontrol_channel_in_use(unsigned char channel);
unsigned char volumecontrol_get_nr_output_channels();
unsigned char volumecontrol_get_nr_channels();

void volumecontrol_set_volume(short volume_half_db);
void volumecontrol_mute();
void volumecontrol_unmute();

// Internal
static void volumecontrol_mute_master1();
static void volumecontrol_unmute_master1();
static void volumecontrol_mute_master2();
static void volumecontrol_unmute_master2();
static void volumecontrol_mute_master3();
static void volumecontrol_unmute_master3();
static void volumecontrol_set_master1_volume(short volume_half_db);
static void volumecontrol_set_master2_volume(short volume_half_db);
static void volumecontrol_set_master3_volume(short volume_half_db);
static unsigned char volumecontrol_use_subwoofer();
static unsigned char volumecontrol_use_center();
static unsigned char volumecontrol_use_rear();

void volumecontrol_set_channel_offset(unsigned char channel, short offset_half_db);
short volumecontrol_get_channel_offset(unsigned char channel);
void volumecontrol_mute_channel(unsigned char channel);
void volumecontrol_unmute_channel(unsigned char channel);

void volumecontrol_set_input_mode(unsigned char input_mode);
void volumecontrol_set_output_mode(unsigned char output_mode);
unsigned char volumecontrol_get_output_mode();
unsigned char volumecontrol_next_output_mode();

unsigned char volumecontrol_is_hafler_mode();
unsigned char volumecontrol_has_signal_level();
unsigned char volumecontrol_has_headphones();

#endif
