/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : fpga.c
 * Author        : yexin
 * Date          : 2018-07-27
 * Description   : fpga command
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-07-27
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"


s32 do_fpgatest ( cmd_tbl_t *cmdtp, s32 flag, s32 argc, s8 *const argv[])
{

    u16  repeat,index;
    u16   readdata, wregdata;
    u32   regaddr;
    /*check input arg number*/
    if (argc < 3)
    {
        shellprintf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    regaddr = simple_strtoul(argv[1], NULL, 16);
    wregdata = simple_strtoul(argv[2], NULL, 16);
    repeat = simple_strtoul(argv[3], NULL, 16);
    for(index = 0; index < repeat; index++)
    {
        WriteFpgaRegister(regaddr, wregdata);
        readdata = ReadFpgaRegister(regaddr);
        if(readdata != (~wregdata))
        {
           shellprintf("failure;write 0x%x to fpga ,read 0x%x \n", wregdata, readdata);
           return 1;
        }

    }
    shellprintf("fpga write and read test passed \n");

}


#pragma DATA_SECTION   (fpgacmd,"shell_lib");
far cmd_tbl_t fpgacmd[] =
{

	{
		"fpgatest", CONFIG_SYS_MAXARGS, 1,	do_fpgatest,
		"test fpga",
		"regaddr regvalue repeat\n"
		"regaddr :	fpga or cpld absolut address(byte address)\n"
		"regvalue :  write data\n"
		"repeat :	 repeate time\n"
		"example:  fpgatest  0x80000 0x55aa 10"

	},


};

void FpgaCmdInitialzie()
{
    s8 index;
    for (index = 0; index < sizeof(fpgacmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(fpgacmd[index]);
}


