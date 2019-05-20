#ifndef  UTIL_H
#define  UTIL_H
#include "shellctype.h"
#define DIV_ROUND_UP(n,d)	(((n) + (d) - 1) / (d))





u32 simple_strtoull (s8 *cp, s8 **endp, u32 base);
u32 simple_strtoul(s8 *cp, s8 **endp, u32 base);
s32 simple_strtol(s8 *cp, s8 **endp, u32 base);
s32 print_buffer (u32 addr, void *data, u32 width, u32 count, u32 linelen);


#endif
