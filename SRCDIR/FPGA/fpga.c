/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : fpga.c
 * Author        : yexin
 * Date          : 2018-07-27
 * Description   : fpga function
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-07-27
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include <stdio.h>
#include "F2812_datatype.h"
#include "boardcfg.h"
#include "fpga.h"

void WriteFpgaRegister( UINT32 regaddr, UINT16 regvalue)
{
    volatile UINT16 *regPtr;

    regPtr = (volatile UINT16 *)regaddr;
    *(regPtr) = regvalue;
}

UINT16 ReadFpgaRegister( UINT32 regaddr )
{
    volatile UINT16 *regPtr;
    regPtr = (volatile UINT16 *)regaddr;
    return ((*regPtr) & 0xffff);
}



void WriteFpgaRegisterBit(UINT32 regaddr, UINT8 bitpos, UINT8 bitvalue)
{

    UINT16 regValue;
    regValue = ReadFpgaRegister(regaddr);
    if(0 == bitvalue)
        regValue = regValue & ( ~(1 << bitpos));
    else
        regValue = regValue | (1 << bitpos);
    WriteFpgaRegister(regaddr, regValue);

}


UINT8 ReadFpgaRegisterBit(UINT32 regaddr, UINT8 bitpos, UINT8 bitvalue)
{

    UINT16 regValue;
    regValue = ReadFpgaRegister(regaddr);
    return((regValue >> bitpos) & 0x1);
}


typedef struct isr_info_t {
    UINT16       counter;
    ISR_HANDLER  isr_handler;
} isr_info_t;

#define MAX_IRQ_NUM          8

static isr_info_t g_isr_info[MAX_IRQ_NUM];

int RegisterIsr(int bit_pos, ISR_HANDLER isr) {
    if(bit_pos >= MAX_IRQ_NUM) {
        return 1;
    }

    g_isr_info[bit_pos].isr_handler = isr;
    g_isr_info[bit_pos].counter = 0;
    return 0;
}

void XINT_Isr1(void) {
    UINT16 isr_sts = FPGA_REG16_R(FPGA_XINT1_STATUS_REG);
    UINT16 isr_mask = FPGA_REG16_R(FPGA_XINT1_MASK_REG);
    UINT16 args = 0x8;

    if( isr_sts & 0x1 ) {
        args = 0x0;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[0].isr_handler ) { 
            (g_isr_info[0].isr_handler)(&args);
        }
        g_isr_info[0].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x2 ) {
        args = 0x1;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[1].isr_handler ) {
            (g_isr_info[1].isr_handler)(&args);
        }
        g_isr_info[1].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x4 ) {
        args = 0x2;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[2].isr_handler ) {
            (g_isr_info[2].isr_handler)(&args);
        }
        g_isr_info[2].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x8 ) {
        args = 0x3;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[3].isr_handler ) {
            (g_isr_info[3].isr_handler)(&args);
        }
        g_isr_info[3].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x10 ) {
        args = 0x4;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[4].isr_handler ) {
            (g_isr_info[4].isr_handler)(&args);
        }
        g_isr_info[4].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x20 ) {
        args = 0x5;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[5].isr_handler ) {
            (g_isr_info[5].isr_handler)(&args);
        }
        g_isr_info[5].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x40 ) {
        args = 0x6;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[6].isr_handler ) {
            (g_isr_info[6].isr_handler)(&args);
        }
        g_isr_info[6].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    if( isr_sts & 0x80 ) {
        args = 0x7;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask & ~(1 << args)));
        if( g_isr_info[7].isr_handler ) {
            (g_isr_info[7].isr_handler)(&args);
        }
        g_isr_info[7].counter++;
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (isr_mask | (1 << args)));
    }

    FPGA_REG16_W(FPGA_XINT1_STATUS_REG, isr_sts);
}

void XINT_Isr2(void) {
    // used for master/slave role switch
}
