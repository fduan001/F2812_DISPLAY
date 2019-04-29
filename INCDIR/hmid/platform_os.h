/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : boarddrv.h
 * Author        : yexin technologic
 * Date          : 2018-6-27
 * Description   : platform osal function
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-6-27
 *   Author      : yexin technologic
 *   Modification: Created file

*************************************************************************************************************/
#ifndef PLATFORM_OS_H
#define PLATFORM_OS_H
//#include <xdc/std.h>
#include <ti/sysbios/knl/Queue.h>
#include "F2812_datatype.h"



//task

HANDLE Osal_TaskSelf();
INT16 Osal_TaskSetPri(HANDLE h, UINT16 priority);
INT16 Osal_TaskGetPri(HANDLE h);
void Osal_TaskSleep(UINT16 delay);
void Osal_TaskYield();
void Osal_TaskExit();
HANDLE Osal_TaskCreate( void(*pFun)(), INT8 *Name, UINT16 Priority, UINT16 StackSize,
                        UINT16 Arg1, UINT16 Arg2, UINT16 Arg3 );

//semphore
HANDLE Osal_SemCreate(UINT16 Count);
HANDLE Osal_SemCreateBinary(UINT16 Count);
void Osal_SemDelete(HANDLE hSem);
INT16 Osal_SemCount(HANDLE hSem);
INT16 Osal_SemPend(HANDLE hSem, UINT32 Timeout);
void Osal_SemPost(HANDLE hSem);
void Osal_SemReset(HANDLE hSem, UINT16 Count);

//mutex
void Osal_GateMutexPriLeave(HANDLE hmutexpri, UINT16 key);
void Osal_GateMutexPriLeave(HANDLE hmutexpri, UINT16 key);
HANDLE Osal_GateMutexPriCreate();


//critical
void Osal_EnterGlobalCriticalSection();
void Osal_ExitGlobalCriticalSection();

//clock
HANDLE Osal_ClockCreate(UINT8* clkname,void(*pClkFun)(UINT32),UINT32 arg,UINT32 timeout,UINT8 mode);
void Osal_StartClock(HANDLE clkhandle);
void Osal_StopClock(HANDLE clkhandle);
UINT32 Osal_GetClockTimeout(HANDLE clkhandle);

//timer
HANDLE Osal_CreateTimer(UINT8* name,UINT32 period, void(*ptimerfunc)(UINT32 ),UINT32 parameter );
void  Osal_StartTimer(HANDLE timer );
void  Osal_StopTimer(HANDLE timer );


//PIE ISR

extern HANDLE Osal_InstallPIEIsr(void(*pIsrFunc)(UINT32),UINT8 intnum);
extern INT16 Osal_DisablePIEIsr(UINT8 intnum);
extern INT16 Osal_EnablePIEIsr(UINT8 intnum);
extern void Osal_EnableIsr(UINT8 intNum);
extern void Osal_DisableIsr(UINT8 intNum);
extern void MidCmdInitialize();

//queue
typedef struct S_QUEUE_ELEMENT
{
	 Queue_Elem elem;
    UINT32 data;
}  S_QUEUE_ELEMENT;

HANDLE Osal_QueueCreate();
void Osal_Enqueue(HANDLE qhandle, S_QUEUE_ELEMENT usrdata);
INT16 Osal_QueueEmpty(HANDLE qhandle);
HANDLE Osal_Dequeue(HANDLE qhandle);
HANDLE Osal_QueueGet(HANDLE qhandle);
void Osal_QueuePut(HANDLE qhandle, S_QUEUE_ELEMENT usrdata);



#endif

