/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : soft_i2c.h
 * Author        : yexin technologic
 * Date          : 2018-6-27
 * Description   : soft i2c controller
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-6-27
 *   Author      : yexin technologic
 *   Modification: Created file

*************************************************************************************************************/
#ifndef SOFT_I2C_H
#define SOFT_I2C_H
#include "F2812_datatype.h"

void GpioI2cInit(UINT16 speed, UINT16 slaveadd);
INT16 GpioI2cWriteBytes(UINT8 chip, UINT16 addr, UINT8 alen, UINT8 *buffer, UINT16 len);
INT16 GpioI2cReadBytes(UINT8 chip, UINT16 addr, UINT8 alen, UINT8 *buffer, UINT16 len);
INT16 GpioI2cProbe(UINT8 chip);

#endif



