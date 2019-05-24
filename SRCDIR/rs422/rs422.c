#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"
#include "rs422.h"
#include "fpga.h"

#if 0
#define RS422_USR_INTR_MODE   1
#endif

typedef int BOOL;

#ifndef ERROR
#define ERROR   (-1)
#endif

#ifndef OK
#define OK    (0)
#endif

#define RX_LEN       256
#define RS422_NUM    1

#define   RS422_CHIP1_resetBit   0
#define   RS422_CHIP2_resetBit   1


#define  RS422_CHIP1_intrBit      0
#define  RS422_CHIP2_intrBit      1

#define   RS422_BASE_ADDR    (0x80000 + 0x70)

#define UART_REG(reg, pchan) \
(*(volatile unsigned short *)(((UINT32)(pchan->baseAddr)) + (reg)))


typedef struct{
    BOOL     isOpen;

#ifdef RS422_USR_INTR_MODE
    UINT8    RXBuffer[RX_LEN]; // [# of buffers][RX_LEN bytes]
    UINT32   msgrd;
    UINT32   msgwr;
    HANDLE   rxSemSync;
#endif
    
    UINT32 baseAddr;
    UINT8 resetBit;
    UINT8 intrBit;
}  UART_CTX_T;

INT32       uartintUserCnt = 0;
UART_CTX_T  gUartCtx[RS422_NUM];

UINT8 gUartResetBit[RS422_NUM] = {
    RS422_CHIP2_resetBit,
};

UINT8 gUartIntrBit[RS422_NUM] = {
    RS422_CHIP2_intrBit,
};

UINT8 BitPos2Chan(UINT8 bit_pos) {
    UINT8 i = 0;
    for( i = 0; i < RS422_NUM; ++i ) {
        if( gUartIntrBit[i] == bit_pos ) {
            return i;
        }
    }

    return 0xFF;
}

void RS422SysDataInit(void) {
	memset(gUartCtx, 0, sizeof(gUartCtx));
}

void uartReset(UINT8 chipNo);
INT32 uartTransBytes(UART_CTX_T *  pdevFd, INT8 *pBuf, INT32 nBytes);

#ifdef RS422_USR_INTR_MODE
void uartRecvHandle(UART_CTX_T * pDev);
INT32 uartRecvBytes(UART_CTX_T *  pdevFd, INT8 *pBuf, INT32 nBytes);
#else
INT32 uartPollRead(UART_CTX_T * pDev, INT8 *pBuf, INT32 nBytes);
#endif

void DebugSysReg(void)
{
    shellprintf("%08lx: = 0x%04x\r\n",  FPGA_PERIPHERAL_RST_REG,  FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG));
    shellprintf("%08lx = 0x%04x\r\n",  FPGA_XINT1_STATUS_REG,   FPGA_REG16_R(FPGA_XINT1_STATUS_REG));
    shellprintf("%08lx = 0x%04x\r\n",  FPGA_XINT1_MASK_REG,   FPGA_REG16_R(FPGA_XINT1_MASK_REG));
}
void DebugUartReg(UART_CTX_T *pdevFd)
{
    if(pdevFd == NULL)
        return;
    shellprintf("%08lx:IER = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(IER,pdevFd));
    shellprintf("%08lx:IIR = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(IIR,pdevFd));
    shellprintf("%08lx:LSR = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(LSR,pdevFd));
    shellprintf("%08lx:LCR = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(LCR,pdevFd));
}

void DebugUartRegInfo(UINT8 chipNo)
{
    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    if(chipNo >= RS422_NUM)
        return;

    // PRINTF("SYS INT REG = 0x%x\r\n",FPGA_REG16_R(SYS_FPGA_INTF_REG));
    DebugSysReg();
    DebugUartReg(pdevFd);
}

#ifdef RS422_USR_INTR_MODE
void RS422Isr(void* data)
{
    UART_CTX_T *pdevFd = NULL;
    UINT8 channel = 0;
    UINT8 bit_pos = *(UINT8*)(data);

    FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) & (~( 1 << bit_pos))));
    channel = BitPos2Chan(bit_pos);

    pdevFd = &gUartCtx[channel];

    if(pdevFd == NULL || pdevFd->isOpen != TRUE) {
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << bit_pos)));
        return;
    }

    if((FPGA_REG16_R(FPGA_XINT1_STATUS_REG) & (1 << pdevFd->intrBit)) == 0) {
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << bit_pos)));
        return;
    }

    if((UART_REG(IIR, pdevFd) & IIR_INT) != IIR_INT)
    {
        uartRecvHandle(pdevFd);	    	  
    }

    FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << bit_pos)));

    return;
}
#endif


