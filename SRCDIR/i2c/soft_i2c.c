/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : soft_i2c.c
 * Author        : yexin
 * Date          : 2018-06-27
 * Description   : soft i2c
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-06-27
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include "F2812_datatype.h"
#include "DSP28_Device.h"
#include "F2812_gpio.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "shellconsole.h"

#define DELAY_TIME        1 

#define GPIO_I2C_ACK_READY       0
#define GPIO_I2C_NO_ACK          1

#define GPIO_I2C_DELAY(x)         PlatformDelay((x))

#define GPIO_BASE_ADDR           (0x7000)

/* GPIOA13 as SCL GPIOA14 as SDA */
#define GPIO_I2C_SDA_INDEX       (14)
#define GPIO_I2C_SCL_INDEX       (13)

#define GPIO_I2C_DIR_REG         (GPIO_BASE_ADDR  + 0xC1)
#define GPIO_I2C_OUT_REG         (GPIO_BASE_ADDR  + 0xE0)
#define GPIO_I2C_SET_REG         (GPIO_BASE_ADDR  + 0xE1)
#define GPIO_I2C_CLR_REG         (GPIO_BASE_ADDR  + 0xE2)
#define GPIO_I2C_IN_REG          (GPIO_BASE_ADDR  + 0xE0)

#define I2C_ACK_OK     0
#define I2C_ACK_NOK    1

#define GPIO_OUTPUT     0
#define GPIO_INPUT      1

#define READL(r)             (*(volatile UINT16 *)(r))
#define WRITEL(v, r)         (*(volatile UINT16 *)(r) = (v))

#define SOFT_I2C_LOG   PRINTF

 
// #pragma GCC push_options
// #pragma GCC optimize("O0")
INT16 GpioI2cSetSdaDir(INT16 dir)
{
    EALLOW;

	if( dir == GPIO_OUTPUT )
	{
		GpioMuxRegs.GPADIR.bit.GPIOA14 = GPIO_DIR_OUTPUT;
	}
	else
	{
		GpioMuxRegs.GPADIR.bit.GPIOA14 = GPIO_DIR_INPUT;
	}

	EDIS;

	return 0;
}

INT16 GpioI2cSetSclDir(INT16 dir)
{
    EALLOW;

	if( dir == GPIO_OUTPUT ) {
		GpioMuxRegs.GPADIR.bit.GPIOA13 = GPIO_DIR_OUTPUT;
	} else {
		GpioMuxRegs.GPADIR.bit.GPIOA13 = GPIO_DIR_INPUT;
	}

	EDIS;

	return 0;
}

INT16 GpioI2cSetScl(void)
{
	GpioDataRegs.GPASET.bit.GPIOA13 = 1;
	PlatformDelay(2);
    return 0;
}

INT16 GpioI2cSetSda(void)
{
	GpioDataRegs.GPASET.bit.GPIOA14 = 1;
	PlatformDelay(2);
    return 0;
}

INT16 GpioI2cClrSda(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIOA14 = 1;
	PlatformDelay(2);
    return 0;
}

INT16 GpioI2cClrScl(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIOA13 = 1;
	PlatformDelay(2);
    return 0;
}

INT16 GpioI2cEnableWrite(void)
{
	EALLOW;
	GpioMuxRegs.GPADIR.bit.GPIOA14 = GPIO_DIR_OUTPUT;
	EDIS;
    return 0;
}

INT16 GpioI2cEnableRead(void)
{
	EALLOW;
	GpioMuxRegs.GPADIR.bit.GPIOA14 = GPIO_DIR_INPUT;
	EDIS;
    return 0;
}

/******************************************************************************
 *  Name:  GpioI2cReadPort
 *
 *  Description:  read from the soft I2C portter one bit
 *
 *  Inputs:     port - pointer to I2C portter
 *
 *  Outputs:    None
 *  
 *  Returns:    bit value from I2C portter
 *
 ******************************************************************************/
static UINT8 GpioI2cReadPort(void)
{
	if( (GpioDataRegs.GPADAT.bit.GPIOA14) != 0 ) {
		return 1;
	} else {
		return 0;
	}
}

void GpioI2cSend_0(void)
{
    UINT8   delay;

    delay = 2;

    GpioI2cClrScl();
    PlatformDelay(1);
    GpioI2cClrSda();
    PlatformDelay(delay);
    GpioI2cSetScl();
    PlatformDelay(delay);
    GpioI2cClrScl();
    PlatformDelay(delay);
}

