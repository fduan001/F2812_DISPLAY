/*
 *  ======== hello.c ========
 *  The hello example serves as a basic sanity check program for SYS/BIOS. It
 *  demonstrates how to print the string "hello world" to stdout. 
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>
#include <string.h>
#include <ti/sysbios/BIOS.h>
#include "F2812_datatype.h"
#include "boardcfg.h"
#include "boarddrv.h"
#include "platform_os.h"
#include "global_var.h"
#include "watchdog.h"

/******************************
   option 1
*********************************/

//#define  TEST_CPUCNT
//#define TEST_HARDTIMER
extern void I2CCmdInitialize();
extern int EepromCmdInit(void);
extern int rs422_cmd_init(void);
extern void FpgaCmdInitialzie();
extern  int FpgaSpiCmdInit(void);
extern void AD567XCmdInitialize();
extern void ADS124S08CmdInitialize();
extern void FuelCmdInitialzie();
extern void FlowCmdInitialzie();

/******************************
   option 2
*********************************/

#define  TEST_CPUCNT
// #define  TEST_KEYINT
#define  KEYINT  35

#define   XINT_ISR1    35
#define   XINT_ISR2    36

extern void XINT_Isr1(void);
extern void XINT_Isr2(void);

void MemoryTest(UINT32 startAddr, UINT32 endAddr) 
{
	UINT32 i = 0;
	UINT32 loopCnt = (endAddr - startAddr) / sizeof(UINT16);
	UINT16 pattern = 0x55AA;
	UINT16 val = 0;

	if( startAddr > endAddr ) {
		shellprintf("invalid input %lx %lx\n", startAddr, endAddr);
		return ;
	}

	for( i = 0; i < loopCnt; i+= sizeof(UINT16) ) {
		pattern += (UINT16)i;
		*(volatile UINT16*)(startAddr + i) = pattern;
		val = *(volatile UINT16*)(startAddr + i);
		if( val != pattern ) {
			shellprintf("MEMTEST Failed@%lx: %x %x\n", startAddr + i, pattern, val);
			return ;
		}
		pattern = ~pattern;
	}
	shellprintf("MEMTEST PASSED\n");
}

void StackTest()
{
#ifdef TASK_TEST
	while(1)
	{
		while(index--)
		UartPrintf("stack task is running %d\n",index);
		index=10;
		//Osal_TaskYield();
		Osal_TaskSleep(3000);
	}
#endif

#ifdef TEST_HARDTIMER
     while(1)
     {
	Osal_SemPend(semhandle,BIOS_WAIT_FOREVER);
	testfar1[1]++;
      
      UartPrintf("get semphore \n");
      PlatformDelay(4000);
      UartPrintf("get semphore2 \n");     
     }

#endif

#ifdef TEST_KEYINT
 	while(1)
 	{
		Osal_SemPend(semhandle,BIOS_WAIT_FOREVER);
		testfar1[2]++;
  		
  		UartPrintf("get key  \n");  
 	}


#endif


}

#ifdef TEST_CLOCK
void ClkFunc(UINT32 cnt)
{
	UartPrintf("clock is triggered \n");
}
#endif

#ifdef TEST_HARDTIMER
void TimerFunc(UINT32 cnt)
{
	Osal_SemPost(semhandle);
	testfar1[0]++;
}
#endif

#ifdef TEST_KEYINT
unsigned  int  *LedADDR  = (unsigned int *)0x2800;
UINT16	Temp;
void KeyIntFunc(UINT32 key)
{
	//Temp^=0xFF;
	//*LedADDR = Temp;
	Osal_SemPost(semhandle);
	testfar1[0]++;	
}
#endif

void ShellTask()
{
    UartPrintf("shell task is running\n");
    set_shellprompt(CONFIG_BOARD_PROMPT);
    WatchdogKick();

    I2CCmdInitialize();
    EepromCmdInit();
    MidCmdInitialize();
    FpgaSpiCmdInit();
    AD567XCmdInitialize();
    ADS124S08CmdInitialize();
    FuelCmdInitialzie();
    FlowCmdInitialzie();
    WatchdogKick();
    WatchdogCmdInit();
    WatchdogKick();

	Osal_InstallPIEIsr(XINT_Isr1, XINT_ISR1); /* register DDC interrupt for 1553B */
	Osal_InstallPIEIsr(XINT_Isr2, XINT_ISR2); /* register MBI interrupt for HOST ACCESS DPRAM */

	// enable the interrupts
	Osal_EnableIsr(XINT_ISR2);
	Osal_EnableIsr(XINT_ISR1);

	WatchdogKick();
	shell_loop();
}

/*
 *  ======== main ========
 */
Void main()
{ 
     // System_printf("hello world\n");

	BoardInit();
	MemoryTest(0x2000, 0x2000 + (8 * 1024) - 2); /* test 8K memory */

	//DdcInit(DDC_MODE_RT, 0);
	UartPrintf("Init HW WATCHDOG\n");
	WatchdogInit();
	UartPrintf("Enable HW WATCHDOG\n");
	WatchdogEnable();
	WatchdogKick();
	UartPrintf("create shell task\n");
	WatchdogKick();
	Osal_TaskCreate( ShellTask, "ShellTask", 1,  0x2000, (UINT16)0, (UINT16)0, 0 );
	WatchdogKick();
	Osal_TaskCreate( WatchdogTask, "wdogTask", 2,  0x400, (UINT16)0, (UINT16)0, 0 );
	WatchdogKick();

	BIOS_start ();

    /*
     *  normal BIOS programs, would call BIOS_start() to enable interrupts
     *  and start the scheduler and kick BIOS into gear.  But, this program
     *  is a simple sanity test and calls BIOS_exit() instead.
     */
//    BIOS_exit(0);  /* terminates program and dumps SysMin output */
}
