#include <stdio.h>
#include <string.h>

#include "PreampController.h"
#include "InputSelect.h"
#include "DAC.h"
#include "VolumeControl.h"

#include "infrared.h"
#include "lcd.h"
#include "time_delay.h"
#include "terminal.h"

#include "Preamp.h"
#include "menu.h"
#include "EventHandler.h"

// Device configuration bits
#pragma config OSC=HSPLL,FCMEN=OFF,IESO=OFF
#pragma config PWRT=ON,BOREN=ON,BORV=2
#pragma config WDT=OFF,WDTPS=128
#pragma config MCLRE=ON,LPT1OSC=OFF,CCP2MX=PORTC,PBADEN=OFF
#pragma config STVREN=ON,LVP=OFF,XINST=OFF,DEBUG=OFF
#pragma config CP0=OFF,CP1=OFF,CP2=OFF,CP3=OFF
#pragma config CPB=OFF,CPD=OFF
#pragma config WRT0=OFF,WRT1=OFF,WRT2=OFF,WRT3=OFF
#pragma config WRTC=OFF,WRTB=OFF,WRTD=OFF
#pragma config EBTR0=OFF,EBTR1=OFF,EBTR2=OFF,EBTR3=OFF
#pragma config EBTRB=OFF

// Interrupt service routine
#define TIMER1_TICKS_PER_SECOND		19		// CLOCK_FREQ/(4*65536*2)
#define TIMER1_TITLE_TICKS		5				// About 4x per second
volatile uint8_t _timer0_ticks = 0;		// Number of ticks
volatile uint8_t _timer1_ticks = 0;		// Number of ticks
volatile uint8_t _timer1_title_ticks = 0;		// Number of ticks for title update
volatile uint8_t _timer1_seconds = 0;		// Number of seconds
volatile uint8_t _timer1_minutes = 0;		// Number of minutes
volatile float _signal_level = 0;			// Signal level (in Volt)
volatile float _signal_offset = 0.0;			// Signal offset (in Volt)
volatile uint8_t _silent_ticks = 0;		// Number of ticks that no signal is detected
volatile uint8_t _silent_seconds = 0;		// Amount of seconds that no signal is detected
volatile uint8_t _silent_minutes = 0;		// Amount of minutes that no signal is detected
volatile uint8_t _standby_seconds = 0;		// Standby time in seconds
volatile uint8_t _standby_minutes = 0;		// Standby time in minutes
volatile uint8_t _standby_hours = 0;		// Standby time in hours
volatile uint8_t _standby_days = 0;		// Standby time in days
volatile uint8_t _on_seconds = 0;			// On time in seconds
volatile uint8_t _on_minutes = 0;			// On time in minutes
volatile uint8_t _on_hours = 0;			// On time in hours
volatile uint8_t _on_days = 0;			// On time in days
volatile uint8_t _encoder_counter = 0;		// Encoder counter

void ad_handler(void)
{
	// Signal level
	uint8_t level = ADRESH;
	float voltage_level = level * (5.0f/256);	// AD register count = Vout/5*256

	_signal_offset = 0.01f * voltage_level + 0.99f * _signal_offset;	// Moving average filter to determine offset
	if (_signal_offset > 0.5f) _signal_offset = 0.5f;
	_signal_level = voltage_level - _signal_offset;			// Change in signal level
}

void tmr0_handler(void)
{
	static uint8_t _enc_a = 1;

	_timer0_ticks++;
	// Update encoder register
	if (_enc_a && !ENC_A) {				// Falling edge of ENC_A
		if (ENC_B) _encoder_counter++;
		else _encoder_counter--;
	} else if (!_enc_a && ENC_A) {			// Rising edge of ENC_A
		if (ENC_B) _encoder_counter--;
		else _encoder_counter++;
	}
	_enc_a = ENC_A;

	// Update infrared timing data
	// At half the interrupt rate (every 102.4us instead of 51.2us)
	if (_timer0_ticks % 2) ir_interrupt_handler(IR==1?1:0);
}

