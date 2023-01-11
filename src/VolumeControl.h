#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <stdint.h>
#include <stdbool.h>

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
void volumecontrol_init(uint8_t type);
int16_t volumecontrol_limit(int16_t input, int16_t min, int16_t max);
void volumecontrol_set_hafler(uint8_t mode);

// Channel related functions
const char *volumecontrol_channel_name_string(uint8_t channel);
const char *volumecontrol_channel_string(uint8_t channels);
bool volumecontrol_channel_in_use(uint8_t channel);
uint8_t volumecontrol_get_nr_output_channels(void);
uint8_t volumecontrol_get_nr_channels(void);
void volumecontrol_set_volume(int16_t volume_half_db);
void volumecontrol_mute(void);
void volumecontrol_unmute(void);

// Internal
static void volumecontrol_mute_master1(void);
static void volumecontrol_unmute_master1(void);
static void volumecontrol_mute_master2(void);
static void volumecontrol_unmute_master2(void);
static void volumecontrol_mute_master3(void);
static void volumecontrol_unmute_master3(void);
static void volumecontrol_set_master1_volume(short volume_half_db);
static void volumecontrol_set_master2_volume(short volume_half_db);
static void volumecontrol_set_master3_volume(short volume_half_db);
static bool volumecontrol_use_subwoofer(void);
static bool volumecontrol_use_center(void);
static bool volumecontrol_use_rear(void);
void volumecontrol_set_channel_offset(uint8_t channel, int8_t offset_half_db);
int8_t volumecontrol_get_channel_offset(uint8_t channel);
void volumecontrol_mute_channel(uint8_t channel);
void volumecontrol_unmute_channel(uint8_t channel);
void volumecontrol_set_input_mode(uint8_t input_mode);
void volumecontrol_set_output_mode(uint8_t output_mode);
uint8_t volumecontrol_get_output_mode(void);
uint8_t volumecontrol_next_output_mode(void);
bool volumecontrol_is_hafler_mode(void);
bool volumecontrol_has_signal_level(void);
bool volumecontrol_has_headphones(void);

#endif
