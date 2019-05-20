/*
 * config.h
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "shellctype.h"

#define __BYTE_ORDER  __LITTLE_ENDIAN

#define  CONFIG_SYS_CBSIZE  256
#define  CONFIG_SYS_PBSIZE  128
#define  CONFIG_SYS_MAXARGS 16

#define  CONFIG_SYS_CMDSIZE  30
#define  CONFIG_SYS_LONGHELP

#define  CONFIG_SYS_PROMPT   "MIB->"


#define CONFIG_SYS_MEMTEST_START   0x80000000
#define CONFIG_SYS_MEMTEST_END     0x81000000

#define CONFIG_SYS_MAX_FLASH_SECT   127
#define CONFIG_SYS_MAX_FLASH_BANKS  1
#define CONFIG_SYS_FLASH_BASE       0x41BC30
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_HZ               1000
#define CONFIG_CMDLINE_EDITING
#define SHELL_DELAY      100
#define DEBUG
#define CONFIG_ENV_SIZE  0x1000
#define CONFIG_SYS_ALT_MEMTEST
/***********************************************
  only support serial shell ,not netshell
************************************************/
//#define SERIALSHELL

/***********************************************
  the shell support bios
************************************************/
#define BIOSSHELL

#ifdef  DEBUG
#define debug         shellprintf
#else
#define debug
#endif

#endif /* CONFIG_H_ */
