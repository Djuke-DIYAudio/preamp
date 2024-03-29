#include "InputSelect.h"
#include "mcp23s08.h"
#include "terminal.h"

void inputselect_init(void) {
	uint8_t direction;
	mcp23s08_set_direction(0b11111111);	// 8 inputs
	direction = mcp23s08_get_direction();

	if (direction != 0xff) error("InputSelect not detected");
	else info("InputSelect detected");

	mcp23s08_set_direction(0b00000000);	// 8 outputs
}

void inputselect_set_input(uint8_t input) {
	mcp23s08_set_gpio(input);
}

/*unsigned char inputselect_get_input(void) {
	return mcp23s08_get_gpio();
}*/
