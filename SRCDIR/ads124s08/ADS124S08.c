/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*
 * ADS124S08.c
 *
 */
#include "F2812_datatype.h"
/* Also include device specific header */
#include "ADS124S08.h"
#include "spi.h"
#include "fpga.h"
#include "fpga_spi.h"

/* Global variables */
UINT8 registers[NUM_REGISTERS];

#define AD124S08_SPI_CHANNEL    1
#define AD124S08_SPI_CLK (4)
#define AD124S08_DELAY  (10)

S_SPI_CFG_TYPE  ad124s08_spicfg =
{
    0,
    AD124S08_SPI_CLK,
    8,
    SPI_MSB_ORDER,
    SPI_TX_DATA_FALLING,
    SPI_RX_DATA_RISING
};

/*
 * Writes the nCS pin low and waits a while for the Tiva to finish working before
 * handing control back to the caller for a SPI transfer.
 */
void clearChipSelect(void){
	FpgaSpiCs(AD124S08_SPI_CHANNEL,  1);
}
/*
 * Pulls the nCS pin high. Performs no waiting.
 */
void setChipSelect(void){
	FpgaSpiCs(AD124S08_SPI_CHANNEL,  0);
}

/*
 * Initializes device for use in the ADS124S08 EVM.
 *
 * \return True if device is in correct hardware defaults and is connected
 *
 */
int ADS124S08_Init(void)
{
	/* Default register settings */
	registers[ID_ADDR_MASK] 	= 0x08;
	registers[STATUS_ADDR_MASK] = 0x80;
	registers[INPMUX_ADDR_MASK]= 0x01;
	registers[PGA_ADDR_MASK] 	= 0x00;
	registers[DATARATE_ADDR_MASK] 	= 0x14;
	registers[REF_ADDR_MASK] 	= 0x10;
	registers[IDACMAG_ADDR_MASK] 	= 0x00;
	registers[IDACMUX_ADDR_MASK] 	= 0xFF;
	registers[VBIAS_ADDR_MASK] 	= 0x00;
	registers[SYS_ADDR_MASK]= 0x10;
	registers[OFCAL0_ADDR_MASK] 	= 0x00;
	registers[OFCAL1_ADDR_MASK] 	= 0x00;
	registers[OFCAL2_ADDR_MASK] 	= 0x00;
	registers[FSCAL0_ADDR_MASK] 	= 0x00;
	registers[FSCAL1_ADDR_MASK] 	= 0x00;
	registers[FSCAL2_ADDR_MASK] 	= 0x40;
	registers[GPIODAT_ADDR_MASK] 	= 0x00;
	registers[GPIOCON_ADDR_MASK]= 0x00;
	setChipSelect();

	ADS124S08_DeassertStart();

	FpgaSpiConfig(AD124S08_SPI_CHANNEL, ad124s08_spicfg);
	return 1;
}
/*
 * Reads a single register contents from the specified address
 *
 * \param regnum identifies which address to read
 *
 */
UINT8 ADS124S08_ReadReg(UINT8 regnum)
{

	if( regnum >=  NUM_REGISTERS) {
		return 0xFF;
	}

    UINT8 ulDataTx[3];
    UINT8 ulDataRx[1];

	ulDataTx[0] = REGRD_OPCODE_MASK + (regnum & 0x1f);
	ulDataTx[1] = 0x00;
	clearChipSelect();

	FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, ulDataTx, 2, ulDataRx, 1);
	registers[regnum] = ulDataRx[0];
	setChipSelect();
	return ulDataRx[0];
}
/*
 * Reads a group of registers starting at the specified address
 *
 * \param regnum is addr_mask 8-bit mask of the register from which we start reading
 * \param count The number of registers we wish to read
 * \param *location pointer to the location in memory to write the data
 *
 */
void ADS124S08_ReadRegs(UINT8 regnum, UINT8 *data, UINT8 count)
{
	int i;
    UINT8 ulDataTx[2];
	ulDataTx[0] = REGRD_OPCODE_MASK + (regnum & 0x1f);
	ulDataTx[1] = count-1;
	clearChipSelect();
	FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, ulDataTx, 2, data, count);
	for(i = 0; i < count; i++)
	{
		if(regnum+i < NUM_REGISTERS)
			registers[regnum+i] = data[i];
	}
	setChipSelect();
}

