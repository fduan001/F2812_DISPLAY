#include "F2812_datatype.h"
#include "spi.h"
#include "fpga_spi.h"
#include "fpga.h"
#include "shellconsole.h"

//fpga spi bit
#define FPGA_SPI_BASE  (0x80000 + 0x60)
#define FPGA_SPI1_RESET_BIT    7
#define FPGA_SPI2_RESET_BIT    8
#define FPGA_SPI3_RESET_BIT    9
#define FPGA_SPI4_RESET_BIT    0
#define FPGA_SPI5_RESET_BIT    0
#define FPGA_SPI6_RESET_BIT    0
#define FPGA_SPI7_RESET_BIT    0
#define FPGA_SPI8_RESET_BIT    0

#define BSP_DRV_OK  (1)
#define BSP_DRV_FAIL (0)

#define FPGA_SPI_CTL_WRDIR_BIT15         (15)
//1 for write , 0 for read only used for 3wire
#define FPGA_SPI_CTL_WIREMODE_BIT14  (14)
#define FPGA_SPI_CTL_ASS_BIT13 (13)
#define FPGA_SPI_CTL_IE_BIT12   (12)
#define FPGA_SPI_CTL_LSB_BIT11 (11)
#define FPGA_SPI_CTL_TX_NEG_BIT10   (10)
#define FPGA_SPI_CTL_RX_NEG_BIT9    (9)
#define FPGA_SPI_CTL_GO_BSY_BIT8    (8)

#define FPGA_SPI_TIMEOUT   10 /* 10 ms */

unsigned char SYSSPI_RESET_BIT[] = {
  FPGA_SPI1_RESET_BIT,FPGA_SPI2_RESET_BIT,FPGA_SPI3_RESET_BIT,FPGA_SPI4_RESET_BIT, \
  FPGA_SPI5_RESET_BIT,FPGA_SPI6_RESET_BIT,FPGA_SPI7_RESET_BIT,FPGA_SPI8_RESET_BIT
};

static inline void __msleep__(unsigned int ms)
{
	Osal_TaskSleep(ms);
}

/******************************************************************************
 *
 * Function:		FpgaSpiWRMode
 *
 * Description:       This function config the spi controller  to write or read mode
                             it is only used for 3 wire mode
 *
 * Parameters:	channel:  uart channel number
                      wrmode:  1   write mode , 0 read mode
 *
 *
 * Return Value:

 *
 *****************************************************************************/

void FpgaSpiWRMode(UINT8 channel , UINT8 wrmode)
{
    S_SPI_CTRL_TYPE *spicontroller;
    UINT16 regdata, bitvalue;
    spicontroller = (S_SPI_CTRL_TYPE *)(FPGA_SPI_BASE + channel  * 16);

    // write or read dir control
    regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
    bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_WIREMODE_BIT14, FPGA_SPI_CTL_WIREMODE_BIT14);
    //wire 4 mode
    if(SPI_4WIRE_MODE == bitvalue)
    {

    }
    // wire 3 mode
    else
    {
        //write mode
        if(SPI_WRITE_MODE == wrmode)
        {
            regdata = FPGA_SET_BITFIELD(regdata, SPI_WRITE_MODE, FPGA_SPI_CTL_WRDIR_BIT15, FPGA_SPI_CTL_WRDIR_BIT15);
            FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);
        }
        //read mode
        else
        {
            regdata = FPGA_SET_BITFIELD(regdata, SPI_READ_MODE, FPGA_SPI_CTL_WRDIR_BIT15, FPGA_SPI_CTL_WRDIR_BIT15);
            FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);
        }
    }
}

/******************************************************************************
 *
 * Function:		FpgaSpiConfig
 *
 * Description:       This function config the spi controller using the paramter
 *
 * Parameters:	channel:  uart channel number
                             commconfig:  the uart configuration parameter
 *
 *
 * Return Value:

 *
 *****************************************************************************/