void GpioI2cSend_1(void)
{
    UINT8  delay;

    delay = 2;

    GpioI2cClrScl();
    PlatformDelay(1);
    GpioI2cSetSda();
    PlatformDelay(delay);
    GpioI2cSetScl();
    PlatformDelay(delay);
    GpioI2cClrScl();
    PlatformDelay(delay);
}

static void GpioI2cStart(void)
{
    UINT8 delay = 1;

    GpioI2cSetSda();
    PlatformDelay(delay * 2);
    GpioI2cSetScl();
    PlatformDelay(delay * 2);
    GpioI2cClrSda();
    PlatformDelay(delay * 2);
    GpioI2cClrScl();
    PlatformDelay(delay);
}

static void GpioI2cStop(void)
{
    UINT8 delay = 1;

    GpioI2cClrSda();
    PlatformDelay(delay * 2);
    GpioI2cSetScl();
    PlatformDelay(delay * 4);
    GpioI2cSetSda();
    PlatformDelay(delay * 2);
}

static INT16 GpioI2cWriteByte(UINT8 data)
{
    INT32 i = 0;
    UINT8  ack = 1;
    UINT8  delay = 2;
	INT32 counter = 0;
    UINT8 mask = 0x80;

	GpioI2cClrScl();
	PlatformDelay(1);
	GpioI2cEnableWrite();
    for(; i < 8; ++i)
    {
        if((data & mask) == 0)
        {
            GpioI2cSend_0();
        }
        else
        {
            GpioI2cSend_1();
        }

        mask >>= 1;
    }
    GpioI2cClrScl();
    PlatformDelay(2);
	GpioI2cSetSda(); /* set SDA to High for ack */
	GpioI2cEnableRead();
    PlatformDelay(delay);
    GpioI2cSetScl();
	while(ack)
	{
		ack = GpioI2cReadPort();
		if( counter++ > 100 )
		{
			return 1;
		}
		PlatformDelay(delay);
	}

    GpioI2cClrScl();
	PlatformDelay(2);
	GpioI2cEnableWrite();
	return 0;
}

static UINT8 GpioI2cReadByte(void)
{
    UINT8 temp;
    UINT8 value;
    UINT8 delay;
    UINT8 bit_counter;

    temp = 0;
    value = 0;
    delay = 1;
    bit_counter = 8;

    PlatformDelay(delay);
    do
    {
        GpioI2cClrScl();
        PlatformDelay(delay * 2);
        GpioI2cSetScl();
        PlatformDelay(delay * 2);
    
        value = GpioI2cReadPort();

        PlatformDelay(delay * 2);
        GpioI2cClrScl();
        PlatformDelay(delay);

        if(value == 1)
        {
            temp |= 0x01;
        }
        else
        {
            temp &= 0xFE;
        }

        if(bit_counter - 1)
        {
            temp <<= 1;
        }

        bit_counter--;
    } while(bit_counter);

    return temp;
}

INT16 GpioI2cReadBytes(UINT8 chip, UINT16 addr, UINT8 alen, UINT8 *buffer, UINT16 len)
{
    int i = 0;

	/* send chip address */
	GpioI2cClrScl();
	PlatformDelay(5);
    GpioI2cEnableWrite();
    GpioI2cStart();

    if( GpioI2cWriteByte((chip << 1)) != 0 )
	{
		PRINTF("write chip failed\n");
		GpioI2cClrScl();
		PlatformDelay(5);
		GpioI2cEnableWrite();
		GpioI2cStop();
		return 1;
	}

	switch(alen)
	{
		case 2:
			if( GpioI2cWriteByte(((addr) >> 8) & 0xFF) != 0 )
			{
				SOFT_I2C_LOG("write high8 addr failed\n");
				GpioI2cClrScl();
				PlatformDelay(1);
				GpioI2cEnableWrite();
				GpioI2cStop();
				return 2;
			}
		case 1:
			if( GpioI2cWriteByte(((addr) & 0xFF)) != 0 )
			{
				SOFT_I2C_LOG("write addr failed\n");
				GpioI2cClrScl();
				PlatformDelay(1);
				GpioI2cEnableWrite();
				GpioI2cStop();
				return 3;
			}
	}

	GpioI2cClrScl();
	PlatformDelay(5);
    GpioI2cEnableWrite();
    GpioI2cStart();

    if( GpioI2cWriteByte(((chip << 1) | 1)) != 0 )
	{
		SOFT_I2C_LOG("write chip with RD failed\n");
		GpioI2cClrScl();
		PlatformDelay(1);
		GpioI2cEnableWrite();
		GpioI2cStop();
		return 2;
	}

	for( i = 0; i < len; ++i )
	{
		GpioI2cClrScl();
		PlatformDelay(1);
		GpioI2cEnableRead();
		buffer[i] = GpioI2cReadByte();
		GpioI2cClrScl();
		PlatformDelay(1);
		GpioI2cEnableWrite();
		if( i != (len - 1) )
		{
            GpioI2cSend_0();
		}
        else
        {
            GpioI2cSend_1();
        }
		GpioI2cClrScl();
		PlatformDelay(10);
	}

	GpioI2cClrScl();
	PlatformDelay(1);
	GpioI2cEnableWrite();
	GpioI2cStop();

	return 0;
}

