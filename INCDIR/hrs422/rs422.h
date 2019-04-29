#ifndef RS422_H
#define RS422_H

#define   SYS_CLK  100000000

#define   RBR     0x00
#define   THR     0x00
#define   IER     0x02
#define   IIR     0x04
#define   FCR     0x04
#define   LCR     0x06
#define   LSR     0x0a

#define   DLL    0x00
#define   DLH    0x02


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

int RS422Open(unsigned char chipNo,char party,unsigned char stop,unsigned char data_bit,unsigned int baud);
int RS422Close(unsigned char chipNo);
int RS422Read(unsigned char chipNo,char * buf,unsigned int nBytes);
int RS422Write(unsigned char chipNo,char * buf,unsigned int nBytes);

int RS422Init(unsigned char chipNo);
int RS422SetOpt(unsigned char chipNo,char party,unsigned char stop_bit,unsigned char data_bit);
int RS422SetBaud(unsigned char chipNo,unsigned int baud);

#endif

