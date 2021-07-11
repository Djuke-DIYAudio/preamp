#include "Preamp.h"
#include <string.h>
#include <stdio.h>

#include "PreampController.h"
#include "InputSelect.h"
#include "VolumeControl.h"
#include "DAC.h"
#include "infrared.h"
#include "eeprom.h"
#include "terminal.h"
#include "EventHandler.h"

#include "menu.h"

// Preamp state and settings
volatile state_t _state;
volatile settings_t _settings;

// Preamp info
const char *preamp_name() { return "Djuke PreAmplifier"; }
const char *preamp_version() { return "v1.3"; }
const char *preamp_date() { return "9 jul 2021"; }

// Input names
const char input_names[][13] =
{
	"Audio Cast",
	"Auxillary",
	"Blu-Ray",
	"Cable",
	"CD Player",
	"Computer",
	"DVD Player",
	"HTPC",
	"Media Player",
	"Phono",
	"Receiver",
	"Satellite",
	"Tape",
	"Television",
	"Tuner",
	"TV Receiver",
	"Video"
};

void update_volume()
{
	uint16_t volume;
	uint8_t offset = get_input_offset(get_input());
	volume = get_volume() + offset;

	if (get_input() > nr_inputs() || is_muted() || !is_powered()) {
		if (_settings.use_cs3318_volumecontrol_pcb) volumecontrol_mute();
		else if (_settings.use_dac_pcb) dac_mute();
		return;
	}

	if (is_dac_input() && _settings.use_dac_pcb) {
		if (!dac_is_connected() || dac_receiver_error()) {
			dac_mute();
			if (_settings.use_cs3318_volumecontrol_pcb) volumecontrol_mute(); // Also mute the volume control output
			return;
		} else {
			dac_unmute();
		}
	}

	if (_settings.use_cs3318_volumecontrol_pcb) {
		volumecontrol_set_volume(volume);
		volumecontrol_unmute();	// If it has not been done before
	} else if (_settings.use_dac_pcb) {	// Without VolumeControl, use the DAC to control volume
		dac_set_volume(volume);
		dac_unmute(); // If it has not been done before
	}
}

unsigned char get_nr_hw_setups() { return NR_HW_SETUPS; }	// Get number of default hw setups

const char *hw_setup_name(unsigned char hw_setup) {
	switch(hw_setup)
	{
		case STEREO_INPUT: return "Stereo input?";
		case MULTI_INPUT: return "5.1 input?";
		case DAC_INPUT: return "DAC input?";
		case CS3318_VOL_CONTROL: return "Use CS3318?";
		case CS3318_TYPE: return "CS3318 type?";
		default: return "?";
	}
}

const char *hw_setup_value(unsigned char hw_setup) {
	switch(hw_setup)
	{
		case STEREO_INPUT: if (_settings.use_inputselect_pcb) return "yes"; else return "no";
		case MULTI_INPUT: if (_settings.use_multi_input_pcb) return "yes"; else return "no";
		case DAC_INPUT: if (_settings.use_dac_pcb) return "yes"; else return "no";
		case CS3318_VOL_CONTROL: if (_settings.use_cs3318_volumecontrol_pcb) return "yes"; else return "no";
		case CS3318_TYPE:
			if (_settings.cs3318_type == VC_NORMAL) return "5.1ch";
			else if (_settings.cs3318_type == VC_8CH) return "1x8ch";
			else if (_settings.cs3318_type == VC_8CHx2) return "2x8ch";
		default: return "?";
	}
}

void toggle_hw_setup(unsigned char hw_setup) {
	switch(hw_setup)
	{
		case STEREO_INPUT: _settings.use_inputselect_pcb = _settings.use_inputselect_pcb ? 0 : 1; break;
		case MULTI_INPUT: _settings.use_multi_input_pcb = _settings.use_multi_input_pcb || !_settings.use_inputselect_pcb ? 0 : 1; break;
		case DAC_INPUT: _settings.use_dac_pcb = _settings.use_dac_pcb ? 0 : 1; break;
		case CS3318_VOL_CONTROL: _settings.use_cs3318_volumecontrol_pcb = _settings.use_cs3318_volumecontrol_pcb ? 0 : 1; break;
		case CS3318_TYPE:
			if (_settings.cs3318_type < 2) _settings.cs3318_type++;
			else _settings.cs3318_type = 0;
			break;
	}
}

