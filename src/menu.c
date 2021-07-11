#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "PreampController.h"		// Pin settings
#include "Preamp.h"			// Global functions and variables
#include "menu.h"
#include "DAC.h"
#include "VolumeControl.h"
#include "lcd.h"
#include "time_delay.h"
#include "infrared.h"
#include "terminal.h"

#define INACTIVE_SECONDS_LIMIT		20 // Return after 20s of inactivity
#define MAX_TITLE                 35

// Current menu selection
static const menu_t *current_menu = NULL;
static unsigned char menu_idx = 0;
static char _title[MAX_TITLE] = "";
static unsigned char _title_input = 0;
static unsigned char _show_title = 0;
static unsigned char _title_idx = 0;

const menu_t input_names_menu =
    {
        "Input names",
        &nr_inputs,
        &input_names_line1,
        &input_names_line2,
        &change_input_name,
        &toggle_enabled,
        0
    };

const menu_t input_offsets_menu =
    {
        "Input offsets",
        &nr_inputs,
        &input_offsets_line1,
        &input_offsets_line2,
        &add_input_offset,
        NULL,
        0
    };

const menu_t channel_offsets_menu =
    {
        "Channel offsets",
        &nr_channels,
        &channel_offsets_line1,
        &channel_offsets_line2,
        &add_channel_offset,
        NULL,
        0
    };

const menu_t info_menu =
    {
        "Information",
        &nr_info_items,
        &info_line1,
        &info_line2,
        NULL,
        NULL,
        0
    };

const menu_t parameter_menu =
    {
        "Parameters",
        &nr_parameters,
        &parameters_line1,
        &parameters_line2,
        &change_parameter,
        NULL,
        0
    };

const menu_t main_menu =
    {
        "Main Menu",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        5,
        {&input_names_menu, &input_offsets_menu, &channel_offsets_menu, &info_menu, &parameter_menu}
    };

char in_menu()
{
	if (current_menu == NULL) return 0;
	else return 1;
}

void menu_left()
{
	unsigned char nr_submenus = current_menu->nr_submenus;
	unsigned char nr_items = 0;

	if (nr_submenus) {	// Menu has submenus
		if (menu_idx > 0) menu_idx--;
		else menu_idx = nr_submenus-1;
	} else if (current_menu->nr_items != NULL)
		nr_items = current_menu->nr_items();
		if (nr_items) {	// Menu has sub items
			if (menu_idx > 0) menu_idx--;
			else menu_idx = nr_items-1;
	}

	display_menu();
}

void menu_right()
{
	unsigned char nr_submenus = current_menu->nr_submenus;
	unsigned char nr_items = 0;

	if (nr_submenus) {	// Menu has submenus
		if (menu_idx < current_menu->nr_submenus-1) menu_idx++;
		else menu_idx = 0;
	} else if (current_menu->nr_items != NULL)
		nr_items = current_menu->nr_items();
		if (nr_items) {	// Menu has sub items
			if (menu_idx < nr_items-1) menu_idx++;
			else menu_idx = 0;
	}

	display_menu();
}

void menu_main()
{
	current_menu = &main_menu;
	menu_idx = 0;

	display_menu();
}

void menu_exit()
{
	current_menu = NULL;

	display_input(1);
	display_volume(1);
}

void menu_select()
{
	const menu_t *submenu = current_menu->nr_submenus ? current_menu->submenu[menu_idx] : NULL;
	unsigned char nr_items;

	if (submenu != NULL) {  // Go to submenu
		current_menu = submenu;
		menu_idx = 0;
	} else if (current_menu->nr_items != NULL && current_menu->item_toggle != NULL) {
		nr_items = current_menu->nr_items();
		if (nr_items) current_menu->item_toggle(menu_idx);
	}


	display_menu();
}

void menu_change(signed char value)
{
	unsigned char nr_items;

	if (current_menu->nr_items != NULL && current_menu->item_change != NULL) {
		nr_items = current_menu->nr_items();
		if (nr_items) current_menu->item_change(menu_idx, value);
	}
	display_menu();
}

