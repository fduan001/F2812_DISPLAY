/*
 * command_table.c
 *
 */

#include "F2812_datatype.h"
#include "command.h"
#include "ossupport.h"
#include "string.h"
#include "shellconsole.h"


extern INT32 do_netshellhelp(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_help(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_nothing(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_base (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_cmp (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_cp ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_loop (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_loopw (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_md (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_mm (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_mtest (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_mw (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);
extern INT32 do_mem_nm (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);

#pragma DATA_SECTION   (cmd_table,"shell_lib");
cmd_tbl_t  cmd_table[CONFIG_SYS_CMDSIZE] =
{
    {
        "help",	CONFIG_SYS_MAXARGS,	1,	do_help,
        "print command description/usage",
        "\n"
        "	- print brief description of all commands\n"
        "help command ...\n"
        "	- print detailed usage of 'command'"
    },


    {
        "md",	3,	1,	do_mem_md,
        "memory display",
        "[.b, .w, .l] address [# of objects]"
    },
#if 0
    {
        "mm",	2,	1,	do_mem_mm,
        "memory modify (auto-incrementing address)",
        "[.b, .w, .l] address"
    },
    {
        "nm",	2,	1,	do_mem_nm,
        "memory modify (constant address)",
        "[.b, .w, .l] address"
    },
#endif

    {
        "mw",	4,	1,	do_mem_mw,
        "memory write (fill)",
        "[.b, .w, .l] address value [count]"
    },

    {
        "mtest",	5,	1,	do_mem_mw,
        "memory test (fill)",
        "start end [pattern] [count]"
    },



    /*************************************************************************
     *  the last null command
     *************************************************************************/
    {
        "END",	CONFIG_SYS_MAXARGS,	1,	do_nothing,
        "print command description/usage",
        "\n"
        "	- print brief description of all commands\n"
        "help command ...\n"
        "	- print detailed usage of 'command'"
    },


};



cmd_tbl_t endcmmand =
{
    "END",	CONFIG_SYS_MAXARGS,	1,	do_nothing,
    "print command description/usage",
    "\n"
    "	- print brief description of all commands\n"
    "help command ...\n"
    "	- print detailed usage of 'command'"
};





#if defined BIOS_OS_SUPPORT &defined CMDTBL_MUTEX

GateMutex_Params cmdtbl_mutex_prms;
Error_Block cmdtbl_mutex_eb;
GateMutex_Handle  cmdtbl_mutex_handle;
uint16_t cmdtbl_mutex_key;

#define BSP_CMDTBL_MUTEX_LOCK      c66xi2c_mutex_key=GateMutex_enter (cmdtbl_mutex_handle)
#define BSP_CMDTBL_MUTEX_UNLOCK    GateMutex_leave(cmdtbl_mutex_handle,cmdtbl_mutex_key)


#elif defined CMDTBL_HARDWARE_SEMAPHORE

#define BSP_CMDTBL_MUTEX_LOCK      platform_C66xCmdTblSemaphoreEnter()
#define BSP_CMDTBL_MUTEX_UNLOCK    platform_C66xCmdTblSemaphoreExit()

#else


#define BSP_CMDTBL_MUTEX_LOCK
#define BSP_CMDTBL_MUTEX_UNLOCK

#endif

UINT8 RegisterCommand (cmd_tbl_t commanditem)
{
    INT32 index = 0;
    INT32 endindex = 0;

    //find the END command index
    BSP_CMDTBL_MUTEX_LOCK;
    endindex = 0xffff;
    for(index = 0; index < CONFIG_SYS_CMDSIZE; index++)
    {
        if(0 == strcmp(cmd_table[index].name, "END"))
        {
            endindex = index;
            break;
        }
    }
    if((0xffff == endindex) | (index == (CONFIG_SYS_CMDSIZE - 1)))
    {
        shellprintf("the command table is full \n");
        BSP_CMDTBL_MUTEX_UNLOCK;
        return 0;
    }
    else
    {
        cmd_table[index] = commanditem;
        cmd_table[index + 1] = endcmmand;
    }
    BSP_CMDTBL_MUTEX_UNLOCK;
    return 1;

}


UINT8 CommandTableInit()
{
    INT32 index = 0;
    INT32 endindex = 0;

#if defined BIOS_OS_SUPPORT  &&  defined CMDTBL_MUTEX
    Error_init(&cmdtbl_mutex_eb);
    GateMutex_Params_init (&cmdtbl_mutex_prms);
    c66xi2c_mutex_handle = GateMutex_create(&cmdtbl_mutex_prms, &cmdtbl_mutex_eb);
    if(NULL = cmdtbl_mutex_handle)
    {
        shellprintf("initialize c66x i2c mutex error\n");
    }
#endif
    BSP_CMDTBL_MUTEX_LOCK;
    //find the END command index
    for(index = 0; index < CONFIG_SYS_CMDSIZE; index++)
    {
        if(0 == strcmp(cmd_table[index].name, "END"))
        {
            endindex = index;
            break;
        }
    }
    //clear all other null index;
    for(index = endindex + 1; index < CONFIG_SYS_CMDSIZE; index++)
    {
        memset(cmd_table + index, 0, sizeof(cmd_tbl_t));
    }
    cmd_table[endindex] = endcmmand;



    BSP_CMDTBL_MUTEX_UNLOCK;

}


