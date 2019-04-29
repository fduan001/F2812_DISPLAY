#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "flow_measure.h"

s32 do_flow( cmd_tbl_t *cmdtp, s32 flag, s32 argc, s8 *const argv[])
{
	char *ops = NULL;
	u8 chan = 0;

	if( argc < 2 ) {
		shellprintf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	ops = argv[1];

	if( strcmp(ops, "init") == 0 ) {
		FlowMeasInit();
		return 0;
	}

	if( strcmp(ops, "start") == 0 ) {
		chan = simple_strtoul(argv[2], NULL, 10);
		FlowMeasStart(chan);
		return 0;
	}

	if( strcmp(ops, "reset") == 0 ) {
		return 0;
	}

	if( strcmp(ops, "dump") == 0 ) {
		FlowMeasDump();
		return 0;
	}

	if( strcmp(ops, "test") == 0 ) {
		FlowMeasSelfTest();
		return 0;
	}
	return 1;
}


#pragma DATA_SECTION   (flow_cmd,"shell_lib");
far cmd_tbl_t flow_cmd[] =
{

	{
		"flow", CONFIG_SYS_MAXARGS, 1,	do_flow,
		"flow init/start/reset/dump/test",
		"init - initialize the flow measurement system\n"
		"start chan - start specified channel flow measurement\n"
		"reset -  reset the flow measurement system\n"
		"dump - dump  the flow measurment result\n"
		"test - flow measurement self test\n"
		"example:  flow start 0 : start channel 0 flow measurement\n"
	},
};

void FlowCmdInitialzie()
{
    s8 index;
    for (index = 0; index < sizeof(flow_cmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(flow_cmd[index]);
}
