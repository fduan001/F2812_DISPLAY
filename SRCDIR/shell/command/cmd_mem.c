/*
 * cmd_mem.c
 *
 *  Created on: 
 *      Author: Administrator
 */

#include "F2812_datatype.h"
#include "config.h"
#include "command.h"
#include "util.h"
#include "shellconsole.h"


#define WATCHDOG_RESET()

#pragma DATA_SECTION   (base_address,"shell_lib");
static   far UINT32 base_address = 0x0;
#pragma DATA_SECTION   (dp_last_addr,"shell_lib");
#pragma DATA_SECTION   (dp_last_size,"shell_lib");
UINT32  	far dp_last_addr, dp_last_size;
#pragma DATA_SECTION   (dp_last_length,"shell_lib");
UINT32	far dp_last_length = 0x40;
#pragma DATA_SECTION   (mm_last_addr,"shell_lib");
#pragma DATA_SECTION   (mm_last_size,"shell_lib");
UINT32	far mm_last_addr, mm_last_size;

INT32 do_mem_mw ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    UINT32	addr, writeval, count;
    INT32	size;

    if ((argc < 3) || (argc > 4))
        return cmd_usage(cmdtp);

    /* Check for size specification.
    */
    if ((size = cmd_get_data_size(argv[0], 4)) < 1)
        return 1;

    /* Address is specified since argc > 1
    */
    addr = simple_strtoul(argv[1], NULL, 16);
    addr += base_address;

    /* Get the value to write.
    */
    writeval = simple_strtoul(argv[2], NULL, 16);

    /* Count ? */
    if (argc == 4)
    {
        count = simple_strtoul(argv[3], NULL, 16);
    }
    else
    {
        count = 1;
    }

    while (count-- > 0)
    {
        if (size == 4)
            *((ulong *)addr) = (ulong )writeval;
        else if (size == 2)
            *((ushort *)addr) = (ushort)writeval;
        else
            *((u_char *)addr) = (u_char)writeval;
        // addr += size;
        addr += 1;
    }
    return 0;
}


/* Memory Display
 *
 * Syntax:
 *	md{.b, .w, .l} {addr} {len}
 */
#define DISP_LINE_LEN	16

INT32 do_mem_md ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    ulong	addr, length;
    int	size;
    int rc = 0;

    /* We use the last specified parameters, unless new ones are
     * entered.
     */
    addr = dp_last_addr;
    size = dp_last_size;
    length = dp_last_length;

    if (argc < 2)
        return cmd_usage(cmdtp);

    if ((flag & CMD_FLAG_REPEAT) == 0)
    {
        /* New command specified.  Check for a size specification.
         * Defaults to long if no or incorrect specification.
         */
        if ((size = cmd_get_data_size(argv[0], 4)) < 0)
            return 1;

        /* Address is specified since argc > 1
        */
        addr = simple_strtoul(argv[1], NULL, 16);
        addr += base_address;

        /* If another parameter, it is the length to display.
         * Length is the number of objects, not number of bytes.
         */
        if (argc > 2)
        {
            length = simple_strtoul(argv[2], NULL, 16);
        }
    }


        /* Print the lines. */
        print_buffer(addr, (void *)addr, size, length, DISP_LINE_LEN / size);
        //addr += size * length;
        addr += length;


    dp_last_addr = addr;
    dp_last_length = length;
    dp_last_size = size;
    return (rc);
}



/* Modify memory.
 *
 * Syntax:
 *	mm{.b, .w, .l} {addr}
 *	nm{.b, .w, .l} {addr}
 */