INT32 FpgaSpiConfig(UINT8 channel , S_SPI_CFG_TYPE spicfg)
{
    S_SPI_CTRL_TYPE *spicontroller;
    UINT16 regdata, bitvalue;
    UINT16  timeout = FPGA_SPI_TIMEOUT;
    spicontroller = (S_SPI_CTRL_TYPE *)(FPGA_SPI_BASE + (channel ) * 16);
    //check the go_bsy=0?  
    
    regdata = FPGA_REG16_R(FPGA_PERIPHERAL_RST_REG);
    regdata = (regdata | ((1 << SYSSPI_RESET_BIT[channel] )));

    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, regdata); // HI
    __msleep__(1);

    regdata = (regdata & (~(1 << SYSSPI_RESET_BIT[channel])));
 	FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, regdata);  // LO
     __msleep__(1);

     regdata = (regdata | ((1 << SYSSPI_RESET_BIT[channel])));  
    FPGA_REG16_W(FPGA_PERIPHERAL_RST_REG, regdata);   // HI
    __msleep__(1);   
    
    do
    {
        regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
        bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
        __msleep__(1);
        timeout--;
        if(0 == timeout) {
            return BSP_DRV_FAIL;
        }
    } while(SPI_GO_START == bitvalue);
    //set the  clock  rate;

    FPGA_REG16_W(&(spicontroller->divider), spicfg.spisclk);

    regdata = FPGA_REG16_R(&(spicontroller->divider));
    PRINTF("divider=0x%04x  spisclk=%u 0x%08x\n", regdata, spicfg.spisclk, &(spicontroller->divider));
    //set the spi timing protocol ,default is read mode
    regdata = 0;
    //regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
    //set the cs generate manually
    regdata = FPGA_SET_BITFIELD(regdata, spicfg.spicsgen, FPGA_SPI_CTL_ASS_BIT13, FPGA_SPI_CTL_ASS_BIT13);
    //set the bit order;
    regdata = FPGA_SET_BITFIELD(regdata, spicfg.bitorder, FPGA_SPI_CTL_LSB_BIT11, FPGA_SPI_CTL_LSB_BIT11);
    //set the tx data timing
    regdata = FPGA_SET_BITFIELD(regdata, spicfg.spitxdatrising, FPGA_SPI_CTL_TX_NEG_BIT10, FPGA_SPI_CTL_TX_NEG_BIT10);
    //set the rx data timing
    regdata = FPGA_SET_BITFIELD(regdata, spicfg.spirxdatrising, FPGA_SPI_CTL_RX_NEG_BIT9, FPGA_SPI_CTL_RX_NEG_BIT9);
    //set the data length
    regdata = FPGA_SET_BITFIELD(regdata, spicfg.spidatalen, 6, 0);
    //set 3wire or 4wire spi mode
    regdata = FPGA_SET_BITFIELD(regdata, spicfg.wiremode, FPGA_SPI_CTL_WIREMODE_BIT14 , FPGA_SPI_CTL_WIREMODE_BIT14);
    FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);

    __msleep__(1);
    
    return BSP_DRV_OK;

}


/******************************************************************************
 *
 * Function:		FpgaSpiWrite
 *
 * Description:       This function send all the data to spi
 *
 * Parameters:	channel:  spi channel number
                             sendbuffer:  send buffer
                             sendlen:      send buffer length( byte)
 *
 *
 * Return Value:

 *
 *****************************************************************************/

INT32 FpgaSpiWrite(UINT8 channel , UINT8 *sendbuffer, UINT8 sendlen)
{
    S_SPI_CTRL_TYPE *spicontroller;
    UINT16 regdata, bitvalue, timeout = FPGA_SPI_TIMEOUT;
    UINT8   index;
    spicontroller = (S_SPI_CTRL_TYPE *)(FPGA_SPI_BASE + channel * 16);
    //first check the go_bsy=0?
    do
    {
        regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
        bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
        __msleep__(1);
        timeout--;
        if(0 == timeout) {
            return BSP_DRV_FAIL;
        }
    }
    while(SPI_GO_START == bitvalue);
    FpgaSpiWRMode(channel, SPI_WRITE_MODE);
    //write the data byte by byte
    for(index = 0; index < sendlen; index++)
    {
        //send one byte
        FPGA_REG16_W(&(spicontroller->txdat), sendbuffer[index]);
        regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
        regdata = FPGA_SET_BITFIELD(regdata, SPI_GO_START, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
        FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);
        //check ready for next;
        timeout = FPGA_SPI_TIMEOUT;
        do
        {
            regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
            bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
            __msleep__(1);
            timeout--;
            if(0 == timeout) {
                return BSP_DRV_FAIL;
            }
        } while(SPI_GO_START == bitvalue);
    }
    return BSP_DRV_OK;

}

/******************************************************************************
 *
 * Function:		FpgaSpiWriteRead
 *
 * Description:       This function send all the command to spi and read the
                             data;
 *
 * Parameters:	channel:  spi channel number
                             sendbuffer:  send buffer
                             sendlen:      send buffer length( byte)
                             readbuffer :   read buffer;
                             readlen   :    real length
 *
 *
 * Return Value:

 *
 *****************************************************************************/

