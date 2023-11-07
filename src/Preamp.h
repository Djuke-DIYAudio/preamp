#ifndef PREAMP_H
#define PREAMP_H

#include <stdint.h>
#include <stdbool.h>

// Global variables

// EEPROM offsets
#define EEP_CHECKSUM		0x00
#define EEP_CURRENT_INPUT	0x01
#define EEP_DATA            0x10

// Limits
#define MAX_INPUTS          12
#define MAX_CHANNELS		16
#define MAX_INPUT_OFFSET	40      // +/-20dB
#define MAX_VOLUME          20      // +10 dB
#define MIN_VOLUME          -192    // -96 dB

// Function definitions
int8_t load_input(void);
void save_input(void);
int8_t load_settings(void);
void save_settings(void);
void default_settings(void);
void clear_settings(void);
void setup_settings(uint8_t hw_type);
void preamp_init(void);
void preamp_on(void);
void preamp_off(void);
uint8_t checksum(void *dat, uint8_t len );
uint8_t get_nr_hw_setups(void);                 // Get number of hw setup related questions
const char *hw_setup_name(uint8_t hw_setup);	// Get description of hw setup
const char *hw_setup_value(uint8_t hw_setup);	// Get value of hw setup
void toggle_hw_setup(uint8_t hw_setup);         // Toggle value for hw setup question
const char *preamp_name(void);
const char *preamp_version(void);
const char *preamp_date(void);
void update_volume(void);
bool is_powered(void);
bool is_muted(void);
void set_powered(bool val);
void set_muted(bool val);
bool settings_saved(void);
void set_settings_saved(bool saved);
uint8_t get_checksum(void);

// Input related functions
bool is_enabled(uint8_t input);
void toggle_enabled(uint8_t input);
uint8_t nr_inputs(void);				// Get number of inputs
uint8_t get_input_type(void);			// Get current input type
uint8_t get_analog_select(void);		// Get analog input select value
uint8_t get_dac_select(void);			// Get DAC input select value
uint8_t get_input(void);				// Get current input
void set_input(uint8_t val);			// Set current input
uint8_t next_input(void);				// Select next input
uint8_t previous_input(void);			// Select previous input
bool input_changed(void);				// Has input changed since previous call
bool is_dac_input(void);
bool is_analog_input(void);
const char *input_type_string(uint8_t input);
uint8_t get_current_input_type(void);                       // Get type of current input
const char *input_name_string(uint8_t input);
void change_input_name(uint8_t input, signed char change);	// Select different name for input
int8_t get_input_offset(uint8_t input);                     // Get offset of input
void add_input_offset(uint8_t input, int8_t offset);		// Add value to input offset
uint8_t get_current_input_mode(void);                       // Get current input mode
uint8_t get_current_output_mode(void);                      // Get current output mode
void set_current_output_mode(uint8_t channels);             // Set current output mode
uint8_t next_output_mode(void);                             // Select next output mode

// Volume related functions
int16_t get_volume(void);				// Get global volume
void set_volume(int16_t vol);			// Set global volume
void add_volume(int8_t val);			// Add val to global volume
const char *volume_string(void);		// returns volume string
bool has_volume_control(void);          // Indicates whether volume control is used
bool has_signal_level(void);            // Indicates whether signal level is measured
bool has_headphones(void);              // Indicates whether headphones input is used
uint8_t nr_channels(void);				// Get number of channels
uint8_t get_nr_output_channels(void);	// Get current number of output channels
int8_t get_channel_offset(uint8_t channel);                 // Get offset of channel
void add_channel_offset(uint8_t channel, int8_t offset);	// Add value to channel offset
void set_channel_offset(uint8_t channel, int8_t offset);	// Set channel offset
int8_t get_current_input_offset(void);                      // Get offset of current input
int8_t get_output_mode_offset(uint8_t channel);             // Get channel offset of current output mode
const char *channel_name_string(uint8_t channel);
const char *surround_mode_string(uint8_t mode);
const char *input_channel_string(void);
const char *output_channel_string(void);
const char *channel_string(uint8_t channels);

// Parameter related functions
int16_t get_parameter(uint8_t par);                 // Get parameter value
void set_parameter(uint8_t par, int16_t value);     // Set parameter value
void change_parameter(uint8_t par, int8_t change);	// Change parameter value
const char *get_parameter_name(uint8_t par);		// Get parameter name
const char *get_parameter_cmd_name(uint8_t par);	// Get parameter command name
const char *get_parameter_unit(uint8_t par);		// Get parameter unit
uint8_t nr_parameters(void);                        // Number of parameters

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
	AUDIOCAST = 0,
	AUX,
	BLURAY,
	CABLE,
	CD,
	COMPUTER,
	DVD,
	HTPC,
	MPLAYER,
	PHONO,
	RECEIVER,
	SAT,
	TAPE,
	TV,
	TUNER,
	TVRECEIVER,
	VIDEO,
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
	int8_t offset;				// Channel offset (in half dB steps)
} channel_t;

typedef struct
{
	int8_t offset;				// Input offset (in half dB steps)
	bool enabled;               // Is input enabled?
	uint8_t analog_select;		// Value to select the analog input
	uint8_t dac_select;         // Value to select the DAC input
	uint8_t name;               // input name
	uint8_t type;               // type (analog, coax, opt..)
	uint8_t input_mode;         // input channels mode
	uint8_t output_mode;		// output channels mode
} input_t;

typedef struct
{
	int16_t value;
} parameter_t;

typedef struct
{
	parameter_t parameter[MAX_PARAMETER];	// List of parameters
	channel_t channel[MAX_CHANNELS];		// Channel information
	input_t input[MAX_INPUTS];              // Input information
	uint8_t nr_inputs;                      // Number of inputs
	bool use_dac_pcb;                       // Use DAC pcb
	bool use_inputselect_pcb;               // Use Input select pcb
	bool use_multi_input_pcb;               // Use 5.1 input pcb
	bool use_cs3318_volumecontrol_pcb;		// Use Volume control pcb
	uint8_t cs3318_type;		            // Volume control type (0=normal, 1=1x8ch, 2=2x8ch)
} settings_t;

// Global state
typedef struct
{
	bool powered;				// Current power state
	bool muted;					// Curent mute state
	int16_t volume;				// Global volume (in half dB steps)
	uint8_t current_input;		// Currently selected input
	bool settings_saved;		// Settings saved state
	uint8_t checksum;			// Settings checksum
} state_t;

#endif
