/*
 * console.c
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */

//#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "serial.h"
#include "config.h"
#include "command.h"
#include "shellctype.h"


#define putnstr(str,n)	do {			\
		shellprintf ("%.*s", (s32)n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)

#define MAX_CMDBUF_SIZE		CONFIG_SYS_CBSIZE

#define CTL_BACKSPACE		('\b')
#define DEL			((s8)255)
#define DEL7			((s8)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	shellputc(ch)
#define getcmd_getch()		shellgetc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

#pragma DATA_SECTION   (hist_max,"shell_lib");
static far s32 hist_max = 0;
#pragma DATA_SECTION   (hist_add_idx,"shell_lib");
static far s32 hist_add_idx = 0;
#pragma DATA_SECTION   (hist_cur,"shell_lib");
static far s32 hist_cur = -1;
#pragma DATA_SECTION   (hist_num,"shell_lib");
far u32 hist_num = 0;
#pragma DATA_SECTION   (hist_list,"shell_lib");
far s8 *hist_list[HIST_MAX];
#pragma DATA_SECTION   (hist_lines,"shell_lib");
far s8 hist_lines[HIST_MAX][HIST_SIZE + 1];	 /* Save room for NULL */

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

#pragma DATA_SECTION   (console_buffer,"shell_lib");
far s8    console_buffer[CONFIG_SYS_CBSIZE + 1];	/* console I/O buffer	*/
#pragma DATA_SECTION   (erase_seq,"shell_lib");
far static s8 erase_seq[] = "\b \b";		/* erase sequence	*/
#pragma DATA_SECTION   (tab_seq,"shell_lib");
far static s8   tab_seq[] = "        ";		/* used to expand TABs	*/





extern s32 readline_into_buffer (const s8 *const prompt, s8 *buffer);
extern s32 cread_line(const s8 *const prompt, s8 *buf, u32 *len);
extern s32 run_netshell_command (const s8 *cmd, s32 flag);

far int udpprintrdy = 0;
// net shell bufferextern char


/**************************************************************************
        console  input and output
****************************************************************************/

void shellsetudpprintmode(u8 enable)
{
    udpprintrdy = enable;
}

u8 shellgetudprintmode()
{
    return udpprintrdy;
}

void shellputs(const s8 *s)
{
    serial_puts(s);
}

/****************************************************************************/

void shellputc(const s8 c)
{
    serial_putc(c);
}

/****************************************************************************/
s32 shellgetc(void)
{
    return serial_getc();
}

/****************************************************************************/
#pragma DATA_SECTION   (printbuffer,"shell_lib");
far  s8 printbuffer[CONFIG_SYS_PBSIZE];

extern void Osal_ExitGlobalCriticalSection();
extern void Osal_EnterGlobalCriticalSection();

s32 shellprintf(const s8 *fmt, ...)
{
    va_list args;
    s32 i;
    //yexin remove stack size

    s32 size;


#ifdef  SERIALSHELL
    {
        va_start(args, fmt);

        /* For this to work, printbuffer must be larger than
         * anything we ever want to print.
         */
        i = vsprintf(printbuffer, fmt, args);
        va_end(args);

        /* Print the string */
        shellputs(printbuffer);
        return i;
    }
#elif defined    BIOSSHELL

      Osal_EnterGlobalCriticalSection();


    {
        va_start(args, fmt);

        /* For this to work, printbuffer must be larger than
         * anything we ever want to print.
         */
        i = vsprintf(printbuffer, fmt, args);
        //    i=System_vsprintf(printbuffer,fmt,args);
        va_end(args);


        /* Print the string */
        shellputs(printbuffer);
        Osal_ExitGlobalCriticalSection();		
    }
#if 0
    if(udpprintrdy != 0)
    {

        /* For this to work, printbuffer must be larger than
         * anything we ever want to print.
         */
        size = NetPrintfbuffer(printbuffer, i);

    }
#endif

#endif
    return i;


}


/***********************************************************************
 *
 ********************************************************************/
static void hist_init(void)
{
    s32 i;

    hist_max = 0;
    hist_add_idx = 0;
    hist_cur = -1;
    hist_num = 0;

    for (i = 0; i < HIST_MAX; i++)
    {
        hist_list[i] = hist_lines[i];
        hist_list[i][0] = '\0';
    }
}

