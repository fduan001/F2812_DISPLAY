#ifndef SPI_H
#define SPI_H

typedef struct
{
    UINT16  txdat;
    UINT16  ctlstatus;
    UINT16  divider;
    UINT16  slavesel;
   // UINT16  reserve[4];
} S_SPI_CTRL_TYPE ;

#define rxdat    txdat

typedef struct
{
    UINT8   spicsgen;
    UINT16  spisclk;
    UINT8   spidatalen;
    UINT8   bitorder;
    UINT8   spitxdatrising;
    UINT8   spirxdatrising;
    UINT8   wiremode;


} S_SPI_CFG_TYPE ;

#define SPI_LSB_ORDER   (1)
#define SPI_MSB_ORDER  (0)

#define SPI_TX_DATA_RISING      (0)
#define SPI_TX_DATA_FALLING  (1)

#define SPI_RX_DATA_RISING    (0)
#define SPI_RX_DATA_FALLING   (1)

#define SPI_CS_HIGH_POLARITY (1)
#define SPI_CS_LOW_POLARITY  (0)

#define SPI_3WIRE_MODE (0)
#define SPI_4WIRE_MODE (1)

#define SPI_WRITE_MODE  (0)
#define SPI_READ_MODE (1)

#define SPI_GO_START  (1)
#define SPI_CS_MANUAL (0)

#endif
