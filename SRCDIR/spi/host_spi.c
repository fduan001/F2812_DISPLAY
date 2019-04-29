#include "DSP28_Device.h"
#include "boardcfg.h"
#include "F2812_datatype.h"
#include "host_spi.h"

#define HOST_SPI_MAX_XFER_BYTES           16 /* 16 words per fifo */

void HostSpiInit(void) {
	UINT16 regval;

    // Initialize SPI-A:
    // Initialize SCI-A:
	EALLOW;
	regval = GpioMuxRegs.GPFMUX.all;
	regval |= SPI_PIN_MUX;
	GpioMuxRegs.GPFMUX.all = regval;
	EDIS;
    /* loopback   8 bit data */
	SpiaRegs.SPICCR.all = SPICCR_CFG;

	SpiaRegs.SPICTL.all = SPICTL_CFG;

    SpiaRegs.SPIBRR = SPIBRR_CFG;//

    SpiaRegs.SPICCR.all = (0x80 | SPICCR_CFG);
	/* initialize the GPIO settings */
	/* set master mode, 8-bits mode */
}

void HostSpiAssertCS(void) {

}

void HostSpiDeassertCS(void) {

}

void HostSpiReset(void) {
	/* Disable SPI Master */
	SpiaRegs.SPICCR.all = 0x0;
	/* Disable FIFO and reset TX/RX FIFO */
	SpiaRegs.SPIFFTX.all = 0x0;
	SpiaRegs.SPIFFRX.all = 0x0;

	SpiaRegs.SPICCR.all = SPICCR_CFG;
	SpiaRegs.SPICTL.all = SPICTL_CFG;
	SpiaRegs.SPIBRR = SPIBRR_CFG;
	SpiaRegs.SPICCR.all = (0x80 | SPICCR_CFG);

    SpiaRegs.SPIFFRX.all = ( 1 << 13) | 0xF; /* no interrupt */
	SpiaRegs.SPIFFTX.all = ( 1 << 15) | (1 << 13);

	return ;
}

int HostSpiXferWrite(spi_msg_t *spi_msg) {
	int  i = 0;
	if( spi_msg->tx_len > 0 ) {
		/* Setup the TX FIFO and RX FIFO */
		if( spi_msg->tx_len < HOST_SPI_MAX_XFER_BYTES ) {
			SpiaRegs.SPIFFTX.all |= spi_msg->tx_len;
		} else {
			SpiaRegs.SPIFFTX.all |= 0xF; /* 16 words */
		}
		spi_msg->tx_done = HOST_SPI_MSG_INP;
		for( i = 0; i < spi_msg->tx_len; ++i ) {
			SpiaRegs.SPITXBUF = spi_msg->tx_buf[i];
		}
	}

	spi_msg->tx_done = HOST_SPI_MSG_DONE;
	return 0;
}

int HostSpiXferRead(spi_msg_t *spi_msg) {
	int i = 0;
	if( spi_msg->rx_len > 0 ) {
		spi_msg->rx_done = HOST_SPI_MSG_INP;
		for( i = 0; i < spi_msg->rx_len; ++i ) {
			SpiaRegs.SPITXBUF = 0xFF; /* dummy byte */
			while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) {};
			spi_msg->rx_buf[i] = SpiaRegs.SPIRXBUF;
		}
	}

	spi_msg->rx_done = HOST_SPI_MSG_DONE;
	return 0;
}

int HostSpiSetupXfer(spi_msg_t *spi_msg) {
	int i = 0;
	int loop = 0;
	int remain = 0;
	spi_msg_t cur_msg;

	/* reset SPI controller */
	HostSpiReset();
	if( spi_msg->tx_buf ) {
		loop = spi_msg->tx_len / HOST_SPI_MAX_XFER_BYTES;
		remain = spi_msg->tx_len % HOST_SPI_MAX_XFER_BYTES;
	/* tx 1st then rx */
		for( i = 0; i < loop; ++i ) {
			cur_msg.tx_len = HOST_SPI_MAX_XFER_BYTES;
			cur_msg.tx_done = HOST_SPI_MSG_INIT;
			cur_msg.tx_buf = spi_msg->tx_buf + (i * HOST_SPI_MAX_XFER_BYTES);
			HostSpiXferWrite(&cur_msg);
		}

		if( remain > 0 ) {
			cur_msg.tx_len = remain;
			cur_msg.tx_done = HOST_SPI_MSG_INIT;
			cur_msg.tx_buf = spi_msg->tx_buf + (i * HOST_SPI_MAX_XFER_BYTES);
			HostSpiXferWrite(&cur_msg);
		}
	}
	
	if( spi_msg->rx_buf ) {
		loop = spi_msg->rx_len / HOST_SPI_MAX_XFER_BYTES;
		remain = spi_msg->rx_len % HOST_SPI_MAX_XFER_BYTES;

		/* tx 1st then rx */
		for( i = 0; i < loop; ++i ) {
			cur_msg.rx_len = HOST_SPI_MAX_XFER_BYTES;
			cur_msg.rx_done = HOST_SPI_MSG_INIT;
			cur_msg.rx_buf = spi_msg->rx_buf + (i * HOST_SPI_MAX_XFER_BYTES);
			HostSpiXferRead(&cur_msg);
		}

		if( remain > 0 ) {
			cur_msg.rx_len = remain;
			cur_msg.rx_done = HOST_SPI_MSG_INIT;
			cur_msg.rx_buf = spi_msg->rx_buf + (i * HOST_SPI_MAX_XFER_BYTES);
			HostSpiXferRead(&cur_msg);
		}
	}
	return 0;
}

int HostSpiXferMsg(spi_msg_t *spi_msg) {
	HostSpiAssertCS();
	HostSpiSetupXfer(spi_msg);
	HostSpiDeassertCS();
	return 0;
}

int HostSpiXfer(UINT8* tx_buf, UINT16 tx_len, UINT8* rx_buf, UINT16 rx_len) {
	spi_msg_t cur_msg;
	cur_msg.tx_len = tx_len;
	cur_msg.tx_buf = tx_buf;
	cur_msg.rx_len = rx_len;
	cur_msg.rx_buf = rx_buf;
	HostSpiXferMsg(&cur_msg);
	return 0;
}


int HostSpiRead(UINT8* buffer, UINT16 length) {
	HostSpiXfer(NULL, 0, buffer, length);
	return 0;
}

int HostSpiWrite(UINT8* buffer, UINT16 length) {
	HostSpiXfer(buffer, length, NULL, 0);
	return 0;
}


