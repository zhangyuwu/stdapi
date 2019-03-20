
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <device.h>
#include <console.h>
#include <system.h>
#include <stdarg.h>
#include <stdio.h>

char WaitKey(unsigned long milliseconds)
{
	unsigned long tick = GetTickCount();
	char ch = 0;

	while (GetTickCount() - tick < milliseconds && !ch) {
		ch = InKey();
		OSTimeDly(1);
	}

	return ch;
}

