/*
 * command.c
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */
#include "stdio.h"
#include "F2812_datatype.h"
#include "command.h"
#include "config.h"
#include "shellconsole.h"
#include "string.h"

#ifndef CONFIG_SYS_HELP_CMD_WIDTH
#define CONFIG_SYS_HELP_CMD_WIDTH	12
#endif

extern cmd_tbl_t  cmd_table[];
extern cmd_tbl_t cmd_netshell_table[];
cmd_tbl_t *boot_cmd_start = &cmd_table[0];
cmd_tbl_t *boot_cmd_end = &cmd_table[CONFIG_SYS_CMDSIZE - 1];



INT32 cpustrcmp(const INT8 *cs, const INT8 *ct)
{
    register INT8 __res;

    while (1)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
    }

    return __res;
}


/***************************************************************************
 * find command table entry for a command
 ***************************************************************************/
cmd_tbl_t *find_cmd_tbl (const INT8 *cmd, cmd_tbl_t *table, INT32 table_len)
{
    cmd_tbl_t *cmdtp;
    cmd_tbl_t *cmdtp_temp = table;	/*Init value */
    const INT8 *p;
    int len;
    int n_found = 0;

    /*
     * Some commands allow length modifiers (like "cp.b");
     * compare command name only until first dot.
     */

    len = ((p = strchr(cmd, '.')) == NULL) ? strlen (cmd) : (p - cmd);

    for (cmdtp = table;
            cmdtp != table + table_len;
            cmdtp++)
    {
        if(0 == strcmp(cmdtp->name, "END"))
            break;
        if (strncmp (cmd, cmdtp->name, len) == 0)
        {
            if (len == strlen (cmdtp->name))
                return cmdtp;	/* full match */

            cmdtp_temp = cmdtp;	/* abbreviated command ? */
            n_found++;
        }
    }
    if (n_found == 1)  			/* exactly one match */
    {
        return cmdtp_temp;
    }

    return NULL;	/* not found or ambiguous command */
}

/****************************************************************************/
cmd_tbl_t *find_cmd (const INT8 *cmd)
{
    INT32 len = (boot_cmd_end - boot_cmd_start);
    return find_cmd_tbl(cmd, boot_cmd_start, len);
    //       INT32 len=group_size;
    //      return find_cmd_tbl(cmd,group_start, len);
}

/****************************************************************************/
cmd_tbl_t *find_netshell_cmd (const INT8 *cmd)
{
#if 0
    INT32 len = (boot_netshellcmd_end - boot_netshellcmd_start);
    return find_cmd_tbl(cmd, boot_netshellcmd_start, len);
    //       INT32 len=group_size;
    //      return find_cmd_tbl(cmd,group_start, len);
#endif

}

INT32 cmd_usage(cmd_tbl_t *cmdtp)
{
    shellprintf("%s - %s\n\n", cmdtp->name, cmdtp->usage);

#ifdef	CONFIG_SYS_LONGHELP
    shellprintf("Usage:\n%s ", cmdtp->name);

    if (!cmdtp->help)
    {
        shellputs ("- No additional help available.\n");
        return 1;
    }
#ifdef  SERIALSHELL
    shellputs (cmdtp->help);
    shellputc ('\n');
#elif defined    BIOSSHELL
    extern int udpprintrdy;

    extern int NetPrintfbuffer(char * buffer, int size);
    shellputs (cmdtp->help);
    shellputc ('\n');
/*
    if(udpprintrdy != 0)
    {
        NetPrintfbuffer(cmdtp->help, strlen(cmdtp->help));
        NetPrintfbuffer('\n', 1);
    }
*/    
#endif

#endif	/* CONFIG_SYS_LONGHELP */
    return 1;
}
#pragma DATA_SECTION   (cmd_array,"shell_lib");
far cmd_tbl_t far *cmd_array[CONFIG_SYS_CMDSIZE];

/****************************************************************************/
INT32 _do_help (cmd_tbl_t *cmd_start, INT32 cmd_items, cmd_tbl_t *cmdtp, INT32
              flag, INT32 argc, INT8 *const argv[])
{
    INT32 i;
    INT32 rcode = 0;
	INT16 actcmditemnum = 0;
	INT32 j, swaps;

    if (argc == 1)  	/*show list of commands */
    {



        /* Make array of commands from .uboot_cmd section */
        cmdtp = cmd_start;
        for (i = 0; i < cmd_items; i++)
        {
            cmd_array[i] = cmdtp++;
            if(0 != strcmp(cmd_array[i]->name, "END"))
                actcmditemnum++;
            else
                break;
        }

        shellprintf("actcmditem=%d ,cmd_items=0x%x\n", actcmditemnum,cmd_items);
        /* Sort command list (trivial bubble sort) */
        for (i = actcmditemnum - 1; i > 0; --i)
        {
            swaps = 0;
            for (j = 0; j < i; ++j)
            {
                if (strcmp (cmd_array[j]->name,
                            cmd_array[j + 1]->name) > 0)
                {
                    cmd_tbl_t *tmp;
                    tmp = cmd_array[j];
                    cmd_array[j] = cmd_array[j + 1];
                    cmd_array[j + 1] = tmp;
                    ++swaps;
                }
            }
            if (!swaps)
                break;
        }

        /* print short help (usage) */
        for (i = 0; i < actcmditemnum; i++)
        {
            const INT8 *usage = cmd_array[i]->usage;

            /* allow user abort */
            if (ctrlc ())
                return 1;
            if (usage == NULL)
                continue;

            shellprintf("%-*s- %s\n", CONFIG_SYS_HELP_CMD_WIDTH,
                        cmd_array[i]->name, usage);
        }
        return 0;
    }
    /*
     * command help (long version)
     */
    for (i = 1; i < argc; ++i)
    {
        if ((cmdtp = find_cmd_tbl (argv[i], cmd_start, cmd_items )) != NULL)
        {
            rcode |= cmd_usage(cmdtp);
        }
        else
        {
            shellprintf ("Unknown command '%s' - try 'help'"
                         " without arguments for list of all"
                         " known commands\n\n", argv[i]
                        );
            rcode = 1;
        }
    }
    return rcode;
}


INT32 do_help(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    return _do_help(boot_cmd_start,
                    //		(boot_cmd_end - boot_cmd_start)/(sizeof(cmd_tbl_t)),
                    (boot_cmd_end - boot_cmd_start),
                    cmdtp, flag, argc, argv);
}





INT32 do_nothing(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    return 0;
}


INT32 cmd_get_data_size(INT8 *arg, INT32 default_size)
{
    /* Check for a size specification .b, .w or .l.
     */
    int len = strlen(arg);
    if (len > 2 && arg[len - 2] == '.')
    {
        switch(arg[len - 1])
        {
        case 'b':
            return 1;
        case 'w':
            return 2;
        case 'l':
            return 4;
        case 's':
            return -2;
        default:
            return -1;
        }
    }
    return default_size;
}


