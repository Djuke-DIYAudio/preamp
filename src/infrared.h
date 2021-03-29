#ifndef INFRARED_H
#define INFRARED_H

#define IR_MAX 	30			// Maximum number of state transitions per code
#define IR_TICKS_TIMEOUT	30	// Receival is ready if this amount of ticks has passed without a transition

// RC5 protocol definitions
#define RC5_MIN_LENGTH		220
#define RC5_MAX_LENGTH		240
#define RC5_HALF_BIT_MIN_TIME	7	// Min ticks for a cycle in RC5
#define RC5_HALF_BIT_MAX_TIME	10	// Max ticks for a cycle in RC5

enum IR_TYPE {
	RC5,
	RC6,
	MAX_IR_TYPE
};

typedef struct
{
	unsigned char ticks[IR_MAX];		// Number of ticks since last transition
	unsigned char transitions;		// Total number of transitions
	unsigned char previous_value;		// Previous input value
	unsigned char ready;			// Data receival ready
	unsigned short length;			// Length in ticks
} ir_raw_t;

typedef struct
{
	enum IR_TYPE type;
	unsigned short data;			// Full code data
	unsigned short previous_data;		// Previuos code data
	unsigned char toggle;			// Toggle bit
	unsigned char previous_toggle;		// Previous toggle bit
	unsigned char address;			// Address
	unsigned char command;			// Command
	unsigned char ready;			// Bit indicating code is ready for use
	unsigned char same_codes;		// Amount of same codes passed after pressing new button
} ir_code_t;

/* Definition for reading infrared receiver  *
 * -1: Nothing                               *
 * -2..-13: Error in bit number              */

#define IR_0				0
#define IR_1				1
#define IR_2				2
#define IR_3				3
#define IR_4				4
#define IR_5				5
#define IR_6				6
#define IR_7				7
#define IR_8				8
#define IR_9				9
#define IR_POWER			12
#define IR_MUTE				13
#define IR_VOLUME_UP			16
#define IR_VOLUME_DOWN			17
#define IR_UP				32
#define IR_DOWN				33
#define IR_LAST				34
#define IR_SELECT			37
#define IR_FASTFORWARD			41
#define IR_REWIND			43
#define IR_STOP				46
#define IR_YELLOW			50
#define IR_BLUE				52
#define IR_GREEN			54
#define IR_RED				55
#define IR_AV				56
#define IR_MENU				59
#define IR_PREVIOUS			60
#define IR_NEXT				63
#define IR_MENU2			82
#define IR_EXIT				83
#define IR_LEFT				85
#define IR_RIGHT			86

// External functions
void ir_interrupt_handler(unsigned char value);	// Interrupt handler to be called when timer interrupt occurs
void ir_init();					// Called once at startup
void ir_set_accept_address(unsigned char address);	// Set the address (only received codes with same address are accepted)
void ir_set_accept_type(unsigned char type);		// Set the type (only received codes of same type are accepted)
char ir_receive();				// Returns the latest received command
unsigned char ir_toggle_bit();			// Toggle bit
unsigned char ir_toggled();			// Button was toggled
unsigned char ir_same_codes();			// Amount of same codes received (button pressed for longer time)
unsigned char ir_code_ready();
const char *ir_type_string();
void ir_decode();

// Functions for debugging
unsigned char ir_raw_value();
unsigned char ir_raw_transitions();
unsigned char ir_raw_ticks(unsigned char at);
unsigned short ir_raw_length();
unsigned char ir_raw_ready();
unsigned char ir_address();
unsigned char ir_command();
unsigned short ir_data();

// Internal functions
static void rc5_decode();
static void rc6_decode();

#endif