void tmr1_handler(void)
{
	uint16_t auto_off_minutes = (uint16_t)get_parameter(AUTO_OFF_MINUTES);

	_timer1_ticks++;
	if (_timer1_ticks >= TIMER1_TICKS_PER_SECOND) {
		_timer1_seconds++;
		_timer1_ticks=0;
		if (_timer1_seconds >= 60) { _timer1_minutes++; _timer1_seconds = 0; }
		if (POWER) {
			_on_seconds++;
			if (_on_seconds >= 60) { _on_minutes++; _on_seconds = 0; }
			if (_on_minutes >= 60) { _on_hours++; _on_minutes = 0; }
			if (_on_hours >= 24) { _on_days++; _on_hours = 0; }
		} else {
			_standby_seconds++;
			if (_standby_seconds >= 60) { _standby_minutes++; _standby_seconds = 0; }
			if (_standby_minutes >= 60) { _standby_hours++; _standby_minutes = 0; }
			if (_standby_hours >= 24) { _standby_days++; _standby_hours = 0; }
		}
	}

	_timer1_title_ticks++;
	if (_timer1_title_ticks >= TIMER1_TITLE_TICKS) {
					_timer1_title_ticks=0;
					if (is_powered() && show_title()) TriggerEvent(EVENT_DISPLAY_TITLE);
	}


	// determine how long no input signal is detected
	if (_signal_level < 0.1) _silent_ticks++;
	else {
		_silent_ticks = 0;
		_silent_seconds = 0;
		_silent_minutes = 0;
	}
	if (_silent_ticks >= TIMER1_TICKS_PER_SECOND) {
		_silent_seconds++;
		_silent_ticks=0;
		if (_silent_seconds >= 60) {
			_silent_minutes++;
			_silent_seconds = 0;
			if (is_powered() && auto_off_minutes != 0 && _silent_minutes == auto_off_minutes) TriggerEvent(EVENT_AUTO_POWEROFF);
		}
	}

	if (button_power_pressed()) TriggerEvent(EVENT_POWER);
   if (button_menu_pressed()) TriggerEvent(EVENT_MENU);
   if (button_left_pressed()) TriggerEvent(EVENT_LEFT);
   if (button_right_pressed()) TriggerEvent(EVENT_RIGHT);
   if (button_select_pressed()) TriggerEvent(EVENT_SELECT);
   if (encoder_counter_changed()) TriggerEvent(EVENT_ENCODER);
	if (is_powered()) {
	   if (headphones_connected_changed()) TriggerEvent(EVENT_HP);
	   if (is_dac_input() && dac_input_changed()) TriggerEvent(EVENT_DAC);
	   if (signal_level_changed()) TriggerEvent(EVENT_SIGNAL_LEVEL);
   }

	if (!ADCON0bits.GO) ADCON0bits.GO = 1; // Trigger A/D conversion
}

void lvd_handler(void)
{
	if (is_powered()) {
		volumecontrol_mute();
        
		delay_ms(16);
        
		SSPCON1bits.SSPEN = 0; // Disable serial port
		POWER = 0;
		inputselect_set_input(0);
		dac_set_input(0);
		HAFLER_RELAY = 0;
		set_powered(0);
	}
	save_input();
//	save_settings();	// Temporarily switched off
}

void rx_handler(void)
{
	term_receive(RCREG);
}

//uint8_t get_timer0_ticks(void) { return _timer0_ticks; }
//uint8_t get_timer1_seconds(void) { return _timer1_seconds; }
//uint8_t get_timer1_minutes(void) { return _timer1_minutes; }
uint8_t get_silent_minutes(void) { return _silent_minutes; }
uint8_t get_silent_seconds(void) { return _silent_seconds; }

void reset_silent_counter(void) {
	INTCONbits.GIE = 0;	// Disable interrupts
	// Reset silent time values
	_silent_ticks = 0;
	_silent_seconds = 0;
	_silent_minutes = 0;
	INTCONbits.GIE = 1;	// Enable interrupts

}

