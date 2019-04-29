//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Xintf.h
//
// TITLE:	DSP28 Device External Interface Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_XINTF_H
#define DSP28_XINTF_H
#include "F2812_datatype.h"

// XINTF timing register bit definitions:
struct XTIMING_BITS    // bits  description
{
    UINT16 XWRTRAIL: 2;   // 1:0   Write access trail timing
    UINT16 XWRACTIVE: 3;  // 4:2   Write access active timing
    UINT16 XWRLEAD: 2;    // 6:5   Write access lead timing
    UINT16 XRDTRAIL: 2;   // 8:7   Read access trail timing
    UINT16 XRDACTIVE: 3;  // 11:9  Read access active timing
    UINT16 XRDLEAD: 2;    // 13:12 Read access lead timing
    UINT16 USEREADY: 1;   // 14    Extend access using HW waitstates
    UINT16 READYMODE: 1;  // 15    Ready mode
    UINT16 XSIZE: 2;      // 17:16 XINTF bus width - must be written as 11b
    UINT16 rsvd1: 4;      // 21:18 reserved
    UINT16 X2TIMING: 1;   // 22    Double lead/active/trail timing
    UINT16 rsvd3: 9;      // 31:23 reserved
};

union XTIMING_REG
{
    UINT32                 all;
    struct XTIMING_BITS  bit;
};

// XINTF control register bit definitions:
struct XINTCNF2_BITS    // bits  description
{
    UINT16 WRBUFF: 2;      // 1:0   Write buffer depth
    UINT16 CLKMODE: 1;     // 2     Ratio for XCLKOUT with respect to XTIMCLK
    UINT16 CLKOFF: 1;      // 3     Disable XCLKOUT
    UINT16 rsvd1: 2;       // 5:4   reserved
    UINT16 WLEVEL: 2;      // 7:6   Current level of the write buffer
    UINT16 MPNMC: 1;       // 8     Micro-processor/micro-computer mode
    UINT16 HOLD: 1;        // 9     Hold enable/disable
    UINT16 HOLDS: 1;       // 10    Current state of HOLDn input
    UINT16 HOLDAS: 1;      // 11    Current state of HOLDAn output
    UINT16 rsvd2: 4;       // 15:12 reserved
    UINT16 XTIMCLK: 3;     // 18:16 Ratio for XTIMCLK
    UINT16 rsvd3: 13;      // 31:19 reserved
};

union XINTCNF2_REG
{
    UINT32                  all;
    struct XINTCNF2_BITS  bit;
};

// XINTF bank switching register bit definitions:
struct XBANK_BITS      // bits  description
{
    UINT16  BANK: 2;      // 2:0   Zone for which banking is enabled
    UINT16  BCYC: 3;      // 5:3   XTIMCLK cycles to add
    UINT16  rsvd: 10;     // 15:6  reserved
};

union XBANK_REG
{
    UINT16               all;
    struct XBANK_BITS  bit;
};


//---------------------------------------------------------------------------
// XINTF Register File:
//
struct XINTF_REGS
{
    union XTIMING_REG XTIMING0;
    union XTIMING_REG XTIMING1;
    union XTIMING_REG XTIMING2;
    UINT32  rsvd1[3];
    union XTIMING_REG XTIMING6;
    union XTIMING_REG XTIMING7;
    UINT32  rsvd2[2];
    union XINTCNF2_REG XINTCNF2;
    UINT32  rsvd3;
    union XBANK_REG    XBANK;
    UINT16  rsvd4;
    UINT16  XREVISION;
    UINT16  rsvd5[5];
};

//---------------------------------------------------------------------------
// XINTF External References & Function Declarations:
//
extern volatile struct XINTF_REGS XintfRegs;

#endif  // end of DSP28_XINTF_H definition

//===========================================================================
// No more.
//===========================================================================
