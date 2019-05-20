/*
 * serial.h
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include "shellctype.h"
s32 serial_getc();
void serial_putc(const s8 c);
void serial_puts(const s8 *s);
s32 serial_tstc (void);


#endif /* SERIAL_H_ */
