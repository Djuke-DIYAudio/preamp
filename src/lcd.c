#include "lcd.h"
#include "time_delay.h"
#include "terminal.h"
#include <stdio.h>

static unsigned char _mode_4bit = 0;
static unsigned char _delay_100us = 0;
static unsigned char _min_delay_cnt = 0;
static unsigned char _max_delay_cnt = 0;
static unsigned char _delay_cnt = 0;
static unsigned char _lcd_width = 20;

// specific delay with variable as a parameter
// see details in "time_delay_40MHz.c
void PA_delay_100us(unsigned int val)
{

    for( unsigned int i = val; i > 0; i--)
    {
        delay_100us(1);        
    }    
}

unsigned char LCD_width() { return _lcd_width; }
void LCD_set_width(unsigned char width) { _lcd_width = width; }

void LCD_delay(void) {
	unsigned char busy = 0;
	if (_delay_100us) {
        
		PA_delay_100us(_delay_100us);	
        
	} else {
		LCD_BUSY_DIRECTION = INPUT_BIT;
		LCD_RS = 0;	// command
		LCD_RW = 1;	// Read
		_delay_cnt=0;
		
		do {
			delay_us(8);
			LCD_E = 1;
			delay_us(1);
			busy = LCD_BUSY;
			LCD_E = 0;
			delay_us(1);
			_delay_cnt++;
		} while (_delay_cnt<200 && busy);

		LCD_BUSY_DIRECTION = OUTPUT_BIT;
		if (_delay_cnt > _max_delay_cnt) _max_delay_cnt = _delay_cnt;
		if (_min_delay_cnt == 0 || _delay_cnt < _min_delay_cnt) _min_delay_cnt = _delay_cnt;
	}
}

void LCD_row1_pos(unsigned char pos) {
	if (pos < 0x28) LCD_command(LCD_ROW1+pos);
}

/*void LCD_row1_clear(void) {
	unsigned char i;
	LCD_row1();
	for(i=0;i<_lcd_width;i++) LCD_putc(' ');
	LCD_row1();
}*/

void LCD_row2_pos(unsigned char pos) {
	if (pos < 0x28) LCD_command(LCD_ROW2+pos);
}

void LCD_row2_clear(void) {
	unsigned char i;
	LCD_row2();
	for(i=0;i<_lcd_width;i++) LCD_putc(' ');
	LCD_row2();
}

/*void LCD_brightness(char brightness) {
	LCD_command(LCD_4BIT + (brightness & 0x3));
}*/

void LCD_command(unsigned char command)
{
	char str[50];
	if (_mode_4bit) {
	} else {
		LCD_RW = 0;		// command
		LCD_RS = 0;		// write
		LCD_E = 1;
		LCD_DATA = command;
		LCD_E = 0;		// Clock data at high-low transition
		LCD_delay();
	}
	sprintf(str, "lcd cmd 0x%X,delay=%d [us]", command, 100*_delay_cnt);
	debug(3, str);
}

void LCD_putc(char ascii)
{
	if (_mode_4bit) {
	} else {
		LCD_RW = 0;
		LCD_RS = 1;
		LCD_E = 1;
		LCD_DATA = ascii;
		LCD_E = 0;		// Clock data at high-low transition
		LCD_delay();
	}
}

unsigned char LCD_puts(const char *lcd_string)
{
	unsigned char i=0;
	while(*lcd_string) {
		LCD_putc(*lcd_string++);
		i++;
	}
	return i;
}

void LCD_init_8bit(unsigned char delay_100us)
{
	_mode_4bit = 0;
	_delay_100us = delay_100us;

	LCD_command(LCD_8BIT);
	LCD_command(LCD_8BIT);
	LCD_command(LCD_ENTRYMODE_RIGHT);

}

/*void LCD_init_4bit()
{
	_mode_4bit = 0;		// Send following commands in 8-bit mode
	LCD_command(LCD_8BIT);	// If for some reason, the module started in 4-bit mode
				// it will accept this command, but also the next
	LCD_command(LCD_4BIT);
	_mode_4bit = 1;		// Next commands will be sent in 4-bit mode
	LCD_command(LCD_4BIT);
	LCD_command(LCD_ENTRYMODE_RIGHT);
//	LCD_command(LCD_CURSOR_ON);
}*/

void LCD_display(const char *string, unsigned char pos, unsigned char clear, unsigned char level)
{
	if (clear) LCD_clear();
	LCD_command(LCD_ROW1+pos);
	LCD_puts(string);
	switch(level) {
		case INFO:
			info(string);
			break;
		case WARNING:
			warning(string);
			break;
		case ERROR:
			error(string);
			break;
	}
}