static INT32
mod_mem(cmd_tbl_t *cmdtp, INT32 incrflag, INT32 flag, INT32 argc, INT8 *const argv[])
{
    UINT32	addr, i;
    INT32	nbytes, size;
    extern INT8 console_buffer[];

    if (argc != 2)
        return cmd_usage(cmdtp);

#ifdef CONFIG_BOOT_RETRY_TIME
    reset_cmd_timeout();	/* got a good command to get here */
#endif
    /* We use the last specified parameters, unless new ones are
     * entered.
     */
    addr = mm_last_addr;
    size = mm_last_size;

    if ((flag & CMD_FLAG_REPEAT) == 0)
    {
        /* New command specified.  Check for a size specification.
         * Defaults to long if no or incorrect specification.
         */
        if ((size = cmd_get_data_size(argv[0], 4)) < 0)
            return 1;

        /* Address is specified since argc > 1
        */
        addr = simple_strtoul(argv[1], NULL, 16);
        addr += base_address;
    }

#ifdef CONFIG_HAS_DATAFLASH
    if (addr_dataflash(addr))
    {
        shellputs ("Can't modify DataFlash in place. Use cp instead.\n\r");
        return 0;
    }
#endif



    /* Print the address, followed by value.  Then accept input for
     * the next value.  A non-converted value exits.
     */
    do
    {
        shellprintf("%08lx:", addr);
        if (size == 4)
            shellprintf(" %08x", *((uint *)addr));
        else if (size == 2)
            shellprintf(" %04x", *((ushort *)addr));
        else
            shellprintf(" %02x", *((u_char *)addr));

        nbytes = readline (" ? ");
        if (nbytes == 0 || (nbytes == 1 && console_buffer[0] == '-'))
        {
            /* <CR> pressed as only input, don't modify current
             * location and move to next. "-" pressed will go back.
             */
            if (incrflag) {
                // addr += nbytes ? -size : size;
            	addr += nbytes ? -1 : 1;
            }
            nbytes = 1;
#ifdef CONFIG_BOOT_RETRY_TIME
            reset_cmd_timeout(); /* good enough to not time out */
#endif
        }
#ifdef CONFIG_BOOT_RETRY_TIME
        else if (nbytes == -2)
        {
            break;	/* timed out, exit the command	*/
        }
#endif
        else
        {
            char *endp;
            i = simple_strtoul(console_buffer, &endp, 16);
            nbytes = endp - console_buffer;
            if (nbytes)
            {
#ifdef CONFIG_BOOT_RETRY_TIME
                /* good enough to not time out
                 */
                reset_cmd_timeout();
#endif
                if (size == 4)
                    *((uint *)addr) = i;
                else if (size == 2)
                    *((ushort *)addr) = i;
                else
                    *((u_char *)addr) = i;
                if (incrflag) {
                    // addr += size;
                    addr += 1;
                }
            }
        }
    }
    while (nbytes);

    mm_last_addr = addr;
    mm_last_size = size;
    return 0;
}


INT32 do_mem_mm ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    return mod_mem (cmdtp, 1, flag, argc, argv);
}
INT32 do_mem_nm ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    return mod_mem (cmdtp, 0, flag, argc, argv);
}

INT32 do_mem_base (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    if (argc > 1)
    {
        /* Set new base address.
        */
        base_address = simple_strtoul(argv[1], NULL, 16);
    }
    /* Print the current base address.
    */
    shellprintf("Base Address: 0x%08lx\n", base_address);
    return 0;
}


INT32 do_mem_cmp (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    UINT32	addr1, addr2, count, ngood;
    INT32	size;
    INT32     rcode = 0;

    if (argc != 4)
        return cmd_usage(cmdtp);

    /* Check for size specification.
    */
    if ((size = cmd_get_data_size(argv[0], 4)) < 0)
        return 1;

    addr1 = simple_strtoul(argv[1], NULL, 16);
    addr1 += base_address;

    addr2 = simple_strtoul(argv[2], NULL, 16);
    addr2 += base_address;

    count = simple_strtoul(argv[3], NULL, 16);

#ifdef CONFIG_HAS_DATAFLASH
    if (addr_dataflash(addr1) | addr_dataflash(addr2))
    {
        shellputs ("Comparison with DataFlash space not supported.\n\r");
        return 0;
    }
#endif



    ngood = 0;

    while (count-- > 0)
    {
        if (size == 4)
        {
            UINT32 word1 = *(UINT32 *)addr1;
            UINT32 word2 = *(UINT32 *)addr2;
            if (word1 != word2)
            {
                shellprintf("word at 0x%08lx (0x%08lx) "
                            "!= word at 0x%08lx (0x%08lx)\n",
                            addr1, word1, addr2, word2);
                rcode = 1;
                break;
            }
        }
        else if (size == 2)
        {
            UINT16 hword1 = *(UINT16 *)addr1;
            UINT16 hword2 = *(UINT16 *)addr2;
            if (hword1 != hword2)
            {
                shellprintf("halfword at 0x%08lx (0x%04x) "
                            "!= halfword at 0x%08lx (0x%04x)\n",
                            addr1, hword1, addr2, hword2);
                rcode = 1;
                break;
            }
        }
        else
        {
            UINT8 byte1 = *(UINT8 *)addr1;
            UINT8 byte2 = *(UINT8 *)addr2;
            if (byte1 != byte2)
            {
                shellprintf("byte at 0x%08lx (0x%02x) "
                            "!= byte at 0x%08lx (0x%02x)\n",
                            addr1, byte1, addr2, byte2);
                rcode = 1;
                break;
            }
        }
        ngood++;
        //addr1 += size;
        //addr2 += size;

        addr1 += 1;
        addr2 += 1;
    }

    shellprintf("Total of %ld %s%s were the same\n",
                ngood, size == 4 ? "word" : size == 2 ? "halfword" : "byte",
                ngood == 1 ? "" : "s");
    return rcode;
}


