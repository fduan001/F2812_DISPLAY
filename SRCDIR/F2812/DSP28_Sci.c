//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Sci.c
//
// TITLE:	DSP28 SCI Initialization & Support Functions.
//

//###########################################################################


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DSP28_Device.h"
#include "F2812_datatype.h"
#include "boardcfg.h"
#include "boarddrv.h"

#include "platform_os.h"



//---------------------------------------------------------------------------
// InitSPI:
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSci(void)
{


    UINT16 regval;
    EALLOW;
    regval = GpioMuxRegs.GPFMUX.all;
    regval |= SCI_PIN_MUX;
    GpioMuxRegs.GPFMUX.all = regval;

    regval = GpioMuxRegs.GPGMUX.all;
    regval |= SCI_PIN_MUX;
    GpioMuxRegs.GPGMUX.all = regval;
    EDIS;
    SciaRegs.SCICTL1.all = 0x0;
    /*   8 bit data */
    SciaRegs.SCICCR.all = SCICCR_CFG;

    SciaRegs.SCICTL1.all = SCICTL1_CFG;
    SciaRegs.SCICTL2.all = SCICTL2_REG;

    SciaRegs.SCIHBAUD = (SCI_BAUD >> 8) & 0xff;
    SciaRegs.SCILBAUD = (SCI_BAUD) & 0xFF;
    /* enable SCI*/
    SciaRegs.SCICTL1.all = (SCICTL1_CFG | 0x20);

    //SCI B 
    ScibRegs.SCIFFTX.all=0x8000;

     ScibRegs.SCICTL1.all = 0x0;
    /*   8 bit data */
    ScibRegs.SCICCR.all = SCICCR_CFG;

    ScibRegs.SCICTL1.all = SCICTL1_CFG;
    ScibRegs.SCICTL2.all = SCICTL2_REG;

    ScibRegs.SCIHBAUD = (SCI_BAUD >> 8) & 0xff;
    ScibRegs.SCILBAUD = (SCI_BAUD) & 0xFF;
    /* enable SCI*/
    ScibRegs.SCICTL1.all = (SCICTL1_CFG | 0x20);
	
}


/********************************************************************************
	name:	int SciaTx_Ready(void)
	input:	none
	output:	i	1:	ready
			0:	busy
*********************************************************************************/

INT16 SciaTx_Ready(void)
{
    UINT16 i;
    if(SciaRegs.SCICTL2.bit.TXRDY == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}


INT16 ScibTx_Ready(void)
{
    UINT16 i;
    if(ScibRegs.SCICTL2.bit.TXRDY == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}
/********************************************************************************
	name:	int SciaRx_Ready(void)
	input:	none
	output:	i	1:	new data
			0:	none
*********************************************************************************/

INT16 SciaRx_Ready(void)
{
    UINT8 i;
    if(SciaRegs.SCIRXST.bit.RXRDY == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}

INT16 ScibRx_Ready(void)
{
    UINT8 i;
    if(ScibRegs.SCIRXST.bit.RXRDY == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}
/******************************************************************************
 *
 * Function:		UartCharPut
 *
 * Description:		print the char through dsp  uart
 *
 * Parameters:		void
 *
 * Return Value: 	void
 *
 *****************************************************************************/


void UartCharPut(UINT8 byteTx)
{
#ifdef HOST_PC
	putchar(byteTx);
#else
    while(1 != SciaTx_Ready());
    SciaRegs.SCITXBUF = byteTx;
#endif
}

void UartCharPut_B(UINT8 byteTx)
{
#ifdef HOST_PC
	putchar(byteTx);
#else
    while(1 != ScibTx_Ready());
    ScibRegs.SCITXBUF = byteTx;
#endif
}

/**
 * \brief   This function reads a byte entered on the serial console.
 *
 * \return  Returns the entered byte typecasted as an unsigned character.
 */

UINT8 UartGetc(void)
{
#ifdef HOST_PC
	return(getchar());
#else
    while(1 != SciaRx_Ready());
    return (unsigned char)(SciaRegs.SCIRXBUF.all);
#endif
}


UINT8 UartGetc_B(void)
{
#ifdef HOST_PC
	return(getchar());
#else
    while(1 != ScibRx_Ready());
    return (unsigned char)(ScibRegs.SCIRXBUF.all);
#endif
}

/******************************************************************************
 *
 * Function:		UartPutbuffer
 *
 * Description:		print the buffer through dsp  uart
 *
 * Parameters:		void
 *
 * Return Value: 	void
 *
 *****************************************************************************/


void UartPutbuffer(UINT8 *s)
{

    while('\0' != (*s))
    {
        if(*s == '\n')
        {
            UartCharPut(0x0D);
            UartCharPut(0x0A);
        }

        else
            UartCharPut(*s);
        s++;

    }


}


void UartPutbuffer_B(UINT8 *s)
{

    while('\0' != (*s))
    {
        if(*s == '\n')
        {
            UartCharPut_B(0x0D);
            UartCharPut_B(0x0A);
        }

        else
            UartCharPut_B(*s);
        s++;

    }


}


#pragma DATA_SECTION   (g_uartprintbuffer,"shell_lib");
far INT8	 g_uartprintbuffer[60];


/* platform printf function through uart*/
#if 1
void UartPrintf(const char *fmt, ...)
{

   Osal_EnterGlobalCriticalSection();
    va_list 	arg_ptr;
    UINT8 	    length;

    va_start( arg_ptr, fmt );
    length = vsprintf( (char *)g_uartprintbuffer, (char *) fmt, arg_ptr );
    va_end( arg_ptr );	
    UartPutbuffer((unsigned char*)g_uartprintbuffer);
    Osal_ExitGlobalCriticalSection();
}
#endif

UINT8  UartCharGetTimeout(UINT32 timeout, int *result)
{
    if( result ) {
        *result = 0;
    }

    UINT32 timebase = GetTimer2Cnt();
    while((GetTimer2Cnt() - timebase) <= timeout)
    {
        if(1 == SciaRx_Ready())
        {
            return (unsigned char)(SciaRegs.SCIRXBUF.all);

        }
    }

    if( result ) {
        *result = -1;
    }
    return -1;
}

UINT8 UartCharGetTimeout_B(UINT32 timeout, int *result)
{
    if( result ) {
        *result = 0;
    }

    UINT32 timebase = GetTimer2Cnt();
    while((GetTimer2Cnt() - timebase) <= timeout)
    {
        if(1 == SciaRx_Ready())
        {
            return (unsigned char)(SciaRegs.SCIRXBUF.all);

        }
    }

    if( result ) {
        *result = -1;
    }
    return 0xFF;
}

int UartWrite_B(UINT8 *buffer, UINT16 length) {
    int rt = 0;
    UINT16 i = 0;
    for( i = 0; i < length; ++i ) {
        UartCharPut_B(buffer[i]);
    }
    return length;
}

#define GETCHAR_TIMEOUT      5000 /* 5ms */

int UartRead_B(UINT8* buffer, UINT16 length) {
    int rt = -1;
    int result = -1;
    int i = 0;
    UINT8 val = 0;
    for( i = 0; i < length; ++i ) {
        val = UartCharGetTimeout_B(GETCHAR_TIMEOUT, &result);
        if( result != 0 ) {
            break;
        }
        buffer[i] = val;
    }

    return i;
}

//===========================================================================
// No more.
//===========================================================================

