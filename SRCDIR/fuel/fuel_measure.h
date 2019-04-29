#ifndef FUEL_MEASURE_H
#define FUEL_MEASURE_H

int FuelMeasInit(void);
int FuelMeasStart(UINT8 chan);
int FuelGetMeasData(UINT8 chan, UINT16 *data);
void FuelIntrHandler(UINT8 bit_pos);
int FuelMeasSelfTest(void);
void FuelMeasDump(void);

#endif