uint8_t get_standby_seconds(void) { return _standby_seconds; }
uint8_t get_standby_minutes(void) { return _standby_minutes; }
uint8_t get_standby_hours(void) { return _standby_hours; }
uint8_t get_standby_days(void) { return _standby_days; }
uint8_t get_on_seconds(void) { return _on_seconds; }
uint8_t get_on_minutes(void) { return _on_minutes; }
uint8_t get_on_hours(void) { return _on_hours; }
uint8_t get_on_days(void) { return _on_days; }

float get_signal_offset(void) {
	float offset;
	INTCONbits.GIE = 0;	// Disable interrupts
	offset = _signal_offset;
	INTCONbits.GIE = 1;	// Enable interrupts
	return offset;
}

float get_signal_level(void) {
	float level;
	INTCONbits.GIE = 0;	// Disable interrupts
	level = _signal_level;
	INTCONbits.GIE = 1;	// Enable interrupts
	return level;
}

bool signal_level_changed(void) {
	static float prev_level = 0;
	float level = get_signal_level();
	if (level > prev_level+0.1 || level < prev_level-0.1) {
		prev_level = level;
		return true;
	} else return false;
}

bool headphones_connected(void) {
	bool connected;
	connected = (HEADPHONES ? true : false);
	return connected;
}

bool headphones_connected_changed(void) {
	static bool prev_hp_connected = false;
	bool hp_connected = headphones_connected();
	if (hp_connected != prev_hp_connected) {
		prev_hp_connected = hp_connected;
		return true;
	} else {
		prev_hp_connected = hp_connected;
		return false;
	}
}

uint8_t get_encoder_counter(void) { return _encoder_counter; }

bool encoder_counter_changed(void) {
	static uint8_t prev_encoder_counter = 0;
	uint8_t encoder_counter = _encoder_counter;
	if (encoder_counter != prev_encoder_counter) {
		prev_encoder_counter = encoder_counter;
		return true;
	} else return false;
}

int8_t get_encoder_diff(void) {
	static uint8_t prev_encoder_counter = 0;
	uint8_t encoder_counter = _encoder_counter;
	int8_t diff = (int8_t)(encoder_counter - prev_encoder_counter);
	prev_encoder_counter = encoder_counter;
	return diff;
}

void ad_init(void) {
	ADCON1 = 0b00001111; // All digital I/O
//	ADCON1 = 0b00000000; // All analog I/O
	ADCON2 = 0b00000010; // 0TAD, Fosc/32 (1.25 MHz)
	ADCON0 = 0b00001101; // Select RA3, enable AD converter
	IPR1bits.ADIP = 0;	// Low-prio interrupt
	PIR1bits.ADIF = 0;	// Clear interrupt flag
	PIE1bits.ADIE = 1;	// Enable interrupt

}

void timer0_init(void) {
	// Counter overflows at CLOCK_FREQ/4/256/2 times/s (every 51.2us)
	T0CON = 0;
	T0CONbits.T08BIT = 1;	// 8-bit timer
	T0CONbits.T0CS = 0;	// Use internal clock
	T0CONbits.PSA = 0;	// Prescaler enabled
	T0CONbits.T0PS2 = 0;	// Prescaler to 1:2 for PLL
	T0CONbits.T0PS1 = 0;	// Prescaler to 1:2 for PLL
	T0CONbits.T0PS0 = 0;	// Prescaler to 1:2 for PLL
	T0CONbits.TMR0ON = 1;	// Enable timer
	INTCON2bits.TMR0IP = 1;	// High-prio interrupt
	INTCONbits.TMR0IF = 0;	// Clear interrupt flag
	INTCONbits.TMR0IE = 1;	// Enable interrupt
}

void timer1_init(void) {
	_timer1_seconds = 0;

	// Counter overflows at CLOCK_FREQ/4/65536/8 times/s (every 52.43ms)
	T1CON = 0;
	T1CONbits.RD16 = 1;	// Read in 16-bit operation
	T1CONbits.T1CKPS0 = 1;	// Prescaler to 1:8 for PLL
	T1CONbits.T1CKPS1 = 1;  // Prescaler to 1:8 for PLL
//	T1CONbits.T1CKPS0 = 1;	// Prescaler to 1:2 for HS
//	T1CONbits.T1CKPS1 = 0;  // Prescaler to 1:2 for HS
	T1CONbits.T1OSCEN = 0;	// Oscillator disabled
	T1CONbits.TMR1CS = 0;	// Use internal clock
	T1CONbits.TMR1ON = 1;	// Enable timer
	IPR1bits.TMR1IP = 0;	// Low-prio interrupt
	PIR1bits.TMR1IF = 1;	// Clear interrupt flag
	PIE1bits.TMR1IE = 1;	// Enable interrupt
}

