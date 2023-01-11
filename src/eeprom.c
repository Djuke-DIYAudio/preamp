#include "eeprom.h"
#include "PreampController.h"

char PA_eeprom_read(unsigned short address)
{
	// Load address
	EEADRH = (address >> 8) & 0xff;
	EEADR = (address & 0xff);

	// Point to EEPROM memory
	EECON1bits.EEPGD = 0;
	
	// 18F devices also need the CFGS bit to be cleared
	EECON1bits.CFGS = 0;

	// Read, data is available in eedata the next cycle.
	EECON1bits.RD = 1;
//	_asm
//		movf _EEDATA, W
//	_endasm;
	while (EECON1bits.RD){};	// RD is cleared by hardware
	
	// Return value
	return EEDATA;

}

void PA_eeprom_write(unsigned short address, char dat)
{
	// Load address and data
	EEADRH = (address >> 8) & 0xff;
	EEADR = (address & 0xff);
	EEDATA = dat;

	// Point to EEPROM memory
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	// Write enable
	EECON1bits.WREN = 1;
	// Disable interrupts
	INTCONbits.GIE = 0;

	// Required write sequence
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	// Enable interrupts
	INTCONbits.GIE = 1;

	// Wait for write to complete
	while (EECON1bits.WR){};

	// Disable write
	EECON1bits.WREN = 0;
}

void PA_eeprom_bulkwrite(unsigned short start_address, void *dat, unsigned short nbytes)
{
	unsigned short i;

	// Point to EEPROM memory
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	// Write enable
	EECON1bits.WREN = 1;
	// Disable interrupts
	INTCONbits.GIE = 0;

	for (i=0;i<nbytes;i++) {
		// Load address and data
		EEADRH = ((start_address+i) >> 8) & 0xff;
		EEADR = ((start_address+i) & 0xff);
		EEDATA = ((char *) dat)[i];

		// Required write sequence
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1bits.WR = 1;

		// Wait for write to complete
		while (EECON1bits.WR){};
	}

	// Enable interrupts
	INTCONbits.GIE = 1;

	// Disable write
	EECON1bits.WREN = 0;
}

void PA_eeprom_bulkread(unsigned short start_address, void *dat, unsigned short nbytes)
{
	unsigned short i;

	// Point to EEPROM memory
	EECON1bits.EEPGD = 0;
	// 18F devices also need the CFGS bit to be cleared
	EECON1bits.CFGS = 0;

	for (i=0;i<nbytes;i++) {

		// Load address
		EEADRH = ((start_address+i) >> 8) & 0xff;
		EEADR = ((start_address+i) & 0xff);

		// Read, data is available in eedata the next cycle.
		EECON1bits.RD = 1;

		while (EECON1bits.RD){};	// RD is cleared by hardware
		
		((char *)dat)[i] = EEDATA;
	}
}
