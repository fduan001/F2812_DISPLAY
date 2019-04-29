//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_SysCtrl.h
//
// TITLE:	DSP28 Device System Control Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_SYS_CTRL_H
#define DSP28_SYS_CTRL_H
#include "F2812_datatype.h"
//---------------------------------------------------------------------------
// System Control Individual Register Bit Definitions:
//
// High speed peripheral clock register bit definitions:
struct HISPCP_BITS      // bits  description
{
    UINT16 HSPCLK: 3;      // 2:0   Rate relative to SYSCLKOUT
    UINT16 rsvd1: 13;      // 15:3  reserved
};

union HISPCP_REG
{
    UINT16                all;
    struct HISPCP_BITS  bit;
};

// Low speed peripheral clock register bit definitions:
struct LOSPCP_BITS      // bits  description
{
    UINT16 LSPCLK: 3;      // 2:0   Rate relative to SYSCLKOUT
    UINT16 rsvd1: 13;      // 15:3  reserved
};

union LOSPCP_REG
{
    UINT16                all;
    struct LOSPCP_BITS  bit;
};

// Peripheral clock control register bit definitions:
struct PCLKCR_BITS      // bits  description
{
    UINT16 EVAENCLK: 1;    // 0     Enable high speed clk to EV-A
    UINT16 EVBENCLK: 1;    // 1     Enable high speed clk to EV-B
    UINT16 rsvd1: 1;       // 2
    UINT16 ADCENCLK: 1;    // 3     Enable high speed clk to ADC
    UINT16 rsvd2: 4;       // 7:4   reserved
    UINT16 SPIENCLK: 1;    // 8     Enable low speed clk to SPI
    UINT16 rsvd3: 1;       // 9     reserved
    UINT16 SCIENCLKA: 1;   // 10    Enable low speed clk to SCI-A
    UINT16 SCIENCLKB: 1;   // 11    Enable low speed clk to SCI-B
    UINT16 MCBSPENCLK: 1;  // 12    Enable low speed clk to McBSP
    UINT16 rsvd4: 1;       // 13    reserved
    UINT16 ECANENCLK: 1;   // 14    Enable system clk to eCAN
};

union PCLKCR_REG
{
    UINT16                all;
    struct PCLKCR_BITS  bit;
};

// System control and status register bit definitions:
struct SCSR_BITS        // bits  description
{
    UINT16 WDOVERRIDE: 1;  // 0     Allow watchdog disable
    UINT16 WDENINT: 1;     // 1     Enable/disable WD interrupt
    UINT16 rsvd1: 14;      // 15:2  reserved
};

union SCSR_REG
{
    UINT16              all;
    struct SCSR_BITS  bit;
};

//---------------------------------------------------------------------------
// System Control Register File:
//
struct SYS_CTRL_REGS
{
    UINT16  rsvd1[10];            // 0-9
    union HISPCP_REG HISPCP;      // 10: High-speed peripheral clock pre-scaler
    union LOSPCP_REG LOSPCP;      // 11: Low-speed peripheral clock pre-scaler
    union PCLKCR_REG PCLKCR;      // 12: Peripheral clock control register
    UINT16  rsvd2;                // 13
    UINT16  LPMCR0;               // 14: Low-power mode control register 0
    UINT16  LPMCR1;               // 15: Low-power mode control register 1
    UINT16  rsvd3;                // 16
    UINT16  PLLCR;                // 17: PLL control register
    union SCSR_REG SCSR;          // 18: System control and status register
    UINT16  WDCNTR;               // 19: WD counter register
    UINT16  rsvd4;                // 20
    UINT16  WDKEY;                // 21: WD reset key register
    UINT16  rsvd5[3];             // 22-24
    UINT16  WDCR;                 // 25: WD timer control register
    UINT16  rsvd6[6];             // 26-31
};


/* --------------------------------------------------- */
/* CSM Registers                                       */
/*                                                     */
/* ----------------------------------------------------*/

/* CSM Status & Control register bit definitions */
struct  CSMSCR_BITS        // bit   description
{
    UINT16     SECURE: 1;   // 0     Secure flag
    UINT16     rsvd1: 14;   // 14-1  reserved
    UINT16     FORCESEC: 1; // 15    Force Secure control bit

};

/* Allow access to the bit fields or entire register */
union CSMSCR_REG
{
    UINT16       all;
    struct CSMSCR_BITS bit;
};

/* CSM Register File */
struct  CSM_REGS
{
    UINT16         KEY0;       // KEY reg bits 15-0
    UINT16         KEY1;       // KEY reg bits 31-16
    UINT16         KEY2;       // KEY reg bits 47-32
    UINT16         KEY3;       // KEY reg bits 63-48
    UINT16         KEY4;       // KEY reg bits 79-64
    UINT16         KEY5;       // KEY reg bits 95-80
    UINT16         KEY6;       // KEY reg bits 111-96
    UINT16         KEY7;       // KEY reg bits 127-112
    UINT16         rsvd1;      // reserved
    UINT16         rsvd2;      // reserved
    UINT16         rsvd3;      // reserved
    UINT16         rsvd4;      // reserved
    UINT16         rsvd5;      // reserved
    UINT16         rsvd6;      // reserved
    UINT16         rsvd7;      // reserved
    union CSMSCR_REG CSMSCR;     // CSM Status & Control register
};

