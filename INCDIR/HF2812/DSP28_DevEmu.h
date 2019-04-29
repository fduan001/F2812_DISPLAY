//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_DevEmu.h
//
// TITLE:	DSP28 Device Emulation Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_DEV_EMU_H
#define DSP28_DEV_EMU_H
#include "F2812_datatype.h"
//---------------------------------------------------------------------------
// Device Emulation Register Bit Definitions:
//
// Device Configuration Register Bit Definitions
struct DEVICECNF_BITS      // bits  description
{
    UINT16 rsvd1: 3;          // 2:0   reserved
    UINT16 VMAPS: 1;          // 3     VMAP Status
    UINT16 rsvd2: 1;          // 4     reserved
    UINT16 XRSn: 1;           // 5     XRSn Signal Status
    UINT16 rsvd3: 10;         // 15:6
    UINT16 rsvd4: 3;          // 18:6
    UINT16 ENPROT: 1;         // 19    Enable/Disable pipeline protection
    UINT16 rsvd5: 12;         // 31:20 reserved
};

union DEVICECNF_REG
{
    UINT32                all;
    struct DEVICECNF_BITS  bit;
};


// Device ID Register Bit Definitions
struct DEVICEID_BITS       // bits  description
{
    UINT16 PARTID: 16;        // 15:0  Part ID
    UINT16 REVID: 16;         // 31:16 Revision
};

union DEVICEID_REG
{
    UINT32                   all;
    struct DEVICEID_BITS  bit;
};

struct DEV_EMU_REGS
{
    union DEVICECNF_REG DEVICECNF;
    union DEVICEID_REG DEVICEID;
    UINT16  PROTSTART;
    UINT16  PROTRANGE;
    UINT16 rsvd[202];
    UINT16 M0RAMDFT;
    UINT16 M1RAMDFT;
    UINT16 L0RAMDFT;
    UINT16 L1RAMDFT;
    UINT16 H0RAMDFT;
};

//---------------------------------------------------------------------------
// Device Emulation Register References & Function Declarations:
//
extern volatile struct DEV_EMU_REGS DevEmuRegs;

#endif  // end of DSP28_DEV_EMU_H definition

//===========================================================================
// No more.
//===========================================================================
