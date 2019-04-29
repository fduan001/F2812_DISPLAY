//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_SysCtrl.c
//
// TITLE:	DSP28 Device System Control Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 24 May 2002 | L.H. | Added initialization of RAM control registers
//      |             |      | for TMX samples.
//###########################################################################


#include "DSP28_Device.h"
#include "boardcfg.h"
#include "F2812_datatype.h"
//---------------------------------------------------------------------------
// InitSysCtrl:
//---------------------------------------------------------------------------
// This function initializes the System Control registers to a known state.
//
void InitSysCtrl(void)
{
    UINT16 i;

    EALLOW;

    // On TMX samples, to get the best performance of on chip RAM blocks M0/M1/L0/L1/H0 internal
    // control registers bit have to be enabled. The bits are in Device emulation registers.
#if 0
    DevEmuRegs.M0RAMDFT = 0x0300;
    DevEmuRegs.M1RAMDFT = 0x0300;
    DevEmuRegs.L0RAMDFT = 0x0300;
    DevEmuRegs.L1RAMDFT = 0x0300;
    DevEmuRegs.H0RAMDFT = 0x0300;
#endif

    // Disable watchdog module
    SysCtrlRegs.WDCR = 0x0068;

    // Initalize PLL ,30M*5=150M
    SysCtrlRegs.PLLCR = 0xA;
    // Wait for PLL to lock
    for(i = 0; i < 65000; i++) {};

    // HISPCP/LOSPCP prescale register settings, normally it will be set to default values
    // LSPCLK=150/4=37.5M  HSPCLK=150/2=75MHZ
    SysCtrlRegs.HISPCP.all = 0x0001;
    SysCtrlRegs.LOSPCP.all = 0x0002;
    // Peripheral clock enables set for the selected peripherals.
    SysCtrlRegs.PCLKCR.bit.EVAENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.EVBENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.SCIENCLKA = 1;
    SysCtrlRegs.PCLKCR.bit.SCIENCLKB = 1;
    SysCtrlRegs.PCLKCR.bit.SPIENCLK = 1;

    EDIS;

}


UINT16 UnlockCsm(void)
{

    UINT16 regvalue;

    EALLOW;
    *KEY0 = PRG_KEY0;
    *KEY1 = PRG_KEY1;
    *KEY2 = PRG_KEY2;
    *KEY3 = PRG_KEY3;
    *KEY4 = PRG_KEY4;
    *KEY5 = PRG_KEY5;
    *KEY6 = PRG_KEY6;
    *KEY7 = PRG_KEY7;
    EDIS;

    regvalue = *PWL0;
    regvalue = *PWL1;
    regvalue = *PWL2;
    regvalue = *PWL3;
    regvalue = *PWL4;
    regvalue = *PWL5;
    regvalue = *PWL6;
    regvalue = *PWL7;

    // If the CSM unlocked, return succes, otherwise return
    // failure.
    if ( (*CSMSCR & 0x0001) == 0) return 0;
    else return 1;

}

//---------------------------------------------------------------------------
// KickDog:
//---------------------------------------------------------------------------
// This function resets the watchdog timer.
// Enable this function for using KickDog in the application
/*
void KickDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}
*/

//===========================================================================
// No more.
//===========================================================================
