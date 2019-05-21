/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : mid_cmd.c
 * Author        : yexin
 * Date          : 2018-07-27
 * Description   : mid command
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-07-27
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"

INT32 do_isr ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    char *ops;
    UINT16  num;
    /*check input arg number*/
    if (argc < 3)
    {
        shellprintf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    ops = argv[1];
    num = simple_strtoul(argv[2], NULL, 10);
    if( strcmp(ops, "enable") == 0) {
    	Osal_EnableIsr(num);
        return 0;
    }

    if( strcmp(ops, "disable") == 0 ) {
    	Osal_DisableIsr(num);
        return 0;
    }

    return 1;
}

#pragma DATA_SECTION   (isrcmd,"shell_lib");
far cmd_tbl_t isrcmd[] =
{
	{
		"isr", CONFIG_SYS_MAXARGS, 1,	do_isr,
		"isr ops  intNum",
		"ops: enable - enable ISR\n"
		"     disable - disable\n"
		"intNum :  interrupt number\n"
		"example:  isr enable  35\n"
	},
};

void MidCmdInitialize()
{
    INT8 index;

    for (index = 0; index < sizeof(isrcmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(isrcmd[index]);
}


