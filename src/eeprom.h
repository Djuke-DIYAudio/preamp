#ifndef EEPROM_H
#define EEPROM_H

// eeprom
void eeprom_write(unsigned short address, char dat);
char eeprom_read(unsigned short address);
void eeprom_bulkwrite(unsigned short start_address, void *dat, unsigned short nbytes);
void eeprom_bulkread(unsigned short start_address, void *dat, unsigned short nbytes);

#endif