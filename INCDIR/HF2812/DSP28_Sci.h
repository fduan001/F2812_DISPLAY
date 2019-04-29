//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Sci.h
//
// TITLE:	DSP28 Device SCI Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_SCI_H
#define DSP28_SCI_H
#include "F2812_datatype.h"
//---------------------------------------------------------------------------
// SCI Individual Register Bit Definitions

//----------------------------------------------------------
// SCICCR communication control register bit definitions:
//

struct  SCICCR_BITS        // bit    description
{
    UINT16 SCICHAR: 3;        // 2:0    Character length control
    UINT16 ADDRIDLE_MODE: 1;  // 3      ADDR/IDLE Mode control
    UINT16 LOOPBKENA: 1;      // 4      Loop Back enable
    UINT16 PARITYENA: 1;      // 5      Parity enable
    UINT16 PARITY: 1;         // 6      Even or Odd Parity
    UINT16 STOPBITS: 1;       // 7      Number of Stop Bits
    UINT16 rsvd1: 8;          // 15:8   reserved
};

union SCICCR_REG
{
    UINT16                all;
    struct SCICCR_BITS  bit;
};

//-------------------------------------------
// SCICTL1 control register 1 bit definitions:
//

struct  SCICTL1_BITS       // bit    description
{
    UINT16 RXENA: 1;          // 0      SCI receiver enable
    UINT16 TXENA: 1;          // 1      SCI transmitter enable
    UINT16 SLEEP: 1;          // 2      SCI sleep
    UINT16 TXWAKE: 1;         // 3      Transmitter wakeup method
    UINT16 rsvd: 1;           // 4      reserved
    UINT16 SWRESET: 1;        // 5      Software reset
    UINT16 RXERRINTENA: 1;    // 6      Recieve interrupt enable
    UINT16 rsvd1: 9;          // 15:7   reserved

};

union SCICTL1_REG
{
    UINT16                 all;
    struct SCICTL1_BITS  bit;
};

//---------------------------------------------
// SCICTL2 control register 2 bit definitions:
//

struct  SCICTL2_BITS       // bit    description
{
    UINT16 TXINTENA: 1;       // 0      Transmit interrupt enable
    UINT16 RXBKINTENA: 1;     // 1      Receiver-buffer break enable
    UINT16 rsvd: 4;           // 5:2    reserved
    UINT16 TXEMPTY: 1;        // 6      Transmitter empty flag
    UINT16 TXRDY: 1;          // 7      Transmitter ready flag
    UINT16 rsvd1: 8;          // 15:8   reserved

};

union SCICTL2_REG
{
    UINT16                 all;
    struct SCICTL2_BITS  bit;
};

//---------------------------------------------------
// SCIRXST Receiver status register bit definitions:
//

struct  SCIRXST_BITS       // bit    description
{
    UINT16 rsvd: 1;           // 0      reserved
    UINT16 RXWAKE: 1;         // 1      Receiver wakeup detect flag
    UINT16 PE: 1;             // 2      Parity error flag
    UINT16 OE: 1;             // 3      Overrun error flag
    UINT16 FE: 1;             // 4      Framing error flag
    UINT16 BRKDT: 1;          // 5      Break-detect flag
    UINT16 RXRDY: 1;          // 6      Receiver ready flag
    UINT16 RXERR: 1;          // 7      Receiver error flag

};

union SCIRXST_REG
{
    UINT16                 all;
    struct SCIRXST_BITS  bit;
};

//----------------------------------------------------
// SCIRXBUF Receiver Data Buffer with FIFO bit definitions:
//

struct  SCIRXBUF_BITS      // bits   description
{
    UINT16 RXDT: 8;           // 7:0    Receive word
    UINT16 rsvd: 6;           // 13:8   reserved
    UINT16 SCIFFPE: 1;        // 14     SCI PE error in FIFO mode
    UINT16 SCIFFFE: 1;        // 15     SCI FE error in FIFO mode
};

union SCIRXBUF_REG
{
    UINT16                  all;
    struct SCIRXBUF_BITS  bit;
};

//--------------------------------------------------
// SCIPRI Priority control register bit definitions:
//
//

