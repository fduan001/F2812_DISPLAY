/*
 * dsparch.c
 *
 *  Created on: 2014Äê8ÔÂ12ÈÕ
 *      Author: Administrator
 */

#include "F2812_datatype.h"


#ifdef HOST_PC

#define UDELAY
#define RESETIMER

#else
#include "stdint.h"
//#include "c66x_fpga.h"
extern void platform_c66x_delay(UINT32  usecs);
extern void platform_resettimer();
extern UINT32 platform_gettimer(UINT32 base);

extern void PlatformDelay (UINT16 usec);
extern void PlatformResetTimer();
extern UINT16  PlatformGetTimer();


#if 0
#define UDELAY                platform_c66x_delay
#define RESETIMER             platform_resettimer
#define GETTIMER              platform_gettimer
#endif
#define UDELAY                PlatformDelay 
#define RESETIMER             PlatformResetTimer
#define GETTIMER              PlatformGetTimer


#endif




void sync()
{

}

/* it depends on cpu architecture and hardware design  ,need 1ms resolution timer */


void reset_timer()
{
#ifdef HOST_PC
#else
    RESETIMER();
#endif
}
UINT32 get_timer (UINT32 base)
{
#ifdef HOST_PC
    return 0;
#else
    return(GETTIMER(base));
#endif
}






void udelay(UINT32 usec)
{

    UDELAY( usec);
}

int disable_interrupts(void)
{
    return 0;
}

void enable_interrupts (void)
{

}






void *map_physmem(phys_addr_t paddr, UINT32 len, UINT32 flags)
{
    return (void *)paddr;
}

