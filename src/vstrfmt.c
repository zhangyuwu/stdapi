
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/*
 * --- Update history ---
 * 
 * NEW: 原来函数修改成 vstrnfmt 保证缓冲区不溢出
 *      Add by Ken, 2006-02-22 11:54:09
 *
 * MOD: 增加对null字符串参数的处理，填入(null)
 *      Modified by Ken, 2006-02-24 13:52:54
 */

#include <types.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

void vstrnfmt(char *buffer, int maxsize, const char *fmt, va_list ap)
{
	const char * null_str = "(null)";
	int len = strlen(fmt);
	int i = 0, j = 0, mode, itemlen;
	int ival;
	UINT uval;
	double fval;
	char cval;
	char *sval;
	char buf[64];
	
	if (--maxsize < 0) {
		return;
	}
	
	while (i < len) {
		if (fmt[i] == '%') {
			
			switch(fmt[i + 1]) {
			case 'o':
				mode = 8;
				goto proc_int;
			case 'd':
				mode = 10;
				goto proc_int;
			case 'x':
			case 'X':
				mode = 16;
				goto proc_uint;
			case 'u':
				mode = 10;
				goto proc_uint;
			case 'p':
				if (j == maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					buffer[j++] = '0';
				}
				
				if (j == maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					buffer[j++] = 'x';
				}
				mode = 16;
				goto proc_uint;
				
			proc_int:
				ival = va_arg(ap, int);		// gcc pass char as int
				int2str(ival, buf, mode);
				itemlen = strlen(buf);
				goto proc_int_0;
			proc_uint:
				uval = va_arg(ap, int);		// gcc pass char as int
				uint2str(uval, buf, mode);
				itemlen = strlen(buf);
				goto proc_int_0;
			proc_int_0:
				if (j + itemlen >= maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					memcpy(buffer + j, buf, itemlen);
					i += 2;
					j += itemlen;
				}
				break;

			case 'f':
				fval = va_arg(ap, double);
				float2str(fval, buf);
				itemlen = strlen(buf);
				
				if (j + itemlen >= maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					memcpy(buffer + j, buf, itemlen);
					i += 2;
					j += itemlen;
				}
				break;

			case '%':
				if (j == maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					buffer[j++] = '%';
					i += 2;
				}
				break;

			case 'c':
				cval = (char)va_arg(ap, int);

				if (j == maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					buffer[j++] = cval;
					i += 2;
				}
				break;
				
			case 's':
				sval = va_arg(ap, char *);
				if (sval == NULL) {
					sval = (char *)null_str;
				}
				itemlen = strlen(sval);

				if (j + itemlen >= maxsize) {
					buffer[j] = '\0';
					return;
				}
				else {
					memcpy(buffer + j, sval, itemlen);
					i += 2;
					j += itemlen;
				}
				break;
				
			default:
				i++;
				break;
			}
		}
		else {
			if (j == maxsize) {
				buffer[j] = '\0';
				return;
			}
			else {
				buffer[j++] = fmt[i++];
			}
		}
	}

	buffer[j] = '\0';
}

