#ifndef LCD_H
#define LCD_H

#include "PreampController.h"

#define LCD_4BIT            0x28
#define LCD_8BIT            0x39	// Western font table
#define LCD_OFF             0x08
#define LCD_ON              0x0C
#define LCD_CLEAR           0x01
#define LCD_ORIGIN          0x02
#define LCD_ENTRYMODE_LEFT	0x04
#define LCD_ENTRYMODE_RIGHT	0x06
#define LCD_CURSOR_ON		0x0F
#define LCD_MOVE_LEFT		0x18
#define LCD_MOVE_RIGHT		0x1C
#define LCD_ROW1            0x80	// Set DDRAM address to 0x00 (start of row1)
#define LCD_ROW2            0xC0	// Set DDRAM address to 0x40 (start of row2)
#define LCD_CGRAM           0x40	// Set CGRAM base address (for custom characters)

#define LCD_clear() 		LCD_command(LCD_CLEAR)			// Clear display LCD
#define LCD_origin() 		LCD_command(LCD_ORIGIN)			// Move cursor to origin
#define LCD_cursor_left() 	LCD_command(LCD_ENTRYMODE_LEFT)	// Move cursor to the left
#define LCD_cursor_right() 	LCD_command(LCD_ENTRYMODE_RIGHT)// Move cursor to the right
#define LCD_move_left() 	LCD_command(LCD_MOVE_LEFT)		// Move content to the left
#define LCD_move_right() 	LCD_command(LCD_MOVE_RIGHT)		// Move content to the right
#define LCD_row1()          LCD_command(LCD_ROW1)			// Begin at Line 1
#define LCD_row2()          LCD_command(LCD_ROW2)   		// Begin at Line 2
#define LCD_on()            LCD_command(LCD_ON)
#define LCD_off()           LCD_command(LCD_OFF)

/***************************************************
 * Prototype(s)                                    *
 ***************************************************/
void LCD_delay(void);
void LCD_enable(void);
void LCD_command(unsigned char command);
void LCD_putc(char ascii);
unsigned char LCD_puts(const char *lcd_string);
void LCD_init_8bit(unsigned char delay_100us);
void LCD_row1_clear(void);
void LCD_row1_pos(unsigned char pos);
void LCD_row2_clear(void);
void LCD_row2_pos(unsigned char pos);
void LCD_brightness(char brightness);
void LCD_enable(void);
void LCD_disable(void);
unsigned char LCD_width(void);
void LCD_set_width(unsigned char width);
void LCD_display(const char *string, unsigned char pos, unsigned char clear, unsigned char level);

#endif

