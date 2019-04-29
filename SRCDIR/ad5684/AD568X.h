/***************************************************************************//**
 *   @file   AD568X.h
 *   @brief  Header file of AD568X Driver.
 *   @author Dan Nechita
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
*   SVN Revision: 775
*******************************************************************************/
#ifndef __AD568X_H__
#define __AD568X_H__


/******************************************************************************/
/******************************** AD568X *************************************/
/******************************************************************************/

/* AD568x Versions */
#define AD5684          1   // 12-bit DAC, no internal voltage reference.
#define AD5686          2   // 16-bit DAC, no internal voltage reference.
#define AD5684R         3   // 12-bit DAC, with internal voltage reference.
#define AD5685R         4   // 14-bit DAC, with internal voltage reference.
#define AD5686R         5   // 16-bit DAC, with internal voltage reference.

/* LDAC */
#define AD568X_LDAC_OUT        GPIO1_PIN_OUT
#define AD568X_LDAC_LOW        GPIO1_PIN_LOW
#define AD568X_LDAC_HIGH       GPIO1_PIN_HIGH
/* RESET */
#define AD568X_RESET_OUT       GPIO2_PIN_OUT
#define AD568X_RESET_LOW       GPIO2_PIN_LOW
#define AD568X_RESET_HIGH      GPIO2_PIN_HIGH
/* RSTSEL */
#define AD568X_RSTSEL_OUT      GPIO3_PIN_OUT
#define AD568X_RSTSEL_LOW      GPIO3_PIN_LOW
#define AD568X_RSTSEL_HIGH     GPIO3_PIN_HIGH
/* GAIN */
#define AD568X_GAIN_OUT        GPIO4_PIN_OUT
#define AD568X_GAIN_LOW        GPIO4_PIN_LOW
#define AD568X_GAIN_HIGH       GPIO4_PIN_HIGH
#define AD568X_GAIN_STATE      GPIO4_PIN_STATE

/* SPI slave device ID */
#define AD568X_SLAVE_ID        1

/* AD568X Input Register */
#define AD568X_CMD(x)                  ((0x000F & (x)) << 20)
#define AD568X_ADDR(x)                 ((0x000F & (x)) << 16)
#define AD568X_DATA_BITS(x)            ((0xFFFF & (x)) <<  0)

/* AD568X_CMD(x) options */
#define AD568X_CMD_NOP                 0 // No operation.
#define AD568X_CMD_WR_INPUT_N          1 // Write to Input Register n.
#define AD568X_CMD_UPDATE_DAC_N        2 // Update DAC Register n.
#define AD568X_CMD_WR_UPDT_DAC_N       3 // Write to and update DAC Channel n.
#define AD568X_CMD_POWERMODE           4 // Power down/power up DAC.
#define AD568X_CMD_LDAC_MASK           5 // Hardware LDAC mask register.
#define AD568X_CMD_SOFT_RESET          6 // Software reset(power-on reset).
#define AD568X_CMD_INT_REF_SETUP       7 // Internal reference setup register.
#define AD568X_CMD_SET_DCEN            8 // Set up DCEN register.
#define AD568X_CMD_SET_READBACK        9 // Set up readback register.

/* AD568X_ADDR(x) options */
#define AD568X_ADDR_DAC_A          1  // DAC A
#define AD568X_ADDR_DAC_B          2  // DAC B
#define AD568X_ADDR_DAC_C          4  // DAC C
#define AD568X_ADDR_DAC_D          8  // DAC D
#define AD568X_ADDR_DAC_A_B        3  // DAC A and DAC B
#define AD568X_ADDR_DAC_ALL        15 // All DACs

/* Daisy-Chain Enable Register(DCEN) definition */
#define AD568X_DCEN_DISABLE        0
#define AD568X_DCEN_ENABLE         1

/* Power Setup Register definition */
#define AD568X_PWR_PDD(x)      (((x) & 0x3) << 6)
#define AD568X_PWR_PDC(x)      (((x) & 0x3) << 4)
#define AD568X_PWR_PDB(x)      (((x) & 0x3) << 2)
#define AD568X_PWR_PDA(x)      (((x) & 0x3) << 0)

/* AD568X_PWR_PDn(x) options(n = A, B, C, D) */
#define AD568X_PD_NORMAL       0 // Normal operation
#define AD568X_PD_1K           1 // 1 kOhm to GND
#define AD568X_PD_100K         2 // 100 kOhm to GND
#define AD568X_PD_3STATE       3 // Three-state

/* LDAC Mask Register definition */
#define AD568X_MSK_CH_A        1
#define AD568X_MSK_CH_B        2
#define AD568X_MSK_CH_C        4
#define AD568X_MSK_CH_D        8

/* Reference Setup Register definition */
#define AD568X_INT_REF_ON      0
#define AD568X_INT_REF_OFF     1

/* Clear code options */
#define AD568X_RST_ZERO_SCALE     0
#define AD568X_RST_MIDSCALE       1

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/

/*! Initializes the device. */
unsigned char AD568X_Init(unsigned char ad568x);

/*! Resets the device(clears the outputs to either zero scale or midscale). */
void AD568X_Reset(unsigned char resetOutput);

/*! Puts the device in a specific power mode. */
void AD568X_PowerMode(unsigned char channel, unsigned char pwrMode);

/*! Select internal or external voltage reference. */
void AD568X_InternalVoltageReference(unsigned char vRefMode);

/*!  Writes a 24-bit data-word to the Input Register of the device. */
void AD568X_SetInputRegister(unsigned long registerValue);

/*! Write data to the Input Register or to DAC Register of a channel. */
void AD568X_WriteFunction(unsigned char writeCommand, 
                          unsigned char channel, 
                          unsigned short data);

/*! Reads back the binary value written to one of the channels. */
unsigned short AD568X_ReadBack(unsigned char dacChannelAddr);

/*! Selects the output voltage of the selected channel. */
float AD568X_SetVoltage(unsigned char channel, 
                        float outputVoltage, 
                        float vRef);

#endif // __AD568X_H__