void display_menu()
{
	const menu_t *submenu;
	unsigned char nr_items;

	if (current_menu == NULL) return;	// No menu selected
	if (current_menu->nr_submenus) submenu = current_menu->submenu[menu_idx];
	else submenu = NULL;

	LCD_clear();

	if (submenu != NULL) {	// Menu with submenus
		LCD_row1();
		LCD_puts(current_menu->title);
		LCD_row2();
		LCD_putc('<'); // sign <
		LCD_putc(' ');
		LCD_puts(submenu->title);
		LCD_row2_pos(LCD_width()-1);
		LCD_putc('>'); // sign >
		return;
	}

	if (current_menu->nr_items != NULL &&
	    current_menu->item_line1 != NULL &&
	    current_menu->item_line2 != NULL) { // Menu with items
		nr_items = current_menu->nr_items();
		LCD_row1();
		if (nr_items==0) {
		    LCD_puts(current_menu->title);
		    LCD_row2();
            LCD_puts("no items");
            return;
        }
		LCD_puts(current_menu->item_line1(menu_idx));
		LCD_row2();
		LCD_puts(current_menu->item_line2(menu_idx));
	}
}

static const char *input_offsets_line1(unsigned char item)
{
	const char string[21];
	sprintf(string, "In: < %-12s >", input_name_string(item));
	return string;
}

static const char *input_offsets_line2(unsigned char item)
{
	const char string[21];
	if (get_input_offset(item) < 0)
		sprintf(string, "Offset: -%d.%ddB", (-get_input_offset(item)/2), 5*((-get_input_offset(item))%2));
	else
		sprintf(string, "Offset: +%d.%ddB", (get_input_offset(item)/2), 5*(get_input_offset(item)%2));
	return string;
}

static const char *input_names_line1(unsigned char item)
{
	const char string[21];
	sprintf(string, "In: < %-9s >", input_type_string(item));
	if (is_enabled(item)) strcat(string, " on");
	else strcat(string, "off");
	return string;
}

static const char *input_names_line2(unsigned char item)
{
	const char string[21];
	sprintf(string, "Name: %s", input_name_string(item));
	return string;
}

static const char *channel_offsets_line1(unsigned char item)
{
	const char string[21];
	sprintf(string, "Ch: < %-12s >", channel_name_string(item));
	return string;
}

static const char *channel_offsets_line2(unsigned char item)
{
	const char string[21];
	if (get_channel_offset(item) < 0)
		sprintf(string, "Offset: -%d.%ddB", (-get_channel_offset(item)/2), 5*((-get_channel_offset(item))%2));
	else
		sprintf(string, "Offset: +%d.%ddB", (get_channel_offset(item)/2), 5*(get_channel_offset(item)%2));
	return string;
}

static const char *parameters_line1(unsigned char item)
{
	const char string[21];
	sprintf(string, "< %-16s >", get_parameter_name(item));
	return string;
}

static const char *parameters_line2(unsigned char item)
{
	const char string[21];
	sprintf(string, "Value: %d [%s]", get_parameter(item), get_parameter_unit(item));
	return string;
}

static unsigned char nr_info_items() { return 6; }

static const char *info_line1(unsigned char item)
{
	switch(item)
	{
        case 0: return "Channels"; break;
        case 1: return "Inputs"; break;
        case 2: return "Standby time"; break;
        case 3: return "On time"; break;
        case 4: return "Silent time"; break;
	case 5: return "UART received"; break;
        default: return "?"; break;
    }
}

static const char *info_line2(unsigned char item)
{
	const char string[21];

	switch(item)
	{
		case 0: sprintf(string, "[%d/%d]", get_nr_output_channels(), nr_channels()); break;
		case 1: sprintf(string, "[%d/%d]", get_input()+1, nr_inputs()); break;
		case 2: sprintf(string, "%d days %d:%d:%d", get_standby_days(), get_standby_hours(), get_standby_minutes(), get_standby_seconds()); break;

		case 3: sprintf(string, "%d days %d:%d:%d", get_on_days(), get_on_hours(), get_on_minutes(), get_on_seconds()); break;
		case 4: sprintf(string, "%d:%d", get_silent_minutes(), get_silent_seconds()); break;
		case 5: sprintf(string, "%d bytes", term_bytes_received()); break;
    }
	return string;
}

static unsigned char nr_setup_items() { return 3; }

static const char *setup_line1(unsigned char item)
{
	switch(item)
	{
        case 0: return "Load settings"; break;
        case 1: return "Save settings"; break;
        case 2: return "Clear settings"; break;
        default: return "?"; break;
    }
}

