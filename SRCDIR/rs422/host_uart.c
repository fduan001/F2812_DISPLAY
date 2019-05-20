#include "shellconsole.h"
#include "boarddrv.h"

int HostUartInit(void) {
	return 0;
}

int HostUartSend(UINT8* buffer, UINT16 length) {
	int ret = 0;
	ret = UartWrite_B(buffer, length);
	if( ret != length) {
		PRINTF("Send buffer failed, ret=%d\n", ret);
	}
	return 0;
}

int HostUartRecv(UINT8* buffer, UINT16 length) {
	int ret = 0;

	ret = UartRead_B(buffer, length);
	if( ret != length ) {
		PRINTF("failed to read from uart B, ret=%d\n", ret);
		return -1;
	}
	return 0;
}
