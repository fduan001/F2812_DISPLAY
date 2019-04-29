#ifndef  FPGA_H
#define  FPGA_H

#define FPGA_BASE_ADDR                                  0x8000

#define FPGA_VER_YEAR_REG                               (FPGA_BASE_ADDR + 0x0)
#define FPGA_VER_DATE_REG                               (FPGA_BASE_ADDR + 0x1)
#define FPGA_SELF_TEST_REG                              (FPGA_BASE_ADDR + 0x2)

/* master/slave role switch */
#define FPGA_ROLE_SWITCH_REG                            (FPGA_BASE_ADDR + 0x4)

/* peripherals reset */
#define FPGA_PERIPHERAL_RST_REG                         (FPGA_BASE_ADDR + 0x5)

/* interrupt registers */
#define FPGA_XINT1_STATUS_REG                           (FPGA_BASE_ADDR + 0x6)
#define FPGA_XINT1_MASK_REG                             (FPGA_BASE_ADDR + 0x7)

#define FPGA_XINT2_STATUS_REG                           (FPGA_BASE_ADDR + 0x8)
#define FPGA_XINT2_MASK_REG                             (FPGA_BASE_ADDR + 0x9)

/* IO control */
#define FPGA_IO_STATUS_REG                              (FPGA_BASE_ADDR + 0xA)
#define FPGA_IO_SELF_TEST_REG                           (FPGA_BASE_ADDR + 0xB)

/* FUEL mearsument */
#define FPGA_FUEL_SWITCH_REG                        (FPGA_BASE_ADDR + 0x10)
#define FPGA_FUEL_MEAS_REG                          (FPGA_BASE_ADDR + 0x11)
#define FPGA_FUEL_STATUS_REG                        (FPGA_BASE_ADDR + 0x12)

#define FPGA_FUEL_TUNNEL1_RESULT_H_REG              (FPGA_BASE_ADDR + 0x13)
#define FPGA_FUEL_TUNNEL1_RESULT_L_REG              (FPGA_BASE_ADDR + 0x14)
#define FPGA_FUEL_TUNNEL2_RESULT_H_REG              (FPGA_BASE_ADDR + 0x15)
#define FPGA_FUEL_TUNNEL2_RESULT_L_REG              (FPGA_BASE_ADDR + 0x16)
#define FPGA_FUEL_TUNNEL3_RESULT_H_REG              (FPGA_BASE_ADDR + 0x17)
#define FPGA_FUEL_TUNNEL3_RESULT_L_REG              (FPGA_BASE_ADDR + 0x18)
#define FPGA_FUEL_TUNNEL4_RESULT_H_REG              (FPGA_BASE_ADDR + 0x19)
#define FPGA_FUEL_TUNNEL4_RESULT_L_REG              (FPGA_BASE_ADDR + 0x1A)

/* flow measurment */
#define FPGA_SPI_AS5684_TX_DATA_REG                    (FPGA_BASE_ADDR + 0x20)
#define FPGA_SPI_AS5684_RX_DATA_REG                    (FPGA_BASE_ADDR + 0x20)
#define FPGA_SPI_AS5684_CTRL_REG                       (FPGA_BASE_ADDR + 0x21)
#define FPGA_SPI_AS5684_CLK_REG                        (FPGA_BASE_ADDR + 0x22)
#define FPGA_SPI_AS5684_SS_REG                         (FPGA_BASE_ADDR + 0x23)

#define FPGA_FLOW_SWITCH_REG                           (FPGA_BASE_ADDR + 0x30)
#define FPGA_FLOW_MEAS_REG                             (FPGA_BASE_ADDR + 0x31)
#define FPGA_FLOW_STATUS_REG                           (FPGA_BASE_ADDR + 0x32)
#define FPGA_FLOW_TUNNEL1_RESULT_H_REG                 (FPGA_BASE_ADDR + 0x33)
#define FPGA_FLOW_TUNNEL1_RESULT_L_REG                 (FPGA_BASE_ADDR + 0x34)
#define FPGA_FLOW_TUNNEL2_RESULT_H_REG                 (FPGA_BASE_ADDR + 0x35)
#define FPGA_FLOW_TUNNEL2_RESULT_L_REG                 (FPGA_BASE_ADDR + 0x36)
#define FPGA_FLOW_TUNNEL3_RESULT_H_REG                 (FPGA_BASE_ADDR + 0x37)
#define FPGA_FLOW_TUNNEL3_RESULT_L_REG                 (FPGA_BASE_ADDR + 0x38)
#define FPGA_FLOW_TUNNEL4_RESULT_H_REG                 (FPGA_BASE_ADDR + 0x39)
#define FPGA_FLOW_TUNNEL4_RESULT_L_REG                 (FPGA_BASE_ADDR + 0x3A)

