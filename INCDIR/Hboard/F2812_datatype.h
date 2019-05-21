/********************************************************************************

 **** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : F2812_datatype.h
 * Author        : yexin
 * Date          : 2018-06-18
 * Description   : F2812 data type
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2018-06-18
 *   Author      : yexin
 *   Modification: Created file

*************************************************************************************************************/
#ifndef F2812_DATATYPE_H
#define F2812_DATATYPE_H

typedef void          *HANDLE;


#ifndef _TPYES_H
#define _TPYES_H

#ifndef NULL
#define		NULL	0
#endif

#ifndef UINT32
#define		UINT32	unsigned long
#endif

#ifndef UINT16
#define		UINT16	unsigned int
#endif
#if 1
#ifndef UINT8
#define		UINT8	unsigned char
#endif
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif  /* _SIZE_T */


#ifndef INT32
#define		INT32	 long
#endif

#ifndef INT16
#define		INT16	 int
#endif

#if 1
#ifndef INT8
#define		INT8	 char
#endif
#endif

typedef unsigned long phys_addr_t;

#endif /* _TPYES_H */



#endif



