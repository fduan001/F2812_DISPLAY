/*
 * shellconsole.h
 *
 *  Created on: 2014-8-8
 *      Author:
 */

#ifndef SHELLCONSOLE_H_
#define SHELLCONSOLE_H_
#include "F2812_datatype.h"
#include <stdarg.h>
extern INT8 console_buffer[];
void shellputs(const INT8 *s);
void shellputc(const INT8 c);
INT32 shellgetc(void);
INT32 shellprintf(const INT8 *fmt, ...);
INT32 tstc(void);
INT32 ctrlc(void);
UINT8 set_shellprompt (char* p);
INT32 parse_line (INT8 *line, INT8 *argv[]);
INT32 readline (const INT8 *const prompt);
INT32 readline_into_buffer (const INT8 *const prompt, INT8 *buffer);
void clear_ctrlc(void);
INT32 had_ctrlc (void);
INT32 run_command (const INT8 *cmd, INT32 flag);

int shellvsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

void shellsetudpprintmode(UINT8 enable);
UINT8  shellgetudprintmode();
void shell_log_msg(const char * file, const int line, UINT32 mask, const char* format, ...);

#define PRINTF  shellprintf

#define DL_MASK_LEVEL_DEFAULT   0
#define DL_MASK_LEVEL_LOW       1
#define DL_MASK_LEVEL_2         2
#define DL_MASK_LEVEL_3         3
#define DL_MASK_LEVEL_4         4
#define DL_MASK_LEVEL_HIGH      5

#define LOG_DAEMON_MASK         0x80000000
#define LOG_CMDLINE_MASK        0x40000000
#define LOG_ISR_MASK            0x20000000

#define LOG_DEBUG    7
#define LOG_INFO     6
#define LOG_NOTICE   5
#define LOG_WARN     4
#define LOG_ERR      3
#define LOG_CRIT     2
#define LOG_ALERT    1
#define LOG_EMERG    0

#define dl_log_printf(level, ...) shell_log_msg(__FILE__, __LINE__, level, __VA_ARGS__)

#undef dlog
#define dlog(args...)   dl_log_printf(LOG_DEBUG, args)

#undef elog
#define elog(args...)   dl_log_printf(LOG_ERR, args)

#undef ilog
#define ilog(args...)   dl_log_printf(LOG_INFO, args)

#undef wlog
#define wlog(args...)   dl_log_printf(LOG_WARN, args)

#undef nlog
#define nlog(args...)    dl_log_printf(LOG_NOTICE, args)

#endif /* CONSOLE_H_ */
