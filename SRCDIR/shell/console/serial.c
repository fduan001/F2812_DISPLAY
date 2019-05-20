/*
 * serial.c
 *
 *  Created on: 2014-8-8
 *      Author: yexin
 */
#include <stdio.h>
#include "shellctype.h"




//#define HOST_PC
#ifdef HOST_PC
#define CPU_PUTC     putchar
#define CPU_GETC     getchar
#define CPU_GETTSC()
#else
#include "stdint.h"
//#include "c66x_fpga.h"
/*
#define CPU_PUTC     Shell_16C550_putc
#define CPU_GETC     Shell_16C550_getc
#define CPU_GETTSC()  Shell_16C550_tsc()
*/
//simulate for evm
extern void UartCharPut(u8 byteTx);
extern u8 UartGetc(void);
extern s16 SciaRx_Ready(void);

#define CPU_PUTC     UartCharPut
//#define CPU_GETC     c66xUartReadchar
//support timeout
#define CPU_GETC      UartGetc
#define CPU_GETTSC()  SciaRx_Ready()

#endif

/****************************************************************************/
s32 serial_getc()
{

    return (CPU_GETC());

}
/****************************************************************************/
void serial_putc(const s8 c)
{
    if(c == '\n')
    {
        //		serial_putc('\r');
        //		serial_putc('\n');
        CPU_PUTC('\r');
        CPU_PUTC('\n');
    }
    else
        CPU_PUTC(c);

}
/****************************************************************************/
void serial_puts(const s8 *s)
{
    while(*s)
    {
        serial_putc(*s);
        s++;
    }
}

/****************************************************************************/
s32 serial_tstc (void)
{
#ifdef HOST
    return CPU_GETC();
#else
    return CPU_GETTSC() ;
#endif
}


