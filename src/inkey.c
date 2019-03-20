
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
#include <hotkey.h>

// ----------------------------------------------------------------------------
char InKey()
{
	char ch;

	if (GetStdHandle(STD_INPUT_HANDLE)) {
		if (ReadDevice(GetStdHandle(STD_INPUT_HANDLE), &ch, sizeof(ch))) {
			// ¥¶¿Ì»»º¸
			ProcessHotkey(ch);
			return ch;
		}
	}
	
	return 0;
}
