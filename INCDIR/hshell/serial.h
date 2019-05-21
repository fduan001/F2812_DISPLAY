/*
 * serial.h
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include "F2812_datatype.h"

INT32 serial_getc();
void serial_putc(const INT8 c);
void serial_puts(const INT8 *s);
INT32 serial_tstc (void);


#endif /* SERIAL_H_ */
