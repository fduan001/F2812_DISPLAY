#ifndef _LINUX_CTYPE_H
#define _LINUX_CTYPE_H
#include "_isfuncdcl.h"
/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */
#if 0

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

#endif

#if 0
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif
#define NULL 0

//for 2812
#if 1
typedef unsigned long long     u64;
typedef long long               s64;

typedef void                  *VOID_STAR;      /* kept for compatibility */

typedef volatile unsigned long	vu_long;
typedef unsigned char		uchar;
typedef unsigned long phys_addr_t;

#endif 
#if 0
typedef unsigned long long int u64;
typedef long long int          s64;
typedef unsigned int           UINT32;
typedef unsigned short         UINT16;
typedef unsigned char          UINT8;
typedef          int           INT32;
typedef          short         INT16;
typedef          char          INT8; /* would have been better with signed */
typedef void                  *VOID_STAR;      /* kept for compatibility */
typedef          INT8            ascii;          /* kept for compatibility */
//typedef unsigned int  size_t;
typedef volatile unsigned long	vu_long;
typedef unsigned char		uchar;
typedef unsigned long phys_addr_t;
#endif

static inline unsigned char __tolower(unsigned char c)
{
    if (isupper(c))
        c -= 'A' - 'a';
    return c;
}

static  inline unsigned char __toupper(unsigned char c)
{
    if (islower(c))
        c -= 'a' - 'A';
    return c;
}

#define shelltolower(c) __tolower(c)
#define shelltoupper(c) __toupper(c)

/* bsd */
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

/* sysv */
typedef unsigned char		unchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

/* for gunzip */
typedef ushort          huft_code;
typedef uchar           huft_bits;




#endif
