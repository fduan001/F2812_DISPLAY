/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : platform_os.c
 * Author        : yexin
 * Date          : 2018-07-27
 * Description   : platform os
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-07-27
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/knl/Clock.h> 
#include <ti/sysbios/knl/Queue.h> 


#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/family/c28/Hwi.h>
#include <ti/sysbios/family/c28/Timer.h> 



#include "F2812_datatype.h"
#include "platform_os.h"



//queue




#define OSAL_PLATFORM_LOG_ERROR  shellprintf


//--------------------------------------------------------------------
// Osal_TaskSelf()
//
// Return handle of task itself
//--------------------------------------------------------------------
HANDLE Osal_TaskSelf()
{
    return ((HANDLE)Task_self());
}


//--------------------------------------------------------------------
// Osal_TaskSetPri()
//
// Set a task's priority
//--------------------------------------------------------------------
INT16 Osal_TaskSetPri(HANDLE h, UINT16 priority)
{
    return (Task_setPri((Task_Handle)h, (Int)priority));
}


//--------------------------------------------------------------------
// Osal_TaskGetPri()
//
// Get a task's priority
//--------------------------------------------------------------------
INT16 Osal_TaskGetPri(HANDLE h)
{
    return ((INT16)Task_getPri((Task_Handle)h));
}

//--------------------------------------------------------------------
// Osal_TaskSleep()
//
// Put a task into sleep
//--------------------------------------------------------------------
void Osal_TaskSleep(UINT16 delay)
{
    Task_sleep((UInt)delay);
}


//--------------------------------------------------------------------
// Osal_TaskYield()
//
// Yield task
//--------------------------------------------------------------------
void Osal_TaskYield()
{
    Task_yield();
}

//--------------------------------------------------------------------
// Osal_TaskExit()
//
// Exits and destroys a task
//--------------------------------------------------------------------
void Osal_TaskExit()
{
    Task_exit();
}

//--------------------------------------------------------------------
// Osal_TaskCreate()
//
// Create a task
//--------------------------------------------------------------------
/* ARGSUSED */



HANDLE Osal_TaskCreate( void(*pFun)(), INT8 *Name, UINT16 Priority, UINT16 StackSize,
                        UINT16 Arg1, UINT16 Arg2, UINT16 Arg3 )
{
    Task_Params params;
    Task_Handle htsk;

    Task_Params_init(&params);
    params.instance->name = Name;
    params.priority = Priority;
    params.stackSize = StackSize;
    params.arg0 = Arg1;
    params.arg1 = Arg2;

    htsk = Task_create((Task_FuncPtr)pFun, &params, NULL);

    return ((HANDLE)htsk);
}


//--------------------------------------------------------------------
// Osal_TaskDestroy()
//
// Destroy a task
//--------------------------------------------------------------------
void Osal_TaskDestroy( HANDLE h )
{
    if (h == Osal_TaskSelf())
    {
        Osal_TaskExit();
    }
    else
    {
        Task_delete((Task_Handle *)&h);
    }
}


/*
 *  ======== Osal_SemCreate ========
 *  Create a semaphore.
 */
HANDLE Osal_SemCreate(UINT16 Count)
{
    Error_Block eb;
    HANDLE sem;

    Error_init(&eb);

    /* create the new semaphore */
    sem = (HANDLE)Semaphore_create((Int)Count, NULL, &eb);

    if (Error_check(&eb))
    {
        /* Should not get here */
        OSAL_PLATFORM_LOG_ERROR("SemCreate(): could not create semaphore");
    }

    return (sem);
}


/*
 *  ======== Osal_SemCreateBinary ========
 *  Create a binary semaphore.
 */
HANDLE Osal_SemCreateBinary(UINT16 Count)
{
    Error_Block eb;
    Semaphore_Params semParams;
    HANDLE sem;

    Error_init(&eb);

    /* create the new semaphore */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    sem = (HANDLE)Semaphore_create((Int)Count, &semParams, &eb);

    if (Error_check(&eb))
    {
        /* Should not get here */
        OSAL_PLATFORM_LOG_ERROR("SemCreateBinary(): could not create semaphore");
    }

    return (sem);
}


/*
 *  ======== Osal_SemDelete ========
 *  Delete a semaphore.
 */
void Osal_SemDelete(HANDLE hSem)
{
    Semaphore_delete((Semaphore_Handle *)&hSem);
}

/*
 *  ======== Osal_SemCount ========
 *  Get the current semaphore count.
 */
INT16 Osal_SemCount(HANDLE hSem)
{
    return ((int)Semaphore_getCount((Semaphore_Handle)hSem));
}


