/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : boardcfg.h
 * Author        : yexin technologic
 * Date          : 2017-11-07
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2017-11-07
 *   Author      : yexin technologic
 *   Modification: Created file

*************************************************************************************************************/
#ifndef BOARDCFG_H
#define BOARDCFG_H
#include "F2812_datatype.h"

#define  CONFIG_BOARD_PROMPT   "MIB->"


#define BOOTBITMASK(x,y)      (   (   (  ((UINT16)1 << (((UINT16)x)-((UINT16)y)+(UINT16)1) ) - (UINT16)1 )   )   <<  ((UINT16)y)   )    // Sets a bitmask of 1s from [x:y]
#define BOOT_READ_BITFIELD(z,x,y)   (((UINT16)z) & BOOTBITMASK(x,y)) >> (y)                                                             // Reads the value of register z[x:y]
#define BOOT_SET_BITFIELD(z,f,x,y)  (((UINT16)z) & ~BOOTBITMASK(x,y)) | ( (((UINT16)f) << (y)) & BOOTBITMASK(x,y) )                     // Sets register z[x:y] to f
#define XINTWRITE16(addr,b) (*(UINT16*)(addr))=(UINT16)(b)



#define CPU_FREQ 150000000   //UNIT is HZ ,150MHZE
#define CPU_US_TIMER_MAXCNT (0xFFFFFF00)

#define CONFIG_SYS_I2C_RTC_ADDR  0x68




/* UART(SCI) config*/
#define SCI_PIN_MUX  (0x0030)
#define SCI_BAUD     (0x00F3)
//#define SCI_BAUD     (0x1E7)
/* 1bit stop ,odd parity, no parity, 8bit */
#define SCICCR_CFG  (0x7)
/* tx and rx enable */
#define SCICTL1_CFG (0x3)
/* no interrupt */
#define SCICTL2_REG (0x0)


/* SPI config */
#define SPI_PIN_MUX  (0xf)
/* loopback=0,16bit, clock polarity =0*/
#define SPICCR_CFG   (0x07)
/**************************************
    clock phase =1
    clock parity =0
Data is output one half-cycle before the first rising edge of the SPICLK
signal and on subsequent falling edges of the SPICLK signal;
input data is latched on the rising edge of the SPICLK signa
***************************************************************/

/********************************************************
For the MAX7219, serial data at DIN, sent in 16-bit
packets, is shifted into the internal 16-bit shift register
with each rising edge of CLK regardless of the state of
LOAD. For the MAX7221, CS must be low to clock data
in or out. The data is then latched into either the digit or
control registers on the rising edge of LOAD/CS.
LOAD/CS must go high concurrently with or after the
16th rising clock edge, but before the next rising clock
edge or data will be lost. Data at DIN is propagated
through the shift register and appears at DOUT 16.5
clock cycles later. Data is clocked out on the falling
edge of CLK. Data bits are labeled D0–D15 (Table 1).
D8–D11 contain the register address. D0–D7 contain
the data, and D12–D15 are “don’t care” bits. The first
received is D15, the most significant bit (MSB)

*********************************************************/
/* clock phase =1 */
#define SPICTL_CFG   (0xe)
#define SPIBRR_CFG   (0x23) /*4 divide * 37.5/36 ~1MHZ/


/* GPIO configuration */
/* A0~A15 gpio */
#define GPIOA_PIN_MUX (0x0)
/* B0~B15 gpio */
#define GPIOB_PIN_MUX  (0x0)
/*D0,1,5,6 gpio */
#define GPIOD_PIN_MUX  (0x0)
/*E0,1,2 GPIO */
#define GPIOE_PIN_MUX  (0x7)
/*F6~13 GPIO ,other is peripheral,SCI A*/
#define GPIOF_PIN_MUX  (SCI_PIN_MUX|(1<<14))
/*G4,G5 GPIO */
#define GPIOG_PIN_MUX  (SCI_PIN_MUX)

#define GPIOA_DIR_CFG (0xE700)
#define GPIOA_QUAL_CFG  (0x0)  /* type1 */

#define GPIOB_DIR_CFG (0x0000)
#define GPIOB_QUAL_CFG  (0x0)  /* type1 */


#define GPIOD_DIR_CFG (0x00)
#define GPIOD_QUAL_CFG  (0x0)   /* type1 */


/*GPIO E*/
/*
输出	DSP_GPIOE0	DEC_A
输出	DSP_GPIOE1	DEC_B
输出	DSP_GPIOE2	DEC_C
*/
#define GPIOE_DIR_CFG (0x7)
#define GPIOE_QUAL_CFG  (0x0)   /* type1 */

#define GPIOF_DIR_CFG (0x0)

