#include <string.h>

#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"
#include "soft_i2c.h"

s32 do_i2c ( cmd_tbl_t *cmdtp, s32 flag, s32 argc, s8 *const argv[])
{
    char *ops;
    u16  chip;
    u16 addr;
    u8 alen;
    u16 len;
    u8 data[20];
    u16 mem_addr = 0;
    u16 i = 0;

    /*check input arg number*/
    if (argc < 3)
    {
        shellprintf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    ops = argv[1];
    if( strcmp(ops, "init") == 0) {
    	GpioI2cInit(100000, 0x50);
        return 0;
    }

    if( strcmp(ops, "probe") == 0 ) {
    	chip = simple_strtoul(argv[2], NULL, 16);
    	if( GpioI2cProbe(chip) == 0 ) {
    		shellprintf("%02x \n", chip);
    	} else {
    		shellprintf("%02x not probed\n", chip);
    	}
        return 0;
    }

    if( strcmp(ops, "read") == 0 ) {
    	chip = simple_strtoul(argv[2], NULL, 16);
    	addr = simple_strtoul(argv[3], NULL, 16);
    	alen = simple_strtoul(argv[4], NULL, 10);
    	len = simple_strtoul(argv[5], NULL, 10);
    	if( len > 20 ) { len = 20; }
    	if( GpioI2cReadBytes(chip, addr, alen, data, len) == 0 ) {
    		for( i = 0; i < len; ++i ) {
    			shellprintf("%02x \n", data[i]);
    			if( ((i+1) % 8) == 0 ) {
    				shellprintf("\n");
    			}
    		}
    	} else {
    		shellprintf("GpioI2cReadBytes failed %x %x %u %u\n",
    				chip, addr, alen, len);
    	}
        return 0;
    }

    if( strcmp(ops, "write") == 0 ) {
    	chip = simple_strtoul(argv[2], NULL, 16);
    	addr = simple_strtoul(argv[3], NULL, 16);
    	alen = simple_strtoul(argv[4], NULL, 10);
    	len = simple_strtoul(argv[5], NULL, 10);
    	mem_addr = simple_strtoul(argv[6], NULL, 16);

    	if( GpioI2cWriteBytes(chip, addr, alen, (unsigned char*)mem_addr, len) == 0 ) {
    		shellprintf("GpioI2cWriteBytes write complete\n");
    	} else {
    		shellprintf("GpioI2cWriteBytes failed %x %x %u %x %u\n",
    				chip, addr, alen, mem_addr, len);
    	}

    	return 0;
    }

    return 1;
}

#pragma DATA_SECTION   (i2c_cmd,"shell_lib");
far cmd_tbl_t i2c_cmd[] =
{
	{
		"i2c", CONFIG_SYS_MAXARGS, 1,	do_i2c,
		"i2c init/probe/read/write",
		"i2c operations\n"
		"example:  i2c init\n"
	},
};

void I2CCmdInitialize()
{
    s8 index;

    for (index = 0; index < sizeof(i2c_cmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(i2c_cmd[index]);
}
