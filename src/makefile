SRC=mcp23s08.c cs3318.c cs4398.c cs8416.c DAC.c eeprom.c EventHandler.c infrared.c InputSelect.c lcd.c menu.c Preamp.c PreampController.c terminal.c time_delay_40Mhz.c VolumeControl.c
HEX=DjukePreampV2.hex

FAMILY=pic16
PROC=18f4620
CC=sdcc
CFLAGS=-c --use-non-free -Wa"--warning 1"
LD=sdcc
LDFLAGS=-m$(FAMILY) -p$(PROC) --use-non-free -Wl-c -Wl-m
OBJECTS=$(SRC:.c=.o)
LIBS=/usr/local/share/sdcc/lib/pic16/libsdcc.lib /usr/local/share/sdcc/lib/pic16/libc18f.lib /usr/local/share/sdcc/non-free/lib/pic16/libdev$(PROC).lib

all: $(SRC) $(HEX)

$(HEX): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

.c.o:
	$(CC) -m$(FAMILY) -p$(PROC) $(CFLAGS) $<

clean:
	rm -f $(SRC:.c=.asm) $(SRC:.c=.o) $(SRC:.c=.adb) $(SRC:.c=.lst) $(HEX:.hex=.cod) $(HEX:.hex=.cof) $(HEX:.hex=.lst) $(HEX:.hex=.map) $(HEX:.hex=.hex)

.PHONY: all clean
