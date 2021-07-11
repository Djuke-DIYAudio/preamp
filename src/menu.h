#ifndef MENU_H
#define MENU_H

#define MAX_MENU_TITLE       20
#define MAX_SUBMENUS    10

typedef struct menu_t
{
    char title[MAX_MENU_TITLE];
    unsigned char (*nr_items)();		// Pointer to function returning number of items
    const char * (*item_line1)(unsigned char);	// Pointer to function returning line1 of item
    const char * (*item_line2)(unsigned char);	// Pointer to function returning line2 of item
    void (*item_change)(unsigned char, signed char);	// Pointer to function used to change item
    void (*item_toggle)(unsigned char);	        // Pointer to function used to toggle item
    unsigned char nr_submenus;
    const struct menu_t *submenu[MAX_SUBMENUS];
} menu_t;

// Functions for displaying menu items
static const char *inputs_line1(unsigned char item);
static const char *input_offsets_line2(unsigned char item);
static const char *parameters_line1(unsigned char item);
static const char *parameters_line2(unsigned char item);


static void display_menu_select(unsigned char item);	// Display the menu select item
static void display_menu_input_offsets(unsigned char item);	// Display the input offsets menu item
static void display_menu_input_names(unsigned char item);	// Display the input names menu item
static void display_menu_volume(unsigned char item);	// Display the volume menu item
static void display_menu_channel(unsigned char item);	// Display the channel menu item
static void display_menu_info(unsigned char item);	// Display the information menu item
static void display_menu_settings(unsigned char item);	// Display the settings menu item
static void display_menu_parameters(unsigned char item);// Display the parameters menu item
static void display_menu_hw_setup(unsigned char item);	// Display the HW setup menu item

static char menu_input_offsets();		// Go to the input offset menu
static char menu_input_names();			// Go to the input names menu
static char menu_volume();			// Go to the volume menu
static char menu_channel();			// Go to the channel offset menu
static char menu_info();			// Go to the information menu
static char menu_ir();				// Go to the infrared menu
static char menu_settings();			// Go to the settings menu
static char menu_parameters();			// Go to the parameters menu

// Externally used functions
char in_menu();
void menu_left();
void menu_right();
void menu_select();
void menu_exit();
void menu_main();
void menu_change(signed char value);
void display_menu();

char menu_hw_setup();			// Go to the HW setup menu
void display_volume(unsigned char full_update);	// Display the current volume
void display_input(unsigned char full_update);	// Display the current input
void set_title(unsigned char input, char *title);
void display_title();
unsigned char show_title();

#endif
