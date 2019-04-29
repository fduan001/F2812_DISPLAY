#include "shellconsole.h"
#include "command.h"
#include "util.h"
#include "AD568X.h"

#ifndef NULL
#define NULL   (void*)0
#endif

s32 do_ad568x ( cmd_tbl_t *cmdtp, s32 flag, s32 argc, s8 *const argv[])
{
	char *ops;
	u8 mode = 0;
	u8 chan = 0;
	u32 val = 0;
	u32 ovol = 0;
	u32 vref = 0;

    /*check input arg number*/
	if (argc < 2)
	{
		shellprintf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	ops = argv[1];

	if( strcmp(ops, "init") == 0) {
		AD568X_Init(AD5684);
		return 0;
	}

	if( strcmp(ops, "reset") == 0) {
		AD568X_Reset(AD568X_RST_ZERO_SCALE);
		return 0;
	}

	if( strcmp(ops, "pwrmode") == 0 ) {
		chan = simple_strtoul(argv[2], NULL, 10);
		mode = simple_strtoul(argv[3], NULL, 10);
		AD568X_PowerMode(chan, mode);
		return 0;
	}

	if( strcmp(ops, "ivref") == 0 ) {
		mode = simple_strtoul(argv[2], NULL, 10);
		AD568X_InternalVoltageReference(mode);
		return 0;
	}

	if( strcmp(ops, "ireg") == 0 ) {
		val = simple_strtoul(argv[2], NULL, 10);
		AD568X_SetInputRegister(val);
		return 0;
	} 

	if( strcmp(ops, "rdbk") == 0 ) {
		chan = simple_strtoul(argv[2], NULL, 10);
		val = AD568X_ReadBack(chan);
		PRINTF("val=0x%04x\n", val);
		return 0;
	}

	if( strcmp(ops, "setvol") == 0 ) {
		chan = simple_strtoul(argv[2], NULL, 10);
		ovol = simple_strtoul(argv[3], NULL, 10);
		vref = simple_strtoul(argv[4], NULL, 10);

		AD568X_SetVoltage(chan, (float)ovol/1000, (float)vref/1000);
		return 0;
	}

	if( strcmp(ops, "wfunc") == 0 ) {
		mode = simple_strtoul(argv[2], NULL, 10);
		chan = simple_strtoul(argv[3], NULL, 10);
		val = simple_strtoul(argv[4], NULL, 10);

		AD568X_WriteFunction(mode, chan, val);
		return 0;
	}

	return 1;
}

#pragma DATA_SECTION   (ad568x_cmd,"shell_lib");
far cmd_tbl_t ad568x_cmd[] =
{
	{
		"ad568x", CONFIG_SYS_MAXARGS, 1,	do_ad568x,
		"ad568x init/reset/pwrmode/ivref/ireg/rdbk/setvol/wfunc",
		"ad568x chan: 1(A) 2(B) 4(C) 8(D) 3(A and B) 15(ALL)\n"
		"ad568x init - initialize the ad568x\n"
		"ad568x reset - reset the ad568x\n"
		"ad568x pwrmode mode - set power mode of the ad568x\n"
		"ad568x ivref vRefMode(0 - internal on, 1 - internal off) - set internal voltage reference mode\n"
		"ad568x ireg value - set input register value\n"
		"ad568x setvol channel output(1/1000) vRef(1/1000) - set volatge\n"
		"ad568x wfunc command channel data - send command to specified channel\n"
		"example:  ad568x init\n"
	},
};

void AD567XCmdInitialize()
{
	s8 index;

	for (index = 0; index < sizeof(ad568x_cmd) / sizeof(cmd_tbl_t); index++)
		RegisterCommand(ad568x_cmd[index]);
}
