
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
 * NEW: 增加了 uint2str 对无符号整型的处理函数
 *      Add by Ken, 2006-02-22 11:53:00
 */
 
#include <types.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

char * int2str(int value, char *buffer, int mode)
{
	int i = 0, len;
	char ch, *ret = buffer;
	byte item;
	
	// 符号位
	if (value < 0) {
		value = -value;
		buffer[0] = '-';
		buffer++;
	}
	
	// 逆向保存字符串
	do {
		switch (mode) {
		case 16:	// 十六进制
			item = value % mode;

			if (item >= 10)
				buffer[i++] = (item - 10) + 'A';
			else
				buffer[i++] = item + '0';
				
			value /= mode;
			break;
			
		case 10:	// 十进制
		case 8:		// 八进制
		case 2:		// 二进制
			buffer[i++] = (value % mode) + '0';
			value /= mode;
			break;
			
		default:
			return NULL;
		}
	} while (value > 0);
	
	buffer[i] = '\0';
	
	// 反转
	len = strlen(buffer);
	for (i = 0; i < len / 2; i++) {
		ch = buffer[i];
		buffer[i] = buffer[len - i - 1];
		buffer[len - i - 1] = ch;
	}
	
	return ret;
}

char * uint2str(UINT value, char *buffer, int mode)
{
	int i = 0, len;
	char ch, *ret = buffer;
	byte item;
	
	// 逆向保存字符串
	do {
		switch (mode) {
		case 16:	// 十六进制
			item = value % mode;

			if (item >= 10)
				buffer[i++] = (item - 10) + 'A';
			else
				buffer[i++] = item + '0';
				
			value /= mode;
			break;
			
		case 10:	// 十进制
		case 8:		// 八进制
		case 2:		// 二进制
			buffer[i++] = (value % mode) + '0';
			value /= mode;
			break;
			
		default:
			return NULL;
		}
	} while (value > 0);
	
	buffer[i] = '\0';
	
	// 反转
	len = strlen(buffer);
	for (i = 0; i < len / 2; i++) {
		ch = buffer[i];
		buffer[i] = buffer[len - i - 1];
		buffer[len - i - 1] = ch;
	}
	
	return ret;
}