/*GPIO G*/
/*
输入	DSP_GPIOG4	未使用
输入	DSP_GPIOG5	未使用
*/
#define GPIOG_DIR_CFG (0x0)




/*SPI interface LED driver chipsel */





/* HS 3282 definition */
#define HS3282_WRITE_CWSR_ADD (0x80000)
#define HS3282_WRITE_PL1      (0x80002)
#define HS3282_WRITE_PL2      (0x80004)

#define HS3282_READ_EN1       (0x80000)
#define HS3282_READ_EN2       (0x80002)
/* tx control */
#define HS3282_MR_GPIOD1      (0x1)
#define HS3282_ENTX_GPIOA1    (0x1)
#define HS3282_TXR_GPIOA2     (0x2)


/*rx flag*/
#define HS3282_DR1_GPIOA3     (0x3)
#define HS3282_DR2_GPIOA4     (0x4)
#define HS3282_SEL_GPIOD0     (0x0)

#define HS3282_CWSR_CFG       (0x20)

#define HS3282_TX_DAT1        (0x5455)
#define HS3282_TX_DAT2        (0xAAAA)



/* MAX7129 */
#define MAX7221_NO_OP_ADDR (0x0)
#define MAX7221_DIG0_ADDR (0x1)
#define MAX7221_DIG1_ADDR (0x2)
#define MAX7221_DIG2_ADDR (0x3)
#define MAX7221_DIG3_ADDR (0x4)
#define MAX7221_DIG4_ADDR (0x5)
#define MAX7221_DIG5_ADDR (0x6)
#define MAX7221_DIG6_ADDR (0x7)
#define MAX7221_DIG7_ADDR (0x8)

#define MAX7221_DECODEMOD_ADDR (0x9)
#define MAX7221_INTENSE_ADDR (0xA)
#define MAX7221_SCAN_ADDR (0xB)
#define MAX7221_SHUTDOWN_ADDR (0xC)
#define MAX7221_DISTEST_ADDR (0xF)

/* no decode */
#define MAX7221_DECODE_CFG (0x0)
/*no shutdown */
#define MAX7221_SHUTDOWN_CFG (0x1)
/* intense medium */
#define MAX7221_INTENSE_CFG (0x9)
/* scan limit all scall */
#define MAX7221_SCAN_CFG (0x7)
/* display test mode =normal*/
#define MAX7221_DISPLAYTEST_CFG (0x0)

/*ON all LED */
#define MAX7221_TEST_ON  (0xFF)
/*OFF all LED */
#define MAX7221_TEST_OFF  (0x0)



/* watchdog */
#define WATCHDOG_WDI_GPIOA0 (0)
#define WATCHDOG_WD_EN_GPIOA5 (5)



/* Freq  */
#define DSP_FREQ (150*1000000)



/* CSM KEY */
#define PRG_KEY0  0xFFFF
#define PRG_KEY1  0xFFFF
#define PRG_KEY2  0xFFFF
#define PRG_KEY3  0xFFFF
#define PRG_KEY4  0xFFFF
#define PRG_KEY5  0xFFFF
#define PRG_KEY6  0xFFFF
#define PRG_KEY7  0xFFFF

#define CSMSCR       (volatile UINT16*)0x00000AEF   // CSM status and control register 
#define KEY0         (volatile UINT16*)0x00000AE0   /* low word of the 128-bit key */
#define KEY1         (volatile UINT16*)0x00000AE1   /* next word in 128-bit key */
#define KEY2         (volatile UINT16*)0x00000AE2   /* next word in 128-bit key */
#define KEY3         (volatile UINT16*)0x00000AE3   /* next word in 128-bit key */
#define KEY4         (volatile UINT16*)0x00000AE4   /* next word in 128-bit key */
#define KEY5         (volatile UINT16*)0x00000AE5   /* next word in 128-bit key */
#define KEY6         (volatile UINT16*)0x00000AE6   /* next word in 128-bit key */
#define KEY7         (volatile UINT16*)0x00000AE7   /* high word of the 128-bit key */
#define PWL0         (volatile UINT16*)0x003F7FF8   /* Password 0 */
#define PWL1         (volatile UINT16*)0x003F7FF9   /* Password 1 */
#define PWL2         (volatile UINT16*)0x003F7FFA   /* Password 2 */
#define PWL3         (volatile UINT16*)0x003F7FFB   /* Password 3 */
#define PWL4         (volatile UINT16*)0x003F7FFC   /* Password 4 */
#define PWL5         (volatile UINT16*)0x003F7FFD   /* Password 5 */
#define PWL6         (volatile UINT16*)0x003F7FFE   /* Password 6 */
#define PWL7         (volatile UINT16*)0x003F7FFF   /* Password 7 */






#endif


