/*
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== TMS320F2812.cmd ========
 *  Define the memory block start/length for the F2812
 */

/*  
 *  PAGE 0 will be used to organize program sections
 *  PAGE 1 will be used to organize data sections
 *
 *  Notes: 
 *      Memory blocks on F2812 are uniform (ie same
 *      physical memory) in both PAGE 0 and PAGE 1.  
 *      That is the same memory region should not be
 *      defined for both PAGE 0 and PAGE 1.
 *      Doing so will result in corruption of program 
 *      and/or data. 
 */

MEMORY
{
PAGE 0:    /* Program Memory */

    OTP         : origin = 0x3D7800, length = 0x000800     /* on-chip OTP */
    FLASH       : origin = 0x3D8000, length = 0x01FF80     /* on-chip FLASH */
    CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Program with all 0x0000 when CSM is in use. */
    BEGIN       : origin = 0x3F7FF6, length = 0x000002     /* Used for "boot to Flash" bootloader mode. */
    CSM_PWL     : origin = 0x3F7FF8, length = 0x000008     /* CSM password locations in FLASH */
    /*H0SARAM     : origin = 0x3F8000, length = 0x002000  */   /* on-chip RAM block H0 */

    ROM         : origin = 0x3FF000, length = 0x000FC0     /* Boot ROM available if MP/MCn=0 */
    RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */
    VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM (MP/MCn=0) or XINTF zone 7 (MP/MCn=1) */




PAGE 1 :   /* Data Memory */

    M01SARAM    : origin = 0x000040, length = 0x000780     /* on-chip RAM block M0, M1 */
    /*PIEVECT     : origin = 0xD00,    length = 0x100*/
    L01SARAM    : origin = 0x008000, length = 0x002000     /* on-chip RAM block L0, L1 */
    H0SARAM     : origin = 0x3F8000, length = 0x002000     /* on -chip RAM high H0,H1 */
   /* Peripheral Frame 0:   */
   DEV_EMU    : origin = 0x000880, length = 0x000180
   FLASH_REGS : origin = 0x000A80, length = 0x000060
   CSM        : origin = 0x000AE0, length = 0x000010
   XINTF      : origin = 0x000B20, length = 0x000020
   CPU_TIMER0 : origin = 0x000C00, length = 0x000008
   CPU_TIMER1 : origin = 0x000C08, length = 0x000008		 
   CPU_TIMER2 : origin = 0x000C10, length = 0x000008		 
   PIE_CTRL   : origin = 0x000CE0, length = 0x000020
   PIE_VECT   : origin = 0x000D00, length = 0x000100

   /* Peripheral Frame 1:   */
   ECAN_A     : origin = 0x006000, length = 0x000100
   ECAN_AMBOX : origin = 0x006100, length = 0x000100

   /* Peripheral Frame 2:   */
   SYSTEM     : origin = 0x007010, length = 0x000020
   SPI_A      : origin = 0x007040, length = 0x000010
   SCI_A      : origin = 0x007050, length = 0x000010
   XINTRUPT   : origin = 0x007070, length = 0x000010
   GPIOMUX    : origin = 0x0070C0, length = 0x000020
   GPIODAT    : origin = 0x0070E0, length = 0x000020
   ADC        : origin = 0x007100, length = 0x000020
   EV_A       : origin = 0x007400, length = 0x000040
   EV_B       : origin = 0x007500, length = 0x000040
   SPI_B      : origin = 0x007740, length = 0x000010
   SCI_B      : origin = 0x007750, length = 0x000010
   MCBSP_A    : origin = 0x007800, length = 0x000040

   /* CSM Password Locations */
  CSM_PWL    : origin = 0x3F7FF8, length = 0x000008

  /* XINTF memory  zone0,1 task stack place 16K*/
  XINTF_ZONE01 : origin = 0x2000, length = 0x4000

  /* XINTF memory data section 8K */
  /* XINTF_ZONE01_DATA : origin = 0x4000, length = 0x2000 */

  /* XINTF memory  zone2 512KB*/
  XINTF_ZONE2 : origin = 0x80000, length = 0x80000

  /* XINTF memory  zone67 512KB*/
  XINTF_ZONE67 : origin = 0x100000, length = 0x180000
}

/* 
 *  Allocate sections to memory blocks.
 *  Note:
 *      codestart   user defined section in DSP28_CodeStartBranch.asm
 *                  used to redirect code execution when booting to flash
 *
 *      ramfuncs    user defined section to store functions that will be
 *                  copied from Flash into RAM
 */ 
 
