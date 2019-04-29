#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "fuel_measure.h"

s32 do_fuel( cmd_tbl_t *cmdtp, s32 flag, s32 argc, s8 *const argv[])
{
	char *ops = NULL;
	u8 chan = 0;

	if( argc < 2 ) {
		shellprintf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	ops = argv[1];

	if( strcmp(ops, "init") == 0 ) {
		FuelMeasInit();
		return 0;
	}

	if( strcmp(ops, "start") == 0 ) {
		chan = simple_strtoul(argv[2], NULL, 10);
		FuelMeasStart(chan);
		return 0;
	}

	if( strcmp(ops, "reset") == 0 ) {
		return 0;
	}

	if( strcmp(ops, "dump") == 0 ) {
		FuelMeasDump();
		return 0;
	}

	if( strcmp(ops, "test") == 0 ) {
		FuelMeasSelfTest();
		return 0;
	}
	return 1;
}


#pragma DATA_SECTION   (fuel_cmd,"shell_lib");
far cmd_tbl_t fuel_cmd[] =
{

	{
		"fuel", CONFIG_SYS_MAXARGS, 1,	do_fuel,
		"fuel init/start/reset/dump/test",
		"init - initialize the fuel measurement system\n"
		"start chan - start specified channel fuel measurement\n"
		"reset -  reset the fuel measurement system\n"
		"dump - dump  the fuel measurment result\n"
		"test - fuel measurement self test\n"
		"example:  fuel start 0 : start channel 0 fuel measurement\n"
	},
};

void FuelCmdInitialzie()
{
    s8 index;
    for (index = 0; index < sizeof(fuel_cmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(fuel_cmd[index]);
}
