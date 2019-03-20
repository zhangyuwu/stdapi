
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <device.h>
#include <console.h>
#include <system.h>

void kprintf(const char * fmt, ...)
{
	char buffer[128];
	va_list args;	

	if (GetStdHandle(STD_DEBUG_HANDLE)) {
		memset(buffer, 0, sizeof(buffer));
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer)-1, fmt, args);
		va_end(args);
		WriteDevice(GetStdHandle(STD_DEBUG_HANDLE), buffer, strlen(buffer));
	}
}
