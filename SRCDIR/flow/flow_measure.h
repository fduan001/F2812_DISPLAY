#ifndef FLOW_MEASURE_H
#define FLOW_MEASURE_H

int FlowMeasInit(void);
int FlowMeasStart(UINT8 chan);
int FlowGetMeasData(UINT8 chan, UINT16 *data);
void FlowIntrHandler(UINT8 bit_pos);
int FlowMeasSelfTest(void);
void FlowMeasDump(void);

#endif
