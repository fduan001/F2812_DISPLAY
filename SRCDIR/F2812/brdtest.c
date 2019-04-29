#include <stdio.h>
#include "DSP28_Device.h"
#include "boardcfg.h"

#if 0
void Test_Hs3282(unsigned short var1, unsigned short var2);
void Test_GpioOutput(unsigned short var1, unsigned short var2);
void Test_GpioInput(unsigned short var1, unsigned short var2) ;
void Test_Led(unsigned short var1, unsigned short var2);
void Test_Watchdog(unsigned short var1, unsigned short var2);
interrupt void ISRTimer2(void);





typedef  struct
{
    void (*FuncAddr)(unsigned short, unsigned short);
    char *strhelp;
    char *strsubmenu;
} S_TESTFUNC;

static S_TESTFUNC Test_Functions[] =
{
    {Test_GpioOutput,  "e:exit; a:out 1; b:out 0;h:help ",    "welcome entering Test GpioOutput sub menu "},
    {Test_GpioInput,     "e:exit; a:show input valu;h:help",     "welcome entering Test GpioInput sub menu "},
    {Test_Led, "e:exit; a:led on; b:led off;c: spi write test;h:help  ",    "welcome entering Test Led sub menu "},
    {Test_Hs3282, "e:exit; a:send test data;b: read rev1 ;c:read rev2;d: polling channel 1;f:rev 16times;h:help  ", "welcome entering Test Hs3282 sub menu "},
    {Test_Watchdog, "e:exit; a: kick WD 1000 times; b:enable WD; c:disable WD;d:disable isr kick watchdog;h:help  ", "welcome entering Test Watchdog sub menu "},

};

char *myconst1 = "dfdsfdsfdsf";
char *myconst2 = "aaaabbbb";
int  myglbval = 0x55aa;
int  kickwd_flag = 1;

int  hs3282rev_low[16], hs3282rev_high[16];

void PrintMainhelp()
{
    UartPrintf("input the main test menu key: \n");
    UartPrintf("    --- 1 : Test for GPIO output\n");
    UartPrintf("    --- 2 : Test for GPIO input\n");
    UartPrintf("    --- 3 : Test for LED\n");
    UartPrintf("    --- 4 : Test for Hs3282\n");
    UartPrintf("    --- 5 : Test for watchdog\n");
}
#if 1
void Test_GpioOutput(unsigned short var1, unsigned short var2)
{
    unsigned char keyvalue;
    unsigned short portvalue;
    while(1)
    {
        keyvalue = UartGetc();
        switch(keyvalue)
        {
            //exit function
        case 'e':
            UartPrintf("exit gpio output test function \n");
            return;
            //output swout all 1
        case 'a':
            UartPrintf("output SWOUT5~0(GPIOB15~10) all 1\n");
            portvalue = GpioPortRead('B');
            portvalue = BOOT_SET_BITFIELD(portvalue, 0x3f, 15, 10);
            GpioPortWrite('B', portvalue);
            UartPrintf("output SWOUT13~6(GPIOF13~6) all 1\n");
            portvalue = GpioPortRead('F');
            portvalue = BOOT_SET_BITFIELD(portvalue, 0xff, 13, 6);
            GpioPortWrite('F', portvalue);
            break;

        case 'b':
            UartPrintf("output SWOUT5~0(GPIOB15~10) all 0\n");
            portvalue = GpioPortRead('B');
            portvalue = BOOT_SET_BITFIELD(portvalue, 0x0, 15, 10);
            GpioPortWrite('B', portvalue);
            UartPrintf("output SWOUT13~6(GPIOF13~6) all 0\n");
            portvalue = GpioPortRead('F');
            portvalue = BOOT_SET_BITFIELD(portvalue, 0x0, 13, 6);
            GpioPortWrite('F', portvalue);
            break;
        case 'h':
            UartPrintf("%s\n", (Test_Functions[var1]).strhelp);
            break;
        default:
            UartPrintf("invalid command,only e&a&b is valid \n");
            break;

        }
        UartPrintf("command is completed and waiting for input new command \n");
    }

}

void Test_GpioInput(unsigned short var1, unsigned short var2)
{
    unsigned char keyvalue;
    unsigned short portvalue;
    while(1)
    {
        keyvalue = UartGetc();
        switch(keyvalue)
        {
            //exit function
        case 'e':
            UartPrintf("exit gpio input test function \n");
            return;
            //get swin input value
        case 'a':
            portvalue = GpioPortRead('A');
            UartPrintf("display the SWIN7~0(GPIOA15~8) value=0x%x\n", BOOT_READ_BITFIELD(portvalue, 15, 8));
            portvalue = GpioPortRead('B');
            UartPrintf("display the SWIN15~8(GPIOB7~0) value=0x%x\n", BOOT_READ_BITFIELD(portvalue, 7, 0));

            break;
        case 'h':
            UartPrintf("%s\n", (Test_Functions[var1]).strhelp);
            break;
        default:
            UartPrintf("invalid command \n");
            break;

        }
        UartPrintf("command is completed and waiting for input new command \n");
    }

}