INT16 GpioI2cWriteBytes(UINT8 chip, UINT16 addr, UINT8 alen, UINT8 *buffer, UINT16 len)
{
	int i = 0;
	GpioI2cClrScl();
	PlatformDelay(5);
    GpioI2cEnableWrite();
	GpioI2cStart();
	PlatformDelay(10);

	if( GpioI2cWriteByte((chip << 1)) != 0 )
	{
		SOFT_I2C_LOG("write chip byte ack Timeout\n");
		GpioI2cClrScl();
		PlatformDelay(5);
		GpioI2cEnableWrite();
		GpioI2cStop();
		return 1;
	}

	switch(alen)
	{
		case 2:
			if( GpioI2cWriteByte(((addr) >> 8) & 0xFF) != 0 )
			{
				SOFT_I2C_LOG("write high8 addr failed\n");
				GpioI2cClrScl();
				PlatformDelay(1);
				GpioI2cEnableWrite();
				GpioI2cStop();
				return 2;
			}
		case 1:
			if( GpioI2cWriteByte(((addr) & 0xFF)) != 0 )
			{
				SOFT_I2C_LOG("write addr failed\n");
				GpioI2cClrScl();
				PlatformDelay(1);
				GpioI2cEnableWrite();
				GpioI2cStop();
				return 2;
			}
	}

    for( ; i < len; ++i )
	{
        if( GpioI2cWriteByte(buffer[i]) != 0 )
		{
			SOFT_I2C_LOG("write DATA ack failed\n");
			GpioI2cClrScl();
			PlatformDelay(1);
			GpioI2cEnableWrite();
			GpioI2cStop();
			return 2;
		}
		PlatformDelay(50);
	}

	GpioI2cClrScl();
	PlatformDelay(1);
    GpioI2cEnableWrite();
	GpioI2cStop();

    return 0;
}

INT16 GpioI2cProbe(UINT8 chip)
{
	UINT8 tmp = 0;
	GpioI2cClrScl();
	PlatformDelay(1);
    GpioI2cEnableWrite();
	GpioI2cStart();
	PlatformDelay(10);

	if( GpioI2cWriteByte(((chip << 1) | 1)) != 0 )
	{
		GpioI2cClrScl();
		PlatformDelay(1);
		GpioI2cEnableWrite();
		GpioI2cStop();
		return 1;
	}

	GpioI2cClrScl();
	PlatformDelay(1);
	GpioI2cEnableRead();
	tmp = GpioI2cReadByte();
	GpioI2cClrScl();
	PlatformDelay(1);
	GpioI2cEnableWrite();
	GpioI2cSend_1();
	GpioI2cClrScl();
	PlatformDelay(2);

    GpioI2cEnableWrite();
	GpioI2cStop();

    return 0;
}

int eeprom_write_enable (unsigned dev_addr, int state) {
	EALLOW;
	if( state ) {
		GpioDataRegs.GPACLEAR.bit.GPIOA15 = 1;
	} else {
		GpioDataRegs.GPASET.bit.GPIOA15 = 1;
	}
	EDIS;
	return 0;
}

void GpioI2cInit(UINT16 speed, UINT16 slaveadd)
{
	EALLOW;
	GpioMuxRegs.GPAMUX.bit.C1TRIP_GPIOA13 = 0;
	GpioMuxRegs.GPAMUX.bit.C2TRIP_GPIOA14 = 0;
	GpioMuxRegs.GPAMUX.bit.C3TRIP_GPIOA15 = 0;
	GpioMuxRegs.GPADIR.bit.GPIOA15 = GPIO_DIR_OUTPUT;
	GpioI2cSetSdaDir(GPIO_OUTPUT);
	GpioI2cSetSclDir(GPIO_OUTPUT);
	EDIS;

	return ;
}

