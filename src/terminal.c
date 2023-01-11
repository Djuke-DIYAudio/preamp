#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "PreampController.h"
#include "Preamp.h"
#include "lcd.h"
#include "menu.h"

#include "terminal.h"
#include "EventHandler.h"

#define TERM_MAX_LINE_SIZE  100

//Type definitions
typedef struct {
    char* name;
    void (*func)(char *args);
    const char* doc;
} cmd_t;

#define MK_CMD(x) void term_ ## x (char *args)

//Functions definitions
MK_CMD(show);
MK_CMD(reboot);
MK_CMD(ver);
MK_CMD(help);
MK_CMD(power);
MK_CMD(title);
MK_CMD(par);
MK_CMD(volume);
MK_CMD(input);
MK_CMD(channel);
MK_CMD(mute);
MK_CMD(settings);
MK_CMD(button);
MK_CMD(db);

//The dispatch table
#define CMD(func, help) {#func, term_ ## func, help}
#define CMDS 14
#define MAX_ARGS 5

cmd_t dsp_table[CMDS] ={
    CMD(show,"Show status"),
    CMD(ver,"Show version"),
    CMD(reboot,"Reboot"),
    CMD(par,"Par functions"),
    CMD(volume,"Vol functions"),
    CMD(input,"Inp functions"),
    CMD(channel,"Ch functions"),
    CMD(title,"Display title"),
    CMD(power,"Power on/off"),
    CMD(mute,"Mute/unmute"),
    CMD(settings,"Settings"),
    CMD(button,"Emulate buttons"),
    CMD(db,"Change debug info"),
    CMD(help,"Display help")
};

volatile uint8_t _term_debug_level = 0;
volatile uint8_t _term_bytes_received = 0;
char _term_buf[TERM_MAX_LINE_SIZE];
volatile char _term_buf_pnt = 0;
volatile char _term_buf_busy = 0;

uint8_t term_bytes_received(void) { return _term_bytes_received; }

void info(const char *str) { printf("[INFO]:%s\n", str); }
void warning(const char *str) { printf("[WARNING]:%s\n", str); }
void error(const char *str) { printf("[ERROR]:%s\n", str); }
void debug(uint8_t db_level, const char *str) {
   if (_term_debug_level >= db_level) {
      if (db_level == 1) printf("[EVENT]:%s\n", str);
      else printf("[DEBUG]:%s\n", str);
   }
}
void syntax(const char *cmd, const char *str) { printf("[SYNTAX]: %s %s\n", cmd, str); }


void term_init(void) {
	TXSTAbits.TX9   = 0;        // 8-bit transmission
	TXSTAbits.TXEN  = 1;        // Transmit enabled
	TXSTAbits.SYNC  = 0;        // Asynchronous mode
	TXSTAbits.SENDB = 0;        // Sync break transmission completed
	TXSTAbits.BRGH  = 1;        // High speed mode
	TXSTAbits.TX9D  = 0;

	RCSTAbits.SPEN = 1;         // Serial port enable
	RCSTAbits.RX9  = 0;         // 8-bit reception
	RCSTAbits.CREN  = 1;        // Enable continuous receive

	BAUDCONbits.RXDTP = 0;      // RX data is not inverted
	BAUDCONbits.TXCKP = 0;      // Idle state is high-level
	BAUDCONbits.BRG16 = 1;      // 16-bit baud rate generator
	BAUDCONbits.WUE   = 0;      // No wake-up enable
	BAUDCONbits.ABDEN = 0;      // Disabled auto-baud detect

//	SPBRG = 21;			// CLOCK_FREQ / (16*baudrate), 115200 at 40MHz with BRG16=0
	SPBRG = 86;			// CLOCK_FREQ / (4*baudrate), 115200 at 40MHz with BRG16=1

	IPR1bits.RCIP = 1;		// High-prio interrupt
	PIR1bits.RCIF = 0;		// Clear interrupt flag
	PIE1bits.RCIE = 1;		// Enable interrupt

	IPR1bits.TXIP = 0;		// Low-prio interrupt
	PIE1bits.TXIE = 0;		// Disable interrupt

}

// for printf usage from the XC8 user guide
void putch(char data)
{
    while( ! TXIF)
        continue;
    TXREG = data;
}

