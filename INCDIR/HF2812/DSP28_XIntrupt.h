//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_XIntrupt.h
//
// TITLE:	DSP28 Device External Interrupt Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_XINTRUPT_H
#define DSP28_XINTRUPT_H
#include "F2812_datatype.h"
//---------------------------------------------------------------------------

struct XINTCR_BITS
{
    UINT16   ENABLE: 1;   // 0      enable/disable
    UINT16   rsvd1: 1;    // 1      reserved
    UINT16   POLARITY: 1; // 2      pos/neg triggered
    UINT16   rsvd2: 12;   // 15:3   reserved
};

union XINTCR_REG
{
    UINT16               all;
    struct XINTCR_BITS   bit;
};

struct XNMICR_BITS
{
    UINT16   ENABLE: 1;   // 0      enable/disable
    UINT16   SELECT: 1;   // 1      Timer 1 or XNMI connected to INT13
    UINT16   POLARITY: 1; // 2      pos/neg triggered
    UINT16   rsvd2: 12;   // 15:3   reserved
};

union XNMICR_REG
{
    UINT16               all;
    struct XNMICR_BITS   bit;
};




//---------------------------------------------------------------------------
// External Interrupt Register File:
//
struct XINTRUPT_REGS
{
    union XINTCR_REG XINT1CR;
    union XINTCR_REG XINT2CR;
    UINT16 rsvd1[5];
    union XNMICR_REG XNMICR;
    UINT16 XINT1CTR;
    UINT16 XINT2CTR;
    UINT16 rsvd[5];
    UINT16 XNMICTR;
};

//---------------------------------------------------------------------------
// External Interrupt References & Function Declarations:
//
extern volatile struct XINTRUPT_REGS XIntruptRegs;

#endif  // end of DSP28_XINTF_H definition

//===========================================================================
// No more.
//===========================================================================
