#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"
#include "rs422.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#endif

#ifndef NULL
#define NULL   (void*)0
#endif

#ifndef DISP_LINE_LEN
#define DISP_LINE_LEN	16
#endif

static int do_rs422_init(cmd_tbl_t *cmdtp, s32 flag,  int argc, char * const argv[])
{
	int  rc = 0;
	unsigned char chip;
	char party;
	unsigned char stop;
	unsigned char data_bit;
	unsigned int baud;

	if (argc != 6)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	chip = simple_strtoul(argv[1], NULL, 10);
	party = simple_strtoul(argv[2], NULL, 10);
	stop = simple_strtoul(argv[3], NULL, 10);
	data_bit = simple_strtoul(argv[4], NULL, 10);
	baud = simple_strtoul(argv[5], NULL, 10);

	rc = RS422Open(chip, party, stop, data_bit, baud);
	if( rc != 0 ) {
		PRINTF("RS422Open failed, rc=%d\n", rc);
		return CMD_RET_FAILURE;
	}

	return CMD_RET_SUCCESS;
}

static int do_rs422_show(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	unsigned char chip;

	if (argc != 2)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	chip = simple_strtoul(argv[1], NULL, 10);
	DebugUartRegInfo(chip);

	return CMD_RET_SUCCESS;
}

static int do_rs422_read(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	int rc = 0;
	int chip;
	unsigned int bytes;
	char buff[100];
	int i = 0;

	if (argc != 3)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	chip = simple_strtoul(argv[1], NULL, 10);
	bytes = simple_strtoul(argv[2], NULL, 10);

	if( bytes > 100 ) {
		PRINTF("bytes out of range\n");
		return CMD_RET_FAILURE;
	}
	rc = RS422Read(chip, buff, bytes);
	if( rc != 0 ) {
		PRINTF("RS422Read failed\n");
		return CMD_RET_FAILURE;
	}

	for( i = 0; i < bytes; ++i) {
		PRINTF("0x%02x ", buff[i]);
		if( ((i + 1) % 8) == 0 ) {
			PRINTF("\n");
		}
	}

	return CMD_RET_SUCCESS;
}

static int do_rs422_write(cmd_tbl_t *cmdtp, s32 flag,  int argc, char * const argv[])
{
	int rc = 0;
	int chip;
	unsigned int bytes;
	unsigned int source;

	if (argc != 4)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	chip = simple_strtoul(argv[1], NULL, 10);
	source = simple_strtoul(argv[2], NULL, 16);
	bytes = simple_strtoul(argv[3], NULL, 10);

	rc = RS422Write(chip, (char*)source, bytes);
	if( rc != 0 ) {
		PRINTF("RS422Write failed\n");
		return CMD_RET_FAILURE;
	}

	PRINTF("rs422 WRITE COMPLETE\n");
	return CMD_RET_SUCCESS;
}

#define  UT_LEN   16

static int do_rs422_test(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	int  rc = 0;
	unsigned char cbuf[64];
	int  i = 0;
	int index = 0;
	int  activeNo = 0;

	if (argc != 1)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	rc = RS422Open(0, 'e', 1, 8, 19200);
	if( rc != 0 ) {
		PRINTF("RS422Open failed, rc=%d\n", rc);
		return CMD_RET_FAILURE;
	}

	rc = RS422Open(1, 'e', 1, 8, 19200);
	if( rc != 0 ) {
		PRINTF("RS422Open failed, rc=%d\n", rc);
		return CMD_RET_FAILURE;
	}

	memset(cbuf, 0, sizeof(cbuf));
	     
	for(i = 0;i < UT_LEN;i++)
	{
	    cbuf[i] = i+0x30;
	    	 //cbuf[i] = (i % 2 == 0) ? 0x5a : 0xa5;
	}
	Osal_TaskSleep(100);
	     
	PRINTF("----Send RS422 Frame on chip1-----\r\n");
	    // for(index = 0;index <= 0x100;index++)
	     {
	     PRINTF("send index = %d\r\n",index); 	 
	     RS422Write(0,  (char*)cbuf,  UT_LEN);
	     Osal_TaskSleep(1000);
	     } 
	     activeNo = 1;
	     PRINTF("send ok......\r\n"); 	 
	     
	     for(index = 0;index <= 0x100;index++)
	     { 
	    	 memset(cbuf,0,sizeof(cbuf));
	    	 if(activeNo == 1)
	    	 {
		    	 if(RS422Read(1, (char*)cbuf, UT_LEN) == UT_LEN)
		    	 {
		    		 PRINTF("----Chip2 Recv RS422 Frame -----\r\n");

		    	     for(i = 0;i < UT_LEN;i++)
		    	     {
		    	    	 PRINTF("data[%02d] = %2x\r\n",i,cbuf[i]);
		    	    	 cbuf[i]++;
		    	     }
		    	     
		    	     RS422Write(1, (char*)cbuf, UT_LEN);
		    	     PRINTF("----Send RS422 Frame on chip2-----\r\n");
		    	     activeNo = 0;
		     		 
		    	 }
	    	 }
	    	 else
	    	 {
	    		 if(RS422Read(0, (char*)cbuf, UT_LEN) == UT_LEN)
		    	 {
		    		 PRINTF("----Chip1 Recv RS422 Frame -----\r\n");
	    		 
		    	     for(i = 0;i < UT_LEN;i++)
		    	     {
		    	    	 PRINTF("data[%02d] = %2x\r\n",i,cbuf[i]);
		    	    	 cbuf[i]++;
		    	     }
		    	     
		    	     RS422Write(0, (char*)cbuf, UT_LEN);
		    	     PRINTF("----Send RS422 Frame on chip1-----\r\n");
		    	     activeNo = 1;
		     		 
		    	 } 		 
	    	 }

	    	 
	    	 Osal_TaskSleep(200);
	    	 
	    	 
	     }

	return CMD_RET_SUCCESS;
}

static cmd_tbl_t cmd_rs422_sub[] = {
	{"init", 8, 1, do_rs422_init, "", ""},
	{"show", 2, 1, do_rs422_show, "", ""},
	{"read", 6, 1, do_rs422_read, "", ""},
	{"write", 6, 1, do_rs422_write, "", ""},
	{"test", 1, 1, do_rs422_test, "", ""},
};

static int do_rs422(cmd_tbl_t * cmdtp, s32 flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2) {

		return CMD_RET_USAGE;
	}

	/* Strip off leading 'i2c' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_rs422_sub[0], ARRAY_SIZE(cmd_rs422_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

#pragma DATA_SECTION   (rs422_cmd,"shell_lib");
far cmd_tbl_t rs422_cmd[] =
{
	{
		"rs422", CONFIG_SYS_MAXARGS, 1,	do_rs422,
		"rs422 debug commands",
		"rs422 init chip \n" \
		"rs422 show chip \n" \
		"rs422 test \n" \
		"rs422 read chip bytes\n" \
		"rs422 write chip source bytes\n"
	},
};

int rs422_cmd_init(void)
{
    s8 index;
    for (index = 0; index < sizeof(rs422_cmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(rs422_cmd[index]);
	return 0;
}
