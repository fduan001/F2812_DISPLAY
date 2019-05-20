//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Xintf.c
//
// TITLE:	DSP28 Device External Interface Init & Support Functions.
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
// InitXINTF:
//---------------------------------------------------------------------------
// This function initializes the External Interface to a known state.
//

void InitXintf(void)
{


    // Example of chaning the timing of XINTF Zones.
    // Note acutal values should be based on the hardware
    // attached to the zone - timings presented here are
    // for example purposes.

    // All Zones:
    // Timing for all zones based on XTIMCLK = SYSCLKOUT=150M to get high speed
    //XintfRegs.XINTCNF2.bit.XTIMCLK = 0x0;
    //sysclkout/2
   // XintfRegs.XINTCNF2.bit.XTIMCLK = 0x1;
	//150MHZ
	XintfRegs.XINTCNF2.bit.XTIMCLK = 0x0;
	//XCLKOUT=XTIMCLK
	XintfRegs.XINTCNF2.bit.CLKMODE = 0x0;
    //CLKOUT is enable
  	XintfRegs.XINTCNF2.bit.CLKOFF = 0x0;  
	
    // Zone 0 & 1 ,FPGA internal SRAM for stack
    // Change write access lead active trail timing
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Use timings based on SYSCLKOUT = XTIMCLK
#if 0
    XintfRegs.XTIMING0.bit.XWRTRAIL = 2;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING0.bit.XWRLEAD = 2;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 2;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 4;
    XintfRegs.XTIMING0.bit.XRDLEAD = 2;
    // Do not double lead/active/trail for Zone 0
    XintfRegs.XTIMING0.bit.X2TIMING = 0;
    XintfRegs.XTIMING0.bit.USEREADY = 0;	

    //zone1 
    XintfRegs.XTIMING1.bit.XWRTRAIL = 2;
    XintfRegs.XTIMING1.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING1.bit.XWRLEAD = 2;
    XintfRegs.XTIMING1.bit.XRDTRAIL = 2;
    XintfRegs.XTIMING1.bit.XRDACTIVE = 4;
    XintfRegs.XTIMING1.bit.XRDLEAD = 2;
    // Do not double lead/active/trail for Zone 1
    XintfRegs.XTIMING1.bit.X2TIMING = 0;
    XintfRegs.XTIMING1.bit.USEREADY = 0;	

#endif
    //SRAM for cs0&1
#if 1
    XintfRegs.XTIMING0.bit.XWRTRAIL = 2;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING0.bit.XWRLEAD = 2;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 2;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 4;
    XintfRegs.XTIMING0.bit.XRDLEAD = 2;
    // Do not double lead/active/trail for Zone 0
    XintfRegs.XTIMING0.bit.X2TIMING = 0;
    XintfRegs.XTIMING0.bit.USEREADY = 0;

    //zone1
    XintfRegs.XTIMING1.bit.XWRTRAIL = 2;
    XintfRegs.XTIMING1.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING1.bit.XWRLEAD = 2;
    XintfRegs.XTIMING1.bit.XRDTRAIL = 2;
    XintfRegs.XTIMING1.bit.XRDACTIVE = 4;
    XintfRegs.XTIMING1.bit.XRDLEAD = 2;
    // Do not double lead/active/trail for Zone 1
    XintfRegs.XTIMING1.bit.X2TIMING = 0;
    XintfRegs.XTIMING1.bit.USEREADY = 0;

#endif

    // Zone 2  for Host Dual port SRAM  
    // Ignore XREADY for Zone 2 accesses
    // Change read access lead/active/trail timing
    XintfRegs.XTIMING2.bit.USEREADY = 0;
    XintfRegs.XTIMING2.bit.XRDLEAD = 2;
    XintfRegs.XTIMING2.bit.XRDACTIVE = 4;
    XintfRegs.XTIMING2.bit.XRDTRAIL = 2;
    XintfRegs.XTIMING2.bit.XWRLEAD = 2;
    XintfRegs.XTIMING2.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING2.bit.XWRTRAIL = 2;

    //  Do not double lead/active/trail for Zone 1
    XintfRegs.XTIMING2.bit.X2TIMING = 0;
    XintfRegs.XTIMING2.bit.USEREADY = 0;


    // Zone 6 for 1553B 64843
    // Ignore XREADY for Zone 6 accesses
    // Change read access lead/active/trail timing
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.XRDLEAD = 2;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 5;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 2;
    XintfRegs.XTIMING6.bit.XWRLEAD = 2;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 2;

    //  Do not double lead/active/trail for Zone 1
    XintfRegs.XTIMING6.bit.X2TIMING = 0;
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.XSIZE = 3;
    XintfRegs.XTIMING6.bit.READYMODE = 0;


    // Zone 2 is slow, so add additional BCYC cycles when ever switching
    // from Zone 2 to another Zone.  This will help avoid
    // bus contention.
    XintfRegs.XBANK.bit.BANK = 6;
    XintfRegs.XBANK.bit.BCYC = 3;

}


//===========================================================================
// No more.
//===========================================================================
