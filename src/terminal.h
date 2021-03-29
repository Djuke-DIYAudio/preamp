#ifndef UART_H
#define UART_H

#define NONE			0
#define INFO			1
#define WARNING		2
#define ERROR			3

void term_receive(unsigned char byte);
void term_init();
void term_handle_command();
short term_bytes_received();

void info(const char *str);
void warning(const char *str);
void error(const char *str);
void debug(unsigned char db_level, const char *str);

#endif
