#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"
#include "rs422.h"
#include "fpga.h"

#define RS422_USR_INTR_MODE   1

typedef int BOOL;

#ifndef ERROR
#define ERROR   (-1)
#endif

#ifndef OK
#define OK    (0)
#endif

#define RX_LEN   256
#define RS422_NUM    2

#define   RS422_CHIP1_RESET_BIT   0
#define   RS422_CHIP2_RESET_BIT   1


#define  RS422_CHIP1_INT_BIT      0
#define  RS422_CHIP2_INT_BIT      1

#define   RS422_BASE_ADDR    (0x80000 + 0x60)

#define UART_REG(reg, pchan) \
(*(volatile unsigned short *)(((UINT32)(pchan->baseAddr)) + (reg)))


typedef struct{

    BOOL isOpen;

    UINT8 RXBuffer[RX_LEN]; // [# of buffers][RX_LEN bytes]
    UINT32  msgrd,msgwr;

    HANDLE   rx_semSync;

    UINT32 baseAddr;
    UINT8 reset_bit;
    UINT8 int_bit;

}  UART_BUFF;

int uartintUserCnt = 0;
UART_BUFF  altera_uart_buff[RS422_NUM];

UINT8 UART_RESET_BIT[RS422_NUM]={
    RS422_CHIP1_RESET_BIT,
    RS422_CHIP2_RESET_BIT,
};

UINT8 UART_INT_BIT[RS422_NUM]={
    RS422_CHIP1_INT_BIT,
    RS422_CHIP2_INT_BIT,
};

UINT8 BitPos2Chan(UINT8 bit_pos) {
    UINT8 i = 0;
    for( i = 0; i < RS422_NUM; ++i ) {
        if( UART_INT_BIT[i] == bit_pos ) {
            return i;
        }
    }

    return 0xFF;
}

void RS422SysDataInit(void) {
	memset(altera_uart_buff, 0, sizeof(altera_uart_buff));
}

void uartReset(unsigned char chipNo);
int uartTransBytes(UART_BUFF *  pdevFd, char *pBuf, int nBytes);
void uartRecvHandle(UART_BUFF * pDev);
int uartRecvBytes(UART_BUFF *  pdevFd, char *pBuf, int nBytes);

void DebugSysReg(void)
{
    shellprintf("%08lx: = 0x%04x\r\n",  FPGA_PERIPHERAL_RST_REG,  FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG));
    shellprintf("%08lx = 0x%04x\r\n",  FPGA_XINT1_STATUS_REG,   FPGA_REG16_R(FPGA_XINT1_STATUS_REG));
    shellprintf("%08lx = 0x%04x\r\n",  FPGA_XINT1_MASK_REG,   FPGA_REG16_R(FPGA_XINT1_MASK_REG));
}
void DebugUartReg(UART_BUFF *pdevFd)
{
    if(pdevFd == NULL)
        return;
    shellprintf("%08lx:IER = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(IER,pdevFd));
    shellprintf("%08lx:IIR = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(IIR,pdevFd));
    shellprintf("%08lx:LSR = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(LSR,pdevFd));
    shellprintf("%08lx:LCR = 0x%04x\r\n",pdevFd->baseAddr,UART_REG(LCR,pdevFd));
}

void DebugUartRegInfo(unsigned char chipNo)
{
    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    if(chipNo >= RS422_NUM)
        return;

    // PRINTF("SYS INT REG = 0x%x\r\n",FPGA_REG16_R(SYS_FPGA_INTF_REG));
    DebugSysReg();
    DebugUartReg(pdevFd);
}


void RS422Isr(UINT8 bit_pos)
{
    UART_BUFF *pdevFd = NULL;
    UINT8 channel = 0;

    FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) & (~( 1 << bit_pos))));
    channel = BitPos2Chan(bit_pos);

    pdevFd = &altera_uart_buff[channel];

    if(pdevFd == NULL || pdevFd->isOpen != TRUE) {
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << bit_pos)));
        return;
    }

    if((FPGA_REG16_R(FPGA_XINT1_STATUS_REG) & (1 << pdevFd->int_bit)) == 0) {
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


int RS422Open(unsigned char chipNo,char party,unsigned char stop,unsigned char data_bit,UINT32 baud)
{
    int ret = OK;
    int irqnum = UART_INT_BIT[chipNo];

    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    if(chipNo >= RS422_NUM)
        return(ERROR);

    if(pdevFd->isOpen == TRUE) {
    	PRINTF("is already open\n");
        return(ERROR);
    }

    RS422Init(chipNo);

    if(RS422SetOpt(chipNo, party, stop, data_bit) != OK)
        return(ERROR);


    if(RS422SetBaud(chipNo, baud) != OK)
        return(ERROR);

#ifdef RS422_USR_INTR_MODE
    pdevFd->rx_semSync = Osal_SemCreateBinary(0);

    if(pdevFd->rx_semSync == NULL)
    {
        return ERROR;
    }
#endif

    pdevFd->isOpen = TRUE;

    UART_REG(IER, pdevFd) = IER_RECV_VALID; //EN RECV DATA INT

#ifdef RS422_USR_INTR_MODE
    if(uartintUserCnt == 0)
    {
        if( RegisterIsr(irqnum, RS422Isr) != 0 )
        {
            PRINTF("altera uart irq install error......[irq = %d]\r\n",  irqnum);
        }   
        else
        {
            uartintUserCnt++;
            if(uartintUserCnt == 1)
            {
                // FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) & (~( 1 << irqnum))));
                FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << irqnum)));
            }
        }
    }else uartintUserCnt++;   
