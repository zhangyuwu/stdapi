
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <types.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

void outputf(void (*output)(const char *, int), const char * fmt, ...)
{
	char buffer[256];
	va_list args;

	va_start(args, fmt);
	vstrfmt(buffer, fmt, args);
	va_end(args);

	output(buffer, strlen(buffer));
}