void lvd_init(void) {
	HLVDCONbits.VDIRMAG = 0; // Event when voltage below treshold
	// LVD voltage of 4.33 V
	HLVDCONbits.HLVDL3 = 1;
	HLVDCONbits.HLVDL2 = 1;
	HLVDCONbits.HLVDL1 = 0;
	HLVDCONbits.HLVDL0 = 1;
	PIE2bits.HLVDIE = 0;		// Disable interrupt
	HLVDCONbits.HLVDEN = 1;		// Enable LVD
	while (!HLVDCONbits.IRVST);	// Wait for IRVST
	IPR2bits.HLVDIP = 1;		// High-prio interrupt
	PIR2bits.HLVDIF = 0;		// Clear interrupt flag
	PIE2bits.HLVDIE = 1;		// Enable interrupt
}

void isr_init(void) {
	/*Enable high and low priority interrupts */
	RCONbits.IPEN = 1;	// Enable priority interrupts

	ad_init();
	timer0_init();
	timer1_init();
	lvd_init();
	term_init();

	/* Enable interrupts */
	INTCONbits.PEIE = 1;	// Enable peripheral interrupts
	INTCONbits.GIEH = 1;	// Enable high-prio interrupts
	INTCONbits.GIEL = 1;	// Enable low-prio interrupts
}

void spi_init(void)
{
	TRISCbits.TRISC3 = 0;	// SCK pin is output
	TRISCbits.TRISC4 = 1;	// SDI pin is input
	TRISCbits.TRISC5 = 0;	// SDO pin is output

	// Init the spi port
	SSPSTATbits.SMP = 0;
	SSPSTATbits.CKE = 1; // Data transmitted on rising edge of SCK
	SSPCON1bits.CKP = 0; // Low idle clock polarity
	SSPCON1bits.SSPM0 = 1; // Master SPI, Fosc/16 (2.5Mhz)
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM3 = 0;
	SSPCON1bits.SSPEN = 1; // Enable serial port

	/* Enable interrupts */
//	PIE1bits.SSPIE = 1;
//	IPR1bits.SSPIP = 1;
//	INTCONbits.GIE = 1;
//	INTCONbits.PEIE = 1;

}

uint8_t spi_wait_for_interrupt(void)
{
	while (PIR1bits.SSPIF == 0) {
		if (SSPCON1bits.WCOL) {
			return 1;
		}
	}
	return 0;
}

uint8_t spi_rw(uint8_t val)
{
	SSPCON1bits.WCOL = 0; // Write collision detect bit
	PIR1bits.SSPIF = 0; // SPI interrupt flag
	SSPBUF=val; // Write operation (shift address to left and clear R/W bit)
	if (spi_wait_for_interrupt() == 1) return 1;
	return SSPBUF; // Clears Buffer Full flag
}