INT32 do_mem_loopw (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    UINT32	addr, length, i, data;
    INT32	size;
    volatile UINT32	*longp;
    volatile UINT16 *shortp;
    volatile UINT8	*cp;

    if (argc < 4)
        return cmd_usage(cmdtp);

    /* Check for a size spefication.
     * Defaults to long if no or incorrect specification.
     */
    if ((size = cmd_get_data_size(argv[0], 4)) < 0)
        return 1;

    /* Address is always specified.
    */
    addr = simple_strtoul(argv[1], NULL, 16);

    /* Length is the number of objects, not number of bytes.
    */
    length = simple_strtoul(argv[2], NULL, 16);

    /* data to write */
    data = simple_strtoul(argv[3], NULL, 16);

    /* We want to optimize the loops to run as fast as possible.
     * If we have only one object, just run infinite loops.
     */
    if (length == 1)
    {
        if (size == 4)
        {
            longp = (UINT32 *)addr;
            for (;;)
                *longp = data;
        }
        if (size == 2)
        {
            shortp = (UINT16 *)addr;
            for (;;)
                *shortp = data;
        }
        cp = (UINT8 *)addr;
        for (;;)
            *cp = data;
    }

    if (size == 4)
    {
        for (;;)
        {
            longp = (UINT32 *)addr;
            i = length;
            while (i-- > 0)
                *longp++ = data;
        }
    }
    if (size == 2)
    {
        for (;;)
        {
            shortp = (UINT16 *)addr;
            i = length;
            while (i-- > 0)
                *shortp++ = data;
        }
    }
    for (;;)
    {
        cp = (UINT8 *)addr;
        i = length;
        while (i-- > 0)
            *cp++ = data;
    }
    // return 0;
}

INT32 do_mem_loop (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    UINT8	addr, length, i, junk;
    INT32	size;
    volatile UINT32	*longp;
    volatile UINT16 *shortp;
    volatile UINT8	*cp;

    if (argc < 3)
        return cmd_usage(cmdtp);

    /* Check for a size spefication.
     * Defaults to long if no or incorrect specification.
     */
    if ((size = cmd_get_data_size(argv[0], 4)) < 0)
        return 1;

    /* Address is always specified.
    */
    addr = simple_strtoul(argv[1], NULL, 16);

    /* Length is the number of objects, not number of bytes.
    */
    length = simple_strtoul(argv[2], NULL, 16);

    /* We want to optimize the loops to run as fast as possible.
     * If we have only one object, just run infinite loops.
     */
    if (length == 1)
    {
        if (size == 4)
        {
            longp = (UINT32 *)addr;
            for (;;)
                i = *longp;
        }
        if (size == 2)
        {
            shortp = (UINT16 *)addr;
            for (;;)
                i = *shortp;
        }
        cp = (UINT8 *)addr;
        for (;;)
            i = *cp;
    }

    if (size == 4)
    {
        for (;;)
        {
            longp = (UINT32 *)addr;
            i = length;
            while (i-- > 0)
                junk = *longp++;
        }
    }
    if (size == 2)
    {
        for (;;)
        {
            shortp = (UINT16 *)addr;
            i = length;
            while (i-- > 0)
                junk = *shortp++;
        }
    }
    for (;;)
    {
        cp = (UINT8 *)addr;
        i = length;
        while (i-- > 0)
            junk = *cp++;
    }
    // return 0;
}

/*
 * Perform a memory test. A more complete alternative test can be
 * configured using CONFIG_SYS_ALT_MEMTEST. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 */