/*
 *  ======== Osal_Osal_SemPend ========
 *  Wait for a semaphore.
 */
INT16 Osal_SemPend(HANDLE hSem, UINT32 Timeout)
{
    return ((int)Semaphore_pend((Semaphore_Handle)hSem, (UInt)Timeout));
}

/*
 *  ======== Osal_Osal_SemPost ========
 *  Signal a semaphore.
 */
void Osal_SemPost(HANDLE hSem)
{
    Semaphore_post((Semaphore_Handle)hSem);
}

/*
 *  ======== Osal_SemReset ========
 *  Reset a semaphore's count.
 */
void Osal_SemReset(HANDLE hSem, UINT16 Count)
{
    Semaphore_reset((Semaphore_Handle)hSem, (Int)Count);
}


/*
 *  ======== SemCreateBinary ========
 *  Create a binary semaphore.
 */
HANDLE Osal_GateMutexPriCreate()
{
    Error_Block eb;
    GateMutexPri_Params gatemutexpriParams;
    HANDLE gatemutexpri;

    Error_init(&eb);

    /* create the new semaphore */
    GateMutexPri_Params_init (&gatemutexpriParams);
    gatemutexpriParams.instance->name = "Instance";
    gatemutexpri = (HANDLE)GateMutexPri_create( &gatemutexpriParams, &eb);

    if (Error_check(&eb))
    {
        /* Should not get here */
        OSAL_PLATFORM_LOG_ERROR("GateMutexPriCreate(): could not create gatemutexpri");
        return NULL;
    }

    return (gatemutexpri);
}

INT16 Osal_GateMutexPriEnter(HANDLE hmutexpri)
{
    return (GateMutexPri_enter(hmutexpri));
}

void Osal_GateMutexPriLeave(HANDLE hmutexpri, UINT16 key)
{
    GateMutexPri_leave(hmutexpri, key);
    return;
}

/******************************************************************************
 *
 * Function:        Osal_CreateTimer
 *
 * Description:       This function create the timer
 *
 * Parameters:      period:       timer period ,unit is us
 *                  ptimerfunc:   timer  interrupt isr
 *
 * Return Value:   timer handle

 *
 *****************************************************************************/
HANDLE Osal_CreateTimer(UINT8* name,UINT32 period, void(*ptimerfunc)(UINT32 ),UINT32 parameter )
{
    Timer_Params timerParams;
    Timer_Handle tmptimer;
    Error_Block eb;
    Error_init(&eb);
    Timer_Params_init(&timerParams);
    timerParams.instance->name = name;
    timerParams.arg=parameter;
    timerParams.period = period;
    timerParams.periodType = Timer_PeriodType_MICROSECS;
    timerParams.startMode = Timer_StartMode_USER;
    timerParams.runMode = Timer_RunMode_CONTINUOUS;

    tmptimer = Timer_create(Timer_ANY, ptimerfunc, &timerParams, &eb);
    if (tmptimer == NULL)
    {
        OSAL_PLATFORM_LOG_ERROR("Timer create failed");
        return NULL;
    }
    else
        return (HANDLE)(tmptimer);
}

/******************************************************************************
 *
 * Function:        Osal_StartTimer
 *
 * Description:       This function start the timer
 *
 * Parameters:       timer :  timer handle
 *
 * Return Value:

 *
 *****************************************************************************/
void  Osal_StartTimer(HANDLE timer )
{
    if(NULL != timer)
        Timer_start((Timer_Handle)timer);
}


/******************************************************************************
 *
 * Function:        Osal_StopTimer
 *
 * Description:       This function stop the timer
 *
 * Parameters:       timer: timer handle
 *
 * Return Value:

 *
 *****************************************************************************/
void  Osal_StopTimer(HANDLE timer )
{
    if(NULL != timer)
        Timer_stop((Timer_Handle)timer);
}



/*
 *  ======== Osal_EnterGlobalCriticalSection ========
 *  global disable interrupt.
 */
void Osal_EnterGlobalCriticalSection()
{
    Hwi_disable();
}

/*
 *  ======== Osal_ExitGlobalCriticalSection ========
 *  global enable interrupt.
 */
void Osal_ExitGlobalCriticalSection()
{
    Hwi_enable();
}



