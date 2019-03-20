
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

// ----------------------------------------------------------------------------
typedef struct {
	THandle Stdin;
	THandle Stdout;
	THandle Stderr;
	THandle Debug;
} TConsole;

TConsole g_Console = { 
	.Stdin	= NULL,
	.Stdout	= NULL,
	.Stderr = NULL,
	.Debug  = NULL,
};

// ----------------------------------------------------------------------------
THandle GetStdHandle(int type)
{
	switch (type) {
	case STD_INPUT_HANDLE:
		SetLastError(NO_ERROR);
		return g_Console.Stdin;
		
	case STD_OUTPUT_HANDLE:
		SetLastError(NO_ERROR);
		return g_Console.Stdout;
	
	case STD_DEBUG_HANDLE:
		SetLastError(NO_ERROR);
		return g_Console.Debug;
	
	case STD_ERROR_HANDLE:
		SetLastError(NO_ERROR);
		return g_Console.Stderr;
		
	default:
		SetLastError(ERROR_BAD_PARAMETER);
		return NULL;
	}
}

// ----------------------------------------------------------------------------
void SetStdHandle(int type, THandle value)
{
	switch (type) {
	case STD_INPUT_HANDLE:
		g_Console.Stdin = value;
		SetLastError(NO_ERROR);
		break;
		
	case STD_OUTPUT_HANDLE:
		g_Console.Stdout = value;
		SetLastError(NO_ERROR);
		break;
	
	case STD_DEBUG_HANDLE:
		g_Console.Debug = value;
		SetLastError(NO_ERROR);
		break;

	case STD_ERROR_HANDLE:
		g_Console.Stderr = value;
		SetLastError(NO_ERROR);
		break;
		
	default:
		SetLastError(ERROR_BAD_PARAMETER);
		break;
	}
}
