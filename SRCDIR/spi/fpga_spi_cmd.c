#include "shellconsole.h"
#include "boarddrv.h"
#include "command.h"
#include "util.h"
#include "platform_os.h"
#include "fpga_spi.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#endif

#ifndef NULL
#define NULL   (void*)0
#endif

#ifndef DISP_LINE_LEN
#define DISP_LINE_LEN	16
#endif

int do_fpga_spi_cs(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	int channel;
	unsigned int option;

	if (argc != 3)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	channel = simple_strtoul(argv[1], NULL, 10);
	option = simple_strtoul(argv[2], NULL, 10);

	option = (option == 0) ? 0 : 1;

	FpgaSpiCs(channel, option);

	return CMD_RET_SUCCESS;
}

int do_fpga_spi_read(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	int rc = 0;
	int channel;
	unsigned int length;
	unsigned char buffer[100];
	int i = 0;

	if (argc != 3)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	channel = simple_strtoul(argv[1], NULL, 10);
	length = simple_strtoul(argv[2], NULL, 16);

	if( length > 100 ) {
		PRINTF("length is invalid, out of range=%d\n", length);
		return CMD_RET_FAILURE;
	}
	rc = FpgaSpiRead(channel, buffer, length);
	if( rc != 0 ) {
		PRINTF("FpgaSpiRead failed, rc=%d\n", rc);
		return CMD_RET_FAILURE;
	} else {
		for( i = 0; i < length; ++i) {
			PRINTF("0x%02x ", buffer[i]);
			if( ((i + 1) % 8) == 0 ) {
				PRINTF("\n");
			}
		}
	}

	return CMD_RET_SUCCESS;
}

int do_fpga_spi_write(cmd_tbl_t *cmdtp, s32 flag, int argc, char * const argv[])
{
	int rc = 0;
	int channel;
	unsigned int source;
	unsigned int length;

	if (argc != 4)
	{
		PRINTF("argc=%d\n", argc);
		return cmd_usage(cmdtp);
	}

	channel = simple_strtoul(argv[1], NULL, 10);
	source = simple_strtoul(argv[2], NULL, 16);
	length = simple_strtoul(argv[3], NULL, 16);

	rc = FpgaSpiWrite(channel, (unsigned char*)source, length);
	if( rc != 0 ) {
		PRINTF("FpgaSpiWrite failed, rc=%d\n", rc);
		return CMD_RET_FAILURE;
	}

	PRINTF("FPGA SPI WRITE COMPLETE\n");
	return CMD_RET_SUCCESS;
}

static cmd_tbl_t cmd_fpga_spi_sub[] = {
	{"cs", 6, 1, do_fpga_spi_cs, "", ""},
	{"read", 6, 1, do_fpga_spi_read, "", ""},
	{"write", 6, 1, do_fpga_spi_write, "", ""},
};

int do_fpga_spi(cmd_tbl_t * cmdtp, s32 flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2) {

		return CMD_RET_USAGE;
	}

	/* Strip off leading 'i2c' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_fpga_spi_sub[0], ARRAY_SIZE(cmd_fpga_spi_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

#pragma DATA_SECTION   (fpga_spi_cmd, "shell_lib");
far cmd_tbl_t fpga_spi_cmd[] =
{
	{
		"fpga_spi", CONFIG_SYS_MAXARGS, 1,	do_fpga_spi,
		"fpga_spi ops  args",
		"fpga_spi cs channel option\n"
		"fpga_spi read channel length\n"
		"fpga_spi write channel source length\n"
	},
};

int FpgaSpiCmdInit(void)
{
    s8 index;

    for (index = 0; index < sizeof(fpga_spi_cmd) / sizeof(cmd_tbl_t); index++)
        RegisterCommand(fpga_spi_cmd[index]);
	return 0;
}
