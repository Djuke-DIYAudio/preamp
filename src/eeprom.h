#ifndef EEPROM_H
#define EEPROM_H

// JF avoid confilct with XC8 built in eeprom functions
void PA_eeprom_write(unsigned short address, char dat);
char PA_eeprom_read(unsigned short address);
void PA_eeprom_bulkwrite(unsigned short start_address, void *dat, unsigned short nbytes);
void PA_eeprom_bulkread(unsigned short start_address, void *dat, unsigned short nbytes);

#endif