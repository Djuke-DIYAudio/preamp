#ifndef UART_H
#define UART_H

#include <xc.h>

#define NONE			0
#define INFO			1
#define WARNING         2
#define ERROR			3

void term_receive(char byte);
void term_init(void);
void term_handle_command(void);
uint8_t term_bytes_received(void);

void info(const char *str);
void warning(const char *str);
void error(const char *str);
void debug(unsigned char db_level, const char *str);

// for printf usage from the XC8 user guide
void putch(char data);


#endif
