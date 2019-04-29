
#ifndef _EEPROM_H_
#define _EEPROM_H_


int EepromInit(int i2c_bus_num);
int EepromRead(unsigned dev_addr, unsigned offset, unsigned char *buffer, unsigned cnt);
int EepromWrite (unsigned dev_addr, unsigned offset, unsigned char *buffer, unsigned cnt);


#endif

