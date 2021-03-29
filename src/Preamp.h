#ifndef PREAMP_H
#define PREAMP_H

#include <stdint.h>

// Global variables

// EEPROM offsets
#define EEP_CHECKSUM		0x00
#define EEP_CURRENT_INPUT	0x01
#define EEP_DATA		0x10

// Limits
#define MAX_INPUTS		12
#define MAX_CHANNELS		16
#define MAX_INPUT_OFFSET	40 // +/-20dB
#define MAX_VOLUME		20 // +10 dB
#define MIN_VOLUME		-192 // -96 dB

// Function definitions
char load_input();
void save_input();
char load_settings();
void save_settings();
void default_settings();
void clear_settings();
void setup_settings(unsigned char hw_type);
void preamp_init();
void preamp_on();
void preamp_off();
unsigned char checksum(void *dat, unsigned char len );

// Hardware setup functions
unsigned char get_nr_hw_setups();			// Get number of hw setup related questions
const char *hw_setup_name(unsigned char hw_setup);	// Get description of hw setup
const char *hw_setup_value(unsigned char hw_setup);	// Get value of hw setup
void toggle_hw_setup(unsigned char hw_setup);		// Toggle value for hw setup question

// General functions
const char *preamp_name();
const char *preamp_version();
const char *preamp_date();
void update_volume();
char is_powered();
char is_muted();
void set_powered(char val);
void set_muted(char val);
char settings_saved();
void set_settings_saved(char saved);
unsigned char get_checksum();

// Input related functions
char is_enabled(unsigned char input);
void toggle_enabled(unsigned char input);
unsigned char nr_inputs();				// Get number of inputs
unsigned char get_input_type();				// Get current input type
unsigned char get_analog_select();			// Get analog input select value
unsigned char get_dac_select();				// Get DAC input select value
unsigned char get_input();				// Get current input
void set_input(unsigned char val);			// Set current input
unsigned char next_input();				// Select next input
unsigned char previous_input();				// Select previous input
unsigned char input_changed();				// Has input changed since previous call
char is_dac_input();
char is_analog_input();
const char *input_type_string(unsigned char input);
signed char get_current_input_type();			// Get type of current input
const char *input_name_string(unsigned char input);
void change_input_name(unsigned char input, char change);	// Select different name for input
signed char get_input_offset(unsigned char input);	// Get offset of input
void add_input_offset(unsigned char input, char offset);		// Add value to input offset
unsigned char get_current_input_mode();			// Get current input mode
unsigned char get_current_output_mode();		// Get current output mode
void set_current_output_mode(unsigned char channels);	// Set current output mode
unsigned char next_output_mode();			// Select next output mode

// Volume related functions
short get_volume();				// Get global volume
void set_volume(short vol);			// Set global volume
void add_volume(char val);			// Add val to global volume
const char *volume_string();			// returns volume string
unsigned char has_volume_control();		// Indicates whether volume control is used 
unsigned char has_signal_level();		// Indicates whether signal level is measured
unsigned char has_headphones();        // Indicates whether headphones input is used

// Channel related functions
unsigned char nr_channels();				// Get number of channels
unsigned char get_nr_output_channels();			// Get current number of output channels
short get_channel_offset(unsigned char channel);	// Get offset of channel
void add_channel_offset(unsigned char channel, char offset);	// Add value to channel offset
void set_channel_offset(unsigned char channel, short offset);	// Set channel offset
signed char get_current_input_offset();			// Get offset of current input
signed char get_output_mode_offset(unsigned char channel);	// Get channel offset of current output mode
const char *channel_name_string(unsigned char channel);
const char *surround_mode_string(unsigned char mode);
const char *input_channel_string();
const char *output_channel_string();
const char *channel_string(unsigned char channels);

// Parameter related functions
signed short get_parameter(unsigned char par);			// Get parameter value
void set_parameter(unsigned char par, short value);	// Set parameter value
void change_parameter(unsigned char par, char change);	// Change parameter value
const char *get_parameter_name(unsigned char par);		// Get parameter name
const char *get_parameter_cmd_name(unsigned char par);		// Get parameter command name
const char *get_parameter_unit(unsigned char par);		// Get parameter unit
unsigned char nr_parameters();					// Number of parameters

enum PARAMETER {
	VOLUME_POWERUP = 0,
	AUTO_OFF_MINUTES,
	IR_ADDRESS,
	LCD_DELAY,
	LCD_WIDTH,
	MAX_PARAMETER
};

enum HW_SETUP {
	STEREO_INPUT = 0,
	MULTI_INPUT,
	DAC_INPUT,
	CS3318_VOL_CONTROL,
	CS3318_TYPE,
	NR_HW_SETUPS
};

enum NAME {
	CD = 0,
	DVD,
	RECEIVER,
	TUNER,
	COMPUTER,
	LAPTOP,
	TV,
	DIGTV,
	CABLE,
	SAT,
	TAPE,
	VIDEO,
	AUX,
	PHONO,
	HDREC,
	DVR,
	HTPC,
	BLURAY,
	MPLAYER,
	SPARE,
	NR_INPUT_NAMES
};

enum TYPE {
	ANALOG1 = 0,
	ANALOG2,
	ANALOG3,
	ANALOG4,
	ANALOG5,
	ANALOG6,
	ANALOG7,
	ANALOG8,
	DAC1,
	DAC2,
	DAC3,
	DAC4,
	DAC5,
	DAC6,
	DAC7,
	DAC8,
	NR_INPUT_TYPES
};

//Type definitions
typedef struct
{
	int16_t offset;				// Channel offset (in half dB steps)
} channel_t;

typedef struct
{
	int16_t offset;				// Input offset (in half dB steps)
	signed char enabled;			// Is input enabled?
	unsigned char analog_select;		// Value to select the analog input
	unsigned char dac_select;		// Value to select the DAC input
	unsigned char name;			// input name
	unsigned char type;			// type (analog, coax, opt..)
	unsigned char input_mode;		// input channels mode
	unsigned char output_mode;		// output channels mode
} input_t;

typedef struct
{
	signed short value;
} parameter_t;

typedef struct
{
	parameter_t parameter[MAX_PARAMETER];		// List of parameters
	channel_t channel[MAX_CHANNELS];		// Channel information
	input_t input[MAX_INPUTS];			// Input information
	unsigned char nr_inputs;			// Number of inputs
	char use_dac_pcb;				// Use DAC pcb
	char use_inputselect_pcb;			// Use Input select pcb
	char use_multi_input_pcb;			// Use 5.1 input pcb
	char use_cs3318_volumecontrol_pcb;		// Use Volume control pcb
	char cs3318_type;		            // Volume control type (0=normal, 1=1x8ch, 2=2x8ch)
} settings_t;

// Global state
typedef struct
{
	char powered;					// Current power state
	char muted;					// Curent mute state
	signed short volume;				// Global volume (in half dB steps)
	unsigned char current_input;			// Currently selected input
	char settings_saved;				// Settings saved state
	unsigned char checksum;				// Settings checksum
} state_t;

#endif
