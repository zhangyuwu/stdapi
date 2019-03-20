
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

/* ------------------------------------------------------------------------
 *  函数名：
 *      int bin2str(const byte * bin, char * str, int length)
 *  功能：
 *      把一个字节流转换成字符串
 *  参数：
 *      bin             ----    输入的字节流
 *      str             ----    输出的字符串流
 *      length          ----    要转换的字节流的长度
 *  返回值：
 *      转换后的字符串总长度
 *  说明：
 *      bin --> {A1, B2, C3, D4, FF...}, str --> "A1 B2 C3 D4 FF..."
 * -----------------------------------------------------------------------*/

int bin2str(const byte * bin, char * str, int length)
{
	int i, pos = 0;
	byte b;

	for (i = 0; i < length; i++) {
		b = bin[i] / 0x10;
		if (b <= 9) {
			str[pos] = '0' + b;
		}
		else {
			str[pos] = 'A' + b - 0xA;
		}
		pos++;

		b = bin[i] % 0x10;
		if (b <= 9) {
			str[pos] = '0' + b;
		}
		else {
			str[pos] = 'A' + b - 0xA;
		}
		pos++;

		if (i != length - 1) {
			str[pos] = ' ';
			pos++;
		}
	}
	str[pos] = '\0';
	return pos;
}