HANDLE Osal_ClockCreate(UINT8* clkname,void(*pClkFun)(UINT32),UINT32 arg,UINT32 timeout,UINT8 mode)
{
	Clock_Params clkparam;
    Error_Block eb;
    Error_init(&eb);
	Clock_Handle tmpclkhandle;
	Clock_Params_init(&clkparam);
	clkparam.instance->name = clkname;
	//one-shot
	if(0==mode)
	{
		clkparam.arg=arg;
	}
	else
	{
		clkparam.arg=arg;
		clkparam.period=timeout;
	}
	tmpclkhandle=Clock_create((Clock_FuncPtr) (pClkFun), (UInt) timeout, &clkparam, &eb);
    if (Error_check(&eb))
    {

        OSAL_PLATFORM_LOG_ERROR("create clock failure \n");
		return NULL;
    }   

	if(NULL==tmpclkhandle)
	{
        /* Should not get here */
        OSAL_PLATFORM_LOG_ERROR("Clock_create(): could not create clock timer");
		return NULL;
	}	
	return (HANDLE)(tmpclkhandle);
}

UINT32 Osal_GetClockTimeout(HANDLE clkhandle)
{
	return Clock_getTimeout((Clock_Handle) clkhandle);
}

void Osal_StartClock(HANDLE clkhandle)
{
	Clock_start((Clock_Handle) clkhandle);
}
void Osal_StopClock(HANDLE clkhandle)
{
	Clock_stop((Clock_Handle) clkhandle);
}


HANDLE Osal_InstallPIEIsr(void(*pIsrFunc)(UINT32),UINT8 intnum)
{
    Hwi_Params hwiParams;
    Error_Block  dspeb;
	Hwi_Handle  isrhandle;

    Hwi_Params_init(&hwiParams);
    Error_init(&dspeb);

    hwiParams.arg = intnum;
    hwiParams.enableInt = FALSE;
    hwiParams.enableAck = 1;
    //hwiParams.maskSetting = Hwi_MaskingOption_BITMASK;
    hwiParams.maskSetting = Hwi_MaskingOption_ALL;
    hwiParams.disableMask = 0x0;
    hwiParams.restoreMask = 0x0;	
    
    isrhandle=Hwi_create(intnum, pIsrFunc, &hwiParams, &dspeb);
    if (Error_check(&dspeb))
    {
        OSAL_PLATFORM_LOG_ERROR("install dsp isr error \n");
		return NULL;
    }     
	if(NULL==isrhandle)
	{
        OSAL_PLATFORM_LOG_ERROR("Hwi_create(): could not create Hwi ISR");
		return NULL;
		
	}
	else
	{
		return (HANDLE)isrhandle;
	}
}

void Osal_EnableIsr(UINT8 intNum)
{
	Hwi_enableInterrupt(intNum);
}

void Osal_DisableIsr(UINT8 intNum)
{
	Hwi_disableInterrupt(intNum);
}

INT16 Osal_DisablePIEIsr(UINT8 intnum)
{
	if(intnum<32)
		return 1;
	UINT8 groupnum = (UINT8)((intnum - 32) / 8) + 1;
	UINT16 piemask = 1 << ((intnum -32) % 8);
	Hwi_disablePIEIER(groupnum, piemask); 
	return 0;
}

INT16 Osal_EnablePIEIsr(UINT8 intnum)
{
	if(intnum<32)
		return 1;
	UINT8 groupnum = (UINT8)((intnum - 32) / 8) + 1;
	UINT16 piemask = 1 << ((intnum -32) % 8);
	Hwi_enablePIEIER(groupnum, piemask); 
	return 0;
}


//queue
HANDLE Osal_QueueCreate()
{
	Queue_Handle tmpq;
	Error_Block  dspeb;
	tmpq=Queue_create(NULL, &dspeb);
    if (Error_check(&dspeb))
	{
        OSAL_PLATFORM_LOG_ERROR("create queue error \n");
		return NULL;		
	}
	return (HANDLE) tmpq;
}

void Osal_Enqueue(HANDLE qhandle, S_QUEUE_ELEMENT usrdata)
{
	
	Queue_enqueue((Queue_Handle) qhandle, &(usrdata.elem)); 
}

INT16 Osal_QueueEmpty(HANDLE qhandle)
{
	return (INT16)Queue_empty((Queue_Handle) qhandle);
}

HANDLE Osal_Dequeue(HANDLE qhandle)
{
	return (HANDLE)Queue_dequeue((Queue_Handle) qhandle); 
}

void Osal_QueuePut(HANDLE qhandle, S_QUEUE_ELEMENT usrdata)
{
	return Queue_put((Queue_Handle) qhandle,&(usrdata.elem));
}

HANDLE Osal_QueueGet(HANDLE qhandle)
{
	return (HANDLE)Queue_get(qhandle);
}