/*
 * Writes a single of register with the specified data
 *
 * \param regnum addr_mask 8-bit mask of the register to which we start writing
 * \param data to be written
 *
 */
void ADS124S08_WriteReg(UINT8 regnum, UINT8 data)
{
	UINT8 ulDataTx[3];
	if( regnum >=  NUM_REGISTERS) {
		return ;
	}
	ulDataTx[0] = REGWR_OPCODE_MASK + (regnum & 0x1f);
	ulDataTx[1] = 0x00;
	ulDataTx[2] = data;
	clearChipSelect();
	FpgaSpiWrite(AD124S08_SPI_CHANNEL, ulDataTx, 3);
	setChipSelect();
	return;
}
/*
 * Writes a group of registers starting at the specified address
 *
 * \param regnum is addr_mask 8-bit mask of the register from which we start writing
 * \param count The number of registers we wish to write
 * \param *location pointer to the location in memory to read the data
 *
 */
void ADS124S08_WriteRegs(UINT8 regnum, UINT8 *data, UINT8 count)
{
	UINT8 i;
    UINT8 ulDataTx[2];
	ulDataTx[0] = REGWR_OPCODE_MASK + (regnum & 0x1f);
	ulDataTx[1] = count-1;
	clearChipSelect();
	FpgaSpiWrite(AD124S08_SPI_CHANNEL, ulDataTx, 2);
	FpgaSpiWrite(AD124S08_SPI_CHANNEL, data, count);
	for(i=0; i < count; i++)
	{
		if(regnum+i < NUM_REGISTERS)
			registers[regnum+i] = data[i];
	}
	setChipSelect();
	return;
}
/*
 * Sends a command to the ADS124S08
 *
 * \param op_code is the command being issued
 *
 */
void ADS124S08_SendCmd(UINT8 op_code)
{
	clearChipSelect();
	FpgaSpiWrite(AD124S08_SPI_CHANNEL, &op_code, 1);
	setChipSelect();
	return;
}
/*
 * Sends a STOP/START command sequence to the ADS124S08 to restart conversions (SYNC)
 *
 */
void ADS124S08_RestartSync(void)
{
	ADS124S08_SendCmd(STOP_OPCODE_MASK);
	ADS124S08_SendCmd(START_OPCODE_MASK);
	return;
}
/*
 * Sets the GPIO hardware START pin high (red LED)
 *
 */
void ADS124S08_AssertStart(void)
{
	FPGA_REG16_W(FPGA_TEMP_MEAS_CTRL_REG, 0x3);
}
/*
 * Sets the GPIO hardware START pin low
 *
 */
void ADS124S08_DeassertStart(void)
{
	FPGA_REG16_W(FPGA_TEMP_MEAS_CTRL_REG, 0x0);
}
/*
 * Sets the GPIO hardware external oscillator enable pin high
 *
 */
void ADS124S08_AssertClock(void)
{
	//
}
/*
 * Sets the GPIO hardware external oscillator enable pin low
 *
 */
void ADS124S08_DeassertClock(void)
{
	//
}
/*
 *
 * Read the last conversion result
 *
 */
int ADS124S08_ReadDate(UINT8 *dStatus, UINT8 *dData, UINT8 *dCRC)
{
	UINT8 xcrc;
	UINT8 xstatus;
	clearChipSelect();

	ADS124S08_SendCmd(RDATA_OPCODE_MASK);

	if((registers[SYS_ADDR_MASK] & 0x01) == DATA_MODE_STATUS)
	{
		FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, NULL, 0, &xstatus, 1);
		dStatus[0] = (UINT8)xstatus;
	}

	// get the conversion data
	FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, NULL, 0, &dData[0], 1);
	FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, NULL, 0, &dData[1], 1);
	FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, NULL, 0, &dData[2], 1);

	if((registers[SYS_ADDR_MASK] & 0x02) == DATA_MODE_CRC)
	{
		FpgaSpiWriteRead(AD124S08_SPI_CHANNEL, NULL, 0, &xcrc, 1);
		dCRC[0] = (UINT8)xcrc;
	}
	setChipSelect();
	return 0;
}