unsigned char settings_saved() { return _state.settings_saved; }
void set_settings_saved(unsigned char saved) { _state.settings_saved = saved; }
unsigned char is_powered() { return _state.powered; }
void set_powered(unsigned char val) { _state.powered = val; }
unsigned char is_muted() { return _state.muted; }
void set_muted(unsigned char val) {
	_state.muted = val;
	update_volume();
	display_volume(0);
}
unsigned char is_enabled(unsigned char input) { return _settings.input[input].enabled; }

void toggle_enabled(unsigned char input) {
	if (_settings.input[input].enabled) _settings.input[input].enabled = 0;
	else _settings.input[input].enabled = 1;
	set_settings_saved(0);
}

unsigned char nr_inputs() { return _settings.nr_inputs; }
unsigned char get_input_type() { return _settings.input[get_input()].type; }
unsigned char get_analog_select() { return _settings.input[get_input()].analog_select; }
unsigned char get_dac_select() { return _settings.input[get_input()].dac_select; }
unsigned char get_input() { return _state.current_input; }

void set_input(unsigned char val) {
	unsigned char i;

	if (val > _settings.nr_inputs) return;
	if (_settings.input[val].enabled) {
		_state.current_input = val;
		if (_settings.use_inputselect_pcb && _state.powered) inputselect_set_input(get_analog_select());
		if (_settings.use_dac_pcb && _state.powered) dac_set_input(get_dac_select());
		if (_settings.use_cs3318_volumecontrol_pcb) {
			volumecontrol_set_output_mode(get_current_output_mode());
			volumecontrol_set_input_mode(get_current_input_mode());
			for (i=0;i<nr_channels();i++) {
				volumecontrol_set_channel_offset(i, get_channel_offset(i));
				if (volumecontrol_channel_in_use(i)) volumecontrol_unmute_channel(i);
				else volumecontrol_mute_channel(i);
			}
		}
		update_volume();
		display_input(1);
		display_volume(1);
	}
}

unsigned char next_input() {
	unsigned char start_input = get_input();
	unsigned char new_input = start_input;

	if (!is_powered()) return start_input;

	do {
		new_input++;
		if (new_input>_settings.nr_inputs-1) new_input = 0;
	} while (!_settings.input[new_input].enabled && new_input != start_input);
	set_input(new_input);
	return get_input();
}

unsigned char previous_input() {
	unsigned char start_input = get_input();
	unsigned char new_input = start_input;

	if (!is_powered()) return start_input;

	do {
		if (new_input > 0) new_input--;
		else new_input = _settings.nr_inputs-1;
	} while (!_settings.input[new_input].enabled && new_input != start_input);
	set_input(new_input);
	return get_input();
}

unsigned char input_changed() {
	static unsigned char prev_input = 255;
	unsigned char input = get_input();
	if (input != prev_input) {
		prev_input = input;
		return 1;
	} else {
		prev_input = input;
		return 0;
	}
}

unsigned char is_dac_input() {
	unsigned char type = get_input_type();
	if (type >= DAC1 && type <= DAC8) return 1;
	else return 0;
}

unsigned char is_analog_input() {
	unsigned char type = get_input_type();
	if (type >= ANALOG1 && type <= ANALOG8) return 1;
	else return 0;
}


int16_t get_volume() {
	return _state.volume;
}

void set_volume(signed short vol) {
	_state.muted = 0;	// Unmute after changing volume
	if (vol > MAX_VOLUME) vol = MAX_VOLUME;
	if (vol < MIN_VOLUME) vol = MIN_VOLUME;
	_state.volume = vol;
	update_volume();
   display_volume(0);
}

void add_volume(signed char val) {
	short vol;
	vol = _state.volume + val;
   set_volume(vol);
}

const char *volume_string() {
	char string[8];
	short volume = get_volume();
	char muted = is_muted();

	if (is_muted()) sprintf(string, "  MUTED");
	else if (volume <= -192) sprintf(string, "-inf dB");
	else {
		if (volume < -19) sprintf(string, "-%2d", -volume/2);
		else if (volume < 0) sprintf(string, " -%1d", -volume/2);
		else if (volume > 19) sprintf(string, "%+2d", volume/2);
		else sprintf(string, " %+1d", volume/2);
		if (volume % 2) strcat(string, ".5");
		else strcat(string, ".0");
		strcat(string, "dB");
	}
	return string;
}