#endif

    if(ret) {
        PRINTF("RS422Open success....\r\n");
    }

    DebugSysReg();

    DebugUartReg(pdevFd);

    return ret;
}

int RS422Close(unsigned char chipNo)
{
    int ret = OK;
    int irq_num = 0;

    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    if(chipNo >= RS422_NUM)
        return(ERROR);


    if(pdevFd->isOpen != TRUE)
        return(ERROR);

    UART_REG(IER, pdevFd) = 0;

#ifdef RS422_USR_INTR_MODE
    if(pdevFd->rx_semSync != NULL)
    {
        Osal_SemDelete(pdevFd->rx_semSync );
        pdevFd->rx_semSync  = NULL;
    }     

    if(uartintUserCnt == 1) 	
    {
        FPGA_REG16_W(FPGA_XINT1_MASK_REG, (FPGA_REG16_R(FPGA_XINT1_MASK_REG) | (0x01 << irq_num)));
    }
    if(uartintUserCnt > 0)
        uartintUserCnt--;
#endif

    memset(pdevFd,0,sizeof(UART_BUFF));

    pdevFd->isOpen = FALSE;

    return ret;
}

int RS422Read(unsigned char chipNo,char * buf,unsigned int nBytes)
{
    unsigned int readNum = 0;
    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    if(chipNo >= RS422_NUM)
        return(ERROR);

    if(pdevFd->isOpen != TRUE)
        return(ERROR);

#ifdef RS422_USR_INTR_MODE
    if(ERROR==Osal_SemPend(pdevFd->rx_semSync, ~(0)))
    {	
        return(ERROR);
    }
#endif

    while(((pdevFd->msgrd)!=(pdevFd->msgwr)) && (readNum < nBytes))
    {	
        buf[readNum] = pdevFd->RXBuffer[pdevFd->msgrd];
        pdevFd->msgrd++;
        if(pdevFd->msgrd == RX_LEN)
        {
            pdevFd->msgrd = 0;
        }
        readNum++;
    }
    if((pdevFd->msgrd)!=(pdevFd->msgwr)) 
    {
        Osal_SemPost(pdevFd->rx_semSync);
    }

    // PRINTF("chipNo = %d,read data,len = %d\r\n",chipNo,readNum); 

    return readNum;
}

int RS422Write(unsigned char chipNo,char * buf,unsigned int nBytes)
{
    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    if (pdevFd == NULL)
        return (ERROR);

    if(pdevFd->isOpen != TRUE)
    {
        return ERROR;
    }    

    uartTransBytes(pdevFd,buf,nBytes);	

    return nBytes;   
}

