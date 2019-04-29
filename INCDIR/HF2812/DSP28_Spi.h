//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Spi.h
//
// TITLE:	DSP28 Device SPI Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_SPI_H
#define DSP28_SPI_H
#include "F2812_datatype.h"
//---------------------------------------------------------------------------
// SPI Individual Register Bit Definitions:
//
// SPI FIFO Transmit register bit    definitions:
struct  SPIFFTX_BITS       // bit    description
{
    UINT16 TXFFIL: 5;         // 4:0    Interrupt level
    UINT16 TXFFIENA: 1;       // 5      Interrupt enable
    UINT16 TXFFINTINTCLR: 1;  // 6      Clear INT flag
    UINT16 TXFFINT: 1;        // 7      INT flag
    UINT16 TXFFST: 5;         // 12:8   FIFO status
    UINT16 TXFIFORESET: 1;    // 14     Enhancement enable
    UINT16 rsvd: 1;           // 15     reserved
};

union SPIFFTX_REG
{
    UINT16                 all;
    struct SPIFFTX_BITS  bit;
};

//--------------------------------------------
// SPI FIFO recieve register bit definitions:
//
//
struct  SPIFFRX_BITS       // bits   description
{
    UINT16 RXFFIL: 5;         // 4:0    Interrupt level
    UINT16 RXFFIENA: 1;       // 5      Interrupt enable
    UINT16 RXFFINTCLR: 1;     // 6      Clear INT flag
    UINT16 RXFFINT: 1;        // 7      INT flag
    UINT16 RXFFST: 5;         // 12:8   FIFO status
    UINT16 RXFIFORESET: 1;    // 13     FIFO reset
    UINT16 RXFFOVFCLR: 1;     // 14     Clear overflow
    UINT16 RXFFOVF: 1;        // 15     FIFO overflow

};

union SPIFFRX_REG
{
    UINT16                 all;
    struct SPIFFRX_BITS  bit;
};

//--------------------------------------------
// SPI FIFO control register bit definitions:
//
//
struct  SPIFFCT_BITS       // bits   description
{
    UINT16 TXDLY: 8;          // 7:0    FIFO transmit delay
    UINT16 rsvd: 8;           // 15:8   reserved
};

union SPIFFCT_REG
{
    UINT16                 all;
    struct SPIFFCT_BITS  bit;
};

//---------------------------------------------
// SPI configuration register bit definitions:
//
//
struct  SPICCR_BITS        // bits   description
{
    UINT16 SPICHAR: 4;        // 3:0    Character length control
    UINT16 SPILBK: 1;         // 4      Loop-back enable/disable
    UINT16 rsvd1: 1;          // 5      reserved
    UINT16 CLKPOLARITY: 1;    // 6      Clock polarity
    UINT16 RESET: 1;          // 7      SPI SW Reset
    UINT16 rsvd2: 8;          // 15:8   reserved
};

union SPICCR_REG
{
    UINT16                all;
    struct SPICCR_BITS  bit;
};

//-------------------------------------------------
// SPI operation control register bit definitions:
//
//
struct  SPICTL_BITS        // bits   description
{
    UINT16 SPIINTENA: 1;      // 0      Interrupt enable
    UINT16 TALK: 1;           // 1      Master/Slave transmit enable
    UINT16 MASTER_SLAVE: 1;   // 2      Network control mode
    UINT16 CLK_PHASE: 1;      // 3      Clock phase select
    UINT16 OVERRUN: 1;        // 4      Overrun interrupt enable
    UINT16 rsvd: 11;          // 15:5   reserved
};

union SPICTL_REG
{
    UINT16                all;
    struct SPICTL_BITS  bit;
};

//--------------------------------------
// SPI status register bit definitions:
//
//
struct  SPISTS_BITS        // bits   description
{
    UINT16 rsvd1: 5;          // 4:0    reserved
    UINT16 BUFFULL_FLAG: 1;   // 5      SPI transmit buffer full flag
    UINT16 INT_FLAG: 1;       // 6      SPI interrupt flag
    UINT16 OVERRUN_FLAG: 1;   // 7      SPI reciever overrun flag
    UINT16 rsvd2: 8;          // 15:8   reserved
};

union SPISTS_REG
{
    UINT16                all;
    struct SPISTS_BITS  bit;
};

//------------------------------------------------
// SPI priority control register bit definitions:
//
//
struct  SPIPRI_BITS        // bits   description
{
    UINT16 rsvd1: 4;          // 3:0    reserved
    UINT16 FREE: 1;           // 4      Free emulation mode control
    UINT16 SOFT: 1;           // 5      Soft emulation mode control
    UINT16 PRIORITY: 1;       // 6      Interrupt priority select
    UINT16 rsvd2: 9;          // 15:7   reserved
};

union SPIPRI_REG
{
    UINT16                all;
    struct SPIPRI_BITS  bit;
};

//---------------------------------------------------------------------------
// SPI Register File:
//
struct  SPI_REGS
{
    union SPICCR_REG     SPICCR;      // Configuration register
    union SPICTL_REG     SPICTL;      // Operation control register
    union SPISTS_REG     SPISTS;      // Status register
    UINT16  rsvd1;                      // reserved
    UINT16                 SPIBRR;      // Baud Rate
    UINT16  rsvd2;                      // reserved
    UINT16                 SPIRXEMU;    // Emulation buffer
    UINT16                 SPIRXBUF;    // Serial input buffer
    UINT16                 SPITXBUF;    // Serial output buffer
    UINT16                 SPIDAT;      // Serial data
    union SPIFFTX_REG    SPIFFTX;     // FIFO transmit register
    union SPIFFRX_REG    SPIFFRX;     // FIFO recieve register
    union SPIFFCT_REG    SPIFFCT;     // FIFO control register
    UINT16  rsvd3[2];                   // reserved
    union SPIPRI_REG     SPIPRI;      // FIFO Priority control
};

//---------------------------------------------------------------------------
// SPI External References & Function Declarations:
//
extern volatile struct SPI_REGS SpiaRegs;
extern volatile struct SPI_REGS SpibRegs;

extern unsigned int Spi_VarRx[100];
extern unsigned int i, j;

#endif  // end of DSP28_SPI_H definition

//===========================================================================
// No more.
//===========================================================================