unsigned char has_volume_control() {
	if (_settings.use_cs3318_volumecontrol_pcb) return 1;
	if (_settings.use_dac_pcb) return 1;
	else return 0;
}

unsigned char has_signal_level() {
	if (_settings.use_cs3318_volumecontrol_pcb) return volumecontrol_has_signal_level();
	else return 0;
}

unsigned char has_headphones() {
	if (_settings.use_cs3318_volumecontrol_pcb && volumecontrol_has_headphones()) return 1;
	else return 0;
}

unsigned char nr_channels() {
	if (_settings.use_cs3318_volumecontrol_pcb) return volumecontrol_get_nr_channels();
	else if (_settings.use_dac_pcb) return dac_get_nr_channels();
	else if (_settings.use_multi_input_pcb) return 6;
	else if (_settings.use_inputselect_pcb) return 2;
	else return 0;
}

unsigned char get_nr_output_channels() {
	if (_settings.use_cs3318_volumecontrol_pcb) return volumecontrol_get_nr_output_channels();
	if (_settings.use_dac_pcb) return dac_get_nr_channels();
	else return 0;
}

int8_t get_channel_offset(unsigned char channel) {
	int8_t offset;
	if (channel >= nr_channels()) return 0;
	offset = _settings.channel[channel].offset;
//	offset += get_output_mode_offset(channel);
	return offset;
}

void add_channel_offset(unsigned char channel, int8_t offset) {
	int8_t new_offset;
	if (channel >= nr_channels()) return;
	new_offset = _settings.channel[channel].offset + offset;
	set_channel_offset(channel, new_offset);
}

void set_channel_offset(unsigned char channel, int8_t offset) {
	if (_settings.use_cs3318_volumecontrol_pcb) {
		volumecontrol_set_channel_offset(channel, offset);
		_settings.channel[channel].offset = volumecontrol_get_channel_offset(channel);
		set_settings_saved(0);
	}
	else if (_settings.use_dac_pcb) {
		dac_set_channel_offset(channel, offset);
		_settings.channel[channel].offset = dac_get_channel_offset(channel);
		set_settings_saved(0);
	}
}

int8_t get_current_input_offset() { return _settings.input[get_input()].offset; }
int8_t get_input_offset(unsigned char input) { return _settings.input[input].offset; }

void add_input_offset(unsigned char input, int8_t offset) {
	_settings.input[input].offset += offset;
	if (_settings.input[input].offset < -MAX_INPUT_OFFSET) _settings.input[input].offset = -MAX_INPUT_OFFSET;
	if (_settings.input[input].offset > MAX_INPUT_OFFSET) _settings.input[input].offset = MAX_INPUT_OFFSET;
	set_settings_saved(0);
	update_volume();
}

unsigned char get_current_input_mode() { return _settings.input[get_input()].input_mode; }
unsigned char get_current_output_mode() { return _settings.input[get_input()].output_mode; }

void set_current_output_mode(unsigned char channels) {
	_settings.input[get_input()].output_mode = channels;
	set_settings_saved(0);
	if (_settings.use_cs3318_volumecontrol_pcb) volumecontrol_set_output_mode(channels);
	update_volume();
	display_volume(1);
}

unsigned char next_output_mode() {
	if (_settings.use_cs3318_volumecontrol_pcb) set_current_output_mode(volumecontrol_next_output_mode());
	return get_current_output_mode();
}

signed char get_output_mode_offset(unsigned char channel) {
	if (_settings.use_cs3318_volumecontrol_pcb) {
		if (volumecontrol_is_hafler_mode()) {
			if (channel==REAR_LEFT || channel==REAR_RIGHT) return -5;
			if (channel==CENTER) return -3;
		}
	}
	return 0;
}

const char *output_channel_string() { return channel_string(get_current_output_mode()); }
const char *input_channel_string() { return channel_string(get_current_input_mode()); }

const char *channel_string(unsigned char channels) {
	return volumecontrol_channel_string(channels);
}

unsigned char get_checksum() { return _state.checksum; }
unsigned char get_current_input_type() { return _settings.input[get_input()].type; }