/* temperature measument */
#define FPGA_SPI_ADS1248_TX_DATA_REG                    (FPGA_BASE_ADDR + 0x40)
#define FPGA_SPI_ADS1248_RX_DATA_REG                    (FPGA_BASE_ADDR + 0x40)
#define FPGA_SPI_ADS1248_CTRL_REG                       (FPGA_BASE_ADDR + 0x41)
#define FPGA_SPI_ADS1248_CLK_REG                        (FPGA_BASE_ADDR + 0x42)
#define FPGA_SPI_ADS1248_SS_REG                         (FPGA_BASE_ADDR + 0x43)

#define FPGA_TEMP_MEAS_CTRL_REG                         (FPGA_BASE_ADDR + 0x50)
#define FPGA_TEMP_MEAS_STATUS_REG                       (FPGA_BASE_ADDR + 0x51)

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

#define FPGA_RS422_TRX3_RX_REG                               (FPGA_BASE_ADDR + 0x80)
#define FPGA_RS422_TRX3_TX_REG                               (FPGA_BASE_ADDR + 0x80)

#define FPGA_RS422_TRX3_LSB_REG                              (FPGA_BASE_ADDR + 0x80)
#define FPGA_RS422_TRX3_MSB_REG                              (FPGA_BASE_ADDR + 0x81)
#define FPGA_RS422_TRX3_INTR_CTRL_REG                        (FPGA_BASE_ADDR + 0x81)
#define FPGA_RS422_TRX3_INTR_STATUS_REG                      (FPGA_BASE_ADDR + 0x82)
#define FPGA_RS422_TRX3_FIFO_CTRL_REG                        (FPGA_BASE_ADDR + 0x82)
#define FPGA_RS422_TRX3_LCR_REG                              (FPGA_BASE_ADDR + 0x83)
#define FPGA_RS422_TRX3_LSR_REG                              (FPGA_BASE_ADDR + 0x85)

#define FPGA_RS422_TRX4_RX_REG                               (FPGA_BASE_ADDR + 0x90)
#define FPGA_RS422_TRX4_TX_REG                               (FPGA_BASE_ADDR + 0x90)

#define FPGA_RS422_TRX4_LSB_REG                              (FPGA_BASE_ADDR + 0x90)
#define FPGA_RS422_TRX4_MSB_REG                              (FPGA_BASE_ADDR + 0x91)
#define FPGA_RS422_TRX4_INTR_CTRL_REG                        (FPGA_BASE_ADDR + 0x91)
#define FPGA_RS422_TRX4_INTR_STATUS_REG                      (FPGA_BASE_ADDR + 0x92)
#define FPGA_RS422_TRX4_FIFO_CTRL_REG                        (FPGA_BASE_ADDR + 0x92)
#define FPGA_RS422_TRX4_LCR_REG                              (FPGA_BASE_ADDR + 0x93)
#define FPGA_RS422_TRX4_LSR_REG                              (FPGA_BASE_ADDR + 0x95)

#define FPGA_RS422_TRX5_RX_REG                               (FPGA_BASE_ADDR + 0xA0)
#define FPGA_RS422_TRX5_TX_REG                               (FPGA_BASE_ADDR + 0xA0)

#define FPGA_RS422_TRX5_LSB_REG                              (FPGA_BASE_ADDR + 0xA0)
#define FPGA_RS422_TRX5_MSB_REG                              (FPGA_BASE_ADDR + 0xA1)
#define FPGA_RS422_TRX5_INTR_CTRL_REG                        (FPGA_BASE_ADDR + 0xA1)
#define FPGA_RS422_TRX5_INTR_STATUS_REG                      (FPGA_BASE_ADDR + 0xA2)
#define FPGA_RS422_TRX5_FIFO_CTRL_REG                        (FPGA_BASE_ADDR + 0xA2)
#define FPGA_RS422_TRX5_LCR_REG                              (FPGA_BASE_ADDR + 0xA3)
#define FPGA_RS422_TRX5_LSR_REG                              (FPGA_BASE_ADDR + 0xA5)


typedef void (*ISR_HANDLER)(void*);

#define FPGA_REG16_W(addr,b)    (*(volatile unsigned short*)((unsigned int)(addr)) = b)
#define FPGA_REG16_R(addr) 	    (*(volatile unsigned short*)((unsigned int)(addr)))


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
