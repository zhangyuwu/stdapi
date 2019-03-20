
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
#include <system.h>

void strfmt(char * str, const char * fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vstrnfmt(str, MAX_INT, fmt, args);
	va_end(args);
}

void strnfmt(char * str, int maxsize, const char * fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vstrnfmt(str, maxsize, fmt, args);
	va_end(args);
}
