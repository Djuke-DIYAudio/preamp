#ifndef PREAMPCONTROLLER_H
#define PREAMPCONTROLLER_H

#include <pic18fregs.h>

// Events
enum EVENT {
	EVENT_POWER = 1,
	EVENT_MENU,
	EVENT_LEFT,
	EVENT_RIGHT,
	EVENT_SELECT,
	EVENT_HP,
	EVENT_ENCODER,
	EVENT_INFRARED,
	EVENT_AUTO_POWEROFF,
	EVENT_SIGNAL_LEVEL,
	EVENT_DAC,
	EVENT_UART,
	EVENT_DISPLAY_TITLE
};

// Function definitions

// SPI
void spi_init();
unsigned char spi_rw(unsigned char val);
void spi_write(unsigned char address, unsigned char reg, unsigned char val);
unsigned char spi_read(unsigned char address, unsigned char reg);
void spi_set_cs(unsigned char pin);
unsigned char spi_wait_for_interrupt();

// hardware
unsigned char encoder_counter_changed();
unsigned char get_encoder_counter();
signed char get_encoder_diff();
char button_power();
char button_power_pressed();
char button_menu();
char button_menu_pressed();
char button_left();
char button_left_pressed();
char button_right();
char button_right_pressed();
char button_select();
char button_select_pressed();

void lcd_init();

// interrupt routines and timers
void isr_init();
void timer0_init();
void timer1_init();
void lvd_init();
void uart_init();
unsigned char get_timer0_ticks();
unsigned char get_timer1_seconds();
unsigned char get_timer1_minutes();
float get_signal_offset();
float get_signal_level();
unsigned char signal_level_changed();
unsigned char get_silent_minutes();
unsigned char get_silent_seconds();
void reset_silent_counter();

// Functions for getting uptime
unsigned char get_standby_seconds();
unsigned char get_standby_minutes();
unsigned char get_standby_hours();
unsigned char get_standby_days();

// Functions for getting ontime
unsigned char get_on_seconds();
unsigned char get_on_minutes();
unsigned char get_on_hours();
unsigned char get_on_days();

unsigned char headphones_connected();
unsigned char headphones_connected_changed();

// high level functions
void power_off();
void power_on();

//Set clock frequency
#define CLOCK_FREQ		10000000
#define OUTPUT_BIT		0
#define OUTPUT_BYTE		0x00
#define INPUT_BIT		1
#define INPUT_BYTE		0xff
#define HIGH			1
#define LOW			0

// Buttons + encoder
#define ENC_A			PORTBbits.RB1
#define ENC_A_DIRECTION		TRISBbits.TRISB1
#define ENC_B			PORTBbits.RB2
#define ENC_B_DIRECTION		TRISBbits.TRISB2
#define BUTTON_MENU		PORTBbits.RB4
#define BUTTON_MENU_DIRECTION	TRISBbits.TRISB4
#define BUTTON_LEFT		PORTBbits.RB5
#define BUTTON_LEFT_DIRECTION	TRISBbits.TRISB5
#define BUTTON_RIGHT		PORTBbits.RB6
#define BUTTON_RIGHT_DIRECTION	TRISBbits.TRISB6
#define BUTTON_SELECT		PORTBbits.RB7
#define BUTTON_SELECT_DIRECTION	TRISBbits.TRISB7

// Power control
#define POWER			PORTAbits.RA0
#define POWER_DIRECTION		TRISAbits.TRISA0
#define BUTTON_POWER		PORTBbits.RB3
#define BUTTON_POWER_DIRECTION	TRISBbits.TRISB3

// UART
#define RX			PORTCbits.RC7
#define RX_DIRECTION		TRISCbits.TRISC7
#define TX			PORTCbits.RC6
#define TX_DIRECTION		TRISCbits.TRISC6

// Infrared
#define IR			PORTBbits.RB0
#define IR_DIRECTION		TRISBbits.TRISB0

// Display
#define LCD_RS			PORTEbits.RE0
#define LCD_RS_DIRECTION	TRISEbits.TRISE0
#define LCD_RW			PORTEbits.RE1
#define LCD_RW_DIRECTION	TRISEbits.TRISE1
#define LCD_E			PORTEbits.RE2
#define LCD_E_DIRECTION		TRISEbits.TRISE2
#define LCD_DATA		PORTD
#define LCD_DATA_DIRECTION	TRISD
#define LCD_BUSY		PORTDbits.RD7
#define LCD_BUSY_DIRECTION	TRISDbits.TRISD7

// Volume control
#define VOLUME_CS		PORTAbits.RA1
#define VOLUME_CS_DIRECTION	TRISAbits.TRISA1
#define VOLUME_UNMUTE		PORTAbits.RA2
#define VOLUME_UNMUTE_DIRECTION	TRISAbits.TRISA2
#define SIGNAL_DETECTED		PORTAbits.RA3
#define SIGNAL_DETECTED_DIRECTION	TRISAbits.TRISA3
#define HEADPHONES		PORTAbits.RA4
#define HEADPHONES_DIRECTION	TRISAbits.TRISA4
#define HAFLER_RELAY		PORTAbits.RA5
#define HAFLER_RELAY_DIRECTION	TRISAbits.TRISA5

// DAC
#define DAC_CS			PORTCbits.RC0
#define DAC_CS_DIRECTION	TRISCbits.TRISC0

// Input selection
#define INPUT_CS		PORTCbits.RC2
#define INPUT_CS_DIRECTION	TRISCbits.TRISC2

#define SPARE_CS		PORTCbits.RC1
#define SPARE_CS_DIRECTION	TRISCbits.TRISC1

#endif
