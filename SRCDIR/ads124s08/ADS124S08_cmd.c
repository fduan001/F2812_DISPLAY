#include "F2812_datatype.h"
#include "shellconsole.h"
#include "command.h"
#include "util.h"
#include "ADS124S08.h"

#ifndef NULL
#define NULL   (void*)0
#endif

s32 do_ads124s08 ( cmd_tbl_t *cmdtp, s32 flag, s32 argc, s8 *const argv[])
{
	char *ops;
	u8 regnum = 0;
	u8 val = 0;
	u8 buffer[5];
	s8 i = 0;

    /*check input arg number*/
	if (argc < 2)
	{
		shellprintf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	ops = argv[1];

	if( strcmp(ops, "init") == 0) {
		ADS124S08_Init();
		return 0;
	}

	if( strcmp(ops, "reset") == 0) {
		PRINTF("TBD\n");
		return 0;
	}

	if( strcmp(ops, "rr") == 0 ) {
		regnum = simple_strtoul(argv[2], NULL, 16);
		val = ADS124S08_ReadReg(regnum);
		PRINTF("%x: 0x%02x\n", regnum, val);
		return 0;
	}

	if( strcmp(ops, "wr") == 0 ) {
		regnum = simple_strtoul(argv[2], NULL, 16);
		val = simple_strtoul(argv[3], NULL, 16);
		ADS124S08_WriteReg(regnum, val);
		return 0;
	}

	if( strcmp(ops, "restart") == 0 ) {
		ADS124S08_RestartSync();
		return 0;
	} 

	if( strcmp(ops, "cmd") == 0 ) {
		val = simple_strtoul(argv[2], NULL, 16);
		ADS124S08_SendCmd(val);
		return 0;
	}

	if( strcmp(ops, "rdata") == 0 ) {
		ADS124S08_ReadDate(&buffer[0], &buffer[1], &buffer[4]);
		for( i = 0; i < 5; ++i ) {
			PRINTF("0x%02x ", buffer[i]);
		}
		PRINTF("\n");
		return 0;
	}

	if( strcmp(ops, "clock") == 0 ) {
		val = simple_strtoul(argv[2], NULL, 10);
		if( val  ) {
			ADS124S08_AssertClock();
		} else {
			ADS124S08_DeassertClock();
		}
		return 0;
	}

	return 1;
}

#pragma DATA_SECTION   (ads124s08_cmd,"shell_lib");
far cmd_tbl_t ads124s08_cmd[] =
{
	{
		"ads124", CONFIG_SYS_MAXARGS, 1,	do_ads124s08,
		"ads124 init/reset/rr/wr/restart/cmd/rdata/clock",
		"example:  ads124 init\n"
	},
};


void ADS124S08CmdInitialize()
{
	s8 index;

	for (index = 0; index < sizeof(ads124s08_cmd) / sizeof(cmd_tbl_t); index++)
		RegisterCommand(ads124s08_cmd[index]);
}
