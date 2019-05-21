#ifndef  FPGA_H
#define  FPGA_H

#define FPGA_BASE_ADDR                                  0x80000

#define FPGA_VER_YEAR_REG                               (FPGA_BASE_ADDR + 0x0)
#define FPGA_VER_DATE_REG                               (FPGA_BASE_ADDR + 0x1)
#define FPGA_SELF_TEST_REG                              (FPGA_BASE_ADDR + 0x2)

/* peripherals reset */
#define FPGA_PERIPHERAL_RST_REG                         (FPGA_BASE_ADDR + 0x7)

/* interrupt registers */
#define FPGA_XINT1_STATUS_REG                           (FPGA_BASE_ADDR + 0x8)
#define FPGA_XINT1_MASK_REG                             (FPGA_BASE_ADDR + 0x9)

#define FPGA_XINT2_STATUS_REG                           (FPGA_BASE_ADDR + 0x8)
#define FPGA_XINT2_MASK_REG                             (FPGA_BASE_ADDR + 0x9)

/* IO control */
#define FPGA_IO_STATUS_REG                              (FPGA_BASE_ADDR + 0xA)

/* UDELAY */
#define FPGA_UDELAY_CTRL_REG                            (FPGA_BASE_ADDR + 0xB)
#define FPGA_UDELAY_COUNT_REG                           (FPGA_BASE_ADDR + 0xC)

/* RS422 */
#define FPGA_RS422_TRX1_RX_REG                               (FPGA_BASE_ADDR + 0x60)
#define FPGA_RS422_TRX1_TX_REG                               (FPGA_BASE_ADDR + 0x60)

#define FPGA_RS422_TRX1_LSB_REG                              (FPGA_BASE_ADDR + 0x60)
#define FPGA_RS422_TRX1_MSB_REG                              (FPGA_BASE_ADDR + 0x61)
#define FPGA_RS422_TRX1_INTR_CTRL_REG                        (FPGA_BASE_ADDR + 0x61)
#define FPGA_RS422_TRX1_INTR_STATUS_REG                      (FPGA_BASE_ADDR + 0x62)
#define FPGA_RS422_TRX1_FIFO_CTRL_REG                        (FPGA_BASE_ADDR + 0x62)
#define FPGA_RS422_TRX1_LCR_REG                              (FPGA_BASE_ADDR + 0x63)
#define FPGA_RS422_TRX1_LSR_REG                              (FPGA_BASE_ADDR + 0x65)

#define FPGA_RS422_TRX2_RX_REG                               (FPGA_BASE_ADDR + 0x70)
#define FPGA_RS422_TRX2_TX_REG                               (FPGA_BASE_ADDR + 0x70)

#define FPGA_RS422_TRX2_LSB_REG                              (FPGA_BASE_ADDR + 0x70)
#define FPGA_RS422_TRX2_MSB_REG                              (FPGA_BASE_ADDR + 0x71)
#define FPGA_RS422_TRX2_INTR_CTRL_REG                        (FPGA_BASE_ADDR + 0x71)
#define FPGA_RS422_TRX2_INTR_STATUS_REG                      (FPGA_BASE_ADDR + 0x72)
#define FPGA_RS422_TRX2_FIFO_CTRL_REG                        (FPGA_BASE_ADDR + 0x72)
#define FPGA_RS422_TRX2_LCR_REG                              (FPGA_BASE_ADDR + 0x73)
#define FPGA_RS422_TRX2_LSR_REG                              (FPGA_BASE_ADDR + 0x75)

typedef void (*ISR_HANDLER)(void*);

#define FPGA_REG16_W(addr,b)    WriteFpgaRegister(addr, b)
#define FPGA_REG16_R(addr) 	    ReadFpgaRegister(addr)


#define FPGABITMASK(x,y)      (   (   (  ((UINT16)1 << (((UINT16)x)-((UINT16)y)+(UINT16)1) ) - (UINT16)1 )   )   <<  ((UINT16)y)   )    // Sets a bitmask of 1s from [x:y]
#define FPGA_READ_BITFIELD(z,x,y)   (((UINT16)z) & FPGABITMASK(x,y)) >> (y)                                                             // Reads the value of register z[x:y]
#define FPGA_SET_BITFIELD(z,f,x,y)  (((UINT16)z) & ~FPGABITMASK(x,y)) | ( (((UINT16)f) << (y)) & FPGABITMASK(x,y) )

#ifdef __cplusplus
extern  "C" {
#endif

void WriteFpgaRegister( UINT32 regaddr, UINT16 regvalue);
UINT16 ReadFpgaRegister( UINT32 regaddr );
void WriteFpgaRegisterBit(UINT32 regaddr, UINT8 bitpos, UINT8 bitvalue);
UINT8 ReadFpgaRegisterBit(UINT32 regaddr, UINT8 bitpos, UINT8 bitvalue);
int RegisterIsr(int bit_pos, ISR_HANDLER isr);

#ifdef __cplusplus
}
#endif

#endif