const char *input_type_string(unsigned char input) {
	switch(_settings.input[input].type)
	{
		case DAC1:	return "dac1";
		case DAC2:	return "dac2";
		case DAC3:	return "dac3";
		case DAC4:	return "dac4";
		case DAC5:	return "dac5";
		case DAC6:	return "dac6";
		case DAC7:	return "dac7";
		case DAC8:	return "dac8";
		case ANALOG1:	return "analog1";
		case ANALOG2:	return "analog2";
		case ANALOG3:	return "analog3";
		case ANALOG4:	return "analog4";
		case ANALOG5:	return "analog5";
		case ANALOG6:	return "analog6";
		case ANALOG7:	return "analog7";
		case ANALOG8:	return "analog8";
		default: 	return "?";
	}
}

const char *input_name_string(unsigned char input) {
	if (_settings.input[input].name < NR_INPUT_NAMES) return input_names[_settings.input[input].name];
	else return "?";
}

const char *channel_name_string(unsigned char channel) {
	return volumecontrol_channel_name_string(channel);
}

void change_input_name(unsigned char input, signed char change) {
	signed char new_input = _settings.input[input].name + change;
	if (new_input < 0) new_input = NR_INPUT_NAMES-1;
	if (new_input > NR_INPUT_NAMES-1) new_input = 0;
	_settings.input[input].name = (unsigned char)new_input;
	set_settings_saved(0);
}

char load_input()
{
	unsigned char input = eeprom_read(EEP_CURRENT_INPUT);	// Read current input from eeprom
	if (input > nr_inputs()-1) return -1;
	set_input(input);
	return 0;
}

void save_input()
{
	eeprom_write(EEP_CURRENT_INPUT, get_input());
}

char load_settings()
{
	char string[21];
	unsigned char cksum;

	eeprom_bulkread(EEP_DATA, &_settings, sizeof(settings_t));
	_state.checksum = eeprom_read(EEP_CHECKSUM);		// Read checksum in eeprom
	cksum = checksum(&_settings, sizeof(settings_t));	// Compute checksom
	if (_state.checksum != cksum) {
		clear_settings();
		return -1;
	}
	_state.settings_saved = 1;
	sprintf(string, "eeprom -> %d bytes", sizeof(settings_t));
	debug(2, string);
	return 0;
}

void save_settings()
{
	char string[21];

	unsigned char cksum;
	if (!settings_saved()) {
		eeprom_bulkwrite(EEP_DATA, &_settings, sizeof(settings_t));
		cksum = checksum(&_settings, sizeof(settings_t));
		eeprom_write(EEP_CHECKSUM, cksum);
		set_settings_saved(1);
		sprintf(string, "%d bytes -> eeprom", sizeof(settings_t));
		debug(2, string);
	}
}

void clear_settings()
{
	memset(&_settings, 0 , sizeof(settings_t));

	default_settings();
}

void setup_settings(unsigned char hw_type)
{
	if (hw_type == 0) return;

	if (hw_type == 1) {
		_settings.use_inputselect_pcb = 0;
		_settings.use_multi_input_pcb = 0;
		_settings.use_dac_pcb = 0;
		_settings.use_cs3318_volumecontrol_pcb = 1;
		_settings.cs3318_type = VC_8CH;
	}
	else if (hw_type == 2) {
		_settings.use_inputselect_pcb = 0;
		_settings.use_multi_input_pcb = 0;
		_settings.use_dac_pcb = 0;
		_settings.use_cs3318_volumecontrol_pcb = 1;
		_settings.cs3318_type = VC_8CHx2;
	}
	default_settings();
}