/* Password locations */
struct  CSM_PWL
{
    UINT16         PSWD0;      // PSWD bits 15-0
    UINT16         PSWD1;      // PSWD bits 31-16
    UINT16         PSWD2;      // PSWD bits 47-32
    UINT16         PSWD3;      // PSWD bits 63-48
    UINT16         PSWD4;      // PSWD bits 79-64
    UINT16         PSWD5;      // PSWD bits 95-80
    UINT16         PSWD6;      // PSWD bits 111-96
    UINT16         PSWD7;      // PSWD bits 127-112
};



/* Flash Registers */

/* Flash Option Register bit definitions */
struct  FOPT_BITS               // bit   description
{
    UINT16     ENPIPE: 1;  // 0     Enable Pipeline Mode
    UINT16     rsvd: 15;   // 1-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOPT_REG
{
    UINT16     all;
    struct FOPT_BITS bit;
};

/* Flash Power Modes Register bit definitions */
struct  FPWR_BITS               // bit   description
{
    UINT16     PWR: 2;     // 0-1   Power Mode bits
    UINT16     rsvd: 14;   // 2-15  reserved
};

/* Allow access to the bit fields or entire register */
union FPWR_REG
{
    UINT16     all;
    struct FPWR_BITS bit;
};


/* Flash Status Register bit definitions */
struct  FSTATUS_BITS               // bit   description
{
    UINT16     PWRS: 2;       // 0-1   Power Mode Status bits
    UINT16     STDBYWAITS: 1; // 2     Bank/Pump Sleep to Standby Wait Counter Status bits
    UINT16     ACTIVEWAITS: 1; // 3     Bank/Pump Standby to Active Wait Counter Status bits
    UINT16     rsvd1: 4;      // 4-7   reserved
    UINT16     V3STAT: 1;     // 8     VDD3V Status Latch bit
    UINT16     rsvd2: 7;      // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTATUS_REG
{
    UINT16        all;
    struct FSTATUS_BITS bit;
};

/* Flash Sleep to Standby Wait Counter Register bit definitions */
struct  FSTDBYWAIT_BITS            // bit   description
{
    UINT16     STDBYWAIT: 8;  // 0-7   Bank/Pump Sleep to Standby Wait Count bits
    UINT16     rsvd: 8;       // 8-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTDBYWAIT_REG
{
    UINT16           all;
    struct FSTDBYWAIT_BITS bit;
};

/* Flash Standby to Active Wait Counter Register bit definitions */
struct  FACTIVEWAIT_BITS           // bit   description
{
    UINT16     ACTIVEWAIT: 8; // 0-7   Bank/Pump Standby to Active Wait Count bits
    UINT16     rsvd: 8;       // 8-15  reserved
};

/* Allow access to the bit fields or entire register */
union FACTIVEWAIT_REG
{
    UINT16            all;
    struct FACTIVEWAIT_BITS bit;
};

/* Bank Read Access Wait State Register bit definitions */
struct  FBANKWAIT_BITS             // bit   description
{
    UINT16     RANDWAIT: 4;   // 0-3   Flash Random Read Wait State bits
    UINT16     rsvd1: 4;      // 4-7   reserved
    UINT16     PAGEWAIT: 4;   // 8-11  Flash Paged Read Wait State bits
    UINT16     rsvd2: 4;      // 12-15 reserved
};

/* Allow access to the bit fields or entire register */
union FBANKWAIT_REG
{
    UINT16          all;
    struct FBANKWAIT_BITS bit;
};

/* OTP Read Access Wait State Register bit definitions */
struct  FOTPWAIT_BITS              // bit   description
{
    UINT16     OPTWAIT: 5;    // 0-4   OTP Read Wait State bits
    UINT16     rsvd: 11;      // 5-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOTPWAIT_REG
{
    UINT16         all;
    struct FOTPWAIT_BITS bit;
};


struct FLASH_REGS
{
    union FOPT_REG        FOPT;        // Option Register
    UINT16 rsvd1;                      // reserved
    union FPWR_REG        FPWR;        // Power Modes Register
    union FSTATUS_REG     FSTATUS;     // Status Register
    union FSTDBYWAIT_REG  FSTDBYWAIT;  // Pump/Bank Sleep to Standby Wait State Register
    union FACTIVEWAIT_REG FACTIVEWAIT; // Pump/Bank Standby to Active Wait State Register
    union FBANKWAIT_REG   FBANKWAIT;   // Bank Read Access Wait State Register
    union FOTPWAIT_REG    FOTPWAIT;    // OTP Read Access Wait State Register
};

//---------------------------------------------------------------------------
// System Control External References & Function Declarations:
//
extern volatile struct SYS_CTRL_REGS SysCtrlRegs;
extern volatile struct CSM_REGS CsmRegs;
extern volatile struct CSM_PWL CsmPwl;
extern volatile struct FLASH_REGS FlashRegs;

#endif  // end of DSP28_SYS_CTRL_H definition

//===========================================================================
// No more.
//===========================================================================