void Test_Led(unsigned short var1, unsigned short var2)
{

    unsigned char keyvalue;
    unsigned short i, j, dignum;

    while(1)
    {
        keyvalue = UartGetc();
        switch(keyvalue)
        {
            //exit function
        case 'e':
            UartPrintf("exit led  test function \n");
            return;
            //on all led
        case 'a':
            UartPrintf("all led  on \n");
            for(i = 0; i <= 5; i++)
            {
                for(j = 1; j <= 8; j++)
                    SpiWrite7221Reg(i, j, MAX7221_TEST_ON);
            }
            UartPrintf("test all led on finish \n");
            break;

            //off all led
        case 'b':
            UartPrintf("all led  off \n");
            for(i = 0; i <= 5; i++)
            {
                for(j = 1; j <= 8; j++)
                    SpiWrite7221Reg(i, j, MAX7221_TEST_OFF);

            }
            UartPrintf("test all led off finish \n");
            break;
            //spi write test
        case 'c':
            UartPrintf("test SPI write timing \n");
            UartPrintf("write max7221 chip 0 500 times\n");
            for(i = 0; i < 500; i++)
                SpiWrite7221Reg(0x0, 0x0, 0x55aa);
            break;
        case 'h':
            UartPrintf("%s\n", (Test_Functions[var1]).strhelp);
            break;
        default:
            UartPrintf("invalid command,only e&a&b is valid \n");
            break;


        }
        UartPrintf("command is completed and waiting for input new command \n");

    }

}

void Test_Hs3282(unsigned short var1, unsigned short var2)
{
    unsigned char keyvalue, i;
    unsigned short portvalue;
    unsigned short wordl , wordh;



    while(1)
    {
        keyvalue = UartGetc();
        switch(keyvalue)
        {
            //exit function
        case 'e':
            UartPrintf("exit HS3282  test function \n");
            return;
            //send 429 data
        case 'a':
            UartPrintf("HS3282 send data 100 times \n");
            for(i = 0; i <= 100; i++)
            {
                Hs3282SendData(HS3282_TX_DAT1, HS3282_TX_DAT2);
            }
            UartPrintf("test Hs3282 send data finish \n");
            break;
            //read 429 rev 1 data
        case 'b':
            UartPrintf("HS3282 receive channel 1 data \n");
            Hs3292RevData(0, &wordl, &wordh);
            UartPrintf("receive channel1 wordlow=0x%x wordhigh=0x%x \n", wordl, wordh);
            break;

            //read 429 rev 2 data
        case 'c':
            UartPrintf("HS3282 receive channel 2 data \n");
            Hs3292RevData(1, &wordl, &wordh);
            UartPrintf("receive channel2 wordlow=0x%x wordhigh=0x%x \n", wordl, wordh);
            break;

            //polling 429 rev 1 data
        case 'd':
            UartPrintf("polling HS3282 receive channel 1 data forever,never exit \n");
            while(1)
            {
                Hs3292RevData(0, &wordl, &wordh);
                UartPrintf("receive channel 1 wordlow=0x%x wordhigh=0x%x \n", wordl, wordh);
            }
            break;

        case 'f':
            UartPrintf("receive HS3282 receive channel 1 data 16 times\n");
            for(i = 0; i <= 15; i++)
            {
                hs3282rev_low[i] = 0;
                hs3282rev_high[i] = 0;
            }

            for(i = 0; i <= 15; i++)
            {
                Hs3292RevData(0, &wordl, &wordh);
                hs3282rev_low[i] = wordl;
                hs3282rev_high[i] = wordh;
            }
            for(i = 0; i <= 15; i++)
            {
                UartPrintf("times=0x%x, low=0x%x ,high=0x%x\n", i, hs3282rev_low[i], hs3282rev_high[i]);
            }
            UartPrintf("receive 16 times finished\n");
            break;


        case 'h':
            UartPrintf("%s\n", (Test_Functions[var1]).strhelp);
            break;
        default:
            UartPrintf("invalid command,only e&a is valid \n");
            break;

        }
        UartPrintf("command is completed and waiting for input new command \n");
    }

}