void default_settings()
{
	unsigned char i;

	// Determine the signal routing from the used pcb's
	if (_settings.use_inputselect_pcb) {
		_settings.nr_inputs = 6;
		// Analog stereo inputs
		_settings.input[0].type = ANALOG1;
		_settings.input[0].name = CD;
		_settings.input[0].analog_select = 0b00000001;
		_settings.input[1].type = ANALOG2;
		_settings.input[1].name = TV;
		_settings.input[1].analog_select = 0b00000010;
		_settings.input[2].type = ANALOG3;
		_settings.input[2].name = DVD;
		_settings.input[2].analog_select = 0b00000100;
		_settings.input[3].type = ANALOG4;
		_settings.input[3].name = AUX;
		_settings.input[3].analog_select = 0b00001000;
		_settings.input[4].type = ANALOG5;
		_settings.input[4].name = COMPUTER;
		_settings.input[4].analog_select = 0b00010000;
		if (_settings.use_dac_pcb) {
			_settings.nr_inputs += 4;
			_settings.input[5].type = DAC1;
			_settings.input[5].name = TV;
			_settings.input[5].analog_select = 0b00100000;
			_settings.input[5].dac_select = 0b00000000;
			_settings.input[6].type = DAC2;
			_settings.input[6].name = TVRECEIVER;
			_settings.input[6].analog_select = 0b00100000;
			_settings.input[6].dac_select = 0b00000001;
			_settings.input[7].type = DAC3;
			_settings.input[7].name = COMPUTER;
			_settings.input[7].analog_select = 0b00100000;
			_settings.input[7].dac_select = 0b00000010;
			_settings.input[8].type = DAC4;
			_settings.input[8].name = AUX;
			_settings.input[8].analog_select = 0b00100000;
			_settings.input[8].dac_select = 0b00000011;
			_settings.input[9].type = DAC5;
			_settings.input[9].name = MPLAYER;
			_settings.input[9].analog_select = 0b00100000;
			_settings.input[9].dac_select = 0b00000100;
			for(i=0;i<nr_inputs();i++) {
				_settings.input[i].input_mode = CH2;
				_settings.input[i].output_mode = CH2;
			}
			if (_settings.use_multi_input_pcb) {
				_settings.nr_inputs += 2;
				_settings.input[10].type = ANALOG7;
				_settings.input[10].name = BLURAY;
				_settings.input[10].input_mode = CH5_SUB;
				_settings.input[10].output_mode = CH5_SUB;
				_settings.input[10].analog_select = 0b01000000;
				_settings.input[11].type = ANALOG8;
				_settings.input[11].name = HTPC;
				_settings.input[11].input_mode = CH5_SUB;
				_settings.input[11].output_mode = CH5_SUB;
				_settings.input[11].analog_select = 0b10000000;
			}
		} else {
			_settings.input[5].type = ANALOG6;
			_settings.input[5].name = TUNER;
			_settings.input[5].analog_select = 0b00100000;
			for(i=0;i<nr_inputs();i++) {
				_settings.input[i].input_mode = CH2;
				_settings.input[i].output_mode = CH2;
			}
			if (_settings.use_multi_input_pcb) {
				_settings.nr_inputs += 2;
				_settings.input[6].type = ANALOG7;
				_settings.input[6].name = HTPC;
				_settings.input[6].input_mode = CH5_SUB;
				_settings.input[6].output_mode = CH5_SUB;
				_settings.input[6].analog_select = 0b01000000;
				_settings.input[7].type = ANALOG8;
				_settings.input[7].name = DVD;
				_settings.input[7].input_mode = CH5_SUB;
				_settings.input[7].output_mode = CH5_SUB;
				_settings.input[7].analog_select = 0b10000000;
			}
		}
	} else 	if (_settings.use_dac_pcb) {
		_settings.nr_inputs = 5;
		// Digital inputs
		_settings.input[0].type = DAC1;
		_settings.input[0].name = TV;
		_settings.input[0].dac_select = 0b00000000;
		_settings.input[1].type = DAC2;
		_settings.input[1].name = HTPC;
		_settings.input[1].dac_select = 0b00000001;
		_settings.input[2].type = DAC3;
		_settings.input[2].name = COMPUTER;
		_settings.input[2].dac_select = 0b00000010;
		_settings.input[3].type = DAC4;
		_settings.input[3].name = AUX;
		_settings.input[3].dac_select = 0b00000011;
		_settings.input[4].type = DAC5;
		_settings.input[4].name = MPLAYER;
		_settings.input[4].dac_select = 0b00000100;
		for(i=0;i<nr_inputs();i++) {
			_settings.input[i].input_mode = CH2;
			_settings.input[i].output_mode = CH2;
		}
	} else {	// Only 1 input
		_settings.nr_inputs = 1;
		_settings.input[0].type = ANALOG1;
		_settings.input[0].name = HTPC;
		if (_settings.cs3318_type == VC_NORMAL) {
			_settings.input[0].input_mode = CH5_SUB;
			_settings.input[0].output_mode = CH5_SUB;
		} else if (_settings.cs3318_type == VC_8CH) {
			_settings.input[0].input_mode = CH8;
			_settings.input[0].output_mode = CH8;
		} else if (_settings.cs3318_type == VC_8CHx2) {
			_settings.input[0].input_mode = CH16;
			_settings.input[0].output_mode = CH16;
		}
	}

	for(i=0;i<nr_inputs();i++) {
		_settings.input[i].enabled = 1;		// All inputs are enabled by default
		_settings.input[i].offset = 0;		// All inputs have zero offset by default
	}

	for(i=0;i<nr_channels();i++) {
		_settings.channel[i].offset = 0;	// All channels have zero offset by default
	}

	// Parameters
	_settings.parameter[VOLUME_POWERUP].value = -40;
	_settings.parameter[AUTO_OFF_MINUTES].value = 0;
	_settings.parameter[IR_ADDRESS].value = 0;
	_settings.parameter[LCD_DELAY].value = 0;
	_settings.parameter[LCD_WIDTH].value = 20;

	_state.checksum = checksum(&_settings, sizeof(settings_t));	// Compute checksom
	set_settings_saved(0);		// Default settings will be stored in eeprom when powered off
}

