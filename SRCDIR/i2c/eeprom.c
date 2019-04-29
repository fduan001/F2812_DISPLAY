
#include "shellconsole.h"
#include "platform_os.h"
#include "soft_i2c.h"

#define  CONFIG_SYS_EEPROM_WREN   1
#define  CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS   5

#if defined(CONFIG_SYS_EEPROM_WREN)
extern int eeprom_write_enable (unsigned dev_addr, int state);
#endif

#ifndef  I2C_RXTX_LEN_MAX
#define  I2C_RXTX_LEN_MAX     64
#endif

#ifndef  CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS
#define  CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS    5
#endif

#ifndef CONFIG_SYS_EEPROM_ALEN
#define  CONFIG_SYS_EEPROM_ALEN   2
#endif

int EepromInit(int i2c_bus_num)
{
	GpioI2cInit(100000, 0x50);
	return 0;
}

int EepromRead(unsigned dev_addr, unsigned offset, unsigned char *buffer, unsigned cnt)
{
	unsigned end = offset + cnt;

	/* Read data until done or would cross a page boundary.
	 * We must write the address again when changing pages
	 * because the next page may be in a different device.
	 */
	while (offset < end) {
		unsigned maxlen = 0, len = 0;
		
		len = end - offset;

		maxlen = I2C_RXTX_LEN_MAX;
		if(len > maxlen) len = maxlen;

		if (GpioI2cReadBytes (dev_addr, offset, CONFIG_SYS_EEPROM_ALEN, buffer, len) != 0)
        {
            PRINTF("eeprom_read: %x %d %d %d %d\n", dev_addr, offset, 2, buffer[0], len);
			return -1;
        }

		buffer += len;
		offset += len;
	}

	return 0;
}


int EepromWrite(unsigned int addr, unsigned int offset, unsigned char *buffer, unsigned int count)
{
	unsigned int end = offset + count;

#if defined(CONFIG_SYS_EEPROM_WREN)
	eeprom_write_enable(addr, 1);
#endif
	
	while (offset < end) {
		unsigned int len = 0, maxlen = 0;
		
		len = end - offset;

		maxlen = I2C_RXTX_LEN_MAX;
		if(len > maxlen) len = maxlen;

		if (GpioI2cWriteBytes(addr, offset, CONFIG_SYS_EEPROM_ALEN, buffer, len) != 0) {
			return -1;
		}

		buffer += len;
		offset += len;

#if defined(CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS)
		Osal_TaskSleep(CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS);
#endif
	}

#if defined(CONFIG_SYS_EEPROM_WREN)
	eeprom_write_enable(addr,0);
#endif

    return 0;
}

