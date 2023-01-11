#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_MENU_TITLE       20
#define MAX_SUBMENUS    10

typedef struct menu_t
{
    char title[MAX_MENU_TITLE];
    uint8_t (*nr_items)(void);                // Pointer to function returning number of items   
    const char * (*item_line1)(uint8_t);      // Pointer to function returning line1 of item
    const char * (*item_line2)(uint8_t);      // Pointer to function returning line2 of item
    void (*item_change)(uint8_t, int8_t);// Pointer to function used to change item
    void (*item_toggle)(uint8_t);             // Pointer to function used to toggle item
    uint8_t nr_submenus;
    const struct menu_t *submenu[MAX_SUBMENUS];
} menu_t;

// Functions for displaying menu items
static const char *inputs_line1(uint8_t item);
static const char *input_offsets_line2(uint8_t item);
static const char *parameters_line1(uint8_t item);
static const char *parameters_line2(uint8_t item);


static void display_menu_select(uint8_t item);	// Display the menu select item
static void display_menu_input_offsets(uint8_t item);	// Display the input offsets menu item
static void display_menu_input_names(uint8_t item);	// Display the input names menu item
static void display_menu_volume(uint8_t item);	// Display the volume menu item
static void display_menu_channel(uint8_t item);	// Display the channel menu item
static void display_menu_info(uint8_t item);	// Display the information menu item
static void display_menu_settings(uint8_t item);	// Display the settings menu item
static void display_menu_parameters(uint8_t item);// Display the parameters menu item
static void display_menu_hw_setup(uint8_t item);	// Display the HW setup menu item

static char menu_input_offsets(void);		// Go to the input offset menu
static char menu_input_names(void);			// Go to the input names menu
static char menu_volume(void);			// Go to the volume menu
static char menu_channel(void);			// Go to the channel offset menu
static char menu_info(void);			// Go to the information menu
static char menu_ir(void);				// Go to the infrared menu
static char menu_settings(void);			// Go to the settings menu
static char menu_parameters(void);			// Go to the parameters menu

// Externally used functions
char in_menu(void);
void menu_left(void);
void menu_right(void);
void menu_select(void);
void menu_exit(void);
void menu_main(void);
void display_menu(void);
void menu_change(signed char value);
void menu_hw_setup(void);			// Go to the HW setup menu
void display_volume(bool full_update);	// Display the current volume
void display_input(bool full_update);	// Display the current input
void set_title(uint8_t input, char *title);
void display_title(void);
bool show_title(void);

#endif
