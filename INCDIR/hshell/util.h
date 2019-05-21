#ifndef  UTIL_H
#define  UTIL_H
#include "shellctype.h"
#define DIV_ROUND_UP(n,d)	(((n) + (d) - 1) / (d))





UINT32 simple_strtoull (INT8 *cp, INT8 **endp, UINT32 base);
UINT32 simple_strtoul(INT8 *cp, INT8 **endp, UINT32 base);
INT32 simple_strtol(INT8 *cp, INT8 **endp, UINT32 base);
INT32 print_buffer (UINT32 addr, void *data, UINT32 width, UINT32 count, UINT32 linelen);


#endif