INT32 RS422Open(UINT8 chipNo, INT8 party, UINT8 stop, UINT8 data_bit, UINT32 baud)
{
    INT32 ret = OK;
    UINT8 irqnum = 0;

    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    if(chipNo >= RS422_NUM)
        return(ERROR);

    if(pdevFd->isOpen == TRUE) {
    	PRINTF("is already open\n");
        return(ERROR);
    }

    irqnum = gUartIntrBit[chipNo];
    RS422Init(chipNo);

    if(RS422SetOpt(chipNo, party, stop, data_bit) != OK)
        return(ERROR);


    if(RS422SetBaud(chipNo, baud) != OK)
        return(ERROR);

#ifdef RS422_USR_INTR_MODE
    pdevFd->rxSemSync = Osal_SemCreateBinary(0);

    if(pdevFd->rxSemSync == NULL)
    {
        return ERROR;
    }
#endif

    pdevFd->isOpen = TRUE;

    UART_REG(IER, pdevFd) = IER_RECV_VALID; //EN RECV DATA INT

#ifdef RS422_USR_INTR_MODE
    if( RegisterIsr(irqnum, RS422Isr) != 0 )
    {
    	PRINTF("Register RS422 irq ......[irq = %u]\n",  irqnum);
    	return ERROR;
    }   

    FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << irqnum)));
#else
    FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) & ~(0x01 << irqnum)));
#endif

    if(ret) {
        PRINTF("RS422Open success....\r\n");
    }

    DebugSysReg();

    DebugUartReg(pdevFd);

    return ret;
}

INT32 RS422Close(UINT8 chipNo)
{
    INT32 ret = OK;

    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    if(chipNo >= RS422_NUM) {
        return(ERROR);
    }

    if(pdevFd->isOpen != TRUE) {
        return(ERROR);
    }

    UART_REG(IER, pdevFd) = 0;

#ifdef RS422_USR_INTR_MODE
    if(pdevFd->rxSemSync != NULL) {
        Osal_SemDelete(pdevFd->rxSemSync );
        pdevFd->rxSemSync  = NULL;
    }     

    FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << irq_num)));
#endif

    memset(pdevFd, 0,  sizeof(UART_CTX_T));

    pdevFd->isOpen = FALSE;

    return ret;
}

INT32 RS422Read(UINT8 chipNo, INT8 * buf, UINT32 nBytes)
{
    UINT32 readNum = 0;
    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    if(chipNo >= RS422_NUM) {
        return(ERROR);
    }

    if(pdevFd->isOpen != TRUE) {
        return(ERROR);
    }

#ifdef RS422_USR_INTR_MODE
    if(ERROR==Osal_SemPend(pdevFd->rxSemSync, ~(0))) {	
        return(ERROR);
    }

    while(((pdevFd->msgrd)!=(pdevFd->msgwr)) && (readNum < nBytes)) {
        buf[readNum] = pdevFd->RXBuffer[pdevFd->msgrd];
        pdevFd->msgrd++;
        if(pdevFd->msgrd == RX_LEN) {
            pdevFd->msgrd = 0;
        }
        readNum++;
    }
    if((pdevFd->msgrd)!=(pdevFd->msgwr)) {
        Osal_SemPost(pdevFd->rxSemSync);
    }
#else
    readNum = uartPollRead(pdevFd, buf, nBytes);
#endif

    return readNum;
}

INT32 RS422Write(UINT8 chipNo, INT8 * buf, UINT32 nBytes)
{
    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    if (pdevFd == NULL) {
        return (ERROR);
    }

    if(pdevFd->isOpen != TRUE) {
        return ERROR;
    }    

    uartTransBytes(pdevFd,buf,nBytes);	

    return nBytes;
}

INT32 RS422Init(UINT8 chipNo)
{
    INT32         ret = OK;
    UART_CTX_T    *pdevFd;

    if(chipNo >= RS422_NUM) {
        return(ERROR);
    }
    pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    memset(pdevFd, 0, sizeof(UART_CTX_T));

    pdevFd->baseAddr = RS422_BASE_ADDR + chipNo * 16;
    pdevFd->resetBit = gUartResetBit[chipNo];
    pdevFd->intrBit = gUartIntrBit[chipNo];

    uartReset(chipNo);    

#ifdef RS422_USR_INTR_MODE
    UART_REG(FCR,pdevFd) = FCR_RECV_CLR | FCR_TRANS_CLR | FCR_RECV_TIG1 | 0x01;
    PlatformDelay(1);
    UART_REG(FCR,pdevFd) = FCR_RECV_TIG1 | 0x01;
#else
    UART_REG(FCR,pdevFd) = FCR_RECV_CLR | FCR_TRANS_CLR | FCR_RECV_TIG14 | 0x01;
    PlatformDelay(1);
    UART_REG(FCR,pdevFd) = FCR_RECV_TIG14 | 0x01;
#endif

    return ret;
}

INT32 RS422SetOpt(UINT8 chipNo, INT8 party,UINT8 stop_bit, UINT8 data_bit)
{
    INT32 ret = OK;
    UINT8 lcr;
    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    lcr = 0;

    if(party == 'O' || party == 'o')
    {
        lcr |= LCR_PARITY_EN | LCR_PARITY_ODD;
    }
    else if(party == 'E' || party == 'e')
    {
        lcr |= LCR_PARITY_EN | LCR_PARITY_EVEN;
    }

    if(data_bit == 5)
        lcr |= LCR_DATA_BIT5;
    else if(data_bit == 6)
        lcr |= LCR_DATA_BIT6;
    else if(data_bit == 7)
        lcr |= LCR_DATA_BIT7;
    else
        lcr |= LCR_DATA_BIT8;

    if(stop_bit == 2)
        lcr |= LCR_STOP_BIT2;
    else
        lcr |= LCR_STOP_BIT1;

    UART_REG(LCR,pdevFd) = lcr;

    return ret;
}

