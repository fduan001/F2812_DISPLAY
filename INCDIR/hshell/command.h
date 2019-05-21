/*
 * command.h
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include "config.h"
#include "F2812_datatype.h"

#define CMD_FLAG_REPEAT		0x0001	/* repeat last command		*/
#define CMD_FLAG_BOOTD		0x0002	/* command is from bootd	*/


#if 0
#define Struct_Section  __attribute__ ((unused,section (".u_boot_cmd")))
#ifdef  CONFIG_SYS_LONGHELP
#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help) \
cmd_tbl_t __u_boot_cmd_##name Struct_Section = {#name, maxargs, rep, cmd, usage, help}


#define U_BOOT_CMD_MKENT(name,maxargs,rep,cmd,usage,help) \
{#name, maxargs, rep, cmd, usage, help}

#else	/* no long help info */

#define U_BOOT_CMD(name,maxargs,rep,cmd,usage,help) \
cmd_tbl_t __u_boot_cmd_##name Struct_Section = {#name, maxargs, rep, cmd, usage}

#define U_BOOT_CMD_MKENT(name,maxargs,rep,cmd,usage,help) \
{#name, maxargs, rep, cmd, usage}

#endif	/* CONFIG_SYS_LONGHELP */

#endif


struct cmd_tbl_s
{
    INT8		*name;		/* Command Name			*/
    INT32		maxargs;	/* maximum number of arguments	*/
    INT32		repeatable;	/* autorepeat allowed?		*/
    /* Implementation function	*/
    INT32		(*cmd)(struct cmd_tbl_s *, INT32, INT32, INT8 *const []);
    INT8		*usage;		/* Usage message	(short)	*/
#ifdef	CONFIG_SYS_LONGHELP
    INT8		*help;		/* Help  message	(long)	*/
#endif
#ifdef CONFIG_AUTO_COMPLETE
    /* do auto completion on the arguments */
    INT32		(*complete)(INT32 argc, INT8 *const argv[], INT8 last_char, INT32 maxv, INT8 *cmdv[]);
#endif
};

typedef struct cmd_tbl_s	cmd_tbl_t;
//typedef INT32 (*PCOMMANDFUNC)(INT32, INT32);
typedef INT32 (* PCOMMANDFUNC)(cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[]);



cmd_tbl_t *find_cmd_tbl (const INT8 *cmd, cmd_tbl_t *table, INT32 table_len);
cmd_tbl_t *find_cmd (const INT8 *cmd);
cmd_tbl_t *find_netshell_cmd(const INT8 *cmd);
INT32 cmd_usage(cmd_tbl_t *cmdtp);
INT32 cmd_get_data_size(INT8 *arg, INT32 default_size);
UINT8 RegisterCommand (cmd_tbl_t commanditem);

typedef enum {
	CMD_RET_SUCCESS = 0,
	CMD_RET_USAGE,
	CMD_RET_FAILURE,
} CMD_RET_VAL_T;



#endif /* COMMAND_H_ */
