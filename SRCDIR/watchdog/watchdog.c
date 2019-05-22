#include "DSP28_Device.h"
#include "F2812_datatype.h"
#include "shellconsole.h"
#include "F2812_gpio.h"
#include "watchdog.h"
#include "boardcfg.h"
#include "util.h"

#pragma DATA_SECTION   (g_wdog_counter,  "shell_lib");
static UINT16 g_wdog_counter = 0;
static UINT8 g_kick_dog = 1;

void WatchdogInit(void)
{
	EALLOW;
	GpioMuxRegs.GPBMUX.bit.CAP5Q2_GPIOB9 = 0;
	GpioMuxRegs.GPBMUX.bit.CAP6QI2_GPIOB10 = 0;
	GpioMuxRegs.GPBDIR.bit.GPIOB9 = GPIO_DIR_OUTPUT;
	GpioMuxRegs.GPBDIR.bit.GPIOB10 = GPIO_DIR_OUTPUT;
	EDIS;
	g_kick_dog = 1;
}

void WatchdogEnable(void)
{
	GpioDataRegs.GPBCLEAR.bit.GPIOB9 = 1;
}

void WatchdogDisable(void)
{
	GpioDataRegs.GPBSET.bit.GPIOB9 = 1;
}

void WatchdogKick(void)
{
	if( g_kick_dog ) {
		++g_wdog_counter;
		if( (g_wdog_counter % 2) == 0 ) {
			GpioDataRegs.GPBCLEAR.bit.GPIOB10 = 1;
		} else {
			GpioDataRegs.GPBSET.bit.GPIOB10 = 1;
		}
	}
}

void WatchdogShow(void)
{
	shellprintf("counter=0x%x\n", g_wdog_counter);
}

void WatchdogReset(void) {
	g_kick_dog = 0;
}

void WatchdogTask(void)
{
	// WatchdogInit();
	while(1) {
		WatchdogKick();
		Osal_TaskSleep(300);
	}
}
