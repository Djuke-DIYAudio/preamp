#ifndef INPUTSELECT_H
#define INPUTSELECT_H

#include <stdint.h>

// Function definitions
void inputselect_init(void);
void inputselect_set_input(uint8_t input);
unsigned char inputselect_get_input(void);

#endif