void preampcontroller_init(void)
{
	uint8_t enter_hw_setup = 0;
	signed char result;

	// Disable unused peripherals
	ADCON0 = 0x00;
	ADCON1 = 0b00001111;
	T1CON = 0x00; // Disable timer1, so pins RC0/RC1 are available
	T3CON = 0x00; // Disable timer3, so pins RC0/RC1 are available

	// Direction of digital I/O
	POWER_DIRECTION = OUTPUT_BIT;
	BUTTON_POWER_DIRECTION = INPUT_BIT;
	BUTTON_MENU_DIRECTION = INPUT_BIT;
	BUTTON_LEFT_DIRECTION = INPUT_BIT;
	BUTTON_RIGHT_DIRECTION = INPUT_BIT;
	BUTTON_SELECT_DIRECTION = INPUT_BIT;

	ENC_A_DIRECTION = INPUT_BIT;
	ENC_B_DIRECTION = INPUT_BIT;
	IR_DIRECTION = INPUT_BIT;

	INPUT_CS_DIRECTION = OUTPUT_BIT;
	SPARE_CS_DIRECTION = OUTPUT_BIT;
	DAC_CS_DIRECTION = OUTPUT_BIT;
	VOLUME_CS_DIRECTION = OUTPUT_BIT;

	VOLUME_UNMUTE_DIRECTION = OUTPUT_BIT;
	SIGNAL_DETECTED_DIRECTION = INPUT_BIT;
	HEADPHONES_DIRECTION = INPUT_BIT;
	HAFLER_RELAY_DIRECTION = OUTPUT_BIT;

	RX_DIRECTION = INPUT_BIT;
	TX_DIRECTION = INPUT_BIT;

	// Default outputs
	INPUT_CS = HIGH;
	SPARE_CS = HIGH;
	DAC_CS = LOW;
	VOLUME_CS = HIGH;
	HAFLER_RELAY = LOW;
	VOLUME_UNMUTE = LOW;
	POWER = LOW;

	// Display
	LCD_DATA_DIRECTION = OUTPUT_BYTE;
	LCD_RS_DIRECTION = OUTPUT_BIT;
	LCD_RW_DIRECTION = OUTPUT_BIT;
	LCD_E_DIRECTION = OUTPUT_BIT;

	spi_init();

	ir_init();

	isr_init();

	preamp_init();					// State structure initialisation

	result = load_settings();
	lcd_init();
	LCD_row2_clear();

	if (result < 0) {
		enter_hw_setup = 1;
		LCD_display("Checksum failure", 0x40, 0, ERROR);
		delay_s(5);
//		while(1) { if (button_select()) break; }	// Confirm with select button
	} else {
		LCD_display("Settings loaded!", 0x40, 0, INFO);
		delay_s(1);
	}

	if (enter_hw_setup) {
		POWER = 1;	// Also switches backlight on
		menu_hw_setup();
		POWER = 0;
	}

	LCD_clear();
	LCD_off();

}

void lcd_init(void)
{
	uint8_t lcd_delay, lcd_width;
	char string[21];

	lcd_delay = (uint8_t)get_parameter(LCD_DELAY);
	sprintf(string, "lcd-delay=%d", lcd_delay);
	info(string);

	lcd_width = (uint8_t)get_parameter(LCD_WIDTH);
	sprintf(string, "lcd-width=%d", lcd_width);
	info(string);

	LCD_init_8bit(lcd_delay);		// Init display, use busy flag
	LCD_set_width(lcd_width);

	LCD_command(LCD_CGRAM);		// Custom character 0 (signal level 1)
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b11111);
	LCD_putc(0b00000);

	LCD_command(LCD_CGRAM+0x08);	// Custom character 1 (signal level 2)
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b00000);

	LCD_command(LCD_CGRAM+0x10);	// Custom character 2 (signal level 3)
	LCD_putc(0b00000);
	LCD_putc(0b00000);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b00000);

	LCD_command(LCD_CGRAM+0x18);	// Custom character 3 (signal level 4)
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b11111);
	LCD_putc(0b00000);

	LCD_command(LCD_CGRAM+0x20);	// Custom character 4 (left headphone)
	LCD_putc(0b00111);
	LCD_putc(0b01000);
	LCD_putc(0b10000);
	LCD_putc(0b10110);
	LCD_putc(0b01110);
	LCD_putc(0b01110);
	LCD_putc(0b00110);
	LCD_putc(0b00000);

	LCD_command(LCD_CGRAM+0x28);	// Custom character 5 (right headphone)
	LCD_putc(0b11100);
	LCD_putc(0b00010);
	LCD_putc(0b00001);
	LCD_putc(0b01101);
	LCD_putc(0b01110);
	LCD_putc(0b01110);
	LCD_putc(0b01100);
	LCD_putc(0b00000);

	LCD_on();
	LCD_display(preamp_name(), 0x00, 1, INFO);
	sprintf(string, "%s %s", preamp_version(), preamp_date());
	LCD_display(string, 0x40, 0, INFO);
	delay_s(1);

}

