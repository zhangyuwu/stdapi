
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#ifndef __Console_H__
#define __Console_H__

#include <types.h>
#include <lcd.h>

THandle GetStdHandle(int type);
void SetStdHandle(int type, THandle value);
char InKey();
char GetKey();
char WaitKey(unsigned long milliseconds);
bool InputString(char *buffer, int type, int maxlen, int *length);
void ClearKeyBuffer();
void kprintf(const char * fmt, ...);

#endif
