//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Gpio.c
//
// TITLE:	DSP28 General Purpose I/O Initialization & Support Functions.
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
// InitGpio:
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known state.
//
void InitGpio(void)
{


    EALLOW;
    /* GPIO A*/
    GpioMuxRegs.GPAMUX.all = GPIOA_PIN_MUX;
    GpioMuxRegs.GPADIR.all = GPIOA_DIR_CFG;
    GpioMuxRegs.GPAQUAL.all = GPIOA_QUAL_CFG;


    /*GPIO B*/
    GpioMuxRegs.GPBMUX.all = GPIOB_PIN_MUX;
    GpioMuxRegs.GPBDIR.all = GPIOB_DIR_CFG;
    GpioMuxRegs.GPBQUAL.all = GPIOB_QUAL_CFG;



    /*GPIO D*/
    GpioMuxRegs.GPDMUX.all = GPIOD_PIN_MUX;
    GpioMuxRegs.GPDDIR.all = GPIOD_DIR_CFG;
    GpioMuxRegs.GPDQUAL.all = GPIOD_QUAL_CFG;



    /*GPIO E*/
    GpioMuxRegs.GPEMUX.all = GPIOE_PIN_MUX;
    GpioMuxRegs.GPEDIR.all = GPIOE_DIR_CFG;
    GpioMuxRegs.GPEQUAL.all = GPIOE_QUAL_CFG;



    /*GPIO F*/
    GpioMuxRegs.GPFMUX.all = GPIOF_PIN_MUX;
    GpioMuxRegs.GPFDIR.all = GPIOF_DIR_CFG;


    /*GPIO G*/
    GpioMuxRegs.GPGMUX.all = GPIOG_PIN_MUX;
    GpioMuxRegs.GPGDIR.all = GPIOG_DIR_CFG;


    EDIS;
    GpioDataRegs.GPADAT.all = 0x0;
    GpioDataRegs.GPBDAT.all = 0x0;
    GpioDataRegs.GPDDAT.all = 0x0;
    GpioDataRegs.GPEDAT.all = 0x0;
    GpioDataRegs.GPFDAT.all = 0x0;
    GpioDataRegs.GPGDAT.all = 0x0;

}

void GpioPortWrite(UINT8 portid , UINT16 portdata)
{
    switch(portid)
    {
    case 'A':
        GpioDataRegs.GPADAT.all = portdata;
        break;
    case 'B':
        GpioDataRegs.GPBDAT.all = portdata;
        break;
    case 'D':
        GpioDataRegs.GPDDAT.all = portdata;
        break;

    case 'E':
        GpioDataRegs.GPEDAT.all = portdata;
        break;
    case 'F':
        GpioDataRegs.GPFDAT.all = portdata;
        break;
    case 'G':
        GpioDataRegs.GPGDAT.all = portdata;
        break;

    }
}

UINT16 GpioPortRead(UINT8 portid )
{
    unsigned short regvalue;
    switch(portid)
    {
    case 'A':
        regvalue = GpioDataRegs.GPADAT.all;
        break;
    case 'B':
        regvalue = GpioDataRegs.GPBDAT.all;
        break;
    case 'D':
        regvalue = GpioDataRegs.GPDDAT.all;
        break;

    case 'E':
        regvalue = GpioDataRegs.GPEDAT.all;
        break;
    case 'F':
        regvalue = GpioDataRegs.GPFDAT.all;
        break;
    case 'G':
        regvalue = GpioDataRegs.GPGDAT.all;
        break;

    }
    return regvalue;

}

void GpioPortSetBitHigh(UINT8 portid, UINT8 bitnum)
{
    UINT16 regvalue;
    regvalue = GpioPortRead(portid);
    regvalue = BOOT_SET_BITFIELD(regvalue, 1, bitnum, bitnum);
    GpioPortWrite(portid, regvalue);
}

void GpioPortSetBitLow(UINT8 portid, UINT8 bitnum)
{
    UINT16 regvalue;
    regvalue = GpioPortRead(portid);
    regvalue = BOOT_SET_BITFIELD(regvalue, 0, bitnum, bitnum);
    GpioPortWrite(portid, regvalue);
}

UINT8 GpioPortReadBit(UINT8 portid, UINT8 bitnum)
{
    UINT16 regvalue, bitvalue;
    regvalue = GpioPortRead(portid);
    bitvalue = BOOT_READ_BITFIELD(regvalue, bitnum, bitnum);
    return bitvalue;
}
void DisableWatchDog()
{
    GpioPortSetBitLow('A', WATCHDOG_WD_EN_GPIOA5);
}

void EnableWatchDog()
{
    GpioPortSetBitHigh('A', WATCHDOG_WD_EN_GPIOA5);

}

void KickWatchDog()
{
    UINT16 gpioval;
    gpioval = GpioPortReadBit('A', WATCHDOG_WDI_GPIOA0);
    if(0 == gpioval)
        GpioPortSetBitHigh('A', WATCHDOG_WDI_GPIOA0);
    else
        GpioPortSetBitLow('A', WATCHDOG_WDI_GPIOA0);
}

//===========================================================================
// No more.
//===========================================================================