void Test_Watchdog(unsigned short var1, unsigned short var2)
{
    unsigned char keyvalue, i, j;
    unsigned short portvalue;

    while(1)
    {
        keyvalue = UartGetc();

        switch(keyvalue)
        {
            //exit function
        case 'e':
            UartPrintf("exit watchdog  test function \n");
            return;
            //kick watchdog
        case 'a':

            UartPrintf("kick watchdog 1000 times \n");
            for(i = 0; i < 1000; i++)
            {
                KickWatchDog();
                Delay(10000);

            }
            break;
            //disable watchdog
        case 'b':
            DisableWatchDog();
            UartPrintf("test disable watchdog finish \n");
            break;
            //enable watchdog
        case 'c':
            EnableWatchDog();
            UartPrintf("test enable watchdog finish \n");
            break;
        case 'h':
            UartPrintf("%s\n", (Test_Functions[var1]).strhelp);
            break;
            // disable isr kick watchdog
        case 'd':
            UartPrintf("stop isr kick watchdog,board will be reboot\n");
            kickwd_flag = 0;
        default:
            UartPrintf("invalid command,only e&a&b&c is valid \n");
            break;

        }
        UartPrintf("command is completed and waiting for input new command \n");
    }


}
#endif


void main(void)
{
    unsigned char keyvalue, i, funcnum, j;
    char *tmp;
    unsigned int menuindex;





    /*初始化系统*/
    InitSysCtrl();
    /* 关中断 */
    DINT;
    IER = 0x0000;
    IFR = 0x0000;

    /* 初始化PIE控制寄存器 */
    InitPieCtrl();
    /* 初始化PIE参数表 */
    InitPieVectTable();
    /* 初始化外设寄存器 */
    InitPeripherals();
    if(0 == UnlockCsm())
        UartPrintf("unlock csm for flash programing sucessfully \n");
    else
    {
        UartPrintf("unlock csm for flash programing failure,reset the board \n");
        while(1);
    }

    // for debug serial
#if 0
    while(1)
    {
        UartCharPut('A');
        //keyvalue=UartGetc();
    }
#endif
    // for debug hs3282
#if 0
    while(1)
    {
        Hs3282SendData(HS3282_TX_DAT1, HS3282_TX_DAT2);
    }
#endif

    // for debug spi
#if 0
    while(1)
    {
        for(j = 0; j <= 7; j++)
            SpiWrite7221Reg(1, (j + 1), MAX7221_TEST_ON);
    }
#endif

    /* test  for uart */
    UartPrintf("welcome to run F2812 test program \n");

    funcnum = sizeof(Test_Functions) / sizeof(S_TESTFUNC);

    UartPrintf("funcnum=0x%x\n", funcnum);


    UartPrintf("start cpu timer 2 500ms\n");

    EALLOW;
    PieVectTable.TINT2 = ISRTimer2;
    EDIS;

    /*设置CPU timer*/

    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer2, (float)150, (float)500000);
    StartCpuTimer2();
    KickWatchDog();
    /*开中断*/
#if 1
    IER |= M_INT14;

    EINT;   // Enable Global interrupt INTM
    ERTM;	// Enable Global realtime interrupt DBGM
#endif


    PrintMainhelp();

    while(1)
    {
       // UartPrintf("read key \n");
       // keyvalue = UartGetc();
       // UartPrintf("input key is =0x%x\n", keyvalue);
#if 1
        keyvalue = UartGetc();
        menuindex = (unsigned int)(keyvalue - '1');
        UartPrintf("get key value =0x%x,submenu=0x%x\n", keyvalue, menuindex);
        if(funcnum <= (menuindex))
        {
            UartPrintf("invalid commoand \n");
            PrintMainhelp();
        }
        else
        {
            UartPrintf("%s\n", (Test_Functions[menuindex]).strsubmenu);
            UartPrintf("%s\n", (Test_Functions[menuindex]).strhelp);
            (*Test_Functions[menuindex].FuncAddr)(menuindex, 0);
            UartPrintf("exit current test sub menu and waiting for selecting new test menu \n");
            PrintMainhelp();
        }
#endif

    }

}


/*
interrupt void ISRSciaTx(void)
{
	PieCtrl.PIEACK.bit.ACK6 = 1;
	EINT;
}

interrupt void ISRSciaRx(void)
{
	PieCtrl.PIEACK.bit.ACK6 = 1;
	if(Spi_RxReady() == 1)
	{
		Spi_VarRx[j] = SpiaRegs.SPIRXBUF;
		j++;
		if(j == 100)
		{
			j = 0;
		}
	}
	EINT;
}
*/

interrupt void ISRTimer2(void)
{
    CpuTimer2.InterruptCount++;
    /* kick watchdog */
    if(kickwd_flag == 1)
    {
        KickWatchDog();
    }
    /* do nothing */
    else
    {

    }
}



#endif


//===========================================================================
// No more.
//===========================================================================
