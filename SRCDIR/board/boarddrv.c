/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : platform_os.c
 * Author        : yexin
 * Date          : 2018-07-27
 * Description   : board driver
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-07-27
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include "boarddrv.h"
#include "boardcfg.h"

//delay unit is 500ms
void PlaftormDelay500ms(UINT16 delaycnt)
{
    UINT32 timebase = GetTimer2Cnt();
    while((GetTimer2Cnt() - timebase) <= delaycnt);

}

void PlatformResetTimer ()
{
}

UINT16 PlatformGetTimer ()
{
	return 0;
}

void InitPeripherals(void)
{
    // Initialize External Interface To default State:   
       InitXintf();	
    // Initialize gpio    
       InitGpio();	
    // Initialize SCI Peripherals To default State:    
       InitSci();


}

void InitXint(void)
{
#define XINT1_REG   0x7070
#define XINT2_REG   0x7071

	// enable XIN1 & XINT2
	*(volatile UINT16*)XINT1_REG = 0x1;
	*(volatile UINT16*)XINT2_REG = 0x1;
}


void BoardInit()
{
    /*初始化系统*/
    InitSysCtrl();
#if 0

    /* 关中断 */
    DINT;
    IER = 0x0000;
    IFR = 0x0000;

    /* 初始化PIE控制寄存器 */
    InitPieCtrl();
    /* 初始化PIE参数表 */
    InitPieVectTable();
#endif	


    /* 初始化外设寄存器 */
    InitPeripherals();
    InitXint();
#ifndef HOST_PC
    if(0 == UnlockCsm())
        UartPrintf("unlock csm for flash programing sucessfully \n");
    else
    {
        UartPrintf("unlock csm for flash programing failure,reset the board \n");
    }
#endif
    UartPrintf("register shell command \n");

    //I2cCmdInitialzie();
	FpgaCmdInitialzie();
}