INT32 FpgaSpiWriteRead(UINT8 channel , UINT8 *sendbuffer, UINT8 sendlen , UINT8 *readbuffer , UINT8 readlen  )
{
    S_SPI_CTRL_TYPE *spicontroller;
    UINT16 regdata, bitvalue, timeout = FPGA_SPI_TIMEOUT;
    UINT8   index;
    spicontroller = (S_SPI_CTRL_TYPE *)(FPGA_SPI_BASE + channel  * 16);
    //first check the go_bsy=0?
    do
    {
        regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
        bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
        __msleep__(1);
        timeout--;
        if(0 == timeout) {
            return BSP_DRV_FAIL;
        }
    } while(SPI_GO_START == bitvalue);
    
    if( sendbuffer ) {
        // write mode
        FpgaSpiWRMode(channel, SPI_WRITE_MODE);
        //write the command data byte by byte

        for(index = 0; index < sendlen; index++)
        {
            //send one byte
            FPGA_REG16_W(&(spicontroller->txdat), sendbuffer[index]);
            regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
            regdata = FPGA_SET_BITFIELD(regdata, SPI_GO_START, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
            FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);
            //check ready for next;
            timeout = FPGA_SPI_TIMEOUT;
            do
            {
                regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
                bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
                __msleep__(1);
                timeout--;
                if(0 == timeout) {
                    return BSP_DRV_FAIL;
                }
            } while(SPI_GO_START == bitvalue);
        }
    }

    if( readbuffer ) {
        // read mode
        FpgaSpiWRMode(channel, SPI_READ_MODE);
        // now to send the clock to read buffer;
        for(index = 0; index < readlen; index++)
        {
            //send one byte
            FPGA_REG16_W(&(spicontroller->txdat), 0x0);
            regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
            regdata = FPGA_SET_BITFIELD(regdata, SPI_GO_START, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
            FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);
            //check ready for next;
            timeout = FPGA_SPI_TIMEOUT;
            do
            {
                regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
                bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
                __msleep__(1);
                timeout--;
                if(0 == timeout) {
                    return BSP_DRV_FAIL;
                }
            } while(SPI_GO_START == bitvalue);
            readbuffer[index] = FPGA_REG16_R(&(spicontroller->rxdat));
        }
    }
    return BSP_DRV_OK;

}

/******************************************************************************
 *
 * Function:		FpgaSpiCs
 *
 * Description:       This function set the spi chip select
 *
 * Parameters:	channel:  spi channel number
                              chipsel:   chip select 0 or 1
 *
 *
 * Return Value:

 *
 *****************************************************************************/

void FpgaSpiCs(UINT8 channel , UINT8 chipsel)
{
    S_SPI_CTRL_TYPE *spicontroller;
    spicontroller = (S_SPI_CTRL_TYPE *)(FPGA_SPI_BASE + channel  * 16);
    FPGA_REG16_W(&(spicontroller->slavesel), chipsel);
}


/******************************************************************************
 *
 * Function:		FpgaSpiRead
 *
 * Description:       This function send all the data to spi
 *
 * Parameters:	channel:  spi channel number
                             sendbuffer:  send buffer
                             sendlen:      send buffer length( byte)
 *
 *
 * Return Value:

 *
 *****************************************************************************/

INT32 FpgaSpiRead(UINT8 channel , UINT8 *readbuffer, UINT8 readlen)
{
    S_SPI_CTRL_TYPE *spicontroller;
    UINT16 regdata, bitvalue, timeout = FPGA_SPI_TIMEOUT;
    UINT8   index;
    spicontroller = (S_SPI_CTRL_TYPE *)(FPGA_SPI_BASE + channel  * 16);
    //first check the go_bsy=0?
    do
    {
        regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
        bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
        __msleep__(1);
        timeout--;
        if(0 == timeout) {
            return BSP_DRV_FAIL;
        }
    } while(SPI_GO_START == bitvalue);
    // read mode
    FpgaSpiWRMode(channel, SPI_READ_MODE);
    //write the data byte by byte
    for(index = 0; index < readlen; index++)
    {
        //send one byte
        FPGA_REG16_W(&(spicontroller->txdat), 0x0);
        regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
        regdata = FPGA_SET_BITFIELD(regdata, SPI_GO_START, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
        FPGA_REG16_W(&(spicontroller->ctlstatus), regdata);
        //check ready for next;
        timeout = FPGA_SPI_TIMEOUT;
        do
        {
            regdata = FPGA_REG16_R(&(spicontroller->ctlstatus));
            bitvalue = FPGA_READ_BITFIELD(regdata, FPGA_SPI_CTL_GO_BSY_BIT8, FPGA_SPI_CTL_GO_BSY_BIT8);
            __msleep__(1);
            timeout--;
            if(0 == timeout) {
                return BSP_DRV_FAIL;
            }
        } while(SPI_GO_START == bitvalue);
        readbuffer[index] = FPGA_REG16_R(&(spicontroller->rxdat));
    }
    return BSP_DRV_OK;
}