void term_receive(char byte) {
	static char esc_seq = 0;

	_term_bytes_received++;

	if (byte == 0xC1) {	// handle request from TinyBootloader as software reset
		VOLUME_UNMUTE = LOW;
		POWER = LOW;
		LCD_on();
		LCD_clear();
		LCD_row1();
		LCD_puts("Bootloader reset");
        asm("reset");
	}

	if (RCSTAbits.OERR) {
		error("Overrun");
		// Flush the buffer
		_term_buf_pnt = 0;
		_term_buf_busy = 0;
		// Reset overrun error
		RCSTAbits.CREN  = 0;        // Disable continuous receive
		RCSTAbits.CREN  = 1;        // Enable continuous receive
		return;
	}

	if (_term_buf_busy) {
		error("busy");
		return;
	}

	// Debug info
	//printf("recv: %d\n", byte);

	if (byte > 'z') return;		// Ignore extended characters
	if (byte < ' ') {		// Ignore control characters (with some exceptions)
		if (byte == '\r') {}		// Line feed
		else if (byte == '\n') {}	// New line
		else if (byte == '\b') {}	// Backspace
		else if (byte == 27) {		// Escape
			esc_seq = 2;		// Ignore next two bytes
			return;
		}
		else return;
	}

	// Ignore bytes after escape
	if (esc_seq) {
		esc_seq--;
		return;
	}

	if (byte == '\b') {	// Backspace
		if (_term_buf_pnt>0) {
			_term_buf_pnt--;	// Delete last character
			printf("\b \b");	// Removes characters from terminal screen
			return;
		}
	} else {
		if (_term_buf_pnt > TERM_MAX_LINE_SIZE-1) {
			error("Line too long");
			_term_buf_pnt = 0;
			return;
		} else {
			_term_buf[ _term_buf_pnt++ ] = byte;
			TXREG = byte;	// Echo received characters
		}
	}
	if (byte == '\n' || byte == '\r') {
		if (_term_buf_pnt>1) {
			_term_buf[_term_buf_pnt-1] = 0;
			_term_buf_busy = 1;
			TriggerEvent(EVENT_UART);
		} else {	// Empty new line, continue with empty buffer
			_term_buf_pnt = 0;
			_term_buf_busy = 0;
		}
	}

}

char *
term_strtok (char *str, char *control) 
{
  static char *s = NULL;
  register char *s1;

  if (str)
    s = str;

  if (!s)
    return NULL;

  while (*s)
    {
      if (strchr (control, *s))
        s++;
      else
        break;
    }
	
  s1 = s;

  while (*s)
    {
      if (strchr (control, *s))
        {
          *s++ = '\0';
          return s1;
        }
      ++s;
    }

  s = NULL;

  if (*s1)
    return s1;
  else
    return NULL;
}

char *delim = " \n(,);";

unsigned char term_args_parse(char *args, char *argv[]) {
	unsigned char argc=0;

	do {
		argv[argc] = args;
		argc++;
		args = term_strtok(NULL,delim);
	} while (args!=NULL);

	return argc;
}

void term_handle_command(void) {
	unsigned char i=CMDS;
	char* tok = term_strtok(_term_buf,delim);
	if(!tok) return;

	while(i--) {
		if(!strcmp(tok,dsp_table[i].name)) {
			dsp_table[i].func(tok);	// execute function
			_term_buf_pnt = 0;
			_term_buf_busy = 0;
			return;
		}
	}

	printf("Unknown cmd: %s\n", _term_buf);

	_term_buf_pnt = 0;
	_term_buf_busy = 0;
}

void term_show(char *args) {
	unsigned char powered = is_powered();

	if (powered) {
		printf(" power on\n");
		printf(" inp name: %s\n", input_name_string(get_input()));
		printf(" inp type: %s\n", input_type_string(get_input()));
		printf(" inp ch: %s\n", input_channel_string());
		printf(" outp ch: %s\n", output_channel_string());
		printf(" vol: %d\n", get_volume());
	} else {
		printf(" power off\n");
	}
}

void term_par(char *args) {
	unsigned char i;
	uint8_t npar = nr_parameters();
	short value;
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc == 1) { // No parameter, show full list
		printf("Pars: %d\n", npar);
		for (i=0;i<npar;i++) {
			printf(" %10s (%s): %d [%s]\n", get_parameter_cmd_name(i), 
				get_parameter_name(i), get_parameter(i), get_parameter_unit(i));
		}
		return;
	} else if (argc == 2) {
		for(i=0;i<npar;i++) {
			if (!strcmp(argv[1], get_parameter_cmd_name(i))) {
				printf(" %10s (%s): %d [%s]\n", get_parameter_cmd_name(i), 
					get_parameter_name(i), get_parameter(i), get_parameter_unit(i));
				return;
			}
		}
	} else if (argc == 3) {
		for(i=0;i<npar;i++) {
			if (!strcmp(argv[1], get_parameter_cmd_name(i))) {
				value = atoi(argv[2]);
				set_parameter(i, value);
				return;
			}
		}
	}
	printf("Unknown par: %s\n", argv[1]);
}