static void cread_add_to_hist(s8 *line)
{
    strcpy(hist_list[hist_add_idx], line);

    if (++hist_add_idx >= HIST_MAX)
        hist_add_idx = 0;

    if (hist_add_idx > hist_max)
        hist_max = hist_add_idx;

    hist_num++;
}

static s8 *hist_prev(void)
{
    s8 *ret;
    s32 old_cur;

    if (hist_cur < 0)
        return NULL;

    old_cur = hist_cur;
    if (--hist_cur < 0)
        hist_cur = hist_max;

    if (hist_cur == hist_add_idx)
    {
        hist_cur = old_cur;
        ret = NULL;
    }
    else
        ret = hist_list[hist_cur];

    return (ret);
}

static s8 *hist_next(void)
{
    s8 *ret;

    if (hist_cur < 0)
        return NULL;

    if (hist_cur == hist_add_idx)
        return NULL;

    if (++hist_cur > hist_max)
        hist_cur = 0;

    if (hist_cur == hist_add_idx)
    {
        ret = "";
    }
    else
        ret = hist_list[hist_cur];

    return (ret);
}

#ifndef CONFIG_CMDLINE_EDITING
static void cread_print_hist_list(void)
{
    s32 i;
    u32 n;

    n = hist_num - hist_max;

    i = hist_add_idx + 1;
    while (1)
    {
        if (i > hist_max)
            i = 0;
        if (i == hist_add_idx)
            break;
        shellprintf("%s\n", hist_list[i]);
        n++;
        i++;
    }
}
#endif /* CONFIG_CMDLINE_EDITING */




/****************************************************************************/

static s8 *delete_char (s8 *buffer, s8 *p, s32 *colp, s32 *np, s32 plen)
{
    s8 *s;

    if (*np == 0)
    {
        return (p);
    }

    if (*(--p) == '\t')  			/* will retype the whole line	*/
    {
        while (*colp > plen)
        {
            shellputs (erase_seq);
            (*colp)--;
        }
        for (s = buffer; s < p; ++s)
        {
            if (*s == '\t')
            {
                shellputs (tab_seq + ((*colp) & 07));
                *colp += 8 - ((*colp) & 07);
            }
            else
            {
                ++(*colp);
                shellputc (*s);
            }
        }
    }
    else
    {
        shellputs (erase_seq);
        (*colp)--;
    }
    (*np)--;
    return (p);
}

/****************************************************************************/
/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
s32 readline (const s8 *const prompt)
{
    /*
     * If console_buffer isn't 0-length the user will be prompted to modify
     * it instead of entering it from scratch as desired.
     */
    console_buffer[0] = '\0';

    return readline_into_buffer(prompt, console_buffer);
}

/****************************************************************************/

s32 readline_into_buffer (const s8 *const prompt, s8 *buffer)
{
    s8 *p = buffer;
#ifdef CONFIG_CMDLINE_EDITING
    u32 len = MAX_CMDBUF_SIZE;
    s32 rc;
    static s32 initted = 0;

    /*
     * History uses a global array which is not
     * writable until after relocation to RAM.
     * Revert to non-history version if still
     * running from flash.
     */
    /*if (gd->flags & GD_FLG_RELOC)*/
    if(1)
    {
        if (!initted)
        {
            hist_init();
            initted = 1;
        }

        if (prompt)
            shellputs (prompt);

        rc = cread_line(prompt, p, &len);
        return rc < 0 ? rc : len;

    }
    else
    {
#endif	/* CONFIG_CMDLINE_EDITING */


        s8 *p_buf = p;
        s32	n = 0;				/* buffer index		*/
        s32	plen = 0;			/* prompt length	*/
        s32	col;				/* output column cnt	*/
        s8	c;
        s32   data;

        /* print prompt */
        if (prompt)
        {
            plen = strlen (prompt);
            shellputs (prompt);
        }
        col = plen;

        for (;;)
        {

            //		 c = shellgetc();
            //           yexin add for support timeout
            data = shellgetc();
            // check timeout
            if(0xFFFF == data)
                return (-2);
            else
                c = data & 0xff;
            /*
             * Special character handling
             */
            switch (c)
            {
            case '\r':				/* Enter		*/
            case '\n':
                *p = '\0';
                shellputs ("\r\n");
                return (p - p_buf);

            case '\0':				/* nul			*/
                continue;

            case 0x03:				/* ^C - break		*/
                p_buf[0] = '\0';	/* discard input */
                return (-1);

            case 0x15:				/* ^U - erase line	*/
                while (col > plen)
                {
                    shellputs (erase_seq);
                    --col;
                }
                p = p_buf;
                n = 0;
                continue;

            case 0x17:				/* ^W - erase word	*/
                p = delete_char(p_buf, p, &col, &n, plen);
                while ((n > 0) && (*p != ' '))
                {
                    p = delete_char(p_buf, p, &col, &n, plen);
                }
                continue;

            case 0x08:				/* ^H  - backspace	*/
            case 0x7F:				/* DEL - backspace	*/
                p = delete_char(p_buf, p, &col, &n, plen);
                continue;

            default:
                /*
                 * Must be a normal character then
                 */
                if (n < CONFIG_SYS_CBSIZE - 2)
                {
                    if (c == '\t')  	/* expand TABs		*/
                    {
#ifdef CONFIG_AUTO_COMPLETE
                        /* if auto completion triggered just continue */
                        *p = '\0';
                        if (cmd_auto_complete(prompt, console_buffer, &n, &col))
                        {
                            p = p_buf + n;	/* reset */
                            continue;
                        }
#endif
                        shellputs (tab_seq + (col & 07));
                        col += 8 - (col & 07);
                    }
                    else
                    {
                        ++col;		/* echo input		*/
                        shellputc (c);
                    }
                    *p++ = c;
                    ++n;
                }
                else  			/* Buffer full		*/
                {
                    shellputc ('\a');
                }
            }
        }
#ifdef CONFIG_CMDLINE_EDITING
    }
#endif
}