int RS422Init(unsigned char chipNo)
{
    int ret = OK;

    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    if(chipNo >= RS422_NUM)
        return(ERROR);

    memset(pdevFd, 0, sizeof(UART_BUFF));

    pdevFd->baseAddr = RS422_BASE_ADDR + chipNo * 16;
    pdevFd->reset_bit = UART_RESET_BIT[chipNo];
    pdevFd->int_bit = UART_INT_BIT[chipNo];

    uartReset(chipNo);    

    UART_REG(FCR,pdevFd) = FCR_RECV_CLR | FCR_TRANS_CLR | FCR_RECV_TIG1 | 0x01;
    PlatformDelay(1);
    UART_REG(FCR,pdevFd) = FCR_RECV_TIG1 | 0x01;

    return ret;
}

int RS422SetOpt(unsigned char chipNo,char party,unsigned char stop_bit,unsigned char data_bit)
{
    int ret = OK;
    unsigned char lcr;
    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

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

int RS422SetBaud(UINT8 chipNo,UINT32 baud)
{
    int ret = OK;
    UINT16 dll,dlh;
    UINT32 clkdiv;

    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

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


int uartRecvBytes(UART_BUFF *  pdevFd, char *pBuf, int nBytes)
{
    int index = 0;
    char * ptr = pBuf;
    volatile unsigned int  tptr;	

    if (pdevFd == NULL)
        return (0);

    for(index = 0;index <nBytes;index++)
    {

        tptr =  pdevFd->msgwr+1;

        if(tptr == RX_LEN)
        {
            tptr = 0;
        }

        if(tptr !=pdevFd->msgrd) // ensure write before read, if write pointer to be overwritten, never write to the buffer
        {  

            pdevFd->RXBuffer[pdevFd->msgwr] = *ptr ;

            // logMsg("msgwr =%d,recv = 0x%x\r\n",pdevFd->msgwr,pdevFd->RXBuffer[pdevFd->msgwr],0,0,0,0); 

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

void uartRecvHandle(UART_BUFF * pDev)
{
    UART_BUFF *pdevFd = (UART_BUFF *)pDev;
    UINT8 buf[4];
    UINT16 data;

    while((UART_REG(LSR,pdevFd) & LSR_RECV_VALID) == LSR_RECV_VALID)
    {
        data = (UINT16)UART_REG(RBR,pdevFd) ;
        buf[0] = (data >> 0) & 0xff;

        if(uartRecvBytes(pdevFd, (char*)&buf[0],  sizeof(buf[0])) != 0)
        {
#ifdef RS422_USR_INTR_MODE
            Osal_SemPost(pdevFd->rx_semSync);
#endif
        }
    }
    return;
}

int uartTransBytes(UART_BUFF *  pdevFd, char *pBuf, int nBytes)
{
    int index = 0;
    UINT32 count = 0;
    UINT32 maxCount = 5000;
    char * ptr = pBuf;
    UINT8 data;

    if (pdevFd == NULL)
        return (ERROR);

    //PRINTF("write %d bytes\n", nBytes);

    for(index = 0;index < nBytes;index++)
    {
        while((UART_REG(LSR,pdevFd) & LSR_TRANS_FIFO_EMTPY) != LSR_TRANS_FIFO_EMTPY)
        {	       	
            PlatformDelay(1);
            ++count;
            if( count > maxCount ) {
            	PRINTF("timeout to write\n");
            	return nBytes;
            }
        }

        data = (UINT8)(*ptr);
        // PRINTF("sent: %x\n", data);
        UART_REG(THR,pdevFd) = data;
        ptr++;
    }
    return nBytes;
}

void uartReset(unsigned char chipNo)
{

    UART_BUFF *pdevFd = (UART_BUFF *)&(altera_uart_buff[chipNo]);

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG) | (1 << pdevFd->reset_bit)); // MR = HI;
    PlatformDelay(10); 

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG) & (~(1 << pdevFd->reset_bit))); // MR = LO;
    PlatformDelay(10);

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG) | (1 << pdevFd->reset_bit)); // MR = HI;
    PlatformDelay(10);   
    return;
}


