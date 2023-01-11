#include "infrared.h"

#include "PreampController.h"
#include "EventHandler.h"

static ir_raw_t ir_raw;		// Raw received code timing data
static ir_code_t ir_code;	// Interpreted code data

static unsigned char accept_address = 0;	// Received address should equal accept_address for the code to be accepted
static unsigned char accept_type = PA_RC5;		// Received type should equal accept_type for the code to be accepted

void ir_interrupt_handler(unsigned char value) {
	static unsigned char ticks_since_transition = 0;

	if (ticks_since_transition < 255) ticks_since_transition++;	// Prevent overflow

	if (!ir_raw.ready && value==1 && ticks_since_transition > IR_TICKS_TIMEOUT) {
		ir_raw.ready = 1;
		TriggerEvent(EVENT_INFRARED);
//		ir_decode();	// Now try to decode the data
	}

	if (value != ir_raw.previous_value) {	// State change
		if (ir_raw.ready && value==0) {	// High->Low so new message started
			ir_raw.transitions = 0;
			ir_raw.length = 0;
			ir_raw.ready = 0;
		} else {
			ir_raw.ticks[ir_raw.transitions] = ticks_since_transition;
			ir_raw.length += ticks_since_transition;
			ir_raw.transitions++;
			if (ir_raw.transitions >= IR_MAX) ir_raw.ready = 1;	// Too many transitions, stop further processing
		}
		ir_raw.previous_value = value;
		ticks_since_transition = 0;
	}

}

// Raw data functions
//unsigned char ir_raw_value(void) { return ir_raw.previous_value; }
//unsigned char ir_raw_transitions(void) { return ir_raw.transitions; }
//unsigned char ir_raw_ticks(unsigned char at) { return ir_raw.ticks[at]; }
//unsigned short ir_raw_length(void) { return ir_raw.length; }
//unsigned char ir_raw_ready(void) { return ir_raw.ready; }


// Interpreted code functions
//unsigned char ir_toggle_bit(void) { return ir_code.toggle; }				// Return last received toggle bit
//unsigned char ir_toggled(void) { return (ir_code.toggle!=ir_code.previous_toggle); }	// Toggle bit was changed
unsigned char ir_address(void) { return ir_code.address; }					// Return last received address
unsigned char ir_command(void) { return ir_code.command; }					// Return last received command
//unsigned short ir_data(void) { return ir_code.data; }					// Return last received data
//unsigned char ir_code_ready(void) { return ir_code.ready; }
unsigned char ir_same_codes(void) { return ir_code.same_codes; }

const char *ir_type_string(void) {
	switch(ir_code.type) {
		case PA_RC5: return "RC5";
		case PA_RC6: return "RC6";
		default: return "?";
	}
}

void ir_init(void) {
	ir_raw.transitions = 0;
	ir_raw.ready = 0;
	ir_raw.previous_value = 1;

	ir_code.ready = 0;
	ir_code.previous_toggle = 255;
	ir_code.toggle = 0;
	ir_code.address = 0;
	ir_code.command = 0;
	ir_code.same_codes = 0;

	ir_set_accept_address(0x00);	// TV
	ir_set_accept_type(PA_RC5);
}

int8_t ir_receive(void) {
	if (ir_code.ready) {
		ir_code.ready = 0;	// Do not process multiple times
		if (ir_code.address != accept_address) return -2;
		if (ir_code.type != accept_type) return -3;
		return (int8_t)ir_code.command;
	} else return -1;
}

void ir_set_accept_address(unsigned char address) { accept_address = address; }
void ir_set_accept_type(unsigned char type) { accept_type = type; }

void ir_decode(void) {
	if (ir_raw.length > RC5_MIN_LENGTH && ir_raw.length < RC5_MAX_LENGTH) rc5_decode();
}

static void rc5_decode(void) {
	unsigned char i, value=1;   // Start value
	unsigned char ticks=0;

	if (!ir_raw.ready) return;	// Raw code is being received

	ir_code.data = 1;   // Start bit is always 1
	ir_code.ready = 0;

	for(i=0;i<ir_raw.transitions;i++) {
		ticks += ir_raw.ticks[i];
		if (value) value = 0;
		else value = 1;
	        if (ticks < 2*RC5_HALF_BIT_MIN_TIME) continue; // Not at bit transition yet, read next transition
		if (ticks >= 2*RC5_HALF_BIT_MIN_TIME && ticks <= 2*RC5_HALF_BIT_MAX_TIME) { // We are at the bit transition
			ir_code.data <<= 1;      // Shift data to next bit
			ir_code.data += value;
			ticks = 0;  // 
		} else if (ticks > 2*RC5_HALF_BIT_MAX_TIME) { // Time has passed, but no transition -> exiting
			return;
		}
	}

	if (((ir_code.data >> 12) & 0b1) == 0b1) ir_code.command = 0;	// RC5 code, two start bits of 1
	else ir_code.command = 0b1000000;				// RC5X code, second start bit 0 gives extra command bit

	// Count amount of same codes
	if (ir_code.data == ir_code.previous_data) {
		if (ir_code.same_codes < 255) ir_code.same_codes++;	// Prevent overflow
	} else {
		ir_code.same_codes = 0;					// New code received
		ir_code.previous_data = ir_code.data;
	}

	ir_code.previous_toggle = ir_code.toggle;

	ir_code.toggle = (ir_code.data >> 11) & 0b1;
	ir_code.address = (ir_code.data >> 6) & 0b11111;
	ir_code.command += (ir_code.data) & 0b111111;
	ir_code.type = PA_RC5;
	ir_code.ready = 1;
}