void preamp_init()
{
	memset(&_settings, 0 , sizeof(settings_t));
	_state.muted = 0;
	_state.powered = 0;			// Initialise
	_state.current_input = 0;
}

void preamp_on()
{
	unsigned char ir_address = get_parameter(IR_ADDRESS);

	if (_settings.use_dac_pcb) dac_init();
	if (_settings.use_cs3318_volumecontrol_pcb) volumecontrol_init(_settings.cs3318_type);
	if (_settings.use_inputselect_pcb) inputselect_init();

	_state.volume = 2*get_parameter(VOLUME_POWERUP);
	_state.muted = 0;
	ir_set_accept_address(ir_address);

	set_powered(1);

	set_input(get_input());

	update_volume();
}

void preamp_off()
{
	if (_settings.use_cs3318_volumecontrol_pcb) {
		volumecontrol_mute();
		volumecontrol_set_hafler(0);
	}
	if (_settings.use_inputselect_pcb) inputselect_set_input(0x00);
	if (_settings.use_dac_pcb) dac_mute();

	set_powered(0);
}

unsigned char checksum(void *dat, unsigned char len)
{
	unsigned short sum = 0xff;
	unsigned char i;

	for(i=0;i<len;i++) {
		sum ^= ((char *)dat)[i];
		sum -= i;
	}
	sum = (sum & 0xff) + (sum >> 8);
	return (unsigned char)sum;
}

signed short get_parameter(unsigned char par) {
	if (par >= MAX_PARAMETER) return 0;

	return _settings.parameter[par].value;
}

void set_parameter(unsigned char par, signed short value) {
	signed short min_value, max_value;

	switch(par) {
		case VOLUME_POWERUP:
			min_value = -80;
			max_value = 0;
			break;
		case AUTO_OFF_MINUTES:
			min_value = 0;
			max_value = 120;
			break;
		case IR_ADDRESS:
			min_value = 0;
			max_value = 31;
			break;
		case LCD_DELAY:
			min_value = 0;
			max_value = 50;
			break;
		case LCD_WIDTH:
			min_value = 16;
			max_value = 20;
			break;
		default: return;
	}

	if (value < min_value) value = min_value;
	if (value > max_value) value = max_value;

	_settings.parameter[par].value = value;
	set_settings_saved(0);
}

void change_parameter(unsigned char par, signed char change) {
	if (par >= MAX_PARAMETER) return;

	set_parameter(par, _settings.parameter[par].value+change);
}

const char *get_parameter_name(unsigned char par) {
	switch(par) {
		case VOLUME_POWERUP: return "Powerup volume";
		case AUTO_OFF_MINUTES: return "Auto-off time";
		case IR_ADDRESS: return "Infrared address";
		case LCD_DELAY: return "LCD delay";
		case LCD_WIDTH: return "LCD width";
		default: return "?";
	}
}

const char *get_parameter_cmd_name(unsigned char par) {
	switch(par) {
		case VOLUME_POWERUP: return "vol-powerup";
		case AUTO_OFF_MINUTES: return "time-auto-off";
		case IR_ADDRESS: return "ir-addr";
		case LCD_DELAY: return "lcd-delay";
		case LCD_WIDTH: return "lcd-width";
		default: return "?";
	}
}

const char *get_parameter_unit(unsigned char par) {
	switch(par) {
		case VOLUME_POWERUP: return "dB";
		case AUTO_OFF_MINUTES: return "min";
		case IR_ADDRESS: return "-";
		case LCD_DELAY: return "100us";
		case LCD_WIDTH: return "-";
		default: return "?";
	}
}

unsigned char nr_parameters() { return MAX_PARAMETER; }
