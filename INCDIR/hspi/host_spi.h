#ifndef HOST_SPI_H
#define HOST_SPI_H

typedef enum spi_msg_sts_t {
	HOST_SPI_MSG_INIT = 0,
	HOST_SPI_MSG_INP, /* inprogress */
	HOST_SPI_MSG_DONE,
	HOST_SPI_MSG_INVALID,
	HOST_SPI_MSG_FAIL
} spi_msg_sts_t;

typedef struct spi_msg_t {
	UINT16 tx_len;
	UINT16 rx_len;
	UINT8  tx_done;
	UINT8  rx_done;
	UINT8 *tx_buf;
	UINT8 *rx_buf;
} spi_msg_t;

#endif