static const char *setup_line2(unsigned char item)
{
	switch(item)
	{
        	default: return "?"; break;
	}
}

/*

static char menu_ir()
{
	char string[21];
	unsigned char menu_item = 0;
	unsigned char length, transitions;
	static unsigned short irdata;

//	char ircode;
	LCD_clear();
	LCD_row1();
	LCD_puts("Infrared");
	while(1)
	{
		if (button_menu()) return -1;
		if (button_power()) { power_off(); return -1; }
		if (ir_raw_ready()) {
			length = ir_raw_length();
			transitions = ir_raw_transitions();
			sprintf(string, "L:%d T:%d", length, transitions);
			LCD_row1_pos(9); LCD_puts(string);
		}
		if (ir_code_ready()) {
			if (irdata != ir_data()) {
				irdata = ir_data();
				sprintf(string, "%s: T:%d A:%d C:%d", ir_type_string(), ir_toggle_bit(), ir_address(), ir_command());
				LCD_row2_clear(); LCD_row2(); LCD_puts(string);
			}
		}
	}
}

static char menu_settings()
{
	char ircode;
	unsigned char menu_item = 0, max_item;

	LCD_clear();
	LCD_row1();
	LCD_puts("Settings");
	max_item = 1;	// load/save
	display_menu_settings(menu_item);					// Display the newly selected item
	while(1)
	{
//		enc_update();
		ircode = ir_receive();
		if (ircode == IR_LAST && ir_toggled()) return 0;
		if (button_menu() || (ircode == IR_MENU && ir_toggled())) return -1;
		if (button_power() || (ircode == IR_POWER && ir_toggled())) { power_off(); return -1; }
		if (button_left() || (ircode == IR_DOWN && ir_toggled()))
			{ if (menu_item > 0) menu_item--; else menu_item = max_item; display_menu_settings(menu_item); }
		if (button_right() || (ircode == IR_UP && ir_toggled()))
			{ if (menu_item < max_item) menu_item++; else menu_item = 0; display_menu_settings(menu_item); }
		if (button_select() || (ircode == IR_SELECT && ir_toggled())) {
			if (menu_item==0) { load_settings(); display_menu_settings(menu_item); }
			else if (menu_item==1) { save_settings(); display_menu_settings(menu_item); }
		}
	}
}

static void display_menu_settings(unsigned char item)
{
	char string[21];
	LCD_row2_clear();
	LCD_row2();

	if (item==0) sprintf(string, "Load settings?");
	else if (item==1) sprintf(string, "Save settings?");

	LCD_putc('<'); // sign <
	LCD_putc(' ');
	LCD_puts(string);
	LCD_row2_pos(LCD_width()-1);
	LCD_putc('>'); // sign >
}

*/

char menu_hw_setup()
{
	unsigned char menu_item = 0, max_item;
	signed char value_change;
	static unsigned char prev_encoder_counter;

	LCD_display("Enter HW setup..", 0x00, 1, INFO);
	delay_s(1);

	LCD_clear();
	LCD_row1();
	LCD_puts("Hardware setup");
	max_item = get_nr_hw_setups()-1;		// Number of HW setups
	display_menu_hw_setup(menu_item);	// Display the newly selected item

	prev_encoder_counter = get_encoder_counter();
	while(1)
	{
		value_change = get_encoder_counter() - prev_encoder_counter;
		if (value_change / 2) {
      prev_encoder_counter = get_encoder_counter();
			toggle_hw_setup(menu_item);
			display_menu_hw_setup(menu_item);
		}
		if (button_menu_pressed()) return -1;
		if (button_power_pressed()) return -1;
		if (button_left_pressed()) { if (menu_item > 0) menu_item--; else menu_item = max_item; display_menu_hw_setup(menu_item); }
		if (button_right_pressed()) { if (menu_item < max_item) menu_item++; else menu_item = 0; display_menu_hw_setup(menu_item); }
		if (button_select_pressed()) {
			if (menu_item < max_item) { menu_item++; display_menu_hw_setup(menu_item); }
			else { default_settings(); return -1; }

		}
	}
}

static void display_menu_hw_setup(unsigned char item)
{
    const char string[5];

	// Show the hw setup question on the start of the second row
	LCD_row2_clear();
	LCD_row2();
	LCD_puts(hw_setup_name(item));

	// Show the value on the end of the second row
	sprintf(string, "%s", hw_setup_value(item));
	LCD_row2_pos(LCD_width()-strlen(string));
	LCD_puts(string);

}