/****************************************************************************/
s32 parse_line (s8 *line, s8 *argv[])
{
    s32 nargs = 0;


    while (nargs < CONFIG_SYS_MAXARGS)
    {

        /* skip any white space */
        while ((*line == ' ') || (*line == '\t'))
        {
            ++line;
        }

        if (*line == '\0')  	/* end of line, no more args	*/
        {
            argv[nargs] = NULL;

            return (nargs);
        }

        argv[nargs++] = line;	/* begin of argument string	*/

        /* find end of string */
        while (*line && (*line != ' ') && (*line != '\t'))
        {
            ++line;
        }

        if (*line == '\0')  	/* end of line, no more args	*/
        {
            argv[nargs] = NULL;

            return (nargs);
        }

        *line++ = '\0';		/* terminate current arg	 */
    }

    shellprintf ("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);


    return (nargs);
}

/****************************************************************************/
#pragma DATA_SECTION   (envname,"shell_lib");
far s8 envname[CONFIG_SYS_CBSIZE];

static void process_macros (const s8 *input, s8 *output)
{
    s8 c, prev;
    const s8 *varname_start = NULL;
    s32 inputcnt = strlen (input);
    s32 outputcnt = CONFIG_SYS_CBSIZE;
    s32 state = 0;		/* 0 = waiting for '$'  */

    /* 1 = waiting for '(' or '{' */
    /* 2 = waiting for ')' or '}' */
    /* 3 = waiting for '''  */
#ifdef DEBUG_PARSER
    s8 *output_start = output;

    shellprintf ("[PROCESS_MACROS] INPUT len %d: \"%s\"\n", strlen (input),
                 input);
#endif

    prev = '\0';		/* previous character   */

    while (inputcnt && outputcnt)
    {
        c = *input++;
        inputcnt--;

        if (state != 3)
        {
            /* remove one level of escape characters */
            if ((c == '\\') && (prev != '\\'))
            {
                if (inputcnt-- == 0)
                    break;
                prev = c;
                c = *input++;
            }
        }

        switch (state)
        {
        case 0:	/* Waiting for (unescaped) $    */
            if ((c == '\'') && (prev != '\\'))
            {
                state = 3;
                break;
            }
            if ((c == '$') && (prev != '\\'))
            {
                state++;
            }
            else
            {
                *(output++) = c;
                outputcnt--;
            }
            break;
        case 1:	/* Waiting for (        */
            if (c == '(' || c == '{')
            {
                state++;
                varname_start = input;
            }
            else
            {
                state = 0;
                *(output++) = '$';
                outputcnt--;

                if (outputcnt)
                {
                    *(output++) = c;
                    outputcnt--;
                }
            }
            break;
        case 2:	/* Waiting for )        */
            if (c == ')' || c == '}')
            {
                s32 i;
		   //yexin 20180630 ,remove stack size
               // s8 envname[CONFIG_SYS_CBSIZE], *envval;
                s8  *envval;
                s32 envcnt = input - varname_start - 1;	/* Varname # of chars */

                /* Get the varname */
                for (i = 0; i < envcnt; i++)
                {
                    envname[i] = varname_start[i];
                }
                envname[i] = 0;

                /* Get its value */
                //envval = getenv (envname);

                /* Copy into the line if it exists */
                if (envval != NULL)
                    while ((*envval) && outputcnt)
                    {
                        *(output++) = *(envval++);
                        outputcnt--;
                    }
                /* Look for another '$' */
                state = 0;
            }
            break;
        case 3:	/* Waiting for '        */
            if ((c == '\'') && (prev != '\\'))
            {
                state = 0;
            }
            else
            {
                *(output++) = c;
                outputcnt--;
            }
            break;
        }
        prev = c;
    }

    if (outputcnt)
        *output = 0;
    else
        *(output - 1) = 0;

#ifdef DEBUG_PARSER
    shellprintf ("[PROCESS_MACROS] OUTPUT len %d: \"%s\"\n",
                 strlen (output_start), output_start);
#endif
}

/****************************************************************************
 check input s8
 ***************************************************************************/
s32 tstc(void)
{
    return serial_tstc();

}
/****************************************************************************/
static s32 ctrlc_disabled = 0;	/* see disable_ctrl() */
static s32 ctrlc_was_pressed = 0;
s32 ctrlc(void)
{
    if (!ctrlc_disabled)
    {
        if (tstc())
        {
            switch (shellgetc())
            {
            case 0x03:		/* ^C - Control C */
                ctrlc_was_pressed = 1;
                return 1;
            default:
                break;
            }
        }
    }
    return 0;
}

/****************************************************************************/

void clear_ctrlc(void)
{
    ctrlc_was_pressed = 0;
}

/****************************************************************************/
s32 had_ctrlc (void)
{
    return ctrlc_was_pressed;
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CONFIG_SYS_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */
 
#pragma DATA_SECTION   (cmdbuf,"shell_lib");
 far s8 cmdbuf[CONFIG_SYS_CBSIZE];	/* working copy of cmd		*/
#pragma DATA_SECTION   (finaltoken,"shell_lib");
 far s8 finaltoken[CONFIG_SYS_CBSIZE];
s32 run_command (const s8 *cmd, s32 flag)
{
    cmd_tbl_t *cmdtp;
// yexin 20180630 remove stack size
//    s8 cmdbuf[CONFIG_SYS_CBSIZE];	/* working copy of cmd		*/
    s8 *token;			/* start of token in cmdbuf	*/
    s8 *sep;			/* end of token (separator) in cmdbuf */
// yexin 20180630 remove stack size	
 //   s8 finaltoken[CONFIG_SYS_CBSIZE];
    s8 *str = cmdbuf;
    s8 *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
    s32 argc, inquotes;
    s32 repeatable = 1;
    s32 rc = 0;

#ifdef DEBUG_PARSER
    shellprintf ("[RUN_COMMAND] cmd[%p]=\"", cmd);
    shellputs (cmd ? cmd : "NULL");	/* use puts - string may be loooong */
    shellputs ("\"\n");
#endif

    clear_ctrlc();		/* forget any previous Control C */

    if (!cmd || !*cmd)
    {
        return -1;	/* empty command */
    }

    if (strlen(cmd) >= CONFIG_SYS_CBSIZE)
    {
        shellputs ("## Command too long!\n");
        return -1;
    }

    strcpy (cmdbuf, cmd);

    /* Process separators and check for invalid
     * repeatable commands
     */

#ifdef DEBUG_PARSER
    shellprintf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
    while (*str)
    {

        /*
         * Find separator, or string end
         * Allow simple escape of ';' by writing "\;"
         */
        for (inquotes = 0, sep = str; *sep; sep++)
        {
            if ((*sep == '\'') &&
                    (*(sep - 1) != '\\'))
                inquotes = !inquotes;

            if (!inquotes &&
                    (*sep == ';') &&	/* separator		*/
                    ( sep != str) &&	/* past string start	*/
                    (*(sep - 1) != '\\'))	/* and NOT escaped	*/
                break;
        }

        /*
         * Limit the token to data between separators
         */
        token = str;
        if (*sep)
        {
            str = sep + 1;	/* start of command for next pass */
            *sep = '\0';
        }
        else
            str = sep;	/* no more commands for next pass */
#ifdef DEBUG_PARSER
        shellprintf ("token: \"%s\"\n", token);
#endif

        /* find macros in this token and replace them */
        //process_macros (token, finaltoken);

        /* Extract arguments */
        //		if ((argc = parse_line (finaltoken, argv)) == 0) {
        if ((argc = parse_line (token, argv)) == 0)
        {

            rc = -1;	/* no command at all */
            continue;
        }
        /* Look up command in command table */
        if ((cmdtp = find_cmd(argv[0])) == NULL)
        {
            shellprintf ("Unknown command '%s' - try 'help'\n", argv[0]);
            rc = -1;	/* give up after bad command */
            continue;
        }

        /* found - check max args */
        if (argc > cmdtp->maxargs)
        {
            cmd_usage(cmdtp);
            rc = -1;
            continue;
        }

#if defined(CONFIG_CMD_BOOTD)
        /* avoid "bootd" recursion */
        if (cmdtp->cmd == do_bootd)
        {
#ifdef DEBUG_PARSER
            shellprintf ("[%s]\n", finaltoken);
#endif
            if (flag & CMD_FLAG_BOOTD)
            {
                shellputs ("'bootd' recursion detected\n");
                rc = -1;
                continue;
            }
            else
            {
                flag |= CMD_FLAG_BOOTD;
            }
        }
#endif

        /* OK - call function to do the command */
        if ((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0)
        {
            rc = -1;
        }

        repeatable &= cmdtp->repeatable;

        /* Did the user stop this? */
        if (had_ctrlc ())
            return -1;	/* if stopped then not repeatable */
    }

    return rc ? rc : repeatable;
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CONFIG_SYS_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 *  this command is only used for netshell
 */

s32 run_netshell_command (const s8 *cmd, s32 flag)
{
#if 0
    cmd_tbl_t *cmdtp;
    s8 cmdbuf[CONFIG_SYS_CBSIZE];	/* working copy of cmd		*/
    s8 *token;			/* start of token in cmdbuf	*/
    s8 *sep;			/* end of token (separator) in cmdbuf */
    s8 finaltoken[CONFIG_SYS_CBSIZE];
    s8 *str = cmdbuf;
    s8 *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
    s32 argc, inquotes;
    s32 repeatable = 1;
    s32 rc = 0;

#ifdef DEBUG_PARSER
    shellprintf ("[RUN_COMMAND] cmd[%p]=\"", cmd);;
#endif

    clear_ctrlc();		/* forget any previous Control C */

    if (!cmd || !*cmd)
    {
        return -1;	/* empty command */
    }

    if (strlen(cmd) >= CONFIG_SYS_CBSIZE)
    {
        shellprintf ("## Command too long!\n");
        return -1;
    }

    strcpy (cmdbuf, cmd);

    /* Process separators and check for invalid
     * repeatable commands
     */

#ifdef DEBUG_PARSER
    shellprintf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
#ifdef    BIOSSHELL

    extern  void ti_sysbios_knl_Task_sleep__E(u32 nticks);
    //delay 100ms
    //    ti_sysbios_knl_Task_sleep__E(SHELL_DELAY);
#endif
    while (*str)
    {

        /*
         * Find separator, or string end
         * Allow simple escape of ';' by writing "\;"
         */
        for (inquotes = 0, sep = str; *sep; sep++)
        {
            if ((*sep == '\'') &&
                    (*(sep - 1) != '\\'))
                inquotes = !inquotes;

            if (!inquotes &&
                    (*sep == ';') &&	/* separator		*/
                    ( sep != str) &&	/* past string start	*/
                    (*(sep - 1) != '\\'))	/* and NOT escaped	*/
                break;
        }

        /*
         * Limit the token to data between separators
         */
        token = str;
        if (*sep)
        {
            str = sep + 1;	/* start of command for next pass */
            *sep = '\0';
        }
        else
            str = sep;	/* no more commands for next pass */
#ifdef DEBUG_PARSER
        shellprintf ("token: \"%s\"\n", token);
#endif

        /* find macros in this token and replace them */
        //process_macros (token, finaltoken);

        /* Extract arguments */
        //		if ((argc = parse_line (finaltoken, argv)) == 0) {
        if ((argc = parse_line (token, argv)) == 0)
        {

            rc = -1;	/* no command at all */
            continue;
        }
        /* Look up command in command table */
        if ((cmdtp = find_netshell_cmd(argv[0])) == NULL)
        {
            shellprintf ("Unknown command '%s' - try 'help'\n", argv[0]);
            rc = -1;	/* give up after bad command */
            continue;
        }

        /* found - check max args */
        if (argc > cmdtp->maxargs)
        {
            cmd_usage(cmdtp);
            rc = -1;
            continue;
        }

#if defined(CONFIG_CMD_BOOTD)
        /* avoid "bootd" recursion */
        if (cmdtp->cmd == do_bootd)
        {
#ifdef DEBUG_PARSER
            shellprintf ("[%s]\n", finaltoken);
#endif
            if (flag & CMD_FLAG_BOOTD)
            {
                shellprintf ("'bootd' recursion detected\n");
                rc = -1;
                continue;
            }
            else
            {
                flag |= CMD_FLAG_BOOTD;
            }
        }
#endif

        /* OK - call function to do the command */
        if ((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0)
        {
            rc = -1;
        }

        repeatable &= cmdtp->repeatable;

        /* Did the user stop this? */
        /*if (had_ctrlc ())
        	return -1;	*//* if stopped then not repeatable */
    }

    return rc ? rc : repeatable;
#endif

}

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		shellprintf("%*s", (s32)(eol_num - num), ""); \
		do {					\
			getcmd_putch(CTL_BACKSPACE);	\
		} while (--eol_num > num);		\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}


void cread_add_char(s8 ichar, s32 insert, u32 *num,
                    u32 *eol_num, s8 *buf, u32 len)
{
    u32 wlen;

    /* room ??? */
    if (insert || *num == *eol_num)
    {
        if (*eol_num > len - 1)
        {
            getcmd_cbeep();
            return;
        }
        (*eol_num)++;
    }

    if (insert)
    {
        wlen = *eol_num - *num;
        if (wlen > 1)
        {
            memmove(&buf[*num + 1], &buf[*num], wlen - 1);
        }

        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
        while (--wlen)
        {
            getcmd_putch(CTL_BACKSPACE);
        }
    }
    else
    {
        /* echo the character */
        wlen = 1;
        buf[*num] = ichar;
        putnstr(buf + *num, wlen);
        (*num)++;
    }
}

void cread_add_str(s8 *str, s32 strsize, s32 insert, u32 *num,
                   u32 *eol_num, s8 *buf, u32 len)
{
    while (strsize--)
    {
        cread_add_char(*str, insert, num, eol_num, buf, len);
        str++;
    }
}

/*
	-1 for ctlr +c
	-2 for  timeout
*/
s32 cread_line(const s8 *const prompt, s8 *buf, u32 *len)
{
    u32 num = 0;
    u32 eol_num = 0;
    u32 wlen;
    s8 ichar;
    s32 insert = 1;
    s32 esc_len = 0;
    s8 esc_save[8];
    s32 init_len = strlen(buf);
    s32 data;

    if (init_len)
        cread_add_str(buf, init_len, 1, &num, &eol_num, buf, *len);

    while (1)
    {
#ifdef CONFIG_BOOT_RETRY_TIME
        while (!tstc())  	/* while no incoming data */
        {
            if (retry_time >= 0 && get_ticks() > endtime)
                return (-2);	/* timed out */
            WATCHDOG_RESET();
        }
#endif
        //add by yexin for input timeout
        data = getcmd_getch();
        //ichar = getcmd_getch();
        if(0xffff == data)
            return (-2);
        else
            ichar = data & 0xff;

        if ((ichar == '\n') || (ichar == '\r'))
        {
            shellputc('\n');
            break;
        }

        /*
         * handle standard linux xterm esc sequences for arrow key, etc.
         */
        if (esc_len != 0)
        {
            if (esc_len == 1)
            {
                if (ichar == '[')
                {
                    esc_save[esc_len] = ichar;
                    esc_len = 2;
                }
                else
                {
                    cread_add_str(esc_save, esc_len, insert,
                                  &num, &eol_num, buf, *len);
                    esc_len = 0;
                }
                continue;
            }

            switch (ichar)
            {

            case 'D':	/* <- key */
                ichar = CTL_CH('b');
                esc_len = 0;
                break;
            case 'C':	/* -> key */
                ichar = CTL_CH('f');
                esc_len = 0;
                break;	/* pass off to ^F handler */
            case 'H':	/* Home key */
                ichar = CTL_CH('a');
                esc_len = 0;
                break;	/* pass off to ^A handler */
            case 'A':	/* up arrow */
                ichar = CTL_CH('p');
                esc_len = 0;
                break;	/* pass off to ^P handler */
            case 'B':	/* down arrow */
                ichar = CTL_CH('n');
                esc_len = 0;
                break;	/* pass off to ^N handler */
            default:
                esc_save[esc_len++] = ichar;
                cread_add_str(esc_save, esc_len, insert,
                              &num, &eol_num, buf, *len);
                esc_len = 0;
                continue;
            }
        }

        switch (ichar)
        {
        case 0x1b:
            if (esc_len == 0)
            {
                esc_save[esc_len] = ichar;
                esc_len = 1;
            }
            else
            {
                shellputs("impossible condition #876\n");
                esc_len = 0;
            }
            break;

        case CTL_CH('a'):
            BEGINNING_OF_LINE();
            break;
        case CTL_CH('c'):	/* ^C - break */
            *buf = '\0';	/* discard input */
            return (-1);
        case CTL_CH('f'):
            if (num < eol_num)
            {
                getcmd_putch(buf[num]);
                num++;
            }
            break;
        case CTL_CH('b'):
            if (num)
            {
                getcmd_putch(CTL_BACKSPACE);
                num--;
            }
            break;
        case CTL_CH('d'):
            if (num < eol_num)
            {
                wlen = eol_num - num - 1;
                if (wlen)
                {
                    memmove(&buf[num], &buf[num + 1], wlen);
                    putnstr(buf + num, wlen);
                }

                getcmd_putch(' ');
                do
                {
                    getcmd_putch(CTL_BACKSPACE);
                }
                while (wlen--);
                eol_num--;
            }
            break;
        case CTL_CH('k'):
            ERASE_TO_EOL();
            break;
        case CTL_CH('e'):
            REFRESH_TO_EOL();
            break;
        case CTL_CH('o'):
            insert = !insert;
            break;
        case CTL_CH('x'):
        case CTL_CH('u'):
            BEGINNING_OF_LINE();
            ERASE_TO_EOL();
            break;
        case DEL:
        case DEL7:
        case 8:
            if (num)
            {
                wlen = eol_num - num;
                num--;
                memmove(&buf[num], &buf[num + 1], wlen);
                getcmd_putch(CTL_BACKSPACE);
                putnstr(buf + num, wlen);
                getcmd_putch(' ');
                do
                {
                    getcmd_putch(CTL_BACKSPACE);
                }
                while (wlen--);
                eol_num--;
            }
            break;
        case CTL_CH('p'):
        case CTL_CH('n'):
        {
            s8 *hline;

            esc_len = 0;

            if (ichar == CTL_CH('p'))
                hline = hist_prev();
            else
                hline = hist_next();

            if (!hline)
            {
                getcmd_cbeep();
                continue;
            }

            /* nuke the current line */
            /* first, go home */
            BEGINNING_OF_LINE();

            /* erase to end of line */
            ERASE_TO_EOL();

            /* copy new line into place and display */
            strcpy(buf, hline);
            eol_num = strlen(buf);
            REFRESH_TO_EOL();
            continue;
        }
#ifdef CONFIG_AUTO_COMPLETE
        case '\t':
        {
            s32 num2, col;

            /* do not autocomplete when in the middle */
            if (num < eol_num)
            {
                getcmd_cbeep();
                break;
            }

            buf[num] = '\0';
            col = strlen(prompt) + eol_num;
            num2 = num;
            if (cmd_auto_complete(prompt, buf, &num2, &col))
            {
                col = num2 - num;
                num += col;
                eol_num += col;
            }
            break;
        }
#endif
        default:
            cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
            break;
        }
    }
    *len = eol_num;
    buf[eol_num] = '\0';	/* lose the newline */

    if (buf[0] && buf[0] != CREAD_HIST_CHAR)
        cread_add_to_hist(buf);
    hist_cur = hist_add_idx;

    return 0;
}