SECTIONS
{
    /* Allocate program areas: */
    .cinit              : > FLASH       PAGE = 0
    .pinit              : > FLASH       PAGE = 0
    .text               : > FLASH       PAGE = 0
    codestart        > BEGIN,      PAGE = 0
    codestart           : > BEGIN       PAGE = 0
    ramfuncs            : LOAD = FLASH      PAGE = 0,
                          RUN  = L01SARAM   PAGE = 1,
                          LOAD_START(_RamfuncsLoadStart),
                          LOAD_SIZE(_RamfuncsLoadSize),
                          LOAD_END(_RamfuncsLoadEnd),
                          RUN_START(_RamfuncsRunStart)

    csmpasswds          : > CSM_PWL     PAGE = 0
    csm_rsvd            : > CSM_RSVD    PAGE = 0

    /* Allocate uninitalized data sections: */
    .stack              : > L01SARAM                   PAGE = 1
    .bss                : > M01SARAM                   PAGE = 1
    /* .ebss               : > M01SARAM | L01SARAM     PAGE = 1 */
    .ebss               : > L01SARAM     PAGE = 1

    /* .ebss               : > XINTF_ZONE01     PAGE = 1 */

    .ebss:taskStackSection  :>XINTF_ZONE01             PAGE = 1
    /* .ebss:taskStackSection  :>L01SARAM           PAGE = 1 */
    /*.ebss               : > XINTF_ZONE2                 PAGE =1*/
    /*.esysmem            : > L01SARAM | M01SARAM     PAGE = 1*/
    .esysmem            : > H0SARAM                    PAGE =1
    .cio                : > L01SARAM | M01SARAM        PAGE = 1

    shell_lib           : > H0SARAM                    PAGE =1
    /* shell_lib           : > XINTF_ZONE01            PAGE =1 */
    /* ddc_lib           : > L01SARAM                  PAGE =1 */
    ddc_lib           : > M01SARAM                     PAGE =1
    /*systemHeap          : > H0SARAM            PAGE =1*/

    /* Initalized sections go in Flash */
    /* For SDFlash to program these, they must be allocated to page 0 */
    .econst             : > FLASH       PAGE = 0
    .switch             : > FLASH       PAGE = 0      
    .args               : > FLASH       PAGE = 0

    /* Allocate IQ math areas: */
    IQmath              : > FLASH       PAGE = 0                  /* Math Code */
    IQmathTables        : > ROM         PAGE = 0, TYPE = NOLOAD   /* Math Tables In ROM */

    /* Allocate Peripheral Frame 0 Register Structures:   */
   DevEmuRegsFile      : > DEV_EMU     PAGE = 1
   FlashRegsFile       : > FLASH_REGS  PAGE = 1
   CsmRegsFile         : > CSM         PAGE = 1
   XintfRegsFile       : > XINTF       PAGE = 1
   CpuTimer0RegsFile   : > CPU_TIMER0  PAGE = 1      
   CpuTimer1RegsFile   : > CPU_TIMER1  PAGE = 1      
   CpuTimer2RegsFile   : > CPU_TIMER2  PAGE = 1      
   PieCtrlRegsFile     : > PIE_CTRL    PAGE = 1      
   PieVectTable        : > PIE_VECT    PAGE = 1

   /* Allocate Peripheral Frame 2 Register Structures:   */
   ECanaRegsFile       : > ECAN_A      PAGE = 1   
   ECanaMboxesFile     : > ECAN_AMBOX  PAGE = 1

   /* Allocate Peripheral Frame 1 Register Structures:   */
   SysCtrlRegsFile     : > SYSTEM      PAGE = 1
   SpiaRegsFile        : > SPI_A       PAGE = 1
   SciaRegsFile        : > SCI_A       PAGE = 1
   XIntruptRegsFile    : > XINTRUPT    PAGE = 1
   GpioMuxRegsFile     : > GPIOMUX     PAGE = 1
   GpioDataRegsFile    : > GPIODAT     PAGE = 1
   AdcRegsFile         : > ADC         PAGE = 1
   EvaRegsFile         : > EV_A        PAGE = 1
   EvbRegsFile         : > EV_B        PAGE = 1
   ScibRegsFile        : > SCI_B       PAGE = 1
   McbspaRegsFile      : > MCBSP_A     PAGE = 1

   /* CSM Password Locations */
   /*CsmPwlFile          : > CSM_PWL     PAGE = 1*/
   
}
