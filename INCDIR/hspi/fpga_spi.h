#ifndef FPGA_SPI_H
#define FPGA_SPI_H

#include "spi.h"

void FpgaSpiWRMode(UINT8 channel , UINT8 wrmode);
INT32 FpgaSpiConfig(UINT8 channel , S_SPI_CFG_TYPE spicfg);
INT32 FpgaSpiWrite(UINT8 channel , UINT8 *sendbuffer, UINT8 sendlen);
INT32 FpgaSpiWriteRead(UINT8 channel , UINT8 *sendbuffer, UINT8 sendlen , UINT8 *readbuffer , UINT8 readlen  );
void FpgaSpiCs(UINT8 channel , UINT8 chipsel);
INT32 FpgaSpiRead(UINT8 channel , UINT8 *readbuffer, UINT8 readlen);

#endif