struct  SCIPRI_BITS        // bit    description
{
    UINT16 rsvd: 3;           // 2:0    reserved
    UINT16 FREE: 1;           // 3      Free emulation suspend mode
    UINT16 SOFT: 1;           // 4      Soft emulation suspend mode
    UINT16 rsvd1: 3;          // 7:5    reserved
};

union SCIPRI_REG
{
    UINT16                all;
    struct SCIPRI_BITS  bit;
};

//-------------------------------------------------
// SCI FIFO Transmit register bit definitions:
//
//

struct  SCIFFTX_BITS       // bit    description
{
    UINT16 TXFFILIL: 5;       // 4:0    Interrupt level
    UINT16 TXFFIENA: 1;       // 5      Interrupt enable
    UINT16 TXINTCLR: 1;       // 6      Clear INT flag
    UINT16 TXFFINT: 1;        // 7      INT flag
    UINT16 TXFFST: 5;         // 12:8   FIFO status
    UINT16 TXFIFOXRESET: 1;   // 13     FIFO reset
    UINT16 SCIFFENA: 1;       // 14     Enhancement enable
    UINT16 resvd: 1;          // 15     reserved

};

union SCIFFTX_REG
{
    UINT16                 all;
    struct SCIFFTX_BITS  bit;
};

//------------------------------------------------
// SCI FIFO recieve register bit definitions:
//
//

struct  SCIFFRX_BITS       // bits   description
{
    UINT16 RXFFIL: 5;         // 4:0    Interrupt level
    UINT16 RXFFIENA: 1;       // 5      Interrupt enable
    UINT16 RXFFINTCLR: 1;     // 6      Clear INT flag
    UINT16 RXFFINT: 1;        // 7      INT flag
    UINT16 RXFIFST: 5;        // 12:8   FIFO status
    UINT16 RXFIFORESET: 1;    // 13     FIFO reset
    UINT16 RXOVF_CLR: 1;      // 14     Clear overflow
    UINT16 RXFFOVF: 1;        // 15     FIFO overflow

};

union SCIFFRX_REG
{
    UINT16                 all;
    struct SCIFFRX_BITS  bit;
};

// SCI FIFO control register bit definitions:
struct  SCIFFCT_BITS       // bits   description
{
    UINT16 FFTXDLY: 8;        // 7:0    FIFO transmit delay
    UINT16 rsvd: 5;           // 12:8   reserved
    UINT16 CDC: 1;            // 13     Auto baud mode enable
    UINT16 ABDCLR: 1;         // 14     Auto baud clear
    UINT16 ABD: 1;            // 15     Auto baud detect
};

union SCIFFCT_REG
{
    UINT16                 all;
    struct SCIFFCT_BITS  bit;
};

//---------------------------------------------------------------------------
// SCI Register File:
//
struct  SCI_REGS
{
    union SCICCR_REG     SCICCR;     // Communications control register
    union SCICTL1_REG    SCICTL1;    // Control register 1
    UINT16               SCIHBAUD;   // Baud rate (high) register
    UINT16               SCILBAUD;   // Baud rate (low) register
    union SCICTL2_REG    SCICTL2;    // Control register 2
    union SCIRXST_REG    SCIRXST;    // Recieve status register
    UINT16               SCIRXEMU;   // Recieve emulation buffer register
    union SCIRXBUF_REG   SCIRXBUF;   // Recieve data buffer
    UINT16  rsvd1;                   // reserved
    UINT16               SCITXBUF;   // Transmit data buffer
    union SCIFFTX_REG    SCIFFTX;    // FIFO transmit register
    union SCIFFRX_REG    SCIFFRX;    // FIFO recieve register
    union SCIFFCT_REG    SCIFFCT;    // FIFO control register
    UINT16 rsvd2;                    // reserved
    UINT16 rsvd3;                    // reserved
    union SCIPRI_REG      SCIPRI;    // FIFO Priority control
};

//---------------------------------------------------------------------------
// SCI External References & Function Declarations:
//
extern volatile struct SCI_REGS SciaRegs;
extern volatile struct SCI_REGS ScibRegs;

#endif  // end of DSP28_SCI_H definition

//===========================================================================
// No more.
//===========================================================================
