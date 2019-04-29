
#include "eeprom.h"
#include "shellconsole.h"
#include "command.h"
#include "util.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#endif

int do_eeprom_read(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	unsigned int devaddr;
	unsigned int offset;
	unsigned int length;
	unsigned char buffer[20];
	int rc = 0;
	int i = 0;

	if (argc != 4)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	devaddr = simple_strtoul(argv[1], NULL, 16);
	offset = simple_strtoul(argv[2], NULL, 16);
	length = simple_strtoul(argv[3], NULL, 16);

	rc = EepromRead(devaddr, offset, (unsigned char*)buffer, length);
	if( rc == 0 ) {
		for( i = 0 ; i < 20; ++i ) {
			PRINTF("0x%02x \n", buffer);
			if( ((i+1)%8 == 0) ) {
				PRINTF("\n");
			}
		}
	} else {
		PRINTF("EEPROM read failed\n");
		return CMD_RET_FAILURE;
	}

	return CMD_RET_SUCCESS;
}

int do_eeprom_write(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	unsigned int devaddr;
	unsigned int offset;
	unsigned int source;
	unsigned int length;

	if (argc != 5)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	devaddr = simple_strtoul(argv[1], NULL, 16);
	offset = simple_strtoul(argv[2], NULL, 16);
	source = simple_strtoul(argv[3], NULL, 16);
	length = simple_strtoul(argv[4], NULL, 16);

	if( EepromWrite(devaddr, offset, (unsigned char*)source, length) != 0 ) {
		PRINTF("EEPROM WRITE FAILED\n");
		return CMD_RET_FAILURE;
	}

	PRINTF("EEPROM WRITE COMPLETE\n");
	return CMD_RET_SUCCESS;
}

static cmd_tbl_t cmd_eeprom_sub[] = {
	{"read", 6, 1, do_eeprom_read, "", ""},
	{"write", 6, 1, do_eeprom_write, "", ""},
};

int do_eeprom(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2) {

		return CMD_RET_USAGE;
	}

	/* Strip off leading 'i2c' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_eeprom_sub[0], ARRAY_SIZE(cmd_eeprom_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

#pragma DATA_SECTION   (eeprom_cmd,"shell_lib");
far cmd_tbl_t eeprom_cmd[] =
{
	{
		"eeprom", CONFIG_SYS_MAXARGS, 1,	do_eeprom,
		"eeprom read/write",
		"example:  eeprom read 0x50  0x2  0x10\n"
	},
};

int EepromCmdInit(void)
{
	s8 index;

    for (index = 0; index < sizeof(eeprom_cmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(eeprom_cmd[index]);
	return 0;
}
