/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : global_var.c
 * Author        : yexin
 * Date          : 2018-07-5
 * Description   : global variable definition
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-07-5
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#include "F2812_datatype.h"

#pragma DATA_SECTION   (testfar1,"shell_lib");
far UINT8 testfar1[10];

#pragma DATA_SECTION   (testfar2,"shell_lib");
far UINT8 testfar2[10];

far UINT8 testnear [10];

#pragma DATA_SECTION   (clkhandle,"shell_lib"); 
far HANDLE clkhandle;

#pragma DATA_SECTION   (semhandle,"shell_lib"); 
far HANDLE semhandle;

#pragma DATA_SECTION   (timerhandle,"shell_lib"); 
far HANDLE timerhandle;

