
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 1.00                          *
 *                               Copyright (c)                                *
 *                             2005-06-01 by Ken                              *
 *                               Beijing.China                                *
 *                             sunwzd@hotmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Types_H__
#define __Types_H__

#include <sys/types.h>

// 常用宏
#ifndef NULL
	#define NULL	((void *)0)
#endif

#ifndef true
	#define true	1
#endif

#ifndef false
	#define false	0
#endif

#ifndef TRUE
	#define	TRUE	1
#endif

#ifndef FALSE
	#define FALSE	0
#endif

#ifndef ON
	#define ON		1
#endif

#ifndef OFF
	#define OFF		0
#endif

// 扩展类型定义
typedef unsigned char bool;
typedef unsigned char byte, BYTE;
typedef unsigned char uchar;
typedef unsigned short USHORT, WORD;
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned long ulong, DWORD, COLORREF;
typedef UINT (* Event)(void * arg);
typedef void * THandle;
typedef void (*irq_handler_t)(void);
typedef void (*timer_handler_t)(void *arg);

#endif
