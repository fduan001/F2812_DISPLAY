/*
 * util.c
 *
 *  Created on: 2014Äê8ÔÂ11ÈÕ
 *      Author: Administrator
 */

#include "F2812_datatype.h"
#include "config.h"
#include "shellconsole.h"

UINT32 simple_strtoul(const INT8 *cp, INT8 **endp, UINT32 base)
{
    UINT32 result = 0, value;

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


UINT32 simple_strtol(const INT8 *cp, INT8 **endp, UINT32 base)
{
    if(*cp == '-')
        return -simple_strtoul(cp + 1, endp, base);
    return simple_strtoul(cp, endp, base);
}


UINT32  simple_strtoull (const INT8 *cp, INT8 **endp, UINT32 base)
{
    UINT32  result = 0, value;

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
        UINT32 ui[MAX_LINE_LENGTH_BYTES / sizeof(UINT32) + 1];
        UINT16 us[MAX_LINE_LENGTH_BYTES / sizeof(UINT16) + 1];
        UINT8  uc[MAX_LINE_LENGTH_BYTES / sizeof(UINT8) + 1];
} lb;

INT32 print_buffer (UINT32 addr, void *data, UINT32 width, UINT32 count, UINT32 linelen)
{

    INT32 i;

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
            UINT32 x;
	     //yexin changed for F2812 c compiler 20180707
            if (width == 4)
            	{
                x = lb.ui[i] = *(volatile UINT32 *)data;
                shellprintf(" %08lx", x);
            	}
            else if (width == 2)
            	{
                x = lb.us[i] = *(volatile UINT16 *)data;
		  shellprintf(" %04x",(UINT16) x);
            	}
            else
            	{
                x = lb.uc[i] = *(volatile UINT8 *)data;
                shellprintf(" %02x", (UINT8)x);
            	}
   //         shellprintf(" %0*x", width * 2, x);

            //data += width;
            data += 1;
            //  for c6000 compiler
            //	data= (void*)((UINT32)data+width);
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



