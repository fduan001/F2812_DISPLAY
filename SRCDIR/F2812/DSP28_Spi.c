//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Spi.c
//
// TITLE:	DSP28 SPI Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#include "DSP28_Device.h"

#include "boardcfg.h"
#include "F2812_datatype.h"

//---------------------------------------------------------------------------
// InitSPI:
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//


void InitSpi(void)
{
    UINT16 regval;

    // Initialize SPI-A:
    // Initialize SCI-A:
    EALLOW;
    regval = GpioMuxRegs.GPFMUX.all;
    regval |= SPI_PIN_MUX;
    GpioMuxRegs.GPFMUX.all = regval;
    EDIS;
    /* loopback   8 bit data */
    SpiaRegs.SPICCR.all = SPICCR_CFG;

    SpiaRegs.SPICTL.all = SPICTL_CFG;

    SpiaRegs.SPIBRR = SPIBRR_CFG;//128分频

    SpiaRegs.SPICCR.all = (0x80 | SPICCR_CFG);



}

UINT8 Spi_TxReady(void)
{
    UINT8 i;
    if(SpiaRegs.SPISTS.bit.BUFFULL_FLAG == 1)
    {
        i = 0;//发送缓冲器已满，有数据载入
    }
    else
    {
        i = 1;//数据已经移出，自动标志清除为标志为0
    }
    return(i);
}

UINT8 Spi_RxReady(void)
{
    UINT8 i;
    if(SpiaRegs.SPISTS.bit.INT_FLAG == 1)
    {
        i = 1;//缓冲器数据还没有读
    }
    else
    {
        i = 0;//缓冲器数据已经读了
    }
    return(i);
}

void SpiWriteData(UINT16 data)
{
    while(1 != Spi_TxReady());
    SpiaRegs.SPITXBUF = data;
}

UINT16 SpiReadData(void)
{
    while(1 != Spi_RxReady());
    return (unsigned short)(SpiaRegs.SPIRXBUF);
}




interrupt void SPIRXINTA_ISR(void);   // SPI
interrupt void SPITXINTA_ISR(void);    // SPI



//===========================================================================
// No more.
//===========================================================================
