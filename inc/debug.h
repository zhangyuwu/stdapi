
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <ucos_ii.h>

#if (DEBUG_INFO > 0)
	#define BEGIN_DEBUG_FUNCTION	{ debugf("void %s() {\r\n", __func__); DebugOutputIndent();}
	#define END_DEBUG_FUNCTION		{ DebugOutputUnIndent(); debugf("} <-- %s, %d\r\n", __func__, GetDebugOutputIndent()); }
#else
	#define BEGIN_DEBUG_FUNCTION
	#define END_DEBUG_FUNCTION
#endif

int GetDebugOutputIndent();
void DebugOutputIndent();
void DebugOutputUnIndent();

int debugf(const char *fmt, ...);
void StartDebugTask();

void OSDumpTCB(OS_TCB *p);
void OSTCBListDump();
void OSEventDump(OS_EVENT *event);

#endif
