/*
 * util.c
 *
 *  Created on: 2014Äê8ÔÂ11ÈÕ
 *      Author: Administrator
 */

//#include "shellctype.h"
#include "config.h"
#include "shellconsole.h"
#include <ctype.h>



u32 simple_strtoul(const s8 *cp, s8 **endp, u32 base)
{
    u32 result = 0, value;

    if (*cp == '0')
    {
        cp++;
        if ((*cp == 'x') && isxdigit(cp[1]))
        {
            base = 16;
            cp++;
        }
        if (!base)
        {
            base = 8;
        }
    }
    if (!base)
    {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp)
                                     ? toupper(*cp) : *cp) - 'A' + 10) < base)
    {
        result = result * base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}


u32 simple_strtol(const s8 *cp, s8 **endp, u32 base)
{
    if(*cp == '-')
        return -simple_strtoul(cp + 1, endp, base);
    return simple_strtoul(cp, endp, base);
}


u32  simple_strtoull (const s8 *cp, s8 **endp, u32 base)
{
    u32  result = 0, value;

    if (*cp == '0')
    {
        cp++;
        if ((*cp == 'x') && isxdigit (cp[1]))
        {
            base = 16;
            cp++;
        }
        if (!base)
        {
            base = 8;
        }
    }
    if (!base)
    {
        base = 10;
    }
    while (isxdigit (*cp) && (value = isdigit (*cp)
                                      ? *cp - '0'
                                      : (islower (*cp) ? toupper (*cp) : *cp) - 'A' + 10) < base)
    {
        result = result * base + value;
        cp++;
    }
    if (endp)
        *endp = (char *) cp;
    return result;
}

#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)
    /* linebuf as a union causes proper alignment */


#pragma DATA_SECTION   (lb,"shell_lib");
union linebuf
{
        u32 ui[MAX_LINE_LENGTH_BYTES / sizeof(u32) + 1];
        u16 us[MAX_LINE_LENGTH_BYTES / sizeof(u16) + 1];
        u8  uc[MAX_LINE_LENGTH_BYTES / sizeof(u8) + 1];
} lb;

s32 print_buffer (u32 addr, void *data, u32 width, u32 count, u32 linelen)
{

    s32 i;

    if (linelen * width > MAX_LINE_LENGTH_BYTES)
        linelen = MAX_LINE_LENGTH_BYTES / width;
    if (linelen < 1)
        linelen = DEFAULT_LINE_LENGTH_BYTES / width;

    while (count)
    {
        shellprintf("%08lx:", addr);

        /* check for overflow condition */
        if (count < linelen)
            linelen = count;

        /* Copy from memory into linebuf and print hex values */
        for (i = 0; i < linelen; i++)
        {
            u32 x;
	     //yexin changed for F2812 c compiler 20180707
            if (width == 4)
            	{
                x = lb.ui[i] = *(volatile u32 *)data;
                shellprintf(" %08lx", x);
            	}
            else if (width == 2)
            	{
                x = lb.us[i] = *(volatile u16 *)data;
		  shellprintf(" %04x",(u16) x);
            	}
            else
            	{
                x = lb.uc[i] = *(volatile u8 *)data;
                shellprintf(" %02x", (u8)x);
            	}
   //         shellprintf(" %0*x", width * 2, x);

            //data += width;
            data += 1;
            //  for c6000 compiler
            //	data= (void*)((u32)data+width);
        }

        /* Print data in ASCII characters */
        for (i = 0; i < linelen * width; i++)
        {
            if (!isprint(lb.uc[i]) || lb.uc[i] >= 0x80)
                lb.uc[i] = '.';
        }
        lb.uc[i] = '\0';
        shellprintf("    %s\n", lb.uc);

        /* update references */
        // addr += linelen * width;
        addr += linelen;
        count -= linelen;

        if (ctrlc())
            return -1;
    }

    return 0;
}