void power_on(void)
{
	POWER = 1;

	LCD_on();

	if (button_menu()) menu_hw_setup();

	LCD_display(preamp_name(), 0x00, 1, NONE);
	LCD_display("power on...", 0x40, 0, INFO);

	delay_s(1);

	reset_silent_counter();

	preamp_on();
	LCD_clear();

	display_input(1);
	display_volume(1);

}

void power_off(void)
{
	LCD_display(preamp_name(), 0x00, 1, NONE);
	LCD_display("power off...", 0x40, 0, INFO);
	preamp_off();

	if (!settings_saved()) {
		LCD_display("saving settings...", 0x40, 0, INFO);
		save_settings();
	}
	POWER = 0;
	delay_s(1);
	LCD_off();

	// Reset overrun error
	RCSTAbits.CREN  = 0;        // Disable continuous receive
	RCSTAbits.CREN  = 1;        // Enable continuous receive

}

char button_power(void) {
	if (!BUTTON_POWER) return 1;
	else return 0;
}

char button_power_pressed(void) {
	static char prev_button = 0;
	char button = button_power();
	if (button > prev_button) {		// Low->High transition
		prev_button = button;
		return 1;
	} else {
		prev_button = button;
		return 0;
	}
}

char button_menu(void) {
	if (!BUTTON_MENU) return 1;
	else return 0;
}

char button_menu_pressed() {
	static char prev_button = 0;
	char button = button_menu();
	if (button > prev_button) {		// Low->High transition
		prev_button = button;
		return 1;
	} else {
		prev_button = button;
		return 0;
	}
}

char button_left(void) {
	if (!BUTTON_LEFT) return 1;
	else return 0;
}

char button_left_pressed(void) {
	static char prev_button = 0;
	char button = button_left();
	if (button > prev_button) {		// Low->High transition
		prev_button = button;
		return 1;
	} else {
		prev_button = button;
		return 0;
	}
}

char button_right(void) {
	if (!BUTTON_RIGHT) return 1;
	else return 0;
}

char button_right_pressed(void) {
	static char prev_button = 0;
	char button = button_right();
	if (button > prev_button) {		// Low->High transition
		prev_button = button;
		return 1;
	} else {
		prev_button = button;
		return 0;
	}
}

char button_select(void) {
	if (!BUTTON_SELECT) return 1;
	else return 0;
}

char button_select_pressed(void) {
	static char prev_button = 0;
	char button = button_select();
	if (button > prev_button) {		// Low->High transition
		prev_button = button;
		return 1;
	} else {
		prev_button = button;
		return 0;
	}
}

void EventHeadphonesHandler(void)
{
	debug(1, "Headphones");

	if (is_powered()) {
		reset_silent_counter();
		update_volume();
		display_volume(1);
	}
}

void EventPowerHandler(void)
{
	debug(1, "Power");

	if (!is_powered()) power_on(); 	// Turn on preamp
	else power_off();
}

void EventMenuHandler(void)
{
	debug(1, "Menu");

	if (is_powered()) {
		reset_silent_counter();
		if (in_menu()) menu_exit();
		else menu_main();
	}
}

void EventLeftHandler(void)
{
	debug(1, "Left");

	if (is_powered()) {
		reset_silent_counter();
		if (in_menu()) menu_left();
		else previous_input();
	}
}

void EventRightHandler(void)
{
	debug(1, "Right");

	if (is_powered()) {
		reset_silent_counter();
		if (in_menu()) menu_right();
		else next_input();
	}
}

void EventSelectHandler(void)
{
	debug(1, "Select");

	if (is_powered()) {
		reset_silent_counter();
		if (in_menu()) menu_select();
		else next_output_mode();
	}
}

void EventEncoderHandler(void)
{
	signed char value = get_encoder_diff();

	debug(1, "Encoder");

	if (is_powered() && value) {
		reset_silent_counter();
		if (in_menu()) menu_change(value);
		else add_volume(value);
	}
}