INT32 RS422SetBaud(UINT8 chipNo,UINT32 baud)
{
    INT32 ret = OK;
    UINT16 dll,dlh;
    UINT32 clkdiv;

    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    UART_REG(LCR,pdevFd) = (UART_REG(LCR,pdevFd) | LCR_CLKDIV_ACCESS);

    clkdiv = SYS_CLK / (16 * (UINT32)baud);
    dlh = (clkdiv >> 8) & 0xff;
    dll = clkdiv & 0xff;

    UART_REG(DLH,pdevFd) = dlh;
    UART_REG(DLL,pdevFd) = dll;

    PRINTF("dlh=%x dll=%x reg_dlh=%x reg_dll=%x\n", dlh, dll, UART_REG(DLH,pdevFd), UART_REG(DLL,pdevFd));

    UART_REG(LCR,pdevFd) = (UART_REG(LCR,pdevFd) & (~LCR_CLKDIV_ACCESS));

    return ret;
}

#ifdef RS422_USR_INTR_MODE
INT32 uartRecvBytes(UART_CTX_T *  pdevFd, INT8 *pBuf, INT32 nBytes)
{
    INT32   index = 0;
    char    *ptr = pBuf;
    UINT32  tptr;

    if (pdevFd == NULL) {
        return (0);
    }

    for(index = 0;index <nBytes;index++) {
        tptr =  pdevFd->msgwr+1;

        if(tptr == RX_LEN) {
            tptr = 0;
        }

        if(tptr !=pdevFd->msgrd) {
        // ensure write before read, if write pointer to be overwritten, never write to the buffer
            pdevFd->RXBuffer[pdevFd->msgwr] = *ptr ;
            ptr++;
            pdevFd->msgwr++;
            if(pdevFd->msgwr == RX_LEN) // if overwritten, reset the counter
            {
                pdevFd->msgwr = 0;
            }
        }
        else 
            break;

    }
    return index;
}

void uartRecvHandle(UART_CTX_T * pDev)
{
    UART_CTX_T *pdevFd = (UART_CTX_T *)pDev;
    UINT8 buf[4];
    UINT16 data;

    while((UART_REG(LSR,pdevFd) & LSR_RECV_VALID) == LSR_RECV_VALID) {
        data = (UINT16)UART_REG(RBR,pdevFd) ;
        buf[0] = (data >> 0) & 0xff;

        if(uartRecvBytes(pdevFd, (char*)&buf[0],  sizeof(buf[0])) != 0)
        {
            Osal_SemPost(pdevFd->rxSemSync);
        }
    }
    return;
}

#else

INT32 uartPollRead(UART_CTX_T * pDev, INT8 *pBuf, INT32 nBytes) {
	UINT32 count = 0;
	UINT32 maxCount = 2000; /* 2ms per char */
	INT32 i = 0;
	UINT16 data = 0;
	UART_CTX_T *pdevFd = (UART_CTX_T *)pDev;

	for( i = 0; i < nBytes; ++i ) {
		count = 0;
		while(1) {
			if((UART_REG(LSR, pdevFd) & LSR_RECV_VALID) == LSR_RECV_VALID) {
				data = (UINT16)UART_REG(RBR, pdevFd);
				pBuf[i] = (data >> 0) & 0xff;
				break;
			} else {
				++count;
				if( count > maxCount ) {
					PRINTF("Timeout to read, read %ld bytes\n", i);
					return i;
				}
				PlatformDelay(1); /* delay 1us then check */
			}
		}
	}
	return i;
}
#endif



INT32 uartTransBytes(UART_CTX_T *  pdevFd, INT8 *pBuf, INT32 nBytes)
{
    INT32   index = 0;
    UINT32  count = 0;
    UINT32  maxCount = 5000;
    char    *ptr = pBuf;
    UINT8   data;

    if (pdevFd == NULL) {
        return (ERROR);
    }

    for(index = 0;index < nBytes;index++) {
        while((UART_REG(LSR,pdevFd) & LSR_TRANS_FIFO_EMTPY) != LSR_TRANS_FIFO_EMTPY) {
            PlatformDelay(1);
            ++count;
            if( count > maxCount ) {
            	PRINTF("timeout to write\n");
            	return index;
            }
        }

        data = (UINT8)(*ptr);
        UART_REG(THR,pdevFd) = data;
        ptr++;
    }
    return nBytes;
}

void uartReset(UINT8 chipNo)
{
    UART_CTX_T *pdevFd = (UART_CTX_T *)&(gUartCtx[chipNo]);

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG) | (1 << pdevFd->resetBit)); // MR = HI;
    PlatformDelay(10); 

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG) & (~(1 << pdevFd->resetBit))); // MR = LO;
    PlatformDelay(10);

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG) | (1 << pdevFd->resetBit)); // MR = HI;
    PlatformDelay(10);   
    return;
}

