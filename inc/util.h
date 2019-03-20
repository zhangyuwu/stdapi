
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Util_H__
#define __Util_H__

#include <types.h>
#include <stdarg.h>
#include <lwip/ip_addr.h>

char * int2str(int value, char *buffer, int mode);
char * uint2str(UINT value, char *buffer, int mode);
char * float2str(double f, char *buffer);
int bin2str(const byte * bin, char * str, int length);
ushort checksum_crc16(const byte * buf, int length);
unsigned int checksum_crc32(const byte * buf, int length);
void vstrnfmt(char *buffer, int maxsize, const char *fmt, va_list ap);
void strfmt(char * str, const char * fmt, ...);
char * strtoken(char * str, const char * delimiters);
void outputf(void (*output)(const char *, int), const char * fmt, ...);
int round(double f);
const char *ipaddr2str(struct ip_addr *ipaddr);
void ping(struct ip_addr *ipaddr, int count);

#endif