INT32 do_mem_mtest (cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    UINT16	*addr, *start, *end;
    UINT16	val;
    UINT16	readback;
    UINT16	errs = 0;
    int iterations = 1;
    int iteration_limit;

#if defined(CONFIG_SYS_ALT_MEMTEST)
    UINT16	len;
    UINT32	offset;
    UINT32	test_offset;
    UINT16	pattern;
    UINT16	temp;
    UINT16	anti_pattern;
    UINT32	num_words;
#if defined(CONFIG_SYS_MEMTEST_SCRATCH)
    UINT16 *dummy = (UINT16 *)CONFIG_SYS_MEMTEST_SCRATCH;
#else
    UINT16 *dummy = 0;	/* yes, this is address 0x0, not NULL */
#endif
    int	j;

    static const ulong bitpattern[] =
    {
        0x00000001,	/* single bit */
        0x00000003,	/* two adjacent bits */
        0x00000007,	/* three adjacent bits */
        0x0000000F,	/* four adjacent bits */
        0x00000005,	/* two non-adjacent bits */
        0x00000015,	/* three non-adjacent bits */
        0x00000055,	/* four non-adjacent bits */
        0xaaaaaaaa,	/* alternating 1/0 */
    };
#else
    UINT16	incr;
    UINT16	pattern;
#endif

    if (argc > 1)
        start = (UINT16 *)simple_strtoul(argv[1], NULL, 16) + base_address;
    else
        start = (UINT16 *)CONFIG_SYS_MEMTEST_START;

    if (argc > 2)
        end = (UINT16 *)simple_strtoul(argv[2], NULL, 16) + base_address;
    else
        end = (UINT16 *)(CONFIG_SYS_MEMTEST_END);

    if (argc > 3)
        pattern = (UINT16)simple_strtoul(argv[3], NULL, 16);
    else
        pattern = 0;

    if (argc > 4)
        iteration_limit = (UINT16)simple_strtoul(argv[4], NULL, 16);
    else
        iteration_limit = 0;

#if defined(CONFIG_SYS_ALT_MEMTEST)
    shellprintf ("Testing %08x ... %08x:\n", (uint)start, (uint)end);
    PRINTF("%s:%d: start 0x%p end 0x%p\n",
           __FUNCTION__, __LINE__, start, end);

    for (;;)
    {
        if (ctrlc())
        {
            shellputc ('\n');
            return 1;
        }


        if (iteration_limit && iterations > iteration_limit)
        {
            shellprintf("Tested %d iteration(s) with %lu errors.\n",
                        iterations - 1, errs);
            return errs != 0;
        }

        shellprintf("Iteration: %6d\r", iterations);
        shellprintf("\n");
        iterations++;

        /*
         * Data line test: write a pattern to the first
         * location, write the 1's complement to a 'parking'
         * address (changes the state of the data bus so a
         * floating bus doen't give a false OK), and then
         * read the value back. Note that we read it back
         * into a variable because the next time we read it,
         * it might be right (been there, tough to explain to
         * the quality guys why it prints a failure when the
         * "is" and "should be" are obviously the same in the
         * error message).
         *
         * Rather than exhaustively testing, we test some
         * patterns by shifting '1' bits through a field of
         * '0's and '0' bits through a field of '1's (i.e.
         * pattern and ~pattern).
         */
        addr = start;
        for (j = 0; j < sizeof(bitpattern) / sizeof(bitpattern[0]); j++)
        {
            val = bitpattern[j];
            for(; val != 0; val <<= 1)
            {
                *addr  = val;
                *dummy  = ~val; /* clear the test data off of the bus */
                readback = *addr;
                if(readback != val)
                {
                    shellprintf ("FAILURE (data line): "
                                 "expected %08lx, actual %08lx\n",
                                 val, readback);
                    errs++;
                    if (ctrlc())
                    {
                        shellputc ('\n');
                        return 1;
                    }
                }
                *addr  = ~val;
                *dummy  = val;
                readback = *addr;
                if(readback != ~val)
                {
                    shellprintf ("FAILURE (data line): "
                                 "Is %08lx, should be %08lx\n",
                                 readback, ~val);
                    errs++;
                    if (ctrlc())
                    {
                        shellputc ('\n');
                        return 1;
                    }
                }
            }
        }

        /*
         * Based on code whose Original Author and Copyright
         * information follows: Copyright (c) 1998 by Michael
         * Barr. This software is placed into the public
         * domain and may be used for any purpose. However,
         * this notice must not be changed or removed and no
         * warranty is either expressed or implied by its
         * publication or distribution.
         */

        /*
         * Address line test
         *
         * Description: Test the address bus wiring in a
         *              memory region by performing a walking
         *              1's test on the relevant bits of the
         *              address and checking for aliasing.
         *              This test will find single-bit
         *              address failures such as stuck -high,
         *              stuck-low, and shorted pins. The base
         *              address and size of the region are
         *              selected by the caller.
         *
         * Notes:	For best results, the selected base
         *              address should have enough LSB 0's to
         *              guarantee single address bit changes.
         *              For example, to test a 64-Kbyte
         *              region, select a base address on a
         *              64-Kbyte boundary. Also, select the
         *              region size as a power-of-two if at
         *              all possible.
         *
         * Returns:     0 if the test succeeds, 1 if the test fails.
         */
        len = ((UINT16)end - (UINT16)start) / sizeof(UINT16);
        pattern = (UINT16) 0xaaaa;
        anti_pattern = (UINT16) 0x5555;

        PRINTF("%s:%d: length = 0x%x\n",
               __FUNCTION__, __LINE__,
               len);
        /*
         * Write the default pattern at each of the
         * power-of-two offsets.
         */
        for (offset = 1; offset < len; offset <<= 1)
        {
            start[offset] = pattern;
        }

        /*
         * Check for address bits stuck high.
         */
        test_offset = 0;
        start[test_offset] = anti_pattern;

        for (offset = 1; offset < len; offset <<= 1)
        {
            temp = start[offset];
            if (temp != pattern)
            {
                shellprintf ("\nFAILURE: Address bit stuck high @ 0x%.8lx:"
                             " expected 0x%.8lx, actual 0x%.8lx\n",
                             (UINT16)&start[offset], pattern, temp);
                errs++;
                if (ctrlc())
                {
                    shellputc ('\n');
                    return 1;
                }
            }
        }
        start[test_offset] = pattern;
        WATCHDOG_RESET();

        /*
         * Check for addr bits stuck low or shorted.
         */
        for (test_offset = 1; test_offset < len; test_offset <<= 1)
        {
            start[test_offset] = anti_pattern;

            for (offset = 1; offset < len; offset <<= 1)
            {
                temp = start[offset];
                if ((temp != pattern) && (offset != test_offset))
                {
                    shellprintf ("\nFAILURE: Address bit stuck low or shorted @"
                                 " 0x%x: expected 0x%x, actual 0x%x\n",
                                 (UINT16)&start[offset], pattern, temp);
                    errs++;
                    if (ctrlc())
                    {
                        shellputc ('\n');
                        return 1;
                    }
                }
            }
            start[test_offset] = pattern;
        }

        /*
         * Description: Test the integrity of a physical
         *		memory device by performing an
         *		increment/decrement test over the
         *		entire region. In the process every
         *		storage bit in the device is tested
         *		as a zero and a one. The base address
         *		and the size of the region are
         *		selected by the caller.
         *
         * Returns:     0 if the test succeeds, 1 if the test fails.
         */
        num_words = ((UINT16)end - (UINT16)start) / sizeof(UINT16) + 1;

        /*
         * Fill memory with a known pattern.
         */
        for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++)
        {
            WATCHDOG_RESET();
            start[offset] = pattern;
        }

        /*
         * Check each location and invert it for the second pass.
         */
        for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++)
        {
            WATCHDOG_RESET();
            temp = start[offset];
            if (temp != pattern)
            {
                shellprintf ("\nFAILURE (read/write) @ 0x%x:"
                             " expected 0x%x, actual 0x%x)\n",
                             (UINT16)&start[offset], pattern, temp);
                errs++;
                if (ctrlc())
                {
                    shellputc ('\n');
                    return 1;
                }
            }

            anti_pattern = ~pattern;
            start[offset] = anti_pattern;
        }

        /*
         * Check each location for the inverted pattern and zero it.
         */
        for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++)
        {
            WATCHDOG_RESET();
            anti_pattern = ~pattern;
            temp = start[offset];
            if (temp != anti_pattern)
            {
                shellprintf ("\nFAILURE (read/write): @ 0x%x:"
                             " expected 0x%x, actual 0x%x)\n",
                             (UINT16)&start[offset], anti_pattern, temp);
                errs++;
                if (ctrlc())
                {
                    shellputc ('\n');
                    return 1;
                }
            }
            start[offset] = 0;
        }
    }

