#include "Preamp_tests.h"

#include <stdio.h>
#include <string.h>

#include "time_delay.h"
#include "cs8416.h"
#include "cs4398.h"
#include "lcd.h"

#include "InputSelect.h"
#include "DAC.h"
#include "VolumeControl.h"

void inputselect_test()
{
	unsigned char inputs, outputs, i;
	char string[17];

	for (i=0;i<7;i++) {
		outputs = 1 << i;
		inputselect_set_input(outputs);	// Set outputs
		sprintf(string, "Set output: %x", outputs);
		LCD_row1(); LCD_puts(string);
		inputs = inputselect_get_input(); // Read output state
		sprintf(string, "Get GPIO: %x", inputs);
		LCD_row2(); LCD_puts(string);

		delay_s(1);
	}

}

void dac_test()
{
	unsigned char i;
	unsigned char cs4398_ok, cs8416_ok;
	char string[17], rate[4];

	i=0;
	cs8416_set_input(i);

	while (1) {
		cs4398_ok = cs4398_is_connected();
		cs8416_ok = cs8416_is_connected();
		sprintf(string, "8416: %d 4398: %d", cs8416_ok, cs4398_ok);
		LCD_row1(); LCD_puts(string);

		if (cs8416_ok) {
			sprintf(string, "%d: ", i);
			delay_s(1);

			if (!cs8416_pll_locked()) strcat(string, "no pll");
			else if (!cs8416_data_valid()) strcat(string, "invalid");
			else {
				if (cs8416_is_format_pcm()) {
					strcat(string, "pcm ");
					sprintf(rate, "%d-", cs8416_bit_rate());
					strcat(string, rate);
					sprintf(rate, "%dk", cs8416_sample_rate());
					strcat(string, rate);
				} else strcat(string, "n/a");
			}
			LCD_row2_clear(); LCD_puts(string);
		}
	}
}

void cs8416_test()
{
	unsigned char value, reg;
	char string[17];

	LCD_row1(); LCD_puts("CS8416 test");
	for (reg=0x00;reg<0x0D;reg++) {
		value = cs8416_read_register(reg); // Read register
		sprintf(string, "Reg: %.2x Val: %.2x  ", reg, value);
		LCD_row2(); LCD_puts(string);
		delay_s(1);
	}

}

void cs4398_test()
{
	unsigned char value, reg;
	char string[17];

	cs4398_write_register(CS4398_REG_MISC_CONTROL, 0b01000000);	// CPEN, no freeze
	delay_s(1);

	LCD_row1(); LCD_puts("CS4398 test");
	for (reg=0x01;reg<0x0A;reg++) {
		value = cs4398_read_register(reg); // Read register
		sprintf(string, "Reg: %x Val: %x  ", reg, value);
		LCD_row2(); LCD_puts(string);
		delay_s(1);
	}

}

void volume_test()
{
	LCD_row1(); LCD_puts("Volume test");

	LCD_row2();
	if (!HEADPHONES) LCD_puts("HP connected");
	else LCD_puts("No HP connected");
	delay_s(2);

	HAFLER_RELAY = HIGH;
	LCD_row2(); LCD_puts("Hafler->1");
	delay_s(2);
	HAFLER_RELAY = LOW;
	LCD_row2(); LCD_puts("Hafler->0");
	delay_s(2);
	
}

void button_test()
{
	LCD_row1(); LCD_puts("Button test");

	LCD_row2();
	while (1) {
		if (button_power()) { LCD_row2(); LCD_puts("POWER"); }
		else if (button_menu()) { LCD_row2(); LCD_puts("MENU"); }
		else if (button_left()) { LCD_row2(); LCD_puts("LEFT"); }
		else if (button_right()) { LCD_row2(); LCD_puts("RIGHT"); }
		else if (button_select()) { LCD_row2(); LCD_puts("SELECT"); }
		else { LCD_row2(); LCD_puts("none"); }
	}
}
