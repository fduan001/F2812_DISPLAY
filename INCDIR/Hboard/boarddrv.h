/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : boarddrv.h
 * Author        : yexin technologic
 * Date          : 2018-6-27
 * Description   : .C file function description
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-6-27
 *   Author      : yexin technologic
 *   Modification: Created file

*************************************************************************************************************/
#ifndef BOARD_DRV_H
#define BOARD_DRV_H
#include "F2812_datatype.h"

#define BSP_OK 0
#define BSP_FAIL 1


/*---- shared global function prototypes -----------------------------------*/
extern void InitAdc(void);
extern void InitDevEmu(void);
extern void InitDevice(void);
extern void InitECana(void);
extern void InitEv(void);
extern void InitGpio(void);
extern void InitMcbsp(void);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitSci(void);
extern void InitSpi(void);
extern void InitSysCtrl(void);
extern void InitXintf(void);
extern void InitXIntrupt(void);



//csm
UINT16 UnlockCsm(void);


//gpio
UINT16 GpioPortRead(UINT8 portid );
void GpioPortWrite(UINT8 portid , UINT16 portdata);
void InitPeripherals(void);
void InitXintf(void);
void GpioPortSetBitHigh(UINT8 portid, UINT8 bitnum);
void GpioPortSetBitLow(UINT8 portid, UINT8 bitnum);
UINT8 GpioPortReadBit(UINT8 portid, UINT8 bitnum);
extern void KickDog(void);

//uart 
void UartCharPut(UINT8 byteTx);
INT16 SciaRx_Ready(void);
INT16 SciaTx_Ready(void);
void UartPrintf(const char *fmt, ...);
UINT8 UartGetc(void);
UINT8  UartCharGetTimeout(UINT32 timeout, int *result);
void UartPutbuffer(UINT8 *s);

UINT8 UartCharGetTimeout_B(UINT32 timeout, int *result);
INT32 UartWrite_B(UINT8 *buffer, UINT32 length);
INT32 UartRead_B(UINT8* buffer, UINT32 length);

//spi
void SpiWriteData(UINT16 data);
UINT16 SpiReadData(void);




//delay 
UINT32 GetTimer2Cnt ();
void Delay500ms(unsigned int delaycnt);
void UDelay(UINT16 delay);
void SetCpuUsTimer2();
void PlatformDelay (UINT16 usec);




//shell function
extern UINT8 set_shellprompt (char* p);
void shell_loop (void);


//FPGA function
void WriteFpgaRegister( UINT32 regaddr, UINT16 regvalue);
UINT16 ReadFpgaRegister( UINT32 regaddr );
void WriteFpgaRegisterBit(UINT32 regaddr, UINT8 bitpos, UINT8 bitvalue);
UINT8 ReadFpgaRegisterBit(UINT32 regaddr, UINT8 bitpos, UINT8 bitvalue);
void FpgaCmdInitialzie();


//rtc driver
UINT8 RtcRead (UINT8 reg);
void  RtcWrite (UINT8 reg, UINT8 val);
void I2cCmdInitialzie();


//board init
void BoardInit();





#endif
