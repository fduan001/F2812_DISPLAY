#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"
#include "DSP28_Device.h"
#include "boardcfg.h"

#define RS422_USR_INTR_MODE 1

#ifdef RS422_USR_INTR_MODE
typedef int BOOL;

#ifndef ERROR
#define ERROR   (-1)
#endif

#ifndef OK
#define OK    (0)
#endif

#define RX_LEN   256

#pragma DATA_SECTION   (rxBuffer,"shell_lib");
#pragma DATA_SECTION   (msgrd,"shell_lib");
#pragma DATA_SECTION   (msgwr,"shell_lib");
#pragma DATA_SECTION   (rxSemSync,"shell_lib");

UINT8   rxBuffer[RX_LEN];
UINT32  msgrd,msgwr;
HANDLE  rxSemSync;  

#define HOST_RX_INT_NUM   98
#endif

#ifdef RS422_USR_INTR_MODE
void HostUartIsr(void)
{
	Osal_DisableIsr(HOST_RX_INT_NUM);
	while(ScibRx_Ready()) {
		if( ((msgwr + 1) % RX_LEN) != msgrd ) {
			rxBuffer[msgwr] = (UINT8)(ScibRegs.SCIRXBUF.all);
			msgwr = (msgwr + 1) % RX_LEN;
		}
	}
	Osal_SemPost(rxSemSync);
	ScibRegs.SCIFFRX.bit.RXOVF_CLR=1;   // Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	Osal_EnableIsr(HOST_RX_INT_NUM);
    return;
}
#endif

INT32 HostUartInit(void) {
    //SCI B 
    ScibRegs.SCIFFTX.all=0x8000;

     ScibRegs.SCICTL1.all = 0x0;
    /*   8 bit data */
    ScibRegs.SCICCR.all = SCICCR_CFG;

    ScibRegs.SCICTL1.all = SCICTL1_CFG;
    ScibRegs.SCICTL2.all = SCICTL2_REG;

    ScibRegs.SCIHBAUD = (SCI_BAUD >> 8) & 0xff;
    ScibRegs.SCILBAUD = (SCI_BAUD) & 0xFF;

    ScibRegs.SCIFFTX.all=0xC028;
#ifdef RS422_USR_INTR_MODE
    ScibRegs.SCIFFRX.all=0x0021;
#else
    ScibRegs.SCIFFRX.all=0x0028;
#endif

	ScibRegs.SCIFFCT.all=0x00;

    /* enable SCI*/
    ScibRegs.SCICTL1.all = (SCICTL1_CFG | 0x20);
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
    
#ifdef RS422_USR_INTR_MODE
	memset(rxBuffer, 0, sizeof(rxBuffer));
	msgwr = 0;
	msgrd=0;
	rxSemSync = Osal_SemCreateBinary(0);
	if( rxSemSync == NULL ) {
		PRINTF("Failed to create semaphore for ISR sync with user handler\n");
		return 1;
	}

	Osal_InstallPIEIsr(HostUartIsr, HOST_RX_INT_NUM);
	Osal_EnableIsr(HOST_RX_INT_NUM);
#endif

	return 0;
}

INT32 HostUartSend(UINT8* buffer, UINT32 length) {
	int ret = 0;
	ret = UartWrite_B(buffer, length);
	if( ret != length) {
		PRINTF("Send buffer failed, ret=%d\n", ret);

	}
	return ret;
}

INT32 HostUartRecv(UINT8* buffer, UINT32 length) {
	int ret = 0;
#ifdef RS422_USR_INTR_MODE
	UINT32 rdBytes = 0;
    if(ERROR == Osal_SemPend(rxSemSync, ~(0)))
    {
        return(ERROR);
    }

    while( (rdBytes < length) && (msgwr != msgrd) ) {
    	buffer[rdBytes] = rxBuffer[msgrd];
    	msgrd = (msgrd + 1) % RX_LEN;
    	rdBytes++;
    }

    return rdBytes;
#else
	ret = UartRead_B(buffer, length);
	if( ret != length ) {
		PRINTF("failed to read from uart B, ret=%d\n", ret);
		return -1;
	}
#endif
	return ret;
}