#else /* The original, quickie test */
    incr = 1;
    for (;;)
    {
        if (ctrlc())
        {
            shellputc ('\n');
            return 1;
        }

        if (iteration_limit && iterations > iteration_limit)
        {
            shellprintf("Tested %d iteration(s) with %lu errors.\n",
                        iterations - 1, errs);
            return errs != 0;
        }
        ++iterations;

        shellprintf ("\rPattern %08lX  Writing..."
                     "%12s"
                     "\b\b\b\b\b\b\b\b\b\b",
                     pattern, "");

        for (addr = start, val = pattern; addr < end; addr++)
        {
            //WATCHDOG_RESET();
            *addr = val;
            val  += incr;
        }

        shellputs ("Reading...");

        for (addr = start, val = pattern; addr < end; addr++)
        {
            //WATCHDOG_RESET();
            readback = *addr;
            if (readback != val)
            {
                shellprintf ("\nMem error @ 0x%08X: "
                             "found %08lX, expected %08lX\n",
                             (UINT16)addr, readback, val);
                errs++;
                if (ctrlc())
                {
                    shellputc ('\n');
                    return 1;
                }
            }
            val += incr;
        }

        /*
         * Flip the pattern each time to make lots of zeros and
         * then, the next time, lots of ones.  We decrement
         * the "negative" patterns and increment the "positive"
         * patterns to preserve this feature.
         */
        if(pattern & 0x8000)
        {
            pattern = -pattern;	/* complement & increment */
        }
        else
        {
            pattern = ~pattern;
        }
        incr = -incr;
    }
