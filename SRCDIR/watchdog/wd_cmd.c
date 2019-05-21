/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : ddc.c
 * Author        : yexin
 * Date          : 2018-06-18
 * Description   : DDC interface
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-06-18
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include <string.h>
#include "F2812_datatype.h"
#include "shellconsole.h"
#include "command.h"
#include "util.h"
#include "watchdog.h"

INT32 do_watchdog ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
	char *ops = NULL;
    /*check input arg number*/
	if (argc < 2)
	{
		shellprintf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	ops = argv[1];

	if( strcmp(ops, "init") == 0 ) {
		WatchdogInit();
		return 0;
	}

	if( strcmp(ops, "enable") == 0 ) {
		WatchdogEnable();
		return 0;
	}

	if( strcmp(ops, "disable") == 0 ) {
		WatchdogDisable();
		return 0;
	}

	if( strcmp(ops, "kick") == 0 ) {
		WatchdogKick();
		return 0;
	}

	if( strcmp(ops, "show") == 0 ) {
		WatchdogShow();
		return 0;
	}

	return 1;
}

#pragma DATA_SECTION   (g_wdog_cmd,  "shell_lib");
far cmd_tbl_t g_wdog_cmd[] =
{
	{
		"wdog", CONFIG_SYS_MAXARGS, 1,	do_watchdog,
		"wdog init, enable, disable, kick",
		"wdog init\n"
		"wdog enable\n"
		"wdog disable\n"
		"wdog kick\n"
		"wdog show\n"
	},
};

int WatchdogCmdInit(void)
{
	INT8 index;
	for (index = 0; index < sizeof(g_wdog_cmd) / sizeof(cmd_tbl_t); index++)
		RegisterCommand(g_wdog_cmd[index]);
	return 0;
}
