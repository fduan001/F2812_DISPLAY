/*
 ============================================================================
 Name        : ushell.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F2812_datatype.h"
#include "shellconsole.h"
#include "config.h"
#include "command.h"
//#include "watchdog.h"

#pragma DATA_SECTION   (shellstring,"shell_lib");
far char shellstring[20];


extern INT32 do_nothing(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern UINT32 flash_init (void);
extern UINT8 RegisterCommand (cmd_tbl_t commanditem);

UINT8 set_shellprompt (char* p)
{
	memset(shellstring,0,sizeof(shellstring));
	if(strlen(p)>19)
	{
		strncpy(shellstring,CONFIG_SYS_PROMPT,20);
		return 0;
	}
	else
		strncpy(shellstring,p,strlen(p));
	return 1;
}

#pragma DATA_SECTION   (lastcommand,"shell_lib");
 static far INT8 lastcommand[CONFIG_SYS_CBSIZE] = { 0, };
void shell_loop (void)
{
    INT32 len;
    INT32 rc = 1;
    INT32 flag;
   //yexin remove stack size
   // static INT8 lastcommand[CONFIG_SYS_CBSIZE] = { 0, };
    //	flash_init();

    for (;;)
    {
    	WatchdogKick();
        len = readline (shellstring);
        flag = 0;	/* assume no special flags for now */
        if (len > 0)
            strcpy (lastcommand, console_buffer);
        else if (len == 0)
            flag |= CMD_FLAG_REPEAT;

        if (len == -1)
            shellputs ("<INTERRUPT>\n");
        // timeout
        else if(len == -2)

        {

        }

        else {
            rc = run_command (lastcommand, flag);
        }

        if (rc <= 0)
        {
            /* invalid command or not repeatable, forget it */
            lastcommand[0] = 0;

        }
        WatchdogKick();
    }
}