#endif
    // return 0;	/* not reached */
}


INT32 do_mem_cp ( cmd_tbl_t *cmdtp, INT32 flag, INT32 argc, INT8 *const argv[])
{
    ulong	addr, dest, count;
    int	size;

    if (argc != 4)
        return cmd_usage(cmdtp);

    /* Check for size specification.
    */
    if ((size = cmd_get_data_size(argv[0], 4)) < 0)
        return 1;

    addr = simple_strtoul(argv[1], NULL, 16);
    addr += base_address;

    dest = simple_strtoul(argv[2], NULL, 16);
    dest += base_address;

    count = simple_strtoul(argv[3], NULL, 16);

    if (count == 0)
    {
        shellputs ("Zero length ???\n");
        return 1;
    }

#if 0
#ifndef CONFIG_SYS_NO_FLASH
    /* check if we are copying to Flash */
    if ( (addr2info(dest) != NULL)
#ifdef CONFIG_HAS_DATAFLASH
            && (!addr_dataflash(dest))
#endif
       )
    {
        INT32 rc;

        shellputs ("Copy to Flash... ");

        rc = flash_write ((char *)addr, dest, count * size);
        if (rc != 0)
        {
            flash_perror (rc);
            return (1);
        }
        shellputs ("done\n");
        return 0;
    }
#endif

#ifdef CONFIG_HAS_DATAFLASH
    /* Check if we are copying from RAM or Flash to DataFlash */
    if (addr_dataflash(dest) && !addr_dataflash(addr))
    {
        INT32 rc;

        shellputs ("Copy to DataFlash... ");

        rc = write_dataflash (dest, addr, count * size);

        if (rc != 1)
        {
            dataflash_perror (rc);
            return (1);
        }
        shellputs ("done\n");
        return 0;
    }

    /* Check if we are copying from DataFlash to RAM */
    if (addr_dataflash(addr) && !addr_dataflash(dest)
#ifndef CONFIG_SYS_NO_FLASH
            && (addr2info(dest) == NULL)
#endif
       )
    {
        INT32 rc;
        rc = read_dataflash(addr, count * size, (char *) dest);
        if (rc != 1)
        {
            dataflash_perror (rc);
            return (1);
        }
        return 0;
    }

    if (addr_dataflash(addr) && addr_dataflash(dest))
    {
        shellputs ("Unsupported combination of source/destination.\n\r");
        return 1;
    }
#endif

#endif


    while (count-- > 0)
    {
        if (size == 4)
            *((volatile UINT32 *)dest) = *((volatile UINT32 *)addr);
        else if (size == 2)
            *((volatile UINT16 *)dest) = *((volatile UINT16 *)addr);
        else
            *((volatile UINT8 *)dest) = *((volatile UINT8 *)addr);
        addr += size;
        dest += size;
    }
    return 0;
}

