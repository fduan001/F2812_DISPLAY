#ifndef RS422_H
#define RS422_H

#define   SYS_CLK  150000000

#define   RBR     0x00
#define   THR     0x00
#define   IER     0x01
#define   IIR     0x02
#define   FCR     0x02
#define   LCR     0x03
#define   LSR     0x05

#define   DLL    0x00
#define   DLH    0x01


#define  IER_RECV_VALID     (1 << 0)
#define  IER_TRANS_EMTPY     (1 << 1)
#define  IER_RLINE_STATUS     (1 << 2)

#define  IIR_INT        (1 << 0)
#define  IIR_TYPE_MASK    (0x07 << 1)
#define  IIR_LINE_STATUS   (0x03 << 1)
#define  IIR_RECV_VALID   (0x02 << 1)
#define  IIR_TIME_OUT   (0x06 << 1)
#define  IIR_TRANS_EMPTY   (0x01 << 1)


#define  FCR_RECV_CLR    (1 << 1)
#define  FCR_TRANS_CLR    (1 << 2)
#define  FCR_RECV_TIG_MASK (0x03 << 6)
#define  FCR_RECV_TIG1    (0 << 6)     //1BYTE
#define  FCR_RECV_TIG4    (1 << 6)     //4BYTE
#define  FCR_RECV_TIG8    (2 << 6)     //8BYTE
#define  FCR_RECV_TIG14    (3 << 6)     //14BYTE


#define  LCR_DATA_BIT_MASK (0x3 << 0)
#define  LCR_DATA_BIT5    0x0
#define  LCR_DATA_BIT6    0x1
#define  LCR_DATA_BIT7    0x2
#define  LCR_DATA_BIT8    0x3

#define  LCR_STOP_BIT1    0x0
#define  LCR_STOP_BIT1_5    0x4
#define  LCR_STOP_BIT2    0x4

#define  LCR_PARITY_NONE    0x0
#define  LCR_PARITY_EN     0x08
#define  LCR_PARITY_ODD     0x00
#define  LCR_PARITY_EVEN    0x10

#define  LCR_PARITY_ADD    0x20
#define  LCR_INTER_CTRL    0x40
#define  LCR_CLKDIV_ACCESS    0x80

#define  LSR_RECV_VALID    0x01
#define  LSR_OV_ERROR    0x02
#define  LSR_PARITY_ERROR    0x04
#define  LSR_FRAME_ERROR    0x08
#define  LSR_BI          0x10
#define  LSR_TRANS_FIFO_EMTPY    0x20
#define  LSR_TRANS_EMTPY   0x40
#define  LSR_RECV_FIFO_ERROR 0x80

INT32 RS422Open(UINT8 chipNo, INT8 party, UINT8 stop, UINT8 data_bit, UINT32 baud);
INT32 RS422Close(UINT8 chipNo);
INT32 RS422Read(UINT8 chipNo, INT8 * buf, UINT32 nBytes);
INT32 RS422Write(UINT8 chipNo, INT8 * buf, UINT32 nBytes);

INT32 RS422Init(UINT8 chipNo);
INT32 RS422SetOpt(UINT8 chipNo, INT8 party,UINT8 stop_bit, UINT8 data_bit);
INT32 RS422SetBaud(UINT8 chipNo, UINT32 baud);

INT32 HostUartInit(void);
INT32 HostUartSend(UINT8* buffer, UINT16 length);
INT32 HostUartRecv(UINT8* buffer, UINT16 length);

#endif