void EventInfraredHandler(void)
{
	int8_t received;
	uint8_t address, command, same_codes;
	char string[30];
	uint8_t fast_volume_change_codes = 5;	// Change volume faster after receiving 5 same ir codes

	debug(1, "Infrared");

	ir_decode();
	address = ir_address();
	command = ir_command();
	same_codes = ir_same_codes();
	received = ir_receive();

	sprintf(string, "IR=%s A:%d C:%d S:%d", ir_type_string(), address, command, same_codes);
	debug(2, string);

   if (received>=0) {
	   sprintf(string, "IR cmd accepted: %d", received);
	   debug(2, string);
   }

   if (same_codes == 0 && received == IR_POWER) TriggerEvent(EVENT_POWER);

   if (!is_powered()) return;

	if (same_codes == 0) {	// Only trigger an event the first time a new code is pressed
		if (received == IR_UP || received == IR_RIGHT) TriggerEvent(EVENT_RIGHT);
		else if (received == IR_DOWN || received == IR_LEFT) TriggerEvent(EVENT_LEFT);
		else if (received == IR_MENU || received == IR_MENU2) TriggerEvent(EVENT_MENU);
		else if (received == IR_SELECT) TriggerEvent(EVENT_SELECT);
		else if (received == IR_VOLUME_UP) add_volume(1);
		else if (received == IR_VOLUME_DOWN) add_volume(-1);
      else if (received == IR_AV) next_input();
      else if (received >= IR_1 && received <= IR_9) set_input((uint8_t)(received-IR_1));
		if (received == IR_MUTE) {
			if (is_muted()) set_muted(0);
			else set_muted(1);
		}
	}
	if (same_codes > fast_volume_change_codes) {
		if (received == IR_VOLUME_UP) add_volume(2);
		else if (received == IR_VOLUME_DOWN) add_volume(-2);
	}
}

void EventAutoPoweroffHandler(void)
{
	const char name[] = "Auto power-off";

	debug(1, name);

	if (is_powered()) {
		LCD_display(name, 0x00, 1, INFO);
		delay_s(1);
		power_off();
	}
}

void EventDACHandler(void)
{
	debug(1, "DAC");

	if (is_powered()) {
		update_volume();
		display_input(0);
	}
}

void EventSignalLevelHandler(void)
{
	char string[25];
	int8_t level, offset;

	debug(1, "Signal level");
    if (!has_signal_level()) return;

	if (is_powered()) {
		level = (int8_t)(10*get_signal_level());
		offset = (int8_t)(10*get_signal_offset());
		sprintf(string, "S=%d O=%d", level, offset);
		debug(3, string);
		display_input(0);
	}
}

void EventUARTHandler(void)
{
	debug(1, "UART");

	// TODO: handle event
	term_handle_command();
}

void EventDisplayTitleHandler(void)
{
	debug(1, "Display Title event");

	display_title();
}

void main(void) {

	preampcontroller_init();

	ir_receive();	// Clears the infrared buffer

	RegisterEvent(EVENT_HP, EventHeadphonesHandler);
	RegisterEvent(EVENT_POWER, EventPowerHandler);
	RegisterEvent(EVENT_MENU, EventMenuHandler);
	RegisterEvent(EVENT_LEFT, EventLeftHandler);
	RegisterEvent(EVENT_RIGHT, EventRightHandler);
	RegisterEvent(EVENT_SELECT, EventSelectHandler);
	RegisterEvent(EVENT_ENCODER, EventEncoderHandler);
	RegisterEvent(EVENT_INFRARED, EventInfraredHandler);
	RegisterEvent(EVENT_AUTO_POWEROFF, EventAutoPoweroffHandler);
	RegisterEvent(EVENT_DAC, EventDACHandler);
	RegisterEvent(EVENT_SIGNAL_LEVEL, EventSignalLevelHandler);
	RegisterEvent(EVENT_UART, EventUARTHandler);
	RegisterEvent(EVENT_DISPLAY_TITLE, EventDisplayTitleHandler);

	EventManager();		// Blocking call that handles the events

}