void term_help(char *args)
{
	unsigned char i=CMDS;
	printf("Cmd list:\n");
	while(i--) {
		printf("%10s - %s\n",dsp_table[i].name,dsp_table[i].doc);
	}
}

void term_mute(char *args) {
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc != 2) {
		syntax(argv[0], "<on|off>");
		return;
	}
	if (!strcmp(argv[1], "on")) set_muted(1);
	else if (!strcmp(argv[1], "off")) set_muted(0);
	else printf("unknown par: %s\n", argv[1]);
}

void term_reboot(char *args) { asm("reset");}

void term_settings(char *args) { 
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc > 1) {
		if (!strcmp(argv[1], "load")) load_settings();
		else if (!strcmp(argv[1], "save")) save_settings(); 
		else if (!strcmp(argv[1], "clear")) clear_settings(); 
		else if (!strcmp(argv[1], "default")) default_settings(); 
		else if (!strcmp(argv[1], "setup")) setup_settings((uint8_t)atoi(argv[2])); 
	} else syntax(argv[0], "<load|save|clear|default|setup>");
}

void term_ver(char *args) {
	printf("%s\n", preamp_name());
	printf("%s\n", preamp_date());
	printf("%s\n", preamp_version());
}

void term_volume(char *args) {
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc > 1) {
		if (!strcmp(argv[1], "inc")) {
			int8_t increment = 2;
			if (argc == 3) increment = (int8_t)atoi(argv[2]);
			add_volume(increment);
			return; 
		} else if (!strcmp(argv[1], "dec")) { 
			int8_t increment = -2;
			if (argc == 3) increment = -(int8_t)atoi(argv[2]);
			add_volume(increment);
			return;
		} else if (!strcmp(argv[1], "set")) {
			if (argc == 3) {
				int16_t volume = (int16_t)atoi(argv[2]);
				set_volume(volume);
				return;
			}
		}
	}

	syntax(argv[0], "<set|inc|dec>");
}

void term_input(char *args) {
	uint8_t input;
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc > 1) {
		if (!strcmp(argv[1], "next")) { next_input(); return; } 
		else if (!strcmp(argv[1], "prev")) { previous_input(); return; }
		else if (!strcmp(argv[1], "set")) {
			if (argc == 3) {
				input = (uint8_t)atoi(argv[2]);
				set_input(input);
				return;
			}
		}
	}
	syntax(argv[0], "<set|next|prev> <value>");
}

void term_channel(char *args) {
	uint8_t channel;
    int8_t value;
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc > 1) {
		if (!strcmp(argv[1], "offset")) {
			if (argc == 4) {
				channel = (uint8_t)atoi(argv[2]);
				value = (int8_t)atoi(argv[3]);
				set_channel_offset(channel, value);
				return;
			}
		}
		if (!strcmp(argv[1], "show")) {
			if (argc == 3) {
				channel = (uint8_t)atoi(argv[2]);
				printf("Offset ch %d (%s): %d\n", channel, channel_name_string(channel), get_channel_offset(channel));
				return;
			}
		}
	}
	syntax(argv[0], "<show|offset> <channel> <value [0.5db]>");
}

void term_title(char *args) {
	uint8_t input;
	char *tok = term_strtok(NULL,delim);
	if (tok) {
		input = (uint8_t)atoi(tok);		
		tok = term_strtok(NULL,"\0");
		if (tok) set_title(input, tok);
	}
}

void term_power(char *args) {
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc == 2) {
		if (!strcmp(argv[1], "on") && !is_powered()) TriggerEvent(EVENT_POWER);
		else if (!strcmp(argv[1], "off") && is_powered()) TriggerEvent(EVENT_POWER);
	} else syntax(argv[0], "<on|off>");
}

void term_button(char *args) {
	char *argv[MAX_ARGS];
	unsigned char argc = term_args_parse(args, argv);

	if (argc > 1) {
		if (!strcmp(argv[1], "power")) TriggerEvent(EVENT_POWER);
		else if (!strcmp(argv[1], "menu")) TriggerEvent(EVENT_MENU);
		else if (!strcmp(argv[1], "left")) TriggerEvent(EVENT_LEFT);
		else if (!strcmp(argv[1], "right")) TriggerEvent(EVENT_RIGHT);
		else if (!strcmp(argv[1], "select")) TriggerEvent(EVENT_SELECT);
	} else {
		syntax(argv[0], "<power|menu|left|right|select>");
	}
}

void term_db(char *args) {
	char *argv[MAX_ARGS];
    char string[20];
	unsigned char argc = term_args_parse(args, argv);

	if (argc > 1) {
        _term_debug_level = (uint8_t)atoi(argv[1]);
	}
	sprintf(string, "Debug level = %d", _term_debug_level);
   info(string);
}