void display_volume(unsigned char full_update)		// Display volume
{
	// Display input and output channels on first 7 characters
	// Display volume on last 7 characters
	static short volume;

	static unsigned char input_mode;
	static unsigned char output_mode;
	static char muted;

   if (in_menu()) return;

	if (full_update) LCD_row2_clear();

	if (!has_volume_control()) return;	// Do not display if volume can not be controlled

	if (full_update || headphones_connected_changed()
			|| input_mode != get_current_input_mode()
			|| output_mode != get_current_output_mode()) {
		input_mode = get_current_input_mode();
		output_mode = get_current_output_mode();
		LCD_row2();
		LCD_puts(input_channel_string());
//		LCD_row2_pos(3);
		LCD_putc(0b01111110);	// Right arrow
		if (has_headphones() && headphones_connected()) { LCD_putc(4); LCD_putc(5); LCD_putc(' '); }
		else LCD_puts(output_channel_string());
	}

	if (full_update ||  volume != get_volume()
			|| muted != is_muted()) {
		muted = is_muted();
		volume = get_volume();
		LCD_row2_pos(LCD_width()-7);
		LCD_puts(volume_string());
	}
}

void display_input(unsigned char full_update)		// Display input
{
	// Display input name on first 13 characters
	// Display input level (for analog) or bit/sample rate (for digital) on last 7 characters
	char string[21];
	unsigned char cnt;
	float level;
	static unsigned char dac_error;

   if (in_menu()) return;

	if (full_update) _show_title = 0;

	if (input_changed() || full_update) {
		LCD_row1();
		cnt = LCD_puts(input_name_string(get_input()));
		while(cnt<14) {	// Add remaining spaces
			LCD_putc(' ');
			cnt++;
		}
	}

	if (is_dac_input())
	{
		if (dac_error != dac_receiver_error() || input_changed() || full_update ) {
			LCD_row1_pos(LCD_width()-7);
			LCD_puts("      ");
			if (dac_receiver_error()) {
//				volumecontrol_mute();
//				dac_mute();
				strcpy(string, dac_error_string());
				LCD_row1_pos(LCD_width()-strlen(string));
				LCD_puts(string);
			} else {
//				volumecontrol_unmute();
//				dac_unmute();
				strcpy(string, dac_rate_string());
				LCD_row1_pos(LCD_width()-strlen(string));
				LCD_puts(string);
			}
			dac_error = dac_receiver_error();
		}
	} else if (is_analog_input())
	{
		if ((has_signal_level() && signal_level_changed()) || input_changed() || full_update) {
			// Show visualisation of signal level
			level = get_signal_level();
			LCD_row1_pos(LCD_width()-7);
			LCD_puts("   ");
			if (level > 0.25) LCD_putc(0);
			else LCD_putc(' ');
			if (level > 0.5) LCD_putc(1);
			else LCD_putc(' ');
			if (level > 0.75) LCD_putc(2);
			else LCD_putc(' ');
			if (level > 1.0) LCD_putc(3);
			else LCD_putc(' ');
		}
	}
}

void set_title(unsigned char input, char *title)		// Display title
{
	_title_input = input;
	strncpy(_title, title, MAX_TITLE-1);

	_show_title = 1;
	_title_idx = 0;

	display_title();
}

unsigned char show_title() { return _show_title; }

void display_title()
{
	unsigned char i, idx, len, scroll=0;
	char *str;

	if (!_show_title || !is_powered()) return;
	if (_title_input != get_input()) return;
  if (in_menu()) return;
  
	// Scroll text if title is longer than 13 characters
	len = strlen(_title);
	if (len > 13) {
		scroll = 1;
	}

	// Display title on first 13 characters
	LCD_row1();
	for (i=0;i<13;i++) {
		if (scroll) {
			idx = _title_idx + i;
			if (idx > len+4) idx -= len+5;
			if (idx < len) {
				str = _title + idx;
				LCD_putc(*str);
			} else LCD_putc(' ');
		} else {
			if (i < len) {
				str = _title + i;
				LCD_putc(*str);
			} else LCD_putc(' ');
		}
	}

	if (scroll) {
		_title_idx++;
		if (_title_idx > len+4) _title_idx = 0;
	}
}